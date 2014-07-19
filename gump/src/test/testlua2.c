#include <wrapunix.h>
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

 
static void stackDump (lua_State *L) {
    int i;
    int top = lua_gettop(L);

    for (i = 1; i <= top; i++) {  /* repeat for each level */

       int t = lua_type(L, i);

       switch (t) {
       case LUA_TSTRING:  /* strings */
           printf("`%s'", lua_tostring(L, i));
           break;
       case LUA_TBOOLEAN:  /* booleans */
           printf(lua_toboolean(L, i) ? "true" : "false");
           break;

       case LUA_TNUMBER:  /* numbers */
           printf("%g", lua_tonumber(L, i));
           break;

       default:  /* other values */
           printf("%s", lua_typename(L, t));
           break;
       }
       printf("  ");  /* put a separator */
    }
    printf("\n");     /* end the listing */
}


int main (void) 
{

    lua_State *L = lua_open();

    lua_pushboolean(L, 1); lua_pushnumber(L, 10);

    lua_pushnil(L); lua_pushstring(L, "hello");

    fprintf(stdout, "init --->\n");
    stackDump(L);

    fprintf(stdout, "lua_insert(L, -2);\n");
    lua_insert(L, -2);
    stackDump(L);

    /* true  10  nil  `hello'  */
    fprintf(stdout, "lua_pushvalue(L, -4) -->\n");
    lua_pushvalue(L, -4); stackDump(L);
    /* true  10  nil  `hello'  true  */
    fprintf(stdout, "lua_replace(L, 3) -->\n");
    lua_replace(L, 3); stackDump(L);
    /* true  10  true  `hello'  */
    fprintf(stdout, "lua_settop(L, 6) -->\n");
    lua_settop(L, 6); stackDump(L);
    /* true  10  true  `hello'  nil  nil  */
    lua_remove(L, -3); stackDump(L);
    /* true  10  true  nil  nil  */
    lua_settop(L, -5); stackDump(L);
    /* true  */
    lua_close(L);

    return 0;

}

