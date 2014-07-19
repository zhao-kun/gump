#include <wrapunix.h>
  #include <dlfcn.h>
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
#include <pr_lua_util.h>

 
int open_lua(lua_State * L)
{
    luaopen_base(L);         /* opens the basic library */
    luaopen_table(L);        /* opens the table library */
    luaopen_os(L);           /* opens the os library */
    luaopen_io(L);           /* opens the I/O library */
    luaopen_string(L);       /* opens the string lib. */
    luaopen_math(L);         /* opens the math lib. */
    return 0;
}


void pr_lua_set_servicenm(lua_State * L, const char * svcname)
{
    lua_pushstring(L, svcname);
    lua_setglobal(L, "__SVCNAME");
}





int  lua_util_openbase(lua_State * L)
{
    void * handle;
    lua_CFunction func ;
    handle =  dlopen("libluabase.so", RTLD_NOW);
    if (handle == NULL) {
        fprintf(stderr, "open [libluabase.so] error [%s]\n", dlerror());
        return -1;
    }


    func=dlsym(handle, "luaopen_baselib");
    if (func == NULL) {
        fprintf(stderr, "dlsym [luaopen_baselib] error [%s]\n", dlerror());
        return -1;
    }

    func(L);
    return 0;
}
int main (void)
{

    char buff[256];
    int error;
    int success = 1;
    double number = 1.1;

    lua_State *L = lua_open();  /* opens Lua */
    luaL_openlibs(L);

    lua_util_openbase(L);


    pr_luaset_glb_variable(L, "__SVCNAME", PR_TYPE_STRING, "01001");
    pr_luaset_glb_variable(L, "__SUCCESS", PR_TYPE_BOOLEAN, &success);
    pr_luaset_glb_variable(L, "__AMT", PR_TYPE_DOUBLE, &number);
    while (fgets(buff, sizeof(buff), stdin) != NULL) {
       error = luaL_loadbuffer(L, buff, strlen(buff),
                  "line") || lua_pcall(L, 0, 0, 0);
       if (error) {
           fprintf(stderr, "%s", lua_tostring(L, -1));
           lua_pop(L, 1);/* pop error message from the stack */
       }
    }

    lua_close(L);
    return 0;

}

