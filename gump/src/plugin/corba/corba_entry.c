#include <wrapunix.h>
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
#include "metabus/mp4c.h"


void * corba_call_impl(const char * , const char * , char * );
void corba_object_free(void *);
void * corba_unpack_wrap(const char *);
void * corba_call_impl2(const char * corbaname, void * in , char * errstr);

static int corba_call2(lua_State *L) 
{
    char           errstr[1024];
    const char   * name;  
    void         * pkg;
    void         * result;

    name = lua_tostring(L, 1);
    if (name == NULL || name[0] == 0) {
        return luaL_error(L, "corba svr name is null or empty");
    }

    pkg = lua_touserdata(L, 2);
    if (pkg == NULL) {
        return luaL_error(L, "pkg is null or empty");
    }

    result = corba_call_impl2(name, pkg, errstr);
    if (result == NULL) {
        return luaL_error(L, "corba_call_impl call error[%s]", errstr);
    }

    lua_pushlightuserdata (L, result);
    return 1;

}
/* 
 * Function    : corba_call
 * Parameters  : 
 *          svrname - corba server name [string]
 *          pkg - input data , serialize of CMyPkg [string]
 * Description :
 *          根据输入的CMyPkg序列化字符串内容调用指定的CORBA服务，并返回结果。
 * Return      :
 *          返回结果是一个userdata，表示结果CMyPkg
 */
static int corba_call(lua_State *L) 
{
    const char   * name;  
    const char   * pkg;
    char           errstr[1024];
    void         * result;

    name = lua_tostring(L, 1);
    if (name == NULL || name[0] == 0) {
        return luaL_error(L, "corba svr name is null or empty");
    }

    pkg = lua_tostring(L, 2);
    if (pkg == NULL || pkg[0] == 0) {
        return luaL_error(L, "pkg is null or empty");
    }

    result = corba_call_impl(name, pkg, errstr);
    if (result == NULL) {
        return luaL_error(L, "corba_call_impl call error[%s]", errstr);
    }

    lua_pushlightuserdata (L, result);

    return 1;
}


static int corba_free(lua_State *L)
{
    void         * p = NULL;

    p = lua_touserdata(L, 1);
    if (p == NULL) {
        return luaL_error(L, "free point is null");
    }

    corba_object_free(p);
    return 0;
}

#define CORBA_GET_DATA_CHECK()            \
    void       * pkg = NULL;             \
    const char * name  = NULL;           \
    const char * ns = NULL;      \
    char         defaultstr[] = "DEFAULT";      \
\
    do {                                               \
        pkg = lua_touserdata(L, 1);                    \
        if (pkg == NULL) {                             \
            return luaL_error(L, "pkg point is null"); \
        }                                              \
        name = luaL_checkstring(L, 2);                 \
        if (name == NULL || name[0] == 0) {            \
            return luaL_error(L, "cb_get_init key can't be null or empty"); \
        }                                              \
        ns = luaL_checkstring(L, 3);           \
        if (ns == NULL || ns[0] == 0) {                      \
            ns = defaultstr;                   \
        }                                              \
    }while(0)                                          
    



/*  cb_get_int(void *p, char *name , char * default_ns ) */
static int corba_get_int(lua_State * L)
{
    int            r;
    CORBA_GET_DATA_CHECK();
    r = mpkg_get_int(pkg, ns, name);
    lua_pushnumber(L, r);
    return 1;
}

/*  cb_get_double(void *p, char *name , char * default_ns ) */
static int corba_get_double(lua_State * L)
{
    double            r;
    CORBA_GET_DATA_CHECK();
    r = mpkg_get_double(pkg, ns, name);
    lua_pushnumber(L, r);
    return 1;
}

/*  cb_get_double(void *p, char *name , char * default_ns ) */
static int corba_get_short(lua_State * L)
{
    short            r;
    CORBA_GET_DATA_CHECK();
    r = mpkg_get_double(pkg, ns, name);
    lua_pushnumber(L, r);
    return 1;
}

static int corba_get_float(lua_State * L)
{
    float            r;
    CORBA_GET_DATA_CHECK();
    r = mpkg_get_float(pkg, ns, name);
    lua_pushnumber(L, r);
    return 1;
}

