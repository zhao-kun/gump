#include <wrapunix.h>
#include <gp_runner.h>
#include <gp_util.h>
#include <redis/hiredis.h>

void gp_get_date_time(char *, char *);


redisContext * gp_connect_redis(RUNNERCFG_INFO * info)
{
    redisContext       * context;

    if (info->redis_comm_mode == REDIS_COMM_MODE_TCP) {
        context = redisConnect(info->redis_addr, info->redis_port);
        if (context && context->err) {
            gp_errlog(LVL_ERROR, _FL_, "connect redis ip[%s:%d] error [%s]" ,
                          info->redis_addr, info->redis_port,
                          context->errstr);
            redisFree(context);
            return NULL;
        }
     } else if (info->redis_comm_mode == REDIS_COMM_MODE_UNIX) {
         context = redisConnectUnix(info->redis_addr);
         if (context && context->err) {
             gp_errlog(LVL_ERROR, _FL_, "connect redis unix:[%s] error [%s]\n",
                    info->redis_addr, context->errstr);
                    redisFree(context);
             return NULL;
         }
     } else {
        context = NULL;
        gp_errlog(LVL_ERROR, _FL_, _FL_, "error redis_comm_mode[%d]",
            info->redis_comm_mode);
        return NULL;
    }

   return context;
}

int gp_redis_set_total(redisContext *context, const char * ns, int success)
{
    redisReply  * reply;
    char          total_cmd[512];
    char          total_sf_cmd[512];


    snprintf(total_cmd, sizeof(total_cmd), "INCRBY %s::TOTAL 1", ns);
    if (success) {
        snprintf(total_sf_cmd, sizeof(total_sf_cmd), "INCRBY %s::TOTAL_SUCC 1", ns);
    }else {
        snprintf(total_sf_cmd, sizeof(total_sf_cmd), "INCRBY %s::TOTAL_FAIL 1", ns);
    }

    reply = redisCommand(context, total_cmd);
    if (reply == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "command [%s] execute failed [%s]",
                           total_cmd, context->errstr); 
        return -1;
    }
    freeReplyObject(reply);

    reply = redisCommand(context, total_sf_cmd);
    if (reply == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "command [%s] execute failed [%s]",
                           total_sf_cmd, context->errstr); 
        return -1;
    }
    freeReplyObject(reply);
    return 0;

}

static 
void gb_redis_del_svckey(const char * ns, const char * cfg_tbnm, redisContext *c)
{
    char         r_cmd[1024];
    redisReply * r = NULL;
    redisReply * r1 = NULL;
    int          i;

    snprintf(r_cmd, sizeof(r_cmd), "hgetall %s", cfg_tbnm);
    r = redisCommand(c, r_cmd);
    if (r == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "redisCommand [%s] error[%s]",
                           r_cmd, c->errstr);
        return;
    }

    for (i = 0; i< r->elements; ++i) {
        if (strcmp(r->element[i]->str, "SERVICE_NUM") == 0 ||
            strcmp(r->element[i]->str, "STARTTIME") == 0 ) {
            continue;
        }
        snprintf(r_cmd, sizeof(r_cmd), "DEL %s::%s_TOTAL_CNT",
            ns, r->element[i]->str);
        r1 = redisCommand(c, r_cmd);
        if (r1 == NULL) {
            gp_errlog(LVL_ERROR, _FL_, "redisCommand [%s] error[%s]",
                           r_cmd, c->errstr);
        }else {
            freeReplyObject(r1);
        }

        snprintf(r_cmd, sizeof(r_cmd), "DEL %s::%s_TOTAL_TIME",
            ns, r->element[i]->str);
        r1 = redisCommand(c, r_cmd);
        if (r1 == NULL) {
            gp_errlog(LVL_ERROR, _FL_, "redisCommand [%s] error[%s]",
                           r_cmd, c->errstr);
        }else {
            freeReplyObject(r1);
        }
    }

    freeReplyObject(r);
    return ;
    
}

