#ifndef INCL_MYPKG
#define INCL_MYPKG

#include "mypub.h"

// mypkg 内存使用参数定义
#define MP_SIZE_NSNAME		24			// 命名空间名最大长度(含结束符)
#define MP_SIZE_FLDNAME		24			// 字段名最大长度(含结束符)
#define MP_INIT_HEAD_SIZE	2048		// 包头初始长度
#define MP_INIT_DATA_SIZE	2048		// 包体初始长度
#define MP_MAX_HEAD_SIZE	65536		// 包头最大长度
#define MP_MAX_DATA_SIZE	65536       // 包体最大长度
#define MP_MAX_NS			256			// 命名空间最大个数
#define MP_MAX_FIELD		2048		// 字段最大个数
#define MP_FILE_NSNAME	    "__FILES__" // 文件名保存命名空间

// mypkg 字段变量类型定义
#define MP_T_SHORT		'H'				// 短整型
#define MP_T_USHORT 	'T'				// 无符号短整型
#define MP_T_INT 		'I'				// 整型
#define MP_T_UINT 		'N'				// 无符号整型
#define MP_T_LONG 		'L'				// 长整型
#define MP_T_ULONG		'U'				// 无符号长整型
#define MP_T_FLOAT		'F'				// 单精度数
#define MP_T_DOUBLE 	'D'				// 双精度数
#define MP_T_STRING 	'S'				// 字符串
#define MP_T_BCD 		'B'				// BCD码
#define MP_T_BIN 		'X'				// 二进制数
#define MP_T_ENC_STR 	'E'				// 加密字符串
#define MP_T_MASK 		'M'				// 屏蔽输出字符串

#define MEM_ERROR			1			// 系统内存不足
#define PKG_TOO_LARGE 	    2			// 数据包长度超出最大值(64K)
#define TOO_MANY_NS			3			// 命名空间数超出最大值(256)
#define TOO_MANY_FIELD      4			// 字段数超出最大值(2048)
#define	OUTBUF_TOO_SMALL	5			// 传出数据缓冲长度不足
#define INVALID_FLDNAME	    6			// 字段名非法
#define FLD_NOT_FOUND 	    7			// 找不到字段
#define INVALID_NSNAME 	  	8			// 合名空间名非法
#define NS_NOT_FOUND		9			// 找不到合名空间
#define PKG_ERROR			10			// 数据包内容不正确

//##ModelId=4AD97C4C0261
struct SMyField
{
    //##ModelId=4AD97C4D00AF
    char FldName[MP_SIZE_FLDNAME];

    //##ModelId=4AD97C4D00B0
    char Type;

    //##ModelId=4AD97C4D00BB
    char NsIdx;

    //##ModelId=4AD97C4D00BC
    char DataOffset[2];

    //##ModelId=4AD97C4D00BD
    char DataLen[2];
};

//##ModelId=4AD97C4C0251
class CMyPkg
{
  public:
  	
    //##ModelId=4AD97C4C02C5
    DECL_EXPORT_DLL static void splitNsFld(const char* aStr, char* nsname, char* fldname);

    // 构造方法
    //##ModelId=4AD97C4C02CF
    DECL_EXPORT_DLL CMyPkg();

    // 构造方法
    //##ModelId=4AD97C4C02D0
    DECL_EXPORT_DLL CMyPkg(CMyPkg& aSrcPkg, const char* aNs = NULL);

    // 析构方法
    //##ModelId=4AD97C4C02D3
    DECL_EXPORT_DLL ~CMyPkg();

    // 返回上次操作的出错码
    //##ModelId=4AD97C4C02D4
    DECL_EXPORT_DLL const char* getErrMsg();

    //##ModelId=4AD97C4C02D5
    DECL_EXPORT_DLL void clear();