static int corba_get_string(lua_State * L)
{
    const char     * r;

    CORBA_GET_DATA_CHECK();
    r = mpkg_get_string(pkg, ns, name);
    if (r == NULL) {
        return luaL_error(L, "corba_get_string error");
    }

    lua_pushstring(L, r);
    return 1;
}

static int corba_mp_unpack(lua_State *L)
{
    size_t            len;
    const char      * p;
    void            * result;

    p = lua_tolstring(L, 1, &len);
    if (p == NULL || p[0] == 0)  {
        return luaL_error(L,"cb_str2pkg 1st argument must be a string");
    }

    result = corba_unpack_wrap(p);
    lua_pushlightuserdata(L, result);
    return 1;
}

static int corba_mp_pack(lua_State *L)
{
    int          len;
    void       * p = NULL;
    char       * result;
    
    p = lua_touserdata(L, 1);
    if (p == NULL) {
        return luaL_error(L, "mp_pack 1st argument must be a point");
    }

    len = mp_get_size(p) + 1024;

    result = calloc(1, len);

    if (mp_pack(p, result, &len)) {
        free(result);
        return luaL_error(L, "pack too big to save in buf");
    }


    lua_pushstring(L, result);
    return 1;
}

typedef union
{
    int          ivalue;
    const char * cvalue;
    short        svalue; 
    float        fvalue;
    double       dvalue;
}VALUE;

#define CORBA_SET_DATA_CHECK CORBA_GET_DATA_CHECK

static int corba_set_int(lua_State *L)
{
    VALUE    value;
    CORBA_SET_DATA_CHECK();          

    value.ivalue = lua_tointeger(L,4);
    mpkg_add_int(pkg, ns, name, value.ivalue);
    return 0;
} 

static int corba_set_string(lua_State *L)
{
    VALUE    value;
    size_t   slen;

    CORBA_SET_DATA_CHECK();

    value.cvalue = lua_tolstring(L, 4, &slen);
    if (value.cvalue == NULL) {
        return luaL_error(L, "set string value can't be null");
    }

    fprintf(stdout, "value.cvalue is name[%s] [%s]\n", name, value.cvalue);

    mpkg_add_string(pkg, ns, name, value.cvalue);
    return 0;
}

static int  corba_set_double(lua_State *L)
{
    VALUE       value;

    CORBA_SET_DATA_CHECK();
    value.dvalue = lua_tonumber(L, 4);

    mpkg_add_double(pkg, ns, name, value.dvalue);
    return 0;
}

static int corba_set_short(lua_State *L)
{
    VALUE value;
    CORBA_SET_DATA_CHECK();
    value.svalue = lua_tointeger(L, 4);
    mpkg_add_short(pkg, ns, name, value.svalue);
    return 0;
}

static int corba_set_float(lua_State *L)
{
    VALUE value;

    CORBA_SET_DATA_CHECK();
    value.fvalue = lua_tonumber(L, 4);
    mpkg_add_float(pkg, ns, name, value.fvalue);
    return 0;
}
 
static int corba_new_pkg(lua_State *L)
{
    void    * p ;
    p = mp_new();
    if (p == NULL) {
        return luaL_error(L, "mp_new error");
    }
    lua_pushlightuserdata (L, p);
    return 1;
}

static int corba_free_pkg(lua_State *L)
{
    return corba_free(L);
}

static const struct luaL_reg base_func [] = {
    {"cb_call", corba_call},
    {"cb_call2", corba_call2},
    {"cb_free", corba_free},
    {"cb_get_int", corba_get_int },
    {"cb_get_string", corba_get_string },
    {"cb_get_double", corba_get_double },
    {"cb_get_short", corba_get_short },
    {"cb_get_float", corba_get_float },
    {"cb_set_int", corba_set_int },
    {"cb_set_string", corba_set_string },
    {"cb_set_double", corba_set_double },
    {"cb_set_short", corba_set_short },
    {"cb_set_float", corba_set_float },
    {"cb_str2pkg", corba_mp_unpack},
    {"cb_new_pkg", corba_new_pkg},
    {"cb_free_pkg", corba_free_pkg},
    {"cb_pkg2str", corba_mp_pack },
   {NULL, NULL}  /* sentinel */
};


int luaopen_corbalib(lua_State *L) {
    luaL_openlib(L, "cb", base_func, 0);
    return 1;
}


