#include <wrapunix.h>
#include <gp_runner.h>
#include <gp_util.h>

#define IS_SPACE(ch)      ((ch) == 0 || (ch) == ' ' || (ch) =='\t')


static void  gp_init_cfg(RUNNERCFG_INFO * info)
{
    memset(info->tc_name, 0x0, sizeof(info->tc_name));
    info->service_num = 0;
    memset(info->redis_addr, 0x0, sizeof(info->redis_addr));
    info->services = NULL;
    info->redis_port = 0;
    info->time_elapse = 60;
    info->vuser_num = 0;
    info->think_time = 0;
    info->run_mode = PR_RUN_MODE_THREAD ;
    info->redis_comm_mode = REDIS_COMM_MODE_TCP;
}

void gp_print_cfg(RUNNERCFG_INFO * info)
{
    int          i;

    fprintf(stdout, "info->redis_comm_mode[%d]\n", info->redis_comm_mode);
    fprintf(stdout, "info->report_dir[%s]\n", info->report_dir);
    fprintf(stdout, "info->redis_addr[%s]\n",info->redis_addr);
    fprintf(stdout, "info->redis_port[%d]\n", info->redis_port);
    fprintf(stdout, "info->loglevel[%d]\n",info->loglevel);
    fprintf(stdout, "info->test-case[%s]\n", info->tc_name);
    fprintf(stdout, "info->time_elapse[%lu]\n", info->time_elapse);
    fprintf(stdout, "info->vuser_num[%d]\n", info->vuser_num);
    fprintf(stdout, "info->think_time[%d]\n", info->think_time);
    fprintf(stdout, "info->service_num[%d]\n", info->service_num);
    fprintf(stdout, "info->run_mode[%d]\n", info->run_mode);
    fprintf(stdout, "service info:\n");
    for (i = 0; i < info->service_num; i++) {
        /* code */
        fprintf(stdout, "\tinfo->services[%d].svcname[%s] : %d\n"
                      ,i, info->services[i].svcname, info->services[i].vusers);
    }


    fprintf(stdout, "plugin info:\n");
    for (i = 0; i < info->plugin_num; i++) {
        /* code */
        fprintf(stdout, "\tinfo->plugins[%d].dll_name[%s] [%s]\n",
                      i, info->plugins[i].dll_name,
                      info->plugins[i].func_name);
    }
    
    fprintf(stdout, "sequence info:\n");
    for (i = 0; i < info->sequence_num; i++) {
        /* code */
        fprintf(stdout, "\tinfo->seqence[%d].seq_idx[%s]  default[%d]\n",
                      i, info->sequence[i].seq_idx,
                      info->sequence[i].default_seq);
    }
 }

#define VIRTUAL_USER 1
#define PLUGINS_STATUS 2
#define SEQUENCE_STATUS 3
#define NORMAL 0

