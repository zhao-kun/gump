#include <wrapunix.h>
#include <gp_util.h>
#include <gp_runner.h>
#include <gp_lua_util.h>
#include <lua/lua.h> 
#include <lua/lauxlib.h>
#include <lua/lualib.h>
#include <redis/hiredis.h>


/* global variable definition */
static int                    __bexit__ = 0;
static struct timeb           __epoc_time; 
static RUNNERCFG_INFO         __runner_info__ ;
static VUSERS                 __vusers__;

int gp_load_from_cfg(const char *, RUNNERCFG_INFO *);
void gp_print_cfg(RUNNERCFG_INFO *);
void gp_sig_unblock(int);
void gp_set_log_level(int);
void gp_background();
int gp_redis_init(RUNNERCFG_INFO * info);

typedef struct {
    SERVICES  * p_runner; ;
    int               uid;
}THREAD_ARGS;

/* fucntion forward declaration */

int gp_luaget_glb_var_value(lua_State *,const char *, int, int *);
redisContext * gp_connect_redis(RUNNERCFG_INFO *);
lua_State * gp_init_lua(RUNNERCFG_INFO *);
int gp_redis_set_total(redisContext *, const char * , int);
int gp_redis_set_tpm(redisContext *, const char *, const char *, int, int, int);


void gp_termsig_handler( int signo )                                               
{                                                                               
    gp_errlog(LVL_ERROR, _FL_, "virtual user will stop....");
    fprintf(stdout, "virtual user will stop....[%d]\n", getpid());
    __bexit__ = 1;                                                     
    return ;                                                                    
} 

static void gp_show_version() 
{
    fprintf(stdout, "gump version 1.0\n");
    return ;
}

void gp_usage()
{
    fprintf(stdout, "用法: gump [-u <SVCNAME> -l <LOOP>][-v][-d] SCENARIO\n");
    fprintf(stdout, "\tSECENARIO 测试场景文件名，可以用绝对或相对路径指定，"
                    "如果不带路径默认就是在$RUNNERDIR/scenario目录下\n");
    fprintf(stdout, "选项:\n");
    fprintf(stdout, "\t-u SVCNAME  单元测试某个场景下的服务名\n");
    fprintf(stdout, "\t-l looptime 单元测试某个场景下的服务名"
                    ",循环执行loop次\n");
    fprintf(stdout, "\t-v 显示详细信息，包括gump版本号,scenario文件内容 \n");
    fprintf(stdout, "\t-d 以精灵进程模式运行\n");
}

static 
int gp_init_one_sequence(const char * seq_idx, int default_seq)
{
    char       f[512];
    char       l[512];
    char       p[512];
    FILE     * fp = NULL;


    snprintf(p, sizeof(p), "%s/sequence", getenv("RUNNERDIR"));

    if (access(p, W_OK|R_OK) < 0) {
        mkdir(p, 0700);
    }


    snprintf(f, sizeof(f), "%s/%s.seq", p, seq_idx);

    fp = fopen(f, "w+");
    if (fp == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "fopen [%s] error[%s]\n",
                f, strerror(errno));
        return -1;
    }

    fprintf(fp, "%d\n", default_seq);
    fclose(fp);

    snprintf(l, sizeof(l), "%s/.%s.lock", p, seq_idx);
    fp = fopen(l, "w+");
    if (fp == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "fopen[%s] error[%s]\n",
                l, strerror(errno));
        return -1;
    }

    fprintf(fp, "%d\n", getpid());
    fclose(fp);
    return 0;
}

static 
int gp_init_sequence(RUNNERCFG_INFO *info)
{
    int         i = 0;
    int         ret;

    for (i = 0; i < info->sequence_num; ++i) {
        ret = gp_init_one_sequence(info->sequence[i].seq_idx,
                info->sequence[i].default_seq);
        if (ret < 0) {
            gp_errlog(LVL_ERROR, _FL_, "gp_init_one_sequence");
            return -1;
        }
    }
    return 0;
}

