#include <wrapunix.h>
#include <pay_util.h>
#include <pr_lua_util.h>
#include <lua/lua.h>                                                            
#include <lua/lauxlib.h>                                                        
#include <lua/lualib.h>


void pr_luaset_glb_variable(lua_State *L, const char *var_name,
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

int pr_luacall_va (lua_State *L,const char *func, const char *sig, ...)
{
    va_list vl;

    int narg, nres;   /* number of arguments and results */
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
            pr_errlog(LVL_ERROR, _FL_ , "invalid option (%c)", *(sig - 1));
        }
        narg++;
        luaL_checkstack(L, 1, "too many arguments");
    }

endwhile:

    /* do the call */
    nres = strlen(sig);  /* number of expected results */ 
    /* do the call */
    if (lua_pcall(L, narg, nres, 0) != 0) {
        pr_errlog(LVL_ERROR, _FL_, "error running function `%s': %s",
              func, lua_tostring(L, -1));
        return -1;
    }
    /* retrieve results */

    nres = -nres;     /* stack index of first result */
    while (*sig) {    /* get results */
        switch (*sig++) {
        case 'd':  /* double result */
            if (!lua_isnumber(L, nres)) {
                pr_errlog(LVL_ERROR, _FL_, "wrong result type[need double]");
                return -1;
            }
            *va_arg(vl, double *) = lua_tonumber(L, nres);
            break;
        case 'i':  /* int result */
            if (!lua_isnumber(L, nres)) {
                pr_errlog(LVL_ERROR, _FL_, "wrong result type [need int]");
            }

            *va_arg(vl, int *) = (int)lua_tonumber(L, nres);
            break;
        case 's':  /* string result */
            if (!lua_isstring(L, nres)) {
                pr_errlog(LVL_ERROR, _FL_, "wrong result type [need string]");
            }
            *va_arg(vl, const char **) = lua_tostring(L, nres);
            break;
        default:
            pr_errlog(LVL_ERROR, _FL_ , "invalid option (%c)", *(sig - 1));
        }
        nres++;
    }

    va_end(vl);
    return 0;
}
