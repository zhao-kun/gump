/* MyPkg, C语言风格 API */

#ifdef __WIN32__
#define DECL_EXPORT_DLL 	__declspec(dllexport)
#else
#define DECL_EXPORT_DLL
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/******************* Simple api with no default parameters *******************/

/*+++++++++包管理操作+++++++++++*/

	/* 复制出一个新的数据包 */
	DECL_EXPORT_DLL void* mp_clone(const void *srcpkg);

	/* 创建一个新的数据包 */
	DECL_EXPORT_DLL void* mp_new();

	/* 释放数据包 */
	DECL_EXPORT_DLL void mp_free(void *pkg);

	/* 将数据包打包成字符流 */
	DECL_EXPORT_DLL int mp_pack(void *pkg, char *buf, int *len);

	/* 从字符流解包到数据包中 */
	DECL_EXPORT_DLL int mp_unpack(void *pkg, const char *buf);

	/* 清除数据包内容 */
	DECL_EXPORT_DLL void mp_clear(void *pkg);

	/* 估算数据包的大小 */
	DECL_EXPORT_DLL int mp_get_size(void *pkg);

	/* 读取数据包上次操作错误的详细信息 */
	DECL_EXPORT_DLL const char* mp_errmsg(void* pkg);

	/* 将数据包的内容以可显示格式打印 */
	DECL_EXPORT_DLL void mp_print(void *pkg);

	/* 将数据包转换到易初版 CEasyPkg 数据包 */
	DECL_EXPORT_DLL int ep_to_mp(void* from_epkg, void* to_mpkg);

	/* 将易初版 CEasyPkg 数据包转换到MyPkg数据包 */
	DECL_EXPORT_DLL int mp_to_ep(void* from_mpkg, void* to_epkg);

/*+++++++++命名空间常用操作+++++++++++*/

	/* 查找数据包中的命名空间个数 */
	DECL_EXPORT_DLL unsigned short mp_get_nsnum(void *pkg);

	/* 查找命名空间位置 (从 0 开始) */
	DECL_EXPORT_DLL unsigned short mp_get_nsidx(void *pkg, const char* ns);


	/* 从数据包中删除一个命名空间 */
	DECL_EXPORT_DLL int mp_del_ns(void *pkg, const char *ns);

	/* 以数据包当前变量内容为基础，解释表达式的值 */
	DECL_EXPORT_DLL int	mp_parse(void *pkg, char *inbuf, char *outval, 
									char *outtype,int *outlen);

/*+++++++++文件名操作+++++++++++*/

	/* 往数据包中插入/更新一个文件名 */
	DECL_EXPORT_DLL int mp_add_file(void *pkg, const char *filename);

	/* 取数据包中的文件数 */
	DECL_EXPORT_DLL int mp_get_filenum(void *pkg);

	/* 取数据包中的文件名串( 以'+'连接 ) */
	DECL_EXPORT_DLL int mp_get_files(void *pkg, char* filenames);

	/* 取数据包中的第 idx 个 (从0开始) 文件名 */
	DECL_EXPORT_DLL const char* mp_get_file(void *pkg, int idx);

/*+++++++++变量遍历操作+++++++++++*/

	/* 取数据包变量总数 */
	DECL_EXPORT_DLL unsigned short mp_get_fldnum(void *pkg);

	/* 查找某个命名空间 (按名字) 的变量总数 */
	DECL_EXPORT_DLL unsigned short mp_get_ns_fldnum(void *pkg, const char* ns);

	/* 查找某个命名空间(按位置, 从 0 开始)的变量总数 */
	DECL_EXPORT_DLL unsigned short mp_get_nsidx_fldnum(void *pkg, int nsidx);

	/* 查找某个命名空间(按名字)的首变量位置 */
	DECL_EXPORT_DLL int mp_get_ns_1stfld(void *pkg, const char* ns);

	/* 查找某个命名空间(按位置, 从 0 开始)的首变量位置 */
	DECL_EXPORT_DLL int mp_get_nsidx_1stfld(void *pkg, int nsidx);

	/* 取数据包中第 idx 个 (从0开始) 变量的内容 */
	DECL_EXPORT_DLL int mp_get_fld(void *pkg, int idx, 
			char *ns,  char *key, char *val, char *type, int *len);

	/* 查找数据包第 idx 个(从0开始)变量名 */
	DECL_EXPORT_DLL const char* mp_get_fldname(void *pkg, int idx);

	/* 查找数据包第 idx 个(从0开始)变量值 */
	DECL_EXPORT_DLL int mp_get_fldlen(void *pkg, int idx);

	/* 查找数据包第 idx 个(从0开始)变量类型 */
	DECL_EXPORT_DLL char mp_get_fldtype(void *pkg, int idx);

	/* 查找数据包第 idx 个(从0开始)变量值 */
	DECL_EXPORT_DLL const char* mp_get_fldval(void *pkg, int idx);