int gp_do_init(lua_State *L, const char * svcname) 
{
    char       lua_script[1024];
    int        ret;
    int        res;

    snprintf(lua_script, sizeof(lua_script), "%s/script/%s/init.lua",
             getenv("RUNNERDIR"), svcname);

    /*   load lua script file */
    ret = luaL_loadfile(L, lua_script); 
    if (ret) {
        gp_errlog(LVL_ERROR, _FL_, "luaL_load[%s] failed[%d]" ,
                             lua_script, ret);
        return -1;
    }

    /*   complie lua script file */
    ret = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (ret) {
        gp_errlog(LVL_ERROR, _FL_, "lua_pcall[%s] failed[%d]" ,
                             lua_script, ret);
        return -1;
    }

    /* lua function invoke */
    if (gp_luacall_va(L, "init",">i", &res) < 0) {
        return -1;
    }

    return 0;
}

int gp_do_action(lua_State *L, const char * svcname, int *first)
{
    char       lua_script[1024];
    int        ret;


    if (*first) {
        snprintf(lua_script, sizeof(lua_script), "%s/script/%s/action.lua",
             getenv("RUNNERDIR"), svcname);
        if ((ret=luaL_loadfile(L, lua_script))!=0) {
            gp_errlog(LVL_ERROR, _FL_, "luaL_loadFile[%s] error [%d]",
                     lua_script, ret);
            return -1;
        }
        *first = 0;
        ret = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (ret) {
            gp_errlog(LVL_ERROR, _FL_, "lua_pcall[%s] failed[%d]" ,
                                 lua_script, ret);
            return -1;
        }
    }

    if (gp_luacall_va(L, "action",">i", &ret) < 0) {
        return -1;
    }
    return 0;
}


void gp_do_term(const char * svcname)
{
    return ;
}



/*
 *  uid 代表的是虚拟用户编号
 */
void *gp_runner_loop(void * args, int uid)
{
    struct timeb            begin;
    struct timeb            end;
    int                     ret;
    int                     success = 0;
    SERVICES              * service;
    lua_State             * L = NULL;
    redisContext          * redis_context = NULL;
    int                     first = 1;
    int                     thinktime = 0;
    int                     idx;
    int                     elapse;

    if (__runner_info__.think_time > 0)  {
       thinktime = __runner_info__.think_time * 1000;
    }
    
    service = (SERVICES *) args;

    L = gp_init_lua(&__runner_info__);
    if (L == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "lua_open failed");
        return NULL;
    }

    redis_context = gp_connect_redis(&__runner_info__);
    if (redis_context == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "gp_init_redis error");
        return NULL;
    }

    /*  set global variable __SVCNAME  */
    gp_luaset_glb_variable(L, "__SVCNAME", PR_TYPE_STRING, service->svcname);
    gp_luaset_glb_variable(L, "__VUID", PR_TYPE_INT, &uid);

    ret = gp_do_init(L, service->svcname);
    if (ret < 0) {
        gp_errlog(LVL_ERROR, _FL_, "gp_do_init error");
        return NULL;
    }

    while (!__bexit__) {
        /*  
         *  set global lua variable __SVC_RESULT , 
         *  script must set property value 
         */
        success = 0;
        gp_luaset_glb_variable(L, "__SVC_RESULT", PR_TYPE_BOOLEAN, &success);
        gp_luaset_glb_variable(L, "__VUID", PR_TYPE_INT, &uid);

        ftime(&begin);
        ret = gp_do_action(L, service->svcname, &first);   
        ftime(&end);
        if (ret < 0) {
            gp_errlog(LVL_ERROR, _FL_, "gp_do_action failed");
            success = 0;
            gp_redis_set_total(redis_context, __runner_info__.tc_name, success);
            goto THINKTIME;
        }

        gp_luaget_glb_var_value(L, "__SVC_RESULT", PR_TYPE_BOOLEAN, &success);
        ret = gp_redis_set_total(redis_context, __runner_info__.tc_name, success);
        if (ret < 0) {
            gp_errlog(LVL_ERROR, _FL_, "gp_redis_set_total failed");
            goto THINKTIME;
        }

        idx = (end.time - __epoc_time.time) / 60 ;
        elapse = (end.time - begin.time) * 1000 + 
                         end.millitm - begin.millitm;
        gp_redis_set_tpm(redis_context, __runner_info__.tc_name,
                             service->svcname, idx, elapse, success);
THINKTIME:
        if (thinktime) {
            gp_udelay(thinktime);
        }
    }

    lua_close(L);
    gp_do_term(service->svcname);
    return NULL;
}

