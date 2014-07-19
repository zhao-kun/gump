#ifndef METAORB_H_HEADER_INCLUDED
#define METAORB_H_HEADER_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <metabus.h>
#include <mypkg.h>

#ifdef __cplusplus
extern "C"
{
#endif

	DECL_EXPORT_DLL int orb_init(int argc, char** argv);
	DECL_EXPORT_DLL int orb_destroy();
	DECL_EXPORT_DLL void set_conf_file(const char* file);

#ifdef __cplusplus
};
#endif

class COrbCli
{
  public:
	DECL_EXPORT_DLL COrbCli( );
	DECL_EXPORT_DLL ~COrbCli( );
	DECL_EXPORT_DLL int connect(const char* SvrName, int EnableCb=0);
	DECL_EXPORT_DLL int call(const char* SvcName, CMyPkg* UpPkg, CMyPkg* DownPkg, long WaitTime=0);
	DECL_EXPORT_DLL virtual int whenCB(const char* DownBuf, int len);
	DECL_EXPORT_DLL const char*	getErrMsg();
	DECL_EXPORT_DLL int connectIOR(const char* ior, int EnableCb=0);
	DECL_EXPORT_DLL void setMacOn();
	DECL_EXPORT_DLL void setMacOff();
	DECL_EXPORT_DLL int isMacOn();
  private:
	int sendFile(const char* file);
	int recvFile(const char* file);
	void* CBVar;
	void* Server;
	char *PlugID;
	int  ErrCode;
	char *ErrMsg;
	int	ConnStat;
	char *Ior;
	int MacOn;
};

class COrbSvr
{
  public:
	DECL_EXPORT_DLL COrbSvr();
	DECL_EXPORT_DLL ~COrbSvr();
    DECL_EXPORT_DLL int init(int argc, char** argv);
    DECL_EXPORT_DLL void shutdown();
    DECL_EXPORT_DLL static int callback(CMyPkg* UpPkg,const char* buf,int len);
	DECL_EXPORT_DLL void setMacOn();
	DECL_EXPORT_DLL void setMacOff();
	DECL_EXPORT_DLL int isMacOn();
	
  protected:
    char 	PlugID[32];
      	
  private:
	int 	MacOn;
};

#endif