int  gp_load_from_cfg(const char * cfg_name, RUNNERCFG_INFO  * info)
{
    FILE        * fp = NULL;
    char          buf[4096];
    char        * p = buf;
    char        * p1 = NULL;
    char        * p2 = NULL;
    char        * token = NULL;
    int           i = 0;
    int           status = NORMAL;
    int           raito = 0;

    if (cfg_name == NULL || cfg_name[0] == 0) {
        gp_errlog(LVL_ERROR, _FL_, "cfg is null or empty");
        return -1;
    }

    if (info == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "info is null or empty");
        return -1;
    }

    gp_init_cfg(info);

    fp = fopen(cfg_name, "r");
    if (fp == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "fopen [%s] failed [%s]",
                cfg_name, strerror(errno));
        return -1;        
    }


    while (1) {
        memset(buf, 0x0, sizeof(buf));
        p = fgets(buf, sizeof(buf), fp);
        if (p == NULL) {
            break;
        }

        gp_rtrim(p);

        if (p[0] == '#' || p[0] == 0)  {
            if (status != NORMAL) {
                status = NORMAL;
            }
            continue;
        }


        p1 = gp_strchr(p, '#');
        if (p1 != NULL) {
            p1[0] = 0;
        }

        if ( status == VIRTUAL_USER) {
            if (IS_SPACE(p[0])) {
                gp_ltrim(p);
                p1 = gp_strchr(p, ':');
                if (p1 != NULL) {
                    p1[0] = 0;
                    p1++;
                    raito = atol(p1);
                }else {
                    raito = 1; 
                }
                if (info->service_num == 0) {
                    info->services = calloc(1, sizeof(SERVICES));
                } else {
                    info->services = realloc(info->services,
                          sizeof(SERVICES) * (info->service_num + 1));
                }

                if (info->services == NULL) {
                    gp_errlog(LVL_ERROR, _FL_, "alloc [%d] byte error [%s]"
                               , (info->service_num + 1) * sizeof(SERVICES)
                               , strerror(errno));
                    return  -1;
                }

                info->services[info->service_num].vusers = raito;
                snprintf(info->services[info->service_num].svcname, 
                         sizeof(info->services[info->service_num].svcname),
                         "%s", p);
                info->service_num ++;
                info->vuser_num += raito;
                continue;
            } else {
                status = NORMAL;
                continue;
            }
        } else if (status == PLUGINS_STATUS) {
            if (IS_SPACE(p[0])) {
                gp_ltrim(p);
                token = strtok(p, " \t");
                if (token == NULL) {
                    continue;
                }

                if (info->plugin_num == 0) {
                    info->plugins = calloc(1, sizeof(PLUGINS));
                } else {
                    info->plugins = realloc(info->plugins,
                          sizeof(PLUGINS) * (info->plugin_num + 1));
                }

                if (info->plugins == NULL) {
                    gp_errlog(LVL_ERROR, _FL_, "alloc [%d] byte error [%s]"
                               , (info->plugin_num + 1) * sizeof(PLUGINS)
                               , strerror(errno));
                    return  -1;
                }

                snprintf(info->plugins[info->plugin_num].dll_name,
                         sizeof(info->plugins[info->plugin_num].dll_name),
                         "%s", token);
                p = strtok(NULL, " \t");
                if (p != NULL) {
                    snprintf(info->plugins[info->plugin_num].func_name,
                             sizeof(info->plugins[info->plugin_num].func_name),
                             "%s", p);
                } else {
                    info->plugins[info->plugin_num].func_name[0] = 0;
                }
                info->plugin_num ++ ;
                continue;
            } else {
                status = NORMAL;
                continue;
            }
        } else if (status == SEQUENCE_STATUS) {
            if (IS_SPACE(p[0])) {
                gp_ltrim(p);
                token = strtok(p, " \t");
                if (token == NULL) {
                    continue;
                }

                if (info->sequence_num == 0) {
                    info->sequence = calloc(1, sizeof(SEQUENCE));
                } else {
                    info->sequence = realloc(info->sequence,
                          sizeof(SEQUENCE) * (info->sequence_num + 1));
                }

                if (info->sequence == NULL) {
                    gp_errlog(LVL_ERROR, _FL_, "alloc [%d] byte error [%s]"
                               , (info->sequence_num + 1) * sizeof(SEQUENCE)
                               , strerror(errno));
                    return  -1;
                }

                snprintf(info->sequence[info->sequence_num].seq_idx,
                         sizeof(info->sequence[info->sequence_num].seq_idx),
                         "%s", token);
                p = strtok(NULL, " \t");
                if (p != NULL) {
                    info->sequence[info->sequence_num].default_seq = atoi(p);
                } else {
                    info->sequence[info->sequence_num].default_seq = 0;
                }
                info->sequence_num ++ ;
                continue;
            } else {
                status = NORMAL;
                continue;
            }
        } 

        gp_ltrim(p);


        token = strtok(p, " \t");
        if (strcmp(token, "time-elapse") == 0) {
            p2 = strtok(NULL, " \t");
            if (p2 == NULL) {
                continue;
            }
            gp_ltrim(gp_rtrim(p2));
            info->time_elapse = atol(p2);
        } else if (strcmp(token, "test-case") == 0) {
            p2 = strtok(NULL, " \t");
            if (p2 == NULL) {
                continue;
            }
            gp_ltrim(gp_rtrim(p2));
            strcpy(info->tc_name, p2);
        } else if (strcmp(token, "virtual-user") == 0) {
            status = VIRTUAL_USER;
            continue;
        } else if (strcmp(token, "sequence") == 0) {
            status = SEQUENCE_STATUS;
            continue;
        } else if (strcmp(token, "think-time") == 0) {
            p2 = strtok(NULL, " \t");
            if (p2 == NULL) {
                continue;
            }
            gp_ltrim(gp_rtrim(p2));
            info->think_time = atol(p2);
        } else if (strcmp(token, "redis-server") == 0) {
            char          * p3 = NULL;
            p2 = strtok(NULL, " \t");
            if (p2 == NULL) {
                continue;
            }
            gp_ltrim(gp_rtrim(p2));

            p3 = strstr(p2, "://");
            if (p3 == NULL) {
                p1 = strstr(p2, ":");
                if (p1 == NULL) {
                    info->redis_port = 6379;
                }else {
                    p1[0] = 0;
                    p1 ++;
                    info->redis_port = atoi(p1);
                }
                strcpy(info->redis_addr, p2);
            } else {
                p3[0] = 0;
                if (strcmp(p2, "unix") == 0 || strcmp(p2, "unix") == 0) {
                    p3 += 3;
                    strcpy(info->redis_addr, p3);
                    info->redis_comm_mode = REDIS_COMM_MODE_UNIX;
                } else {
                    p1 = strstr(p2, ":");
                    if (p1 == NULL) {
                        info->redis_port = 6379;
                    }else {
                        p1[0] = 0;
                        p1 ++;
                        info->redis_port = atoi(p1);
                    }
                    strcpy(info->redis_addr, p2);
                }
            }
        } else if (strcmp(token, "log-level") == 0) {
            p2 = strtok(NULL, " \t");
            if (p2 == NULL) {
                continue;
            }
            gp_ltrim(gp_rtrim(p2));
            info->loglevel = atol(p2);
        } else if (strcmp(token, "report-dir") == 0) {
            p2 = strtok(NULL, " \t");
            if (p2 == NULL) {
                continue;
            }
            gp_ltrim(gp_rtrim(p2));
            strcpy(info->report_dir, p2);
        } else if (strcmp(token, "comm-file") == 0) {
            int         raito = 0;
            for (i = 0; ; i ++) {
                p2 = strtok(NULL, " \t"); 
                if (p2 == NULL)
                    break;
                gp_ltrim(gp_rtrim(p2));
                p1 = strstr(p2, ":"); 
                if (p1 == NULL) {
                    raito = 0;
                }else {
                    p1[0] = 0;
                    p1++;
                    raito = atol(p1);
                }

                if (i == 0) {
                    info->services = malloc(sizeof(SERVICES));
                }else{
                    info->services = realloc(info->services,
                                        sizeof(SERVICES) * (i+1) );
                }

                info->services[i].vusers = raito;
                strcpy(info->services[i].svcname, p2);
            }

            info->service_num = i;
        } else if (strcmp(token, "run-mode") == 0) {
            p2 = strtok(NULL, " \t");
            if (p2 == NULL) {
                continue;
            }
            gp_ltrim(gp_rtrim(p2));
            if (strcmp(p2, "PROCESS") == 0) {
                info->run_mode = PR_RUN_MODE_PROCESS;
            }
        } else if (strcmp(token, "plugin") == 0) {
            status = PLUGINS_STATUS;
        }
    }

    if (fp != NULL)
        fclose(fp);
    return 0;
}

