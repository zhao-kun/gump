#ifndef METABUS_H_HEADER_INCLUDED
#define METABUS_H_HEADER_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mypkg.h>

#ifdef __WIN32__
#define DECL_EXPORT_DLL __declspec(dllexport)
#else
#define DECL_EXPORT_DLL
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	DECL_EXPORT_DLL void set_mac_on();
	DECL_EXPORT_DLL void set_mac_off();
	DECL_EXPORT_DLL void set_file_dir(const char* dir);

#ifdef __cplusplus
};
#endif

#define BLOCKSIZE	8096

#ifndef __SVC_FUNC__
#define __SVC_FUNC__

typedef int (*svc_main_t) (const char* aCbPtr, int aCbLen, const char* aInPtr, int aInlen, char** aOutPtr, int *aOutLen);
extern svc_main_t g_svc_main;

#endif

extern CMyPkg g_svclist;

#endif

