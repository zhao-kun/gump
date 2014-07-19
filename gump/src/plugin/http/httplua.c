#include <wrapunix.h>
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
#include <curl/curl.h>
#include <gp_util.h>

typedef struct {
    int      left;
    size_t   datalen;
    char  *  pdata;
}LHTTPDATA;

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    LHTTPDATA  * d;
    int          readbyte;
    int          readed;
    
    d = (LHTTPDATA *)userp;
    readbyte = size * nmemb ;

    fprintf(stdout, "read_callback readbyte[%d]\n", readbyte);
    if (readbyte < 1) {
        return 0;
    }

    if (d->left) {
        readed = min(d->left, readbyte);
        memcpy(ptr, &d->pdata[d->datalen - d->left], readed);
        d->left -= readed;
        return readed;
    }

    return 0;
}

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
    LHTTPDATA  *r;
    int         wlen;

    wlen = size * nmemb;
    if (wlen < 1) {
        return 0;
    }

    r = (LHTTPDATA *)userp;

    if (r->pdata == NULL) {
        int        ac_size;
        ac_size = (wlen/1024 + 1) * 1024;
        r->pdata = malloc(ac_size);
        if (r->pdata == NULL) {
            gp_errlog(LVL_ERROR, _FL_, "malloc [%d] byte error[%s]",
                   ac_size, strerror(errno)); 
            return 0;
        }
        r->datalen = ac_size;
        r->left = 0;
    }

    if (wlen > (r->datalen - r->left)) {
        /*  not more buffer to recved data , expand buff */
        int      ac_size;
        ac_size = (wlen / 1024 + 1) * 1204;
        r->pdata = realloc(r->pdata, r->datalen + ac_size);
        if (r->pdata == NULL) {
            gp_errlog(LVL_ERROR, _FL_, "realloc [%d] byte error[%s]",
                    r->datalen + ac_size, strerror(errno));
            return 0;
        }
        r->datalen += ac_size;
    }

    memcpy(&r->pdata[r->left], ptr, wlen );
    r->left += wlen;
    return wlen;
}


/* result = request(url, data, timeout, GET/POST) */
static int l_httprequest(lua_State *L) 
{
    CURL         * curl;
    char           get[] = "GET";
    const char   * url;
    LHTTPDATA      data;
    LHTTPDATA      rdata;
    const char   * method;
    int            timeout = 0;
    CURLcode       res;


    url = lua_tostring(L, 1);
    if (url == NULL || url[0] == 0) {
        return luaL_error(L, "url must not be null or empty");
    }

    data.pdata = (char *)lua_tolstring(L, 2, &data.datalen);
    data.left = data.datalen;

    timeout = lua_tointeger(L, 3);

    if (timeout <= 0) {
        timeout = 60;
    }

    method = lua_tostring(L, 4);

    if (method == NULL) {
        method = get;
    }

    curl = curl_easy_init();
    if (curl == NULL) {
        gp_errlog(LVL_ERROR, _FL_, "curl_easy_init");
        return 0;
    }

    /* compare with pointer */

    res = curl_easy_setopt(curl, CURLOPT_URL, url);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return luaL_error(L, "set url error [%s]", curl_easy_strerror(res));
    }

        fprintf(stdout, "method[%s] POST method\n", method);
    if (toupper(method[0]) == 'P' && toupper(method[1]) == 'O' && 
               toupper(method[2]) == 'S' && toupper(method[3]) == 'T' && 
               toupper(method[4]) == '\0') {
        res = curl_easy_setopt(curl, CURLOPT_POST, 1); 
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            return luaL_error(L, "set post method error [%s]",
                curl_easy_strerror(res));
        }
        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS , data.pdata); 
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            return luaL_error(L,"set POSTFILDS error [%s]",
                curl_easy_strerror(res));
        }

        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.datalen); 
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            return luaL_error(L,"set CURLOPT_POSTFIELDSIZE error [%s]",
                curl_easy_strerror(res));
        }
    } else if (method == get) { /* pointer compare */
        res = curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            return luaL_error(L, "set HTTPGET error[%s]", 
                curl_easy_strerror(res));
        }
        res = curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            return luaL_error(L, "set HTTPREADFUNC error[%s]", 
                curl_easy_strerror(res));
        }
        res = curl_easy_setopt(curl, CURLOPT_READDATA, &data);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            return luaL_error(L, "set HTTPREADDATA error[%s]", 
                curl_easy_strerror(res));
        }
    }

    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return luaL_error(L, "set WRITEFUNCATION [%s]",
                curl_easy_strerror(res));
    }

    rdata.left = 0;
    rdata.datalen = 0;
    rdata.pdata = NULL;
    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rdata);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return luaL_error(L, "set WRITEFUNCATION [%s]",
                curl_easy_strerror(res));
    }

    res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return luaL_error(L, "set TIMEOUT [%s]",
                curl_easy_strerror(res));
    }

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        if (rdata.pdata) {
            free(rdata.pdata);
        }
        curl_easy_cleanup(curl);
        return luaL_error(L, "curl_easy_perform error [%s]",
                curl_easy_strerror(res));
    }

    lua_pushlstring(L, rdata.pdata, rdata.left);
    if (rdata.pdata) {
        free(rdata.pdata);
    }
    curl_easy_cleanup(curl);
    return 1;
}

static const struct luaL_reg base_func [] = {
    {"request", l_httprequest},
    {NULL, NULL}  /* sentinel */
};


int luaopen_httplib(lua_State *L) {
    luaL_openlib(L, "http", base_func, 0);
    return 1;
}


