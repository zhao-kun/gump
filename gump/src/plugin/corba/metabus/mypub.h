#ifndef INCL_MYPUB
#define INCL_MYPUB

#ifdef __WIN32__
typedef unsigned int	u_int;
typedef unsigned char	u_char;
typedef unsigned short	u_short;
typedef unsigned long	u_long;
#else
#include <sys/types.h>
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
#include "debug_new.h"
*/

#ifdef __WIN32__
#define DECL_EXPORT_DLL 	__declspec(dllexport)
#else
#define DECL_EXPORT_DLL
#endif

#define ERRLOG prt_time(stdout,__FILE__,__LINE__,'E');printf
#define STDLOG1 prt_time(stdout,__FILE__,__LINE__,'1');if (get_log_level()>=1) printf
#define STDLOG2 prt_time(stdout,__FILE__,__LINE__,'2');if (get_log_level()>=2) printf
#define STDLOG3 prt_time(stdout,__FILE__,__LINE__,'3');if (get_log_level()>=3) printf
#define STDLOG4 prt_time(stdout,__FILE__,__LINE__,'4');if (get_log_level()>=4) printf
#define STDLOG5 prt_time(stdout,__FILE__,__LINE__,'5');if (get_log_level()>=5) printf
#define STDLOG6 prt_time(stdout,__FILE__,__LINE__,'6');if (get_log_level()>=6) printf
#define STDLOG7 prt_time(stdout,__FILE__,__LINE__,'7');if (get_log_level()>=7) printf
#define STDLOG8 prt_time(stdout,__FILE__,__LINE__,'8');if (get_log_level()>=8) printf

#ifdef __cplusplus
extern "C"
{
#endif

	DECL_EXPORT_DLL void 	wrt_log(FILE* fp, char *File, int Line, int level, char *Fmt, ...);
	DECL_EXPORT_DLL void 	prt_time(FILE* fp, char *File, int Line, char level);
	DECL_EXPORT_DLL void 	set_log_level(int level);
	DECL_EXPORT_DLL int 	get_log_level();
	DECL_EXPORT_DLL char* 	trim(char* str);
	DECL_EXPORT_DLL int 	rtrim(char *str);
	
#ifdef __cplusplus
}
#endif

#endif

