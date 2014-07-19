//## MyPkg, C语言风格 API

#ifdef __WIN32__
#define DECL_EXPORT_DLL 	__declspec(dllexport)
#else
#define DECL_EXPORT_DLL
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	// 创建一个加密器
	DECL_EXPORT_DLL     void* ms_new();

	// 释放一个加密器
	DECL_EXPORT_DLL     void ms_free(void *hdl);

	// 将明文加密后存放到指定加密器中(文本方式)
	DECL_EXPORT_DLL     int ms_enctxt(void *hdl, const char *indata, int inlen);

	// 将明文加密后存放到指定加密器中(二进制方式)
	DECL_EXPORT_DLL     int ms_encblock(void *hdl, const char *indata, int inlen);

	// 将密文解密后存放到指定加密器中(二进制方式)
	DECL_EXPORT_DLL     int ms_decblock(void *hdl, const char *indata, int inlen);

	// 读出加密器中的输出结果
	DECL_EXPORT_DLL     int ms_get_outdata(void *hdl, char* outbuf);

	// 读出加密器中的输出结果长度
	DECL_EXPORT_DLL     int ms_get_outlen(void *hdl);
	
#ifdef __cplusplus
}
#endif