void *gp_runner_loop_thread(void * args)
{
    THREAD_ARGS      * th_args;
    th_args = (THREAD_ARGS *) args;
    return gp_runner_loop(th_args->p_runner , th_args->uid);
}


static void gp_do_scenario_test(char *u_test_svc, int looptime, 
            char *scenario, int verbose)
{
    char            cfgpath[256];
    int             ret;
    int             i , j;
    int             success = 0;
    lua_State     * L;
    int             first = 1;
    struct timeb    begin;
    struct timeb    end;

    if (scenario[0] != '/' && scenario[0] != '.') {
        snprintf(cfgpath, sizeof(cfgpath), "%s/scenario/%s", getenv("RUNNERDIR"),
                        scenario);
        ret = gp_load_from_cfg(cfgpath, &__runner_info__);
        if (ret < 0) {
            fprintf(stdout, "load [%s] error\n", cfgpath);
            return ;
        }
    } else {
        ret = gp_load_from_cfg(scenario, &__runner_info__);
        if (ret < 0) {
           fprintf(stdout, "load [%s] error\n", scenario);
           return ;
        }
    }

    if (verbose) {
        gp_show_version();
        gp_print_cfg(&__runner_info__);
    }


    ret = gp_init_sequence(&__runner_info__);
    if (ret < 0) {
        fprintf(stdout, "gp_init_sequence failed, check log");
        return ;
    }

    L = gp_init_lua(&__runner_info__);                                          
    if (L == NULL) {                                                            
        fprintf(stdout, "init lua enviorment error\n");
        return ;                                                            
    }


    for (i = 0; i < __runner_info__.service_num; ++i) {
        if (!strcmp(__runner_info__.services[i].svcname, u_test_svc)) {
            break;
        }
    }

    if (i == __runner_info__.service_num) {
        fprintf(stdout, "test service name [%s] can't be found\n",
                       u_test_svc);
        lua_close(L);
        return ;
    }

    /*  set global variable __SVCNAME  */
    gp_luaset_glb_variable(L, "__SVCNAME", PR_TYPE_STRING, 
                    __runner_info__.services[i].svcname);

    ret = gp_do_init(L, __runner_info__.services[i].svcname);
    if (ret < 0) {
        fprintf(stdout, "do svcname [%s] init.lua failed\n",
                    __runner_info__.services[i].svcname);

        lua_close(L);
        return; 
    }

    success = 0;
    gp_luaset_glb_variable(L, "__SVC_RESULT", PR_TYPE_BOOLEAN, &success);

    for (j = 0; j < looptime ; ++j) {
        int uid = 0;
        gp_luaset_glb_variable(L, "__VUID", PR_TYPE_INT, &uid);
        ftime(&begin);
        ret = gp_do_action(L, __runner_info__.services[i].svcname, &first);   
        ftime(&end);
        if (ret < 0) {
            fprintf(stdout, "do svcname [%s] action.lua failed\n",
                        __runner_info__.services[i].svcname);
            success = 0;
            lua_close(L);
            return ;
        }

        gp_luaget_glb_var_value(L, "__SVC_RESULT", PR_TYPE_BOOLEAN, &success);
        if (!success) {
            fprintf(stdout, "do svcname [%s] action.lua application error\n",
                        __runner_info__.services[i].svcname);
        }
    
        fprintf(stdout, "service [%s] totoal elapse [%ld] millseconds\n",
                        __runner_info__.services[i].svcname,
                        (end.time - begin.time) * 1000 + 
                        end.millitm - begin.millitm);
    }
    lua_close(L);
    return ;

}

