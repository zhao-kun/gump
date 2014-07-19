#include <wrapunix.h>
#include <gp_util.h>
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
#include <atmi.h>
#include <fml32.h>

#define CARRAY            0
#define STRING            1
#define FML32             2

typedef struct tux_datahandle
{
    int       type ;
    void    * data ;
    int       len;
} tux_datahandle;

static int l_tpcall(lua_State *L)
{
    const char * s = NULL;
    tux_datahandle * h;
    long            olen;
    int            ret;

    s = lua_tostring(L, 1);
    if (s == NULL || s[0] == 0) {
        return luaL_error(L, "tuxedo service name can't be null");
    }

    h = lua_touserdata(L, 2);
    if (h == NULL) {
        return luaL_error(L, "data can't be null");
    }

    ret = tpcall((char *)s, h->data, h->len, (char **)(&h->data), &olen, 0); 
    if (ret == -1) {
        gp_errlog(LVL_ERROR, _FL_, "tpcall [%s] error [%s]",
            s, tpstrerror(tperrno));
        tpfree(h->data);
        lua_pushlightuserdata(L, NULL);
        return 1;
    }

    h->len = olen;
    lua_pushlightuserdata(L, h);
    return 1;
}



static int l_tpalloc(lua_State *L)
{
    const char          * type = NULL;
    int                   size = 0;
    tux_datahandle      * handle;

    type = lua_tostring(L, 1);
    if (type == NULL || type[0] == 0) {
        return luaL_error(L, "type is null or empty");
    }

    size = lua_tointeger(L, 2);
    if (size < 1) {
        size = 1024;
    }

    handle = malloc(sizeof(tux_datahandle));
    if (!handle) {
        return luaL_error(L, "malloc [%d] byte error [%s]",
                sizeof(tux_datahandle), strerror(errno));
    }

    if (!strcmp(type, "FML32")) {
        handle->type = FML32;
    } else if (!strcmp(type, "STRING")) {
        handle->type = STRING;
    } else if (!strcmp(type, "CARRAY")) {
        handle->type = CARRAY;
    } else {
        return luaL_error(L, "expect type [FML32 or STRING or CARRAY]"
                            " real type [%s] ", type);
    }

    handle->data = tpalloc((char *)type, NULL, size);
    if (handle->data == NULL) {
        return luaL_error(L, "tpalloc(%s, NULL, %d) error [%s]",
                type, size, tpstrerror(tperrno));
    }

    handle->len = size;
    lua_pushlightuserdata(L, handle);

    return 1;
}


static
int Fchg32Ext(tux_datahandle *h, FLDID32 fid, int oc, char *v, int l) {
    int    ret;
    int    newsize  = 0;
    if ((ret = Fchg32(h->data, fid, oc, v, l)) == -1) {
        if (ret == FNOSPACE) {
            newsize = Fsizeof32(h->data) + ((l/4096) + 1) * 1024;
            h->data = Frealloc32(h->data, 0, newsize);
            if (h->data == NULL) {
                gp_errlog(LVL_ERROR, _FL_, "Frealloc32 error [%s]",
                    Fstrerror32(Ferror32));
                return -1;
            }
            h->len = newsize;
            return Fchg32(h->data, fid, oc, v, l);
        }
    }
    return ret;
 }


