#include <wrapunix.h>
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>


int tcp_send_nbytes_tm(int sock, void *buf, int len, int time_out);
int tcp_recv_nbytes(int sock, void *buf, int len, int time_out);

extern int gp_file_waitlock(char *);
extern int gp_file_unlock(int);
extern char * gp_ltrim(char *);
extern char * gp_rtrim(char *);
static int l_sleep(lua_State *L)
{
    int d = lua_tonumber(L, 1);
    sleep(d);
    return 0;
}

static int l_usleep(lua_State *L)
{
    struct timeval timeval;
    int d = lua_tonumber(L, 1);
    timeval.tv_usec = d;
    timeval.tv_sec = 0;
    select(0, NULL, NULL, NULL, &timeval);
    return 0;
}



static int l_loadfile(lua_State *L) 
{
    FILE        * fp = NULL;
    const char  * svcname = NULL;
    char          path[256];
    struct stat   st_stat;
    char        * p;
    int           readed = 0;
    int           ret;
    luaL_Buffer   res;
    const char  * file = NULL;

   
    file = luaL_checkstring(L, 1);
    if (file == NULL || file[0] == 0) {
        return luaL_error(L,  "file is null or empty");
    }

    if (file[0] !='/') {
        lua_getglobal(L, "__SVCNAME"); 
        svcname = lua_tostring(L, -1);
        snprintf(path, sizeof(path), "%s/script/%s/data/%s",
                       getenv("RUNNERDIR"),
                       svcname,
                       file);
        file = path;
    }

    fp = fopen(file, "r");
    if (fp == NULL) {
        return luaL_error(L, "file [%s] open failed[%s]",
            file, strerror(errno));
    }

    ret = stat(file, &st_stat);
    if (ret) {
        fclose(fp);
        return luaL_error(L, "file[%s] state failed[%s]", 
            file, strerror(errno));
    }

    p = calloc(1, st_stat.st_size + 1);
    if (p == NULL) {
        fclose(fp);
        return luaL_error(L, "calloc [%d] byte error [%s]",
                             st_stat.st_size, strerror(errno)); 
    }

    p[st_stat.st_size] = 0;

    while (1) {
        ret = fread(p + readed, 1, st_stat.st_size - readed, fp);
        if (ret < 0) {
            fclose(fp);
            free(p);
            return luaL_error(L, "read fp byte error [%s]",
                             strerror(errno)); 
        }
        readed += ret;
        if (readed == st_stat.st_size) 
            break;
    }


    luaL_buffinit(L, &res); 
    luaL_addlstring(&res, p, st_stat.st_size);
    luaL_pushresult(&res);
    free(p);
    fclose(fp);
    return 1; /* indicate number of return value */
}


