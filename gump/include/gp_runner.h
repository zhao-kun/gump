#ifndef PAY_RUNNER_H
#define PAY_RUNNER_H


#ifdef __cplusplus 
extern C {
#endif 

#define PR_RUN_MODE_THREAD        1
#define PR_RUN_MODE_PROCESS       2

#define REDIS_COMM_MODE_TCP       0
#define REDIS_COMM_MODE_UNIX      1

typedef  struct {
    char  svcname[128];
    int   vusers;
}SERVICES;

typedef struct {
    union {
        pid_t       pid;
        pthread_t   tid;
    };
    int   status;
}VUSER;

typedef struct {
    VUSER       * user;     
    int           num_num;
}VUSERS;


typedef struct {
    char   dll_name[256];
    char   func_name[128];
}PLUGINS;

typedef struct {
    char  seq_idx[128];
    int   default_seq;
}SEQUENCE;

typedef struct {
    unsigned long  time_elapse;
    char           tc_name[256];
    SERVICES     * services;
    PLUGINS      * plugins;
    int            service_num;
    int            plugin_num;
    int            vuser_num;
    int            think_time;
    char           redis_addr[256];
    short          redis_port;
    int            loglevel;
    char           report_dir[512];
    int            run_mode;
    int            redis_comm_mode;
    int            sequence_num;
    SEQUENCE     * sequence;
}RUNNERCFG_INFO;


int  gp_load_from_cfg(const char * cfg_name, RUNNERCFG_INFO  * info);

#ifdef __cplusplus
}
#endif

#endif
