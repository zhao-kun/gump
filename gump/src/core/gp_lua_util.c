#include <wrapunix.h>
#include <gp_runner.h>
#include <gp_util.h>
#include <gp_lua_util.h>
#include <lua/lua.h>                                                            
#include <lua/lauxlib.h>                                                        
#include <lua/lualib.h>
#include <dlfcn.h>

extern int luaopen_baselib(lua_State *L);

static int gp_lua_loadlib(lua_State * L, PLUGINS *plugin)
{
    void            * handle;
    lua_CFunction     func ;

    handle = dlopen(plugin->dll_name, RTLD_NOW);
    if (handle == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "open [%s] error [%s]\n",
                          plugin->dll_name, dlerror());
        return -1;
    }

    func = dlsym(handle, plugin->func_name);
    if (func == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "dlsym [%s] error [%s]\n",
                      plugin->func_name, dlerror());
        return -1;
    }

    func(L);
    return 0;                                                                   
} 

lua_State * gp_init_lua(RUNNERCFG_INFO * cfg) 
{
    lua_State * L;
    int         i;
    int         ret;

    L = lua_open();  /* opens Lua */                                 
    if (L == NULL)
        return NULL;

    /*  init lua base library */
    luaL_openlibs(L);

    /*   init gump base libary     */
    ret = luaopen_baselib(L);
    if (ret < 0) {
        gp_errlog(LVL_ERROR, _FL_, "luaopen_baselib !!!");
        lua_close(L);
        return L;
    }


    for (i = 0; i < cfg->plugin_num; i++) {
        ret = gp_lua_loadlib(L, &cfg->plugins[i]);
        if (ret <0 ) {
            lua_close(L);
            return NULL;
        }
    }
    
    return L;
}

void gp_luaset_glb_variable(lua_State *L, const char *var_name,
                           int type, void * value)
{
    switch(type) {
        case PR_TYPE_INT:
        lua_pushinteger(L, *((int *)value)); 
        break;
        case PR_TYPE_BOOLEAN:
        lua_pushboolean(L, *((int *)value)); 
        break;
        case PR_TYPE_DOUBLE :
        lua_pushnumber(L, *((double *)value));
        break;
        break;
        case PR_TYPE_STRING:
        lua_pushstring(L, (const char *)value);
        break;
        case PR_TYPE_NIL:
        lua_pushnil(L);
        break;
    }

    lua_setglobal(L,var_name);
    return ;
}

int gp_luaget_glb_var_value(lua_State *L, const char * var_name,
                            int type, void *value)
{
    int          t;

    lua_getglobal(L, var_name);

    t =  lua_type(L, -1);
    switch (t) {

    case LUA_TSTRING: 
       if (type == PR_TYPE_STRING) {
           strcpy(value, lua_tostring(L, -1));
           lua_pop(L, 1);
       }else {
           lua_pop(L, 1);
           gp_errlog(LVL_ERROR, _FL_, "type error");
           return -1;
       }
       break;

    case LUA_TBOOLEAN: 
       if (type == PR_TYPE_BOOLEAN) {
           (*(int *)value) = lua_toboolean(L, -1);
           lua_pop(L, 1);
       }else {
           lua_pop(L, 1);
           gp_errlog(LVL_ERROR, _FL_, "type error");
           return -1;
       }
       break;
    case LUA_TNUMBER:
       switch(type) {
       case PR_TYPE_INT:
          (*(int *)value) = lua_tonumber(L, -1);
           break;
       case PR_TYPE_DOUBLE:
          (*(double *)value) = lua_tonumber(L, -1);
           break;
       default:
           lua_pop(L, 1);
           gp_errlog(LVL_ERROR, _FL_, "type error");
           return -1;
       }
       break;
    }

    return 0;
}

int gp_luacall_va (lua_State *L,const char *func, const char *sig, ...)
{
    va_list vl;

    int narg, nres;   /* number of arguments and results */
    int ret;

    va_start(vl, sig);
    lua_getglobal(L, func);  /* get function */
    /* push arguments */
    narg = 0;
    while (*sig) {    /* push arguments */
        switch (*sig++) {
        case 'd':  /* double argument */
            lua_pushnumber(L, va_arg(vl, double));
            break;
        case 'i':  /* int argument */
            lua_pushnumber(L, va_arg(vl, int));
            break;
        case 's':  /* string argument */
            lua_pushstring(L, va_arg(vl, char *));
            break;
        case '>':
            goto endwhile;
        default:
            gp_errlog(LVL_ERROR, _FL_ , "invalid option (%c)", *(sig - 1));
        }
        narg++;
        luaL_checkstack(L, 1, "too many arguments");
    }

endwhile:

    /* do the call */
    nres = strlen(sig);  /* number of expected results */ 
    /* do the call */
    if ((ret=lua_pcall(L, narg, nres, 0)) != 0) {
        gp_errlog(LVL_ERROR, _FL_, "error running function `%s': %s [%d]",
              func, lua_tostring(L, -1), ret);
        return -1;
    }
    /* retrieve results */

    nres = -nres;     /* stack index of first result */
    while (*sig) {    /* get results */
        switch (*sig++) {
        case 'd':  /* double result */
            if (!lua_isnumber(L, nres)) {
                gp_errlog(LVL_ERROR, _FL_, "wrong result type[need double]");
                return -1;
            }
            *va_arg(vl, double *) = lua_tonumber(L, nres);
            break;
        case 'i':  /* int result */
            if (!lua_isnumber(L, nres)) {
                gp_errlog(LVL_ERROR, _FL_, "wrong result type [need int]");
            }

            *va_arg(vl, int *) = (int)lua_tonumber(L, nres);
            break;
        case 's':  /* string result */
            if (!lua_isstring(L, nres)) {
                gp_errlog(LVL_ERROR, _FL_, "wrong result type [need string]");
                return -1;
            }
            *va_arg(vl, const char **) = lua_tostring(L, nres);
            break;
        default:
            gp_errlog(LVL_ERROR, _FL_ , "invalid option (%c)", *(sig - 1));
        }
        nres++;
    }

    va_end(vl);
    return 0;
}
