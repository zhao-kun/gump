#ifndef PR_LUA_UTIL_H
#define PR_LUA_UTIL_H
#include <lua/lua.h>

#define PR_TYPE_NIL     -1
#define PR_TYPE_INT     0
#define PR_TYPE_DOUBLE  1
#define PR_TYPE_STRING  2
#define PR_TYPE_BOOLEAN 3

#ifdef _cplusplus
extern C {
#endif
void gp_luaset_glb_variable(lua_State *, const char *, int , void * );
lua_State * gp_init_lua();
int gp_luacall_va (lua_State *, const char *, const char *, ...);
#ifdef _cplusplus
}
#endif

#endif