    //##ModelId=4AD97C4C02D6
    DECL_EXPORT_DLL int addInt(const char *key, int val, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C02E0
    DECL_EXPORT_DLL int addUInt(const char *key, u_int val, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C02E4
    DECL_EXPORT_DLL int addShort(const char *key, short val, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C02F0
    DECL_EXPORT_DLL int addUShort(const char *key, u_short val, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C02FD
    DECL_EXPORT_DLL int addLong(const char *key, long val, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C0301
    DECL_EXPORT_DLL int addULong(const char *key, u_long val, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C030D
    DECL_EXPORT_DLL int addFloat(const char *key, float val, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C0311
    DECL_EXPORT_DLL int addDouble(const char *key, double val, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C031C
    DECL_EXPORT_DLL int addString(const char *key, const char *val, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C0320
    DECL_EXPORT_DLL int addString(const char *key, const char *val, int len, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C032E
    DECL_EXPORT_DLL int addBin(const char *key, const char *val, int len, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C033C
    DECL_EXPORT_DLL int addEncStr(const char *key, const char *val, int len = 0, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C0341
    DECL_EXPORT_DLL int addMaskStr(const char *key, const char *val, int len = 0, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C034D
    DECL_EXPORT_DLL int addFileName(const char *filename);

    //##ModelId=4AD97C4C034F
    DECL_EXPORT_DLL int setField(const char *key, const char *val, const char type, int len, const char *ns);

    //##ModelId=4AD97C4C035E
    DECL_EXPORT_DLL short getShort(const char *key, const char *ns = "DEFAULT");

    // 读取无符号短整型字段
    //##ModelId=4AD97C4C0361
    DECL_EXPORT_DLL u_short getUShort(const char *key, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C036C
    DECL_EXPORT_DLL int getInt(const char *key, const char *ns = "DEFAULT");

    // 读取无符号整型字段
    //##ModelId=4AD97C4C036F
    DECL_EXPORT_DLL u_int getUInt(const char *key, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C037A
    DECL_EXPORT_DLL long getLong(const char *key, const char *ns = "DEFAULT");

    // 读取无符号长整型字段
    //##ModelId=4AD97C4C037D
    DECL_EXPORT_DLL u_long getULong(const char *key, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C0380
    DECL_EXPORT_DLL float getFloat(const char *key, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C038B
    DECL_EXPORT_DLL double getDouble(const char *key, const char *ns = "DEFAULT");

    // 读取字符串字段并返回地址
    //##ModelId=4AD97C4C038E
    DECL_EXPORT_DLL const char* getString(const char *key, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C0391
    DECL_EXPORT_DLL int getString(const char *key, char *val, int len, const char *NS = "DEFAULT");

    //##ModelId=4AD97C4C039D
    DECL_EXPORT_DLL int getBin(const char *key, char *val, int *len = NULL, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C03AB
    DECL_EXPORT_DLL int getBin(const char *key, char *val, int len, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C03B9
    DECL_EXPORT_DLL int getEncStr(const char *key, char* val, int nmax, const char *ns = "DEFAULT");

    //##ModelId=4AD97C4C03BE
    DECL_EXPORT_DLL int getEncStr(const char *key, char *val, int* len, const char *ns = "DEFAULT");

    // 按位置读出某个文件名
    //##ModelId=4AD97C4C03CA
    DECL_EXPORT_DLL const char* getFileName(int idx);

    //##ModelId=4AD97C4C03CC
    DECL_EXPORT_DLL int getField(const char* key, char* val, char* type, int* len, const char* ns = "DEFAULT");

    //##ModelId=4AD97C4C03DB
    DECL_EXPORT_DLL int getField(int idx, char* ns, char* key, char* val, char* type, int* len);

    //##ModelId=4AD97C4D0003
    DECL_EXPORT_DLL int getFileNames(char* files);

    // 返回总字段数目
    //##ModelId=4AD97C4D0005
    DECL_EXPORT_DLL u_short getFldNum() const;

    // 返回命名空间(按位置, 从0开始)的字段数目
    //##ModelId=4AD97C4D0007
    DECL_EXPORT_DLL u_short getNsFldNum(int nsidx) const;

    // 返回命名空间(按名字)的字段数目
    //##ModelId=4AD97C4D000F
    DECL_EXPORT_DLL u_short getNsFldNum(const char* ns) const;

    //##ModelId=4AD97C4D0012
    DECL_EXPORT_DLL int getFldIdx(const char *key, const char* ns = "DEFAULT");

    //##ModelId=4AD97C4D0015
    DECL_EXPORT_DLL int get1stFldIdx(const char *ns = "DEFAULT") const;

    //##ModelId=4AD97C4D0018
    DECL_EXPORT_DLL int get1stFldIdx(int nsidx) const;

    // 按位置找字段的命名空间，若位置不正确，则返回NULL
    //##ModelId=4AD97C4D001F
    DECL_EXPORT_DLL const char* getFldNs(int idx) const;

    // 按位置找字段的字段名，若位置不正确，则返回NULL
    //##ModelId=4AD97C4D0022
    DECL_EXPORT_DLL const char* getFldName(int idx) const;

    // 按位置找字段的字段类型, 若位置不正确, 则返回空字符'\0'.
    //##ModelId=4AD97C4D0025
    DECL_EXPORT_DLL const char getFldType(int idx) const;

    // 按位置找字段的字段长度, 若位置不正确, 则返回 0
    //##ModelId=4AD97C4D0028
    DECL_EXPORT_DLL int getFldLen(int idx) const;

    // 按位置找字段的字符串值, 若位置不正确, 则返回NULL
    //##ModelId=4AD97C4D002E
    DECL_EXPORT_DLL const char* getFldVal(int idx) const;

    //##ModelId=4AD97C4D0031
    DECL_EXPORT_DLL int getNsIdx(const char *ns = "DEFAULT") const;

    //##ModelId=4AD97C4D0034
    DECL_EXPORT_DLL int pack(char *buf, int *length);

    //##ModelId=4AD97C4D0037
    DECL_EXPORT_DLL int unpack(const char *buf);

    //##ModelId=4AD97C4D003E
    DECL_EXPORT_DLL void* getNsNames() const;

    // 返回总命名空间数目
    //##ModelId=4AD97C4D0040
    DECL_EXPORT_DLL u_short getNsNum() const;

    //##ModelId=4AD97C4D0042
    DECL_EXPORT_DLL int delField(const char* aFldName, const char* aNsName = "DEFAULT");

    //##ModelId=4AD97C4D0045
    DECL_EXPORT_DLL int cpNs(const char* aSrcNs = "DEFAULT", const char* aDstNs = NULL, CMyPkg* aSrcPkg = NULL);

    //##ModelId=4AD97C4D004E
    DECL_EXPORT_DLL int getFileNum();

    //##ModelId=4AD97C4D004F
    DECL_EXPORT_DLL int mvNs(const char* aSrcNs = "DEFAULT", const char* aDstNs = NULL, CMyPkg* aSrcPkg = NULL);

    //##ModelId=4AD97C4D0053
    DECL_EXPORT_DLL int delNs(const char* aNsName = "DEFAULT");

    //##ModelId=4AD97C4D0055
    DECL_EXPORT_DLL int cpField(const char* aSrcNs, const char* aSrcFld, const char* aDstNs, const char* aDstFld, char aDstType = 0, CMyPkg* aSrcPkg = NULL);

    //##ModelId=4AD97C4D0062
    DECL_EXPORT_DLL int parse(const char* buf, char* val, char* type, int* len);

    //##ModelId=4AD97C4D0070
    DECL_EXPORT_DLL void print(FILE* fp = stdout);

    //##ModelId=4AD97C4D0072
    DECL_EXPORT_DLL int getFldLen(const char* key, const char* nsname = "DEFAULT");

    // 获取包头数据空间长度
    //##ModelId=4AD97C4D007E
    DECL_EXPORT_DLL int getHeadSize() const;

    // 获取包体数据空间长度
    //##ModelId=4AD97C4D0080
    DECL_EXPORT_DLL int getDataSize() const;

    // 获取包头数据空间地址
    //##ModelId=4AD97C4D0082
    DECL_EXPORT_DLL const char* getHeadBuf() const;

    // 获取包体数据空间地址
    //##ModelId=4AD97C4D0084
    DECL_EXPORT_DLL const char* getDataBuf() const;

    // 获取当前包总体数据长度
    //##ModelId=4AD97C4D0086
    DECL_EXPORT_DLL int getPkgSize() const;

    //##ModelId=4AD97C4D0088
    DECL_EXPORT_DLL int exist(const char* key, const char* nsname = "DEFAULT");

    // "[]" 操作符重载方法
    //##ModelId=4AD97C4D008D
    DECL_EXPORT_DLL const char* operator[](const char* key);

    // "=" 操作符重载方法
    //##ModelId=4AD97C4D009C
    DECL_EXPORT_DLL CMyPkg& operator=(const CMyPkg& right);

  private:
  	
    //##ModelId=4AD97C4D009E
    int addNs(const char* aNsName = "DEFAULT");

    //##ModelId=4AD97C4D00A0
    int addField(int idx, const char* key, char nsidx, char type, int len, const char* val);

    //##ModelId=4AD97C4C0280
    short ErrCode;

    //##ModelId=4AD97C4C0281
    int HeadSize;

    //##ModelId=4AD97C4C0290
    int DataSize;

    //##ModelId=4AD97C4C0291
    char* HeadBuf;

    //##ModelId=4AD97C4C029F
    char* DataBuf;

    //##ModelId=4AD97C4C02C4
    char (* NsNames)[MP_SIZE_NSNAME];

    //##ModelId=4AD97D380000
    u_short FreeOffset;

    //##ModelId=4AD97D380001
    u_short NsNum;

    //##ModelId=4AD97D380002
    u_short NsFldNum[MP_MAX_NS];

    //##ModelId=4AD97D380003
    u_short FldNum;

    //##ModelId=4AD97C4C02C0
    SMyField *Fields;
};

#endif