/************************** Complex api with default parameters ***************/

/*+++++++++命名空间复制移动+++++++++++*/

	/* 从源数据包中复制一个命名空间内所有的变量至目标数据包指定的命名空间中 */
	DECL_EXPORT_DLL int mpkg_cp_ns(const void *srcpkg, const char *srcns, 
							void *dstpkg, const char *dstns);

	/* 从源数据包中移动一个命名空间内所有的变量至目标数据包指定的命名空间中 */
	DECL_EXPORT_DLL int mpkg_mv_ns(const void *srcpkg, const char *srcns, 
							void *dstpkg, const char *dstns);

/*+++++++++变量增删查+++++++++++*/

	/* 查找某个变量的位置 */
	DECL_EXPORT_DLL int mpkg_get_fldidx(void *pkg, const char *ns, 
									const char *key);

	/* 删除数据包中某个变量 */
	DECL_EXPORT_DLL int mpkg_del_fld(void *pkg, const char *ns, 
									const char *key);

	/* 往数据包中插入/更新一个 16 进制数据块变量 */
	DECL_EXPORT_DLL int mpkg_add_bin(void *pkg, const char *ns, 
							const char *key, const char *val, int len);

	/* 从数据包中取出一个 16 进制数据块变量的内容 */
	DECL_EXPORT_DLL int mpkg_get_bin(void *pkg, const char *ns, 
									const char *key, char *val, int *len);

	/* 往数据包中插入/更新一个整数变量 */
	DECL_EXPORT_DLL int mpkg_add_int(void *pkg, const char *ns, 
									const char *key, int val);

	/* 从数据包中取出一个整数变量的值 */
	DECL_EXPORT_DLL int mpkg_get_int(void *pkg, const char *ns, 
										const char *key);

	/* 往数据包中插入/更新一个短整数变量 */
	DECL_EXPORT_DLL int mpkg_add_short(void *pkg, const char *ns, 
										const char *key, short val);

	/* 从数据包中取出一个短整数变量的值 */
	DECL_EXPORT_DLL short mpkg_get_short(void *pkg, const char *ns, 
											const char *key);

	/* 往数据包中插入/更新一个无符号整数变量 */
	DECL_EXPORT_DLL int mpkg_add_uint(void *pkg, const char *ns, 
										const char *key, unsigned int val);

	/* 从数据包中取出一个无符号整数变量的值 */
	DECL_EXPORT_DLL unsigned int mpkg_get_uint(void *pkg, const char* ns, 
												const char* key);

	/* 往数据包中插入/更新一个无符号短整数变量 */
	DECL_EXPORT_DLL int mpkg_add_ushort(void *pkg, const char *ns, 
										const char *key, unsigned short val);

	/* 从数据包中取出一个无符号短整数变量的值 */
	DECL_EXPORT_DLL unsigned short mpkg_get_ushort(void *pkg, 
										const char *ns, const char *key);

	/* 往数据包中插入/更新一个长整数变量 */
	DECL_EXPORT_DLL int mpkg_add_long(void *pkg, const char *ns, 
											const char *key, long val);

	/* 从数据包中取出一个长整数变量的值 */
	DECL_EXPORT_DLL long mpkg_get_long(void *pkg, const char *ns, 
										const char *key);

	/* 往数据包中插入/更新一个无符号长整数变量 */
	DECL_EXPORT_DLL int mpkg_add_ulong(void *pkg, const char *ns, 
										const char *key, unsigned long val);

	/* 从数据包中取出一个无符号长整数变量的值 */
	DECL_EXPORT_DLL unsigned long mpkg_get_ulong(void *pkg, const char *ns, 
										const char *key);

	/* 往数据包中插入/更新一个浮点数变量 */
	DECL_EXPORT_DLL int mpkg_add_float(void *pkg, const char *ns, 
										const char *key, float val);

	/* 从数据包中取出一个浮点数变量的值 */
	DECL_EXPORT_DLL float mpkg_get_float(void *pkg, const char *ns, 
										const char *key);

	/* 往数据包中插入/更新一个双精度浮点数变量 */
	DECL_EXPORT_DLL int mpkg_add_double(void *pkg, const char *ns, 
										const char *key, double val);

	/* 从数据包中取出一个双精度浮点数变量的值 */
	DECL_EXPORT_DLL double mpkg_get_double(void *pkg,const char *ns, 
										const char *key);

	/* 往数据包中插入/更新一个字符串变量 */
	DECL_EXPORT_DLL int mpkg_add_string(void *pkg, const char *ns, 
										const char *key, const char *val);

	/* 往数据包中插入/更新一个要求屏蔽显示的字符串变量 */
	DECL_EXPORT_DLL int mpkg_add_maskstr(void *pkg, const char *ns, 
										const char *key, const char *val);

	/* 从数据包中取出一个字符串变量的值 */
	DECL_EXPORT_DLL const char* mpkg_get_string(void *pkg, const char *ns, 
										const char *key);
										
	DECL_EXPORT_DLL int mpkg_get_strdata(void *pkg, const char *ns, 
										const char *key, char* val, int len);

	/* 往数据包中插入/更新一个要求加密保存的字符串变量 */
	DECL_EXPORT_DLL int mpkg_add_encstr(void *pkg, const char *ns, 
								const char *key, const char *val, int len);

	/* 从数据包中取出一个已加密过的字符串变量的明文 */
	DECL_EXPORT_DLL int mpkg_get_encstr(void *pkg, const char *ns, 
									const char *key, char* val, int nmax);


