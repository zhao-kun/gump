#include <stdio.h>
#include <stdlib.h>
#include "mp4c.h"

/* COrbCli的C语言 API */

#ifdef __WIN32__
#define DECL_EXPORT_DLL 	__declspec(dllexport)
#else
#define DECL_EXPORT_DLL
#endif

#ifndef __SVC_FUNC__
#define __SVC_FUNC__

typedef int (*svc_main_t) (const char* aCbPtr, int aCbLen, const char* aInPtr, 
							int aInlen, char** aOutPtr, int *aOutLen);

extern svc_main_t g_svc_main;

#endif

#ifdef __cplusplus
extern "C"
{
#endif

	DECL_EXPORT_DLL void wrt_log(FILE* fp, char *File, int Line, int level, char *Fmt, ...);
	DECL_EXPORT_DLL void prt_time(FILE* fp, char *File, int Line, char level);
	DECL_EXPORT_DLL void set_log_level(int level);
	DECL_EXPORT_DLL int get_log_level();

#ifdef __cplusplus
}
#endif

/* 写日志宏定义 */

#define ERRLOG prt_time(stdout,__FILE__,__LINE__,'E');printf
#define STDLOG1 prt_time(stdout,__FILE__,__LINE__,'1');if (get_log_level()>=1) printf
#define STDLOG2 prt_time(stdout,__FILE__,__LINE__,'2');if (get_log_level()>=2) printf
#define STDLOG3 prt_time(stdout,__FILE__,__LINE__,'3');if (get_log_level()>=3) printf
#define STDLOG4 prt_time(stdout,__FILE__,__LINE__,'4');if (get_log_level()>=4) printf
#define STDLOG5 prt_time(stdout,__FILE__,__LINE__,'5');if (get_log_level()>=5) printf
#define STDLOG8 prt_time(stdout,__FILE__,__LINE__,'8');if (get_log_level()>=8) printf

#ifdef __cplusplus
extern "C"
{
#endif

	/* 创建一个 metaorb 通讯客户端对象 */
	DECL_EXPORT_DLL void* mc_new();

	/* 释放一个 metaorb 通讯客户端对象 */
	DECL_EXPORT_DLL void  mc_free(void* p_cli);

	/* 客户端对象申请连接到某个服务引擎(用名字查找服务) */
	DECL_EXPORT_DLL int   mc_connect(void* p_cli, const char* servname);

	/* 客户端对象申请连接到某个服务引擎(用 IOR 串查找服务) */
	DECL_EXPORT_DLL int   mc_connectIOR(void* p_cli, const char* ior);

	/* 客户端申请调用服务功能 */
	DECL_EXPORT_DLL int   mc_call(void* p_cli, const char* svcname, 
						void* p_uppkg, void* p_downpkg, long timeout);

	/* 客户端对象设为启动 MAC 校验 */
	DECL_EXPORT_DLL void  mc_mac_on(void* p_cli);

	/* 客户端对象设为停止 MAC 校验 */
	DECL_EXPORT_DLL void   mc_mac_off(void* p_cli);

	/* 客户端对象报告上次操作失败的具体错误信息 */
	DECL_EXPORT_DLL const char* mc_errmsg(void* p_cli);

	/* 设置当前通讯环境的文件上下传目录 */
	DECL_EXPORT_DLL void  set_file_dir(const char* dir);

	/* 设置当前通讯环境的通讯配置文件路径 */
	DECL_EXPORT_DLL void  set_conf_file(const char* file);

	/* 启动服务引擎 */
    DECL_EXPORT_DLL int   orb_svr_init(int argc, char** argv, int if_mac_on);

	/* 设定服务端主函数入口 */
    DECL_EXPORT_DLL void  set_svc_main(svc_main_t svc_main);

	DECL_EXPORT_DLL void* mb_newbuf(int size);

	DECL_EXPORT_DLL void mb_freebuf(void* buf);

	/* 创建一个跨总线通讯客户端对象 */
	DECL_EXPORT_DLL void* inbus_new();

	/* 释放一个跨总线通讯客户端对象 */
	DECL_EXPORT_DLL void inbus_free(void* p_cli);

	/* 客户端跨总线调用服务功能 */
	DECL_EXPORT_DLL int inbus_call(void* p_cli, const char* busname, 
		const char* svrname, const char* trade, 
		void* p_uppkg, void* p_downpkg, long timeout);

	/* 启动跨总线桥接服务器 */
    DECL_EXPORT_DLL int inbus_svr_init(int argc, char** argv);

#ifdef __cplusplus
};

#endif


