#ifndef INTERBUS_H_HEADER_INCLUDED
#define INTERBUS_H_HEADER_INCLUDED

#include <metaorb.h>

struct s_bus_cfg
{
    char    name[32];
    char    ip[32];
    int     port;
    int     loglevel;
    char    macon[2];
    char    ior[320];
};

extern const s_bus_cfg* get_bus_cfg(const char* aBusName, int fChkIor);

class CInterBusCli : public COrbCli
{
  public:

	using COrbCli::call;
	DECL_EXPORT_DLL int call(const char* BusName, const char* Server, 
		const char* SvcName, CMyPkg* UpPkg, CMyPkg* DownPkg, long WaitTime=0);
};

class CInterBusSvr : public COrbSvr
{
  public:

    DECL_EXPORT_DLL int init(int argc, char** argv);
    DECL_EXPORT_DLL void shutdown();
};

#endif