static void  gp_boot_vuser(RUNNERCFG_INFO * runner_info, VUSERS * vuser)
{
    int        i = 0;
    int        j = 0;
    int        k = 0;
    int        ret;
    THREAD_ARGS    * g;

    i = 0;
    if (runner_info->run_mode == PR_RUN_MODE_THREAD) {
        for (j = 0; j < runner_info->service_num ; ++j) {
            for (k = 0; k < runner_info->services[j].vusers; k ++) {
                /*alloc , no need free it */
                g = malloc(sizeof(THREAD_ARGS));
                if (g == NULL) {
                    gp_errlog(LVL_ERROR, _FL_, "create v user failed"
                        " alloc [%d] byte error[%s]", sizeof(THREAD_ARGS),
                        strerror(errno));
                    continue;
                }
                g->uid = k;
                g->p_runner = &runner_info->services[j];
                ret = pthread_create(&vuser->user[i].tid, NULL,
                                     gp_runner_loop_thread, g);
                if (ret < 0) {
                    fprintf(stderr, "virtual user created failed[%s]",
                            strerror(ret));
                    exit(1);
                }
                i++ ;
            }
        }
    } else {
        for (j = 0; j < runner_info->service_num ; ++j) {
            for (k = 0; k < runner_info->services[j].vusers; k ++) {
                ret = fork();
                switch(ret) {
                    case -1:
                    fprintf(stdout, "fork child process failed[%s]",
                                strerror(errno));
                    exit(1);
                    break;
                    case 0:
                        gp_runner_loop(&runner_info->services[j], k);
                        exit(0);
                    break;
                    default:
                        vuser->user[i].pid = ret;
                    break;
                }
                i++ ;
            }
        }
    }
}