/* fmlchg(buf, name/id, value, oc) */
static int l_fmlchg(lua_State *L)
{
    tux_datahandle       *h ;
    FLDID32               fid;
    const char           *s;
    int                   oc;
    FLDLEN32              len;
    char                 *type;

    h = (tux_datahandle *)lua_touserdata(L, 1);
    if (h == NULL) {
        return 0;
    }

    if (lua_isnumber(L,2)) {
        fid = lua_tointeger(L, 2);
    } else if (lua_isstring(L, 2)) {
        s = lua_tostring(L, 2);
        if (s == NULL || s[0] == 0) {
            return luaL_error(L, "fidname is null");
        }
        fid = Fldid32((char *)s);
    }else {
        return luaL_error(L, "argument error");
    }


    oc = lua_tointeger(L, 4);
    if (oc < 0) {
        oc = 0;
    }

    type = Ftype32(fid);
    if (type == NULL) {
        return luaL_error(L, "Ftype[%d] error [%s]",
                fid, Fstrerror32(Ferror32));
    }

    if (!strcmp(type, "short")) {
        short        s;
        int          r;
        len = sizeof(s);
        r = lua_tointeger(L, 3);
        s = r;
        if (Fchg32Ext(h->data, fid, oc, (char *)&s, len) == -1) {
            return luaL_error(L, "Fchg32Ext [%s][%d] value error[%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));
        }
        return 0;
    } else if (!strcmp(type, "long")) {
        int        s;
        len = sizeof(s);
        s = lua_tointeger(L, 3);
        if (Fchg32Ext(h->data, fid, oc, (char *)&s, len) == -1) {
            return luaL_error(L, "Fget32Ext [%s][%d] value error[%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));
        }
        return 0;
    } else if (!strcmp(type, "float")) {
        float        s;
        double       d;
        len = sizeof(s);
        d = lua_tonumber(L, 3);
        s = d;
        if (Fchg32Ext(h->data, fid, oc, (char *)&s, len) == -1) {
            return luaL_error(L, "Fget32 [%s][%d] value error[%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));
        }
        return 0;
    } else if (!strcmp(type, "double")) {
        double        s;
        len = sizeof(s);
        s = lua_tonumber(L, 3);
        if (Fchg32Ext(h->data, fid, oc, (char *)&s, len) == -1) {
            return luaL_error(L, "Fget32 [%s][%d] value error [%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));
        }
        return 0;
    } else if (!strcmp(type, "char")) {
        char        s;
        len = sizeof(s);
        s = (char )lua_tointeger(L, 3);
        if (Fchg32Ext(h->data, fid, oc, (char *)&s, len) == -1) {
            return luaL_error(L, "Fget32 [%s][%d] value error[%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));
        }
        return 0;
    } else if (!strcmp(type, "string") || !strcmp(type, "carray")) {
        char     * p = NULL;
        size_t     l;

        p = (char *)lua_tolstring(L, 3, &l);
        len = (FLDLEN32)l;
        if (Fchg32Ext(h->data, fid, oc, p, len) == -1) {
            return luaL_error(L, "Fget32 [%s][%d] value error[%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));
        }
        return 0;
    } else {
        return  luaL_error(L, "error Fml32 type [%s]", type);
    }

    return 0;
}
 

static int l_fmlget(lua_State *L)
{
    tux_datahandle       *h ;
    FLDID32               fid;
    const char           *s;
    int                   oc;
    FLDLEN32              len;
    char                 *type;

    h = (tux_datahandle *)lua_touserdata(L, 1);
    if (h == NULL) {
        return 0;
    }

    if (lua_isnumber(L,2)) {
        fid = lua_tointeger(L, 2);
    }else if (lua_isstring(L, 2)) {
        s = lua_tostring(L, 2);
        if (s == NULL || s[0] == 0) {
            return luaL_error(L, "fidname is null");
        }
        fid = Fldid32((char *)s);
    }


    oc = lua_tointeger(L, 3);
    if (oc < 0) {
        oc = 0;
    }

    if (h->type != FML32) {
        return luaL_error(L, "expect type [fml32] but was [%d]", h->type);
    }

    type = Ftype32(fid);
    if (type == NULL) {
        return luaL_error(L, "Ftype[%d] error[%s]", 
            fid, Fstrerror32(Ferror32));
    }

    if (!strcmp(type, "short")) {
         short        s;
         len = sizeof(s);
         if (Fget32(h->data, fid, oc, (char *)&s, &len) == -1) {
             return luaL_error(L, "Fget32 [%s][%d] value error[%s]",
                        Fname32(fid), fid, Fstrerror32(Ferror32));
         }
         lua_pushinteger(L, s);
         return 1;
    } else if (!strcmp(type, "long")) {
        int        s;
        len = sizeof(s);
        if (Fget32(h->data, fid, oc, (char *)&s, &len) == -1) {
            return luaL_error(L, "Fget32 [%s][%d] value error[%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));
        }
        lua_pushinteger(L, s);
        return 1;
    } else if (!strcmp(type, "float")) {
        float        s;
        len = sizeof(s);
        if (Fget32(h->data, fid, oc, (char *)&s, &len) == -1) {
            return luaL_error(L, "Fget32 [%s][%d] value error[%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));
        }
        lua_pushnumber(L, s);
        return 1;
    } else if (!strcmp(type, "double")) {
        double        s;
        len = sizeof(s);
        if (Fget32(h->data, fid, oc, (char *)&s, &len) == -1) {
            return luaL_error(L, "Fget32 [%s][%d] value error [%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));
        }
        lua_pushnumber(L, s);
        return 1;
    } else if (!strcmp(type, "char")) {
        char        s[2];
        s[1] = 0;
        len = sizeof(s);
        if (Fget32(h->data, fid, oc, (char *)s, &len) == -1) {
            return luaL_error(L, "Fget32 [%s][%d] value error[%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));
        }
        lua_pushstring(L, s);
        return 1;
    } else if (!strcmp(type, "string")) {
        char *p = NULL;
        len = Flen32(h->data, fid, oc);
        if (len == (FLDLEN32)-1 ) {
            return luaL_error(L, "Flen32 [%s][%d] error [%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));

        }
        p = calloc(1, len+1);
        if (p == NULL) {
            return luaL_error(L, "calloc [%d] byte error [%s]",
                   len+1, strerror(errno));
        }

        if (Fget32(h->data, fid, oc, (char *)p, &len) == -1) {
            return luaL_error(L, "Fget32 [%s][%d] value error[%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));
        }
        lua_pushstring(L, p);
        return 1;
    } else if (!strcmp(type, "carray")) {
        char *p = NULL;
        len = Flen32(h->data, fid, oc);
        if (len == (FLDLEN32)-1 ) {
            return luaL_error(L, "Flen32 [%s][%d] error [%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));

        }
        p = calloc(1, len);
        if (p == NULL) {
            return luaL_error(L, "calloc [%d] byte error [%s]",
                   len+1, strerror(errno));
        }

        if (Fget32(h->data, fid, oc, (char *)p, &len) == -1) {
            return luaL_error(L, "Fget32 [%s][%d] value error[%s]",
                       Fname32(fid), fid, Fstrerror32(Ferror32));
        }
        lua_pushlstring(L, p, len);
        return 1;
    } else {
        return  luaL_error(L, "error Fml32 type [%s]", type);
    }

    return 0;
}

static int l_tpfree(lua_State *L) 
{
    tux_datahandle       *h ;
    h = (tux_datahandle *)lua_touserdata(L, 1);
    if (h == NULL) {
        return 0;
    }

    tpfree(h->data);
    free(h);
    return 0;
}

static int l_tpinit(lua_State *L) 
{
    TPINIT *tpinfo;
    int         ret;

    if ((tpinfo = (TPINIT *)tpalloc("TPINIT",(char *)NULL,TPMULTICONTEXTS)) == NULL) {
        return luaL_error(L, "tpalloc tpinit with mutilcontext error [%s]", 
            tpstrerror(tperrno));
    }

    ret = tpinit(tpinfo);
    if (ret < 0) {
        tpfree((char *)tpinfo);
        return luaL_error(L, "tpinit with mutilcontext error [%s]", 
            tpstrerror(tperrno));
    }

    tpfree((char *)tpinfo);
    return 0;
}

static int l_tpterm(lua_State *L) 
{
    tpterm();
    return 0;
}

static int l_tpsetdat(lua_State *L)
{
    size_t              l;
    const char        * s;
    tux_datahandle    * h;



    h = (tux_datahandle *)lua_touserdata(L, 1);
    if (h == NULL) {
        return luaL_error(L, "tux_handle is null");
    }

    s = lua_tolstring(L, 2, &l);
    if (s == NULL) {
        return luaL_error(L, "contents can't be null");
    }

    switch(h->type) {
    case CARRAY:
    case STRING:
        break;
    default:
        return luaL_error(L, "fml type not use this method to set data");
    }

    if (l > h->len) {
        return luaL_error(L, "data too long for tuxedo");
    }

    memcpy(h->data, s, l);
    if (h->type == STRING) {
        ((char *)h->data)[l] = 0;
    }

    return 0;
}

static int l_tpgetdat(lua_State *L)
{
    tux_datahandle    * h;



    h = (tux_datahandle *)lua_touserdata(L, 1);
    if (h == NULL) {
        return luaL_error(L, "tux_handle is null");
    }

    switch(h->type) {
    case CARRAY:
    case STRING:
        break;
    default:
        return luaL_error(L, "fml type not use this method to set data");
    }

    lua_pushlstring(L, h->data, h->len);
    return 1;

}


static const struct luaL_reg base_func [] = {
    {"call", l_tpcall},
    {"alloc", l_tpalloc},
    {"setdata", l_tpsetdat},
    {"getdata", l_tpgetdat},
    {"fmlchg", l_fmlchg},
    {"fmlget", l_fmlget},
    {"free", l_tpfree},
    {"init", l_tpinit},
    {"term", l_tpterm},
    {NULL, NULL}  /* sentinel */
};


int luaopen_tuxedolib(lua_State *L) {
    luaL_openlib(L, "tuxedo", base_func, 0);
    return 1;
}