int gp_redis_init(RUNNERCFG_INFO * info)
{
    redisContext     * c = NULL; 
    redisReply       * r = NULL;
    char               cfg_tbnm[64];
    char               r_cmd[1024];
    int                i;
    char               dt_buf[64];

    c = gp_connect_redis(info);
    if (c == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "connect redis server [%s:%d] failed" ,
                             info->redis_addr, info->redis_port);
        goto error_handle;
    }

    snprintf(r_cmd, sizeof(r_cmd), "GET TC_NAME" );
    r = redisCommand(c, r_cmd);
    if (r != NULL) {
        if (r->str != NULL && r->str[0]) {
            snprintf(cfg_tbnm, sizeof(cfg_tbnm), "%s::GLOBAL_INFO", r->str);
            /*  delete all tps , art key  */
            gb_redis_del_svckey(r->str, cfg_tbnm, c);        
            freeReplyObject(r);
            snprintf(r_cmd, sizeof(r_cmd), "DEL %s", cfg_tbnm); 
            r = redisCommand(c, r_cmd);
        }
        if (r != NULL)
            freeReplyObject(r);
    }

    snprintf(r_cmd , sizeof(r_cmd), "SET TC_NAME %s", info->tc_name);
    r = redisCommand(c, r_cmd);
    if (r == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "command [%s] error[%s]",
                          r_cmd, c->errstr);
        goto error_handle;
    }

    snprintf(cfg_tbnm, sizeof(cfg_tbnm), "%s::GLOBAL_INFO", info->tc_name); 

    r_cmd[0] = 0;
    snprintf(r_cmd, sizeof(r_cmd), "HSET %s SERVICE_NUM %d", cfg_tbnm ,
                    info->service_num);
    r = redisCommand(c, r_cmd);
    if (r == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "command [%s] error [%s]", r_cmd ,
                             c->errstr);
        goto error_handle;
    }
    freeReplyObject(r);

    for (i = 0; i < info->service_num; i++) {
        /*   set service info */
        snprintf(r_cmd, sizeof(r_cmd), "HSET %s %s %d", cfg_tbnm ,
                        info->services[i].svcname, info->services[i].vusers);
        r = redisCommand(c, r_cmd);
        if (r == NULL) {
            gp_errlog(LVL_ERROR, _FL_, "command [%s] error [%s]", r_cmd ,
                             c->errstr);
            goto error_handle;
        }
        freeReplyObject(r);

        /*  clear service total cnt table by per minutes */
        snprintf(r_cmd, sizeof(r_cmd), "DEL %s::%s_TOTAL_CNT", info->tc_name,  
                        info->services[i].svcname);
        r = redisCommand(c, r_cmd);
        if (r == NULL) {
            gp_errlog(LVL_ERROR, _FL_, "command [%s] error [%s]", r_cmd ,
                             c->errstr);
            goto error_handle;
        }
        freeReplyObject(r);

        /*  clear total time table by per minutes */
        snprintf(r_cmd, sizeof(r_cmd), "DEL %s::%s_TOTAL_TIME", info->tc_name,  
                        info->services[i].svcname);
        r = redisCommand(c, r_cmd);
        if (r == NULL) {
            gp_errlog(LVL_ERROR, _FL_, "command [%s] error [%s]", r_cmd ,
                             c->errstr);
            goto error_handle;
        }
        freeReplyObject(r);
    }

    snprintf(r_cmd, sizeof(r_cmd), "SET %s::TOTAL 0", info->tc_name);
    r = redisCommand(c, r_cmd);
    if (r == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "command [%s] error [%s]", r_cmd ,
                             c->errstr);
        goto error_handle;
    }
    freeReplyObject(r);

    snprintf(r_cmd, sizeof(r_cmd), "SET %s::TOTAL_SUCC 0", info->tc_name);
    r = redisCommand(c, r_cmd);
    if (r == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "command [%s] error [%s]", r_cmd ,
                             c->errstr);
        goto error_handle;
    }
    freeReplyObject(r);

    snprintf(r_cmd, sizeof(r_cmd), "SET %s::TOTAL_FAIL 0", info->tc_name);
    r = redisCommand(c, r_cmd);
    if (r == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "command [%s] error [%s]", r_cmd ,
                             c->errstr);
        goto error_handle;
    }
    freeReplyObject(r);

    gp_get_date_time("%y%m%d-%h%n%s", dt_buf);
    snprintf(r_cmd, sizeof(r_cmd), "HSET %s STARTTIME %s", cfg_tbnm, dt_buf);
    gp_errlog(LVL_DEBUG, _FL_, "r_cmd[%s]", r_cmd);
    r = redisCommand(c, r_cmd);
    if (r == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "command [%s] error [%s]", r_cmd ,
                             c->errstr);
        goto error_handle;
    }

    freeReplyObject(r);
    if (c) {
        redisFree(c);
        c = NULL;
    }
 
    return 0;

error_handle:
    if (r) {
        freeReplyObject(r);
        r = NULL;
    }

    if (c) {
        redisFree(c);
        c = NULL;
    }
    return -1;
}

int gp_redis_set_tpm(redisContext * c, const char * ns,
                     const char * tb_name, int key, int elapse, int succ)
{
    char             htable[64];
    char             cnt_cmd[1024];
    char             times_cmd[1024];
    redisReply     * r = NULL;

    snprintf(htable, sizeof(htable), "%s::%s_TOTAL_CNT", ns, tb_name);
    if (succ)
        snprintf(cnt_cmd, sizeof(cnt_cmd), "HINCRBY %s %05d 1", htable, key);
    else
        snprintf(cnt_cmd, sizeof(cnt_cmd), "HINCRBY %s %05d 0", htable, key);

    snprintf(htable, sizeof(htable), "%s::%s_TOTAL_TIME", ns, tb_name);
    if (succ)
        snprintf(times_cmd, sizeof(times_cmd), "HINCRBY %s %05d %d",
                      htable, key, elapse);
    else 
        snprintf(times_cmd, sizeof(times_cmd), "HINCRBY %s %05d %d",
                      htable, key, 0);


    r = redisCommand(c, cnt_cmd); 
    if (r == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "execute [%s] cmd failed[%s]",
                           cnt_cmd , c->errstr);
        return -1;
    }
    freeReplyObject(r);

    r = redisCommand(c, times_cmd);
    if (r == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "execute [%s] cmd failed[%s]",
                           cnt_cmd , c->errstr);
        return -1;
    }
    freeReplyObject(r);
    return 0;
}