void gp_do_yeild(RUNNERCFG_INFO * runner, VUSERS *vusers)
{
    int            i;
    int            status;
    long           sleep_time;
    struct timeb   now;
    int            ret;

    sleep_time =  (__runner_info__.time_elapse * 60);

    if (runner->run_mode == PR_RUN_MODE_THREAD) {
        while (!__bexit__) {
            sleep(60);
            ftime(&now);
            if ((now.time - __epoc_time.time) > sleep_time) {
                break;
            }

            for (i = 0; i < runner->vuser_num; ++i) {
                if (vusers->user[i].tid != -1) {
                    ret = pthread_kill(vusers->user[i].tid, 0);
                    if (ret) {
                        gp_errlog(LVL_ERROR, _FL_, "vuser are not exits[%s]",
                                strerror(ret));
                        pthread_join(vusers->user[i].tid, NULL);
                        vusers->user[i].tid = -1;
                    }
                }
            }
        }

        __bexit__ = 1;
        for (i = 0; i < runner->vuser_num; i++) {
            if (vusers->user[i].tid != -1) {
                pthread_join(vusers->user[i].tid, NULL);
            }
        }
    } else {
        while (!__bexit__) {
            sleep(60);
            ftime(&now);
            if ((now.time - __epoc_time.time) > sleep_time) {
                break;
            }

            for (i = 0; i < runner->vuser_num; ++i) {
                if (vusers->user[i].pid != -1) {
                    ret = kill(vusers->user[i].pid, 0);
                    if (ret) {
                        gp_errlog(LVL_ERROR, _FL_, "vuser are not exits[%s]",
                                strerror(ret));
                        waitpid(vusers->user[i].pid, &status, WNOHANG);
                        vusers->user[i].pid = -1;
                    }
                }
            }
        }

        for (i = 0; i < runner->vuser_num; ++i) {
            if (vusers->user[i].pid != -1) {
                kill(vusers->user[i].pid, SIGTERM);
                gp_udelay(1000*200);
                waitpid(vusers->user[i].pid, &status, WNOHANG);
            }
        }


        for (i = 0; i < runner->vuser_num; ++i) {
            if (vusers->user[i].pid != -1) {
                if (kill(vusers->user[i].pid, 0) == -1) {
                    /*  child process are terminated!! */
                    vusers->user[i].pid = -1;
                }else {
                    kill(vusers->user[i].pid, SIGKILL);
                    gp_udelay(1000*200);
                    waitpid(vusers->user[i].pid, &status, WNOHANG);
                    vusers->user[i].pid = -1;
                }
            }
        }

        fprintf(stdout, "all vuser are cleaned !!!!!! ");
        gp_errlog(LVL_FREE, _FL_, "all virtual user are terminated!!");

    }
}
int main(int argc , char ** argv)
{
    int                    ret;
    struct sigaction       act;
    char                   cfgpath[256];
    char                   u_test_svc[256];
    char                   ch;
    int                    looptime = 1;
    int                    verbose = 0;
    int                    background = 0;


    if (argc < 2) {
        gp_usage();
        exit(1);
    }

    u_test_svc[0] = 0;
    while((ch = getopt(argc, argv, "vdl:u:")) != (char) -1) {
        switch(ch) {
        case 'u':
            strcpy(u_test_svc, optarg);
            break;
        case 'l':
            looptime = atoi(optarg); 
            break;
        case 'v':
            verbose = 1;
            break;
        case 'd':
            background = 1;
            break;
        default:
            gp_usage();
            exit(1);
        }
    }

    if (u_test_svc[0] != 0) {
        gp_do_scenario_test(u_test_svc, looptime, argv[optind], verbose);
        exit(1);
    }

    if (optind == argc) {
        if (verbose) {
            gp_show_version();
        }
        gp_usage();
        exit(0);
    }

    if (argv[optind][0] != '/' && argv[optind][0] != '.') {
        snprintf(cfgpath, sizeof(cfgpath), "%s/scenario/%s", getenv("RUNNERDIR"),
                        argv[optind]);
        ret = gp_load_from_cfg(cfgpath, &__runner_info__);
    }else {
        ret = gp_load_from_cfg(argv[optind], &__runner_info__);
    }

    if (__runner_info__.sequence_num > 0) {
        ret = gp_init_sequence(&__runner_info__);
        if (ret < 0) {
            fprintf(stdout, "init sequence error !!!!,"
                            " check log for more information");
            exit(1);
        }
        
    }

    if (ret < 0) {
        fprintf(stdout, "load cfg [%s] error\n", argv[1]);
        exit(1);
    }

    if (verbose) {
        gp_show_version();
        gp_print_cfg(&__runner_info__);
    }

    fprintf(stdout, "test will begin .....\n");
    if (background) {
        gp_background();
    }

    gp_set_log_level(__runner_info__.loglevel);

    ret = gp_redis_init(&__runner_info__);
    if (ret < 0) {
        fprintf(stdout, "初始化redis失败，请检查"
                        "redis是否已经启动，并核对链接参数\n");
        gp_errlog(LVL_ERROR, _FL_, "run_mode [%d], "
              "redis addr[%s:%d], error[%s]",
              __runner_info__.redis_comm_mode, __runner_info__.redis_addr,
              __runner_info__.redis_port, strerror(errno));
        exit(1);
    }


    fprintf(stdout, "create vitual user ....\n");

    
    __vusers__.user = calloc(__runner_info__.vuser_num, sizeof(VUSER));
    if (__vusers__.user == NULL) {
        fprintf(stderr, "alloc [%ud] byte error [%s]" 
                      , __runner_info__.vuser_num * sizeof(VUSER)
                      , strerror(errno));
        exit(1);
    }


    gp_sig_blockall();
    gp_sig_unblock(SIGTERM);
    gp_sig_unblock(SIGCHLD);


    ftime(&__epoc_time);
    gp_boot_vuser(&__runner_info__, &__vusers__);


    gp_sig_unblock(SIGTERM);
    sigemptyset( &act.sa_mask );
    sigaddset( &act.sa_mask, SIGTERM );
    act.sa_handler = gp_termsig_handler;
    act.sa_flags = 0;
    sigaction(SIGTERM, &act, NULL);  

    gp_do_yeild(&__runner_info__, &__vusers__);
    exit(0);
    return 0;
}