#ifdef __cplusplus
}
#endif

/* Simple macros specified default parameter for complex api */

#define mp_add_bin(pkg,key,val,len) mpkg_add_bin(pkg,"DEFAULT",key,val,len)
#define mp_add_int(pkg,key,val) mpkg_add_int(pkg,"DEFAULT",key,val)
#define mp_add_uint(pkg,key,val) mpkg_add_uint(pkg,"DEFAULT",key,val)
#define mp_add_short(pkg,key,val) mpkg_add_short(pkg,"DEFAULT",key,val)
#define mp_add_ushort(pkg,key,val) mpkg_add_ushort(pkg,"DEFAULT",key,val)
#define mp_add_long(pkg,key,val) mpkg_add_long(pkg,"DEFAULT",key,val)
#define mp_add_ulong(pkg,key,val) mpkg_add_ulong(pkg,"DEFAULT",key,val)
#define mp_add_float(pkg,key,val) mpkg_add_float(pkg,"DEFAULT",key,val)
#define mp_add_double(pkg,key,val) mpkg_add_double(pkg,"DEFAULT",key,val)
#define mp_add_string(pkg,key,val) mpkg_add_string(pkg,"DEFAULT",key,val)
#define mp_add_maskstr(pkg,key,val) mpkg_add_maskstr(pkg,"DEFAULT",key,val)
#define mp_add_encstr(pkg,key,val) mpkg_add_encstr(pkg,"DEFAULT",key,val,0)

#define mp_get_bin(pkg,key,plen) mpkg_get_bin(pkg,"DEFAULT",key,plen)
#define mp_get_int(pkg,fld) mpkg_get_int(pkg,"DEFAULT",fld)
#define mp_get_uint(pkg,fld) mpkg_get_uint(pkg,"DEFAULT",fld)
#define mp_get_short(pkg,fld) mpkg_get_short(pkg,"DEFAULT",fld)
#define mp_get_ushort(pkg,fld) mpkg_get_ushort(pkg,"DEFAULT",fld)
#define mp_get_long(pkg,fld) mpkg_get_long(pkg,"DEFAULT",fld)
#define mp_get_ulong(pkg,fld) mpkg_get_ulong(pkg,"DEFAULT",fld)
#define mp_get_float(pkg,fld) mpkg_get_float(pkg,"DEFAULT",fld)
#define mp_get_double(pkg,fld) mpkg_get_double(pkg,"DEFAULT",fld)
#define mp_get_string(pkg,fld) mpkg_get_string(pkg,"DEFAULT",fld)
#define mp_get_encstr(pkg,fld) mpkg_get_encstr(pkg,"DEFAULT",fld)

#define mp_cp_ns(pkg,src_ns,dst_ns) mpkg_cp_ns(pkg,src_ns,dst_ns,NULL)
#define mp_mv_ns(pkg,src_ns,dst_ns) mpkg_mv_ns(pkg,src_ns,dst_ns,NULL)
#define mp_del_fld(pkg,key) mpkg_del_fld(pkg,"DEFAULT",key)
#define mp_get_fldidx(pkg,key) mpkg_get_fldidx(pkg,"DEFAULT",key)

