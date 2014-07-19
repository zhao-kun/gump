#include <iconv.h>
#include <string.h>
#include "metabus/metaorb.h"
#include "metabus/mp4c.h"

#ifndef CORBANAME
#define CORBANAME "TM_SVR"
#endif

#define LOGSIZE 4096





extern "C"
void * corba_unpack_wrap(const char * str)
{
    CMyPkg     *  obj;

    obj = new CMyPkg();

    if (mp_unpack(obj, str)) {
        free(obj);
        return NULL;
    }
    return obj;
}

extern "C"
void * corba_call_impl2(const char * corbaname, void * in , char * errstr)
{
    int          ret;
    char         svcname[128];
    COrbCli      client;
    CMyPkg     * downPkg;
    CMyPkg     * upPkg;

    upPkg = (CMyPkg *)in;
    downPkg = new CMyPkg();

    client.setMacOff();
    strncpy(svcname, upPkg->getString("_jym", "DEFAULT"), sizeof(svcname));


    ret = client.connect(corbaname);
    if (ret) {
        sprintf(errstr, "连接服务器失败, retcode[%d]:%s",
                       ret, client.getErrMsg());
        free(downPkg);
        return NULL;
    }

    ret = client.call(svcname, upPkg, downPkg, 5000);
    if (ret) {
        sprintf(errstr, "交易[%s]失败, retcode[%d]:%s\n" ,
                        svcname, ret, client.getErrMsg());
        free(downPkg);
        return NULL;
    }

    return downPkg;
}


extern "C"
void * corba_call_impl(const char * corbaname, const char * pkg, char * errstr)
{
    CMyPkg       upPkg;

    if(mp_unpack(&upPkg, pkg)) {
        sprintf(errstr, "初始化失败，请检查配置文件");
        return NULL;
    }

    return corba_call_impl2(corbaname, &upPkg, errstr);
}

extern "C" void corba_object_free(void *p)
{
    if (p != NULL) {
        CMyPkg  * pkg = (CMyPkg *)p;
        delete pkg;
    }
    return ;
}