/* int  sockopen(char * addr, short port); */
static int l_sockopen(lua_State *L)
{
    const char         * addr;
    int                  port;
    int                  sockfd;
    struct sockaddr_in   sockaddr;
    int                  ret;

    addr = luaL_checkstring(L, 1);
    if (addr == NULL || addr[0] == 0) {
        return luaL_error(L,  "1st argument address is null or empty");
    }

    port = lua_tointeger(L, 2);
    if (port < 0 || port >65535) {
        return luaL_error(L, "2nd argument must valid port [0 - 65535");
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        return luaL_error(L, "socket error [%s]", strerror(errno));
    }

    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = inet_addr(addr);
    sockaddr.sin_family = AF_INET;

    ret = connect(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
    if (ret < 0) {
        return luaL_error(L, "connect [%s:%d] error [%s]",
                           addr, port, strerror(errno));
    }

    lua_pushinteger(L, sockfd);
    return 1;
}

/* int  sockopen(int sock, contents, timeout); */
static int l_socksend(lua_State *L) 
{
    int                sockfd;
    const char  *      contents; 
    size_t             len;
    int                timeout;
    int                ret;

    sockfd = lua_tointeger(L, 1);
    if (sockfd < 0) {
        return luaL_error(L, "1st argument sockfd is't valid must be great then 0");
    }

    if (!lua_isstring(L, 2)) {
        return luaL_error(L, "2nd argument must be string ");
    }

    contents = lua_tolstring(L, 2, &len);
    if (contents == NULL || len == 0) {
        return luaL_error(L, "send content length is 0");
    }
    
    timeout = lua_tointeger(L, 3);
    ret = tcp_send_nbytes_tm(sockfd, (void *)contents, len, timeout);
    if (ret < 0) {
        return luaL_error(L, "tcp_send_nbyte_tm error sock[%d]"
                             "contents[%p] len [%d] , time[%d]",
                             sockfd, contents, len, timeout);
    }

    lua_pushinteger(L, ret);
    return 1;
}

/* lua_string  sockrecv(int sock, int len, int timeout); */
static int l_sockrecv(lua_State *L)
{
    int              sockfd;
    int              timeout;
    int              len;
    int              ret;
    u_char           buf[4096];
    u_char         * p = buf;

    sockfd = lua_tointeger(L, 1);
    if (sockfd < 0) {
        return luaL_error(L, "1st argument must greater than 0");
    }

    len = lua_tointeger(L, 2);
    if (len < 0) {
        return luaL_error(L, "2nd is length of data be received!");
    }

    timeout = lua_tointeger(L, 3);
    if (timeout < 0) {
        timeout = -1;
    }

    if (len > sizeof (buf)) {
        p = calloc(1, len);
        if (p == NULL) {
            return luaL_error(L, "alloc [%d] byte error[%s]",
                    len, strerror(errno));
        }
    }

    ret = tcp_recv_nbytes(sockfd, p, len, timeout);
    if (ret < 0) {
        if (p != buf) {
            free(p);
        }
        return luaL_error(L, "tcp_rec_nbyte error sockfd[%d"
                             "p [%p] , len[%d] timeout [%d]",
                             sockfd, p, len, timeout);
    }

    lua_pushlstring(L, (const char *)p, len);
    if (p != buf) {
        free(p);
    }
    return 1;
}

static int l_sockclose(lua_State *L) 
{
    int          sock;

    sock = lua_tointeger(L, 1);
    if (sock < 0) {
        return 0;
    }

    close(sock);
    return 0;
}


/* get_seq( seq_id, min_seq, max_seq, step_num) */
static int l_getseq(lua_State  *L)
{
    const char        * seq_idx;
    char                lock_file[512];
    char                seq_file[512];
    int                 lock;
    FILE              * fp = NULL;
    char                buf[64];
    int                 min_seq;
    int                 max_seq;
    int                 c;
    int                 n;
    int                 w;
    int                 step;

    seq_idx = lua_tostring(L, 1);
    if (seq_idx == NULL || seq_idx[0] == 0) {
        return luaL_error(L, "seq_idx is null or empty");
    }


    min_seq = lua_tointeger(L, 2);
    max_seq = lua_tointeger(L, 3);
    if (max_seq < min_seq) {
        return luaL_error(L, "get_seq(seq_idx, min_seq, max_seq, step) "
                             "max_seq value must greater then min_seq ");
    }

    step = lua_tointeger(L, 4);
    if (step == 0) {
        return luaL_error(L, "get_seq(seq_idx, min_seq, max_seq, step) "
                             "step can't be equal zero");
    }

    snprintf(seq_file, sizeof(seq_file), "%s/sequence/%s.seq",
            getenv("RUNNERDIR"), seq_idx);

    if (access(seq_file, W_OK|R_OK)) {
        return luaL_error(L, "seqence file [%s] are not exists !!!\n", seq_file);
    }

    snprintf(lock_file, sizeof(lock_file), "%s/sequence/.%s.lock",
            getenv("RUNNERDIR"), seq_idx);

    if (access(lock_file, R_OK)) {
        return luaL_error(L, "lock file [%s] are not exists !!!\n", lock_file);
    }

    lock = gp_file_waitlock(lock_file);
    if (lock < 0) {
        return luaL_error(L, "gp_file_waitlock failed");
    }

    sleep(1);
    fp = fopen(seq_file, "r+"); 
    if (fp == NULL) {
        gp_file_unlock(lock);
        return luaL_error(L, "open seq_file[%s] error [%s]",
                seq_file, strerror(errno));
    }

    fgets(buf, sizeof(buf), fp);
    c = atol(buf);
    n = c + step;
    w = n ;
               
    if (step > 0) {
        if (n < min_seq) {
            n = min_seq;
            w = min_seq + 1;
        } else if (n >= max_seq) {
            n = max_seq;
            w = min_seq;
        }
    }

    if (step < 0) {
        if (n <= min_seq) {
            n = min_seq;
            w = max_seq;
        } else if (n >= max_seq) {
            n = max_seq;
            w = min_seq;
        }
    }

    if (ftruncate(fileno(fp), 0)) {
        fclose(fp);
        gp_file_unlock(lock);
        return luaL_error(L, "truncate file error [%s]\n", strerror(errno));
    }

    fseek(fp,SEEK_SET,  0);
    fprintf(fp, "%d\n", w);
    fsync(fileno(fp));
    fclose(fp);
    gp_file_unlock(lock);
    lua_pushinteger(L, n);
    return 1;
}

/*  replacestr(str, replace_position, replace_contents ) */
static int l_replacestr(lua_State *L)
{
    size_t         len; 
    const char   * c;
    const char   * r;
    size_t         rlen;
    int            p = 0;
    char         * ok;
    
    c = lua_tolstring(L, 1, &len);
    if (c == NULL || c[0] == 0) {
        return luaL_error(L, "replaced str must be a str");
    }


    if (!lua_isnumber(L, 2)) {
        return luaL_error(L, "replace str second argument must be number");
    }
    p =  lua_tointeger(L, 2);
    if (p < 0) {
        return luaL_error(L, "replacestr, 2nd argument must greater then 0");
    }

    r = lua_tolstring(L, 3, &rlen);
    if (r == NULL || r[0] == 0) {
        return luaL_error(L, "replacestr , 3rd argument must be str ");
    }

    if ((p + rlen) > len) {
        return luaL_error(L, "replace str are too long [%d + %d] > %d",
                p, rlen, len);
    }

    ok = malloc(len);
    if (ok == NULL) {
        return luaL_error(L, "malloc [%d] byte error[%s]",
                len, strerror(errno));
    }

    memset(ok, 0, len);
    memcpy(ok, c, len);
    memcpy(&ok[p], r, rlen);
    lua_pushlstring(L, ok, len);
    free(ok);
    return 1;
}

static int l_loadstarringlog(lua_State *L)
{
    char         * str = NULL;
    const char   * file = NULL;
    const char   * svcname = NULL;
    char           path[512];
    char           buff[512];
    FILE         * fp;
    char         * p = NULL;
    int            capacity = 4096;
    int            readstat = 0 ,len = 0;
    char         * p1, *p2, *asc;

    file = luaL_checkstring(L, 1);
    if (file == NULL || file[0] == 0) {
        return luaL_error(L,  "file is null or empty");
    }

    if (file[0] !='/') {
        lua_getglobal(L, "__SVCNAME"); 
        svcname = lua_tostring(L, -1);
        snprintf(path, sizeof(path), "%s/script/%s/data/%s",
                       getenv("RUNNERDIR"),
                       svcname,
                       file);
        file = path;
    }

    fp = fopen(file, "r");
    if (fp == NULL) {
        return luaL_error(L, "fopen[%s] error [%s]", file, strerror(errno));
    }

    str = malloc(capacity);
    if (str == NULL) {
        return luaL_error(L, "malloc [%d] byte error [%s]",
                capacity, strerror(errno));
    }

    while(1) {
        p = fgets(buff, sizeof(buff), fp); 
        if (p == NULL) {
            break;
        }

        gp_ltrim(gp_rtrim(p));
        if (p == NULL || p[0] == 0) {
            continue;
        }

        switch(readstat) {
        case 0:
            if (p == NULL || p[0] == 0) 
                continue;
            if (strstr(p, "00000000") == NULL) {
                /* 所有其他字符都需要忽略 */
                continue;
            } else {
                readstat = 1;
            }
        case 1:
            if (p == NULL || p[0] == 0)
                continue;
            p1 = strstr(p, "<");
            if (p1 == NULL) {
                if (str)
                    free(str);
                return luaL_error(L, "starring log file is not validate <");
            }
            p1++;
            p2 = strstr(p, ">"); 
            if (p2 == NULL) {
                if (str)
                    free(str);
                return luaL_error(L, "starring log file is not validate >");
            }

            *p2 = '\0';
            gp_ltrim(gp_rtrim(p1));
            asc = strtok(p1, " ");
            if (asc != NULL) {
                str[len] = strtol(asc, NULL, 16);
                len ++;
                while ((asc = strtok(NULL, " "))!= NULL) {
                    str[len] = strtol(asc, NULL, 16);
                    len ++;
                    if (len >= capacity) {
                        capacity += 4096;
                        str = realloc(str, capacity);
                        if (!str)
                            return luaL_error(L, "realloc [%d] byte error[%s]",
                                capacity, strerror(errno));
                    }
                }
            }
            break;
        }
    }

    lua_pushlstring(L, str, len);
    free(str);
    return 1;
}

static int l_exist(lua_State *L)
{
    const char      *p = NULL;
    char             path[1024] ;
    const char      *svcname = NULL;    

    p = luaL_checkstring(L, 1);
    if (p == NULL || p[0] == 0) {
        return luaL_error(L, "exist first string can't be null or empty");
    }

    if (p[0] !='/') {
        lua_getglobal(L, "__SVCNAME"); 
        svcname = lua_tostring(L, -1);
        snprintf(path, sizeof(path), "%s/script/%s/data/%s",
                       getenv("RUNNERDIR"),
                       svcname,
                       p);
        p = path;
    } 

    if (access(p, R_OK) == 0) {
        lua_pushboolean(L, 1);
    }else {
        lua_pushboolean(L, 0);
    }

    return 1;
}

static int l_mkdir(lua_State *L)
{
    const char      *p = NULL;
    char             path[1024] ;
    const char      *svcname = NULL;    

    p = luaL_checkstring(L, 1);
    if (p == NULL || p[0] == 0) {
        return luaL_error(L, "exist first string can't be null or empty");
    }

    if (p[0] !='/') {
        lua_getglobal(L, "__SVCNAME"); 
        svcname = lua_tostring(L, -1);
        snprintf(path, sizeof(path), "%s/script/%s/data/%s",
                       getenv("RUNNERDIR"),
                       svcname,
                       p);
        p = path;
    } 

    mkdir(p, 0777);
    return 0;
}

static int l_unlink(lua_State *L)
{
    const char      *p = NULL;
    char             path[1024] ;
    const char      *svcname = NULL;    

    p = luaL_checkstring(L, 1);
    if (p == NULL || p[0] == 0) {
        return luaL_error(L, "exist first string can't be null or empty");
    }

    if (p[0] !='/') {
        lua_getglobal(L, "__SVCNAME"); 
        svcname = lua_tostring(L, -1);
        snprintf(path, sizeof(path), "%s/script/%s/data/%s",
                       getenv("RUNNERDIR"),
                       svcname,
                       p);
        p = path;
    } 

    unlink(p);
    return 1;
}
static const struct luaL_reg base_func [] = {
    {"sleep", l_sleep},
    {"loadfile", l_loadfile},
    {"sockopen", l_sockopen},
    {"socksend", l_socksend},
    {"sockrecv", l_sockrecv},
    {"sockclose", l_sockclose},
    {"getseq", l_getseq},
    {"replacestr", l_replacestr},
    {"loadstarringlog", l_loadstarringlog},
    {"usleep", l_usleep},
    {"exist", l_exist},
    {"mkdir", l_mkdir},
    {"unlink", l_unlink},
    {NULL, NULL}  /* sentinel */
};


int luaopen_baselib(lua_State *L) {
    luaL_openlib(L, "gb", base_func, 0);
    return 1;
}


