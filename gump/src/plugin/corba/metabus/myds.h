#ifndef INCLUDE_MYDS_H_HEADER_INCLUDED_B7D616C4
#define INCLUDE_MYDS_H_HEADER_INCLUDED_B7D616C4

#include <stdio.h>
#include <stdlib.h>
#include "mypub.h"

#ifdef __WIN32__
#include <winsock2.h>
#else
#include <sys/types.h>
#endif

#define	MD_T_ASCII		'S'		// 普通字符串
#define	MD_T_ENCSTR		'E'     // 加密字符串，读出时需先解密
#define	MD_T_BIN		'X'		// 二进制字符串

#define MD_FMT_WIDTH	'W'		// 固定宽度文件格式
#define MD_FMT_DLMT		'D'		// 分隔符文件格式
#define MD_SIZE_COLNAME	24		// 数据集列名最大长度

#define MD_MAX_COL_NUM	256		// 数据集最大列数
#define MD_MAX_ROW_SIZE	8192	// 数据集单行数据最大长度

//##ModelId=4AD97E8A03A9
typedef char* pchar;

//##ModelId=4AD97E8A035B
class CFileDsR
{
  public:
    // 构造方法
    //##ModelId=4AD97E8B0022
    DECL_EXPORT_DLL CFileDsR();

    // 析构方法
    //##ModelId=4AD97E8B002E
    DECL_EXPORT_DLL ~CFileDsR();

    // 返回上次操作的出错码
    //##ModelId=4AD97E8B002F
    DECL_EXPORT_DLL const char* getErrMsg();

    //##ModelId=4AD97E8B0030
    int getInt(int aColNo);

    //##ModelId=4AD97E8B0032
    int getInt(const char* aColName);

    // 读取无符号整型字段
    //##ModelId=4AD97E8B0034
    DECL_EXPORT_DLL u_int getUInt(int aColNo);

    //##ModelId=4AD97E8B0036
    DECL_EXPORT_DLL u_int getUInt(const char* aColName);

    //##ModelId=4AD97E8B003E
    short getShort(int aColNo);

    //##ModelId=4AD97E8B0040
    short getShort(const char* aColName);

    // 读取无符号短整型字段
    //##ModelId=4AD97E8B0042
    DECL_EXPORT_DLL u_short getUShort(int aColNo);

    //##ModelId=4AD97E8B0044
    DECL_EXPORT_DLL u_short getUShort(const char* aColName);

    //##ModelId=4AD97E8B0046
    long getLong(int aColNo);

    //##ModelId=4AD97E8B004E
    long getLong(const char* aColName);

    // 插入无符号长整型字段
    //##ModelId=4AD97E8B0050
    DECL_EXPORT_DLL u_long getULong(int aColNo);

    //##ModelId=4AD97E8B0052
    DECL_EXPORT_DLL u_long getULong(const char* aColName);

    //##ModelId=4AD97E8B0054
    DECL_EXPORT_DLL float getFloat(int aColNo);

    //##ModelId=4AD97E8B0056
    DECL_EXPORT_DLL float getFloat(const char* aColName);

    //##ModelId=4AD97E8B005D
    DECL_EXPORT_DLL double getDouble(int aColNo);

    //##ModelId=4AD97E8B005F
    DECL_EXPORT_DLL double getDouble(const char* aColName);

    // 读取字符串字段
    //##ModelId=4AD97E8B0061
    DECL_EXPORT_DLL const char* getString(int aColNo);

    //##ModelId=4AD97E8B0063
    DECL_EXPORT_DLL const char* getString(const char* aColName);

    //##ModelId=4AD97E8B006D
    DECL_EXPORT_DLL int setFixWidthFmt(int aColNum, int aRowNum = -1, 
    							const char* aRD = "\n");

    //##ModelId=4AD97E8B0071
    DECL_EXPORT_DLL int setDlmtFmt(int aColNum, int aRowNum = -1, 
    					const char* aFD = "|", const char* aRD = "\n", 
    					const char* aQuote = "\"");

    //##ModelId=4AD97E8B007F
    DECL_EXPORT_DLL int setColDef(int aIdx, const char* aColName, int aColLen = 0);

    //##ModelId=4AD97E8B008C
    DECL_EXPORT_DLL int open(const char *aFilename, bool aReadTitle = false, int aSkipLines = 0);

    //##ModelId=4AD97E8B0090
    DECL_EXPORT_DLL void close();

    //##ModelId=4AD97E8B0091
    DECL_EXPORT_DLL int fetchTitle();

    //##ModelId=4AD97E8B0092
    DECL_EXPORT_DLL int fetchRow();

    // 跳过一个文件数据行
    // 返回值：读出的字符串指针，无数据则为NULL
    //##ModelId=4AD97E8B0093
    DECL_EXPORT_DLL const char* skipOneLine();

    // 返回列名
    //##ModelId=4AD97E8B0094
    DECL_EXPORT_DLL const char* getColName(int pos) const;

    //##ModelId=4AD97E8B009C
    DECL_EXPORT_DLL int getColLen(int pos) const;

    //##ModelId=4AD97E8B009F
    DECL_EXPORT_DLL int getColNum() const;

    //##ModelId=4AD97E8B00A1
    DECL_EXPORT_DLL int getRowNum() const;

  private:
    // 定义数据集的总列数
    //##ModelId=4AD97E8B00A3
    int setColNum(int aColNum);

    //##ModelId=4AD97E8A03B9
    FILE* Fp;

    //##ModelId=4AD97E8A03BA
    short ErrCode;

    //##ModelId=4AD97E8A03D8
    short* ColLens;

    //##ModelId=4AD97E8A03D9
    char* DataBuf;

    //##ModelId=4AD97E8B0000
    char FDStr[4];

    //##ModelId=4AD97E8B0001
    int FDLen;

    //##ModelId=4AD97E8B0002
    char RDStr[4];

    //##ModelId=4AD97E8B000F
    int RDLen;

    //##ModelId=4AD97E8B0010
    char QuoteStr[4];

    //##ModelId=4AD97E8B0011
    int QuoteLen;

    //##ModelId=4AD97E8B0012
    char FileFmt;

    //##ModelId=4AD97E8B001F
    short ColNum;

    //##ModelId=4AD97E8B0020
    int RowNum;

    //##ModelId=4AD97E8B0021
    int CurRow;

    //##ModelId=4AD97E8A03C9
    pchar *ColNames;

    //##ModelId=4AD97E8A03CE
    pchar *ColValues;

};

//##ModelId=4AD97E8A037A
class CFileDsW
{
  public:
    //## 构造方法
    //##ModelId=4AD97E8B00EC
    DECL_EXPORT_DLL CFileDsW();

    //## 析构方法
    //##ModelId=4AD97E8B00ED
    DECL_EXPORT_DLL ~CFileDsW();

    //## 返回上次操作的出错码
    //##ModelId=4AD97E8B00EE
    DECL_EXPORT_DLL const char* getErrMsg();

    //##ModelId=4AD97E8B00EF
    DECL_EXPORT_DLL int setColVal(int aColIdx, int aVal);

    //##ModelId=4AD97E8B00FB
    DECL_EXPORT_DLL int setColVal(const char* aColName, int aVal);

    //##ModelId=4AD97E8B00FE
    DECL_EXPORT_DLL int setColVal(int aColIdx, u_int aVal);

    //##ModelId=4AD97E8B0101
    DECL_EXPORT_DLL int setColVal(const char* aColName, u_int aVal);

    //##ModelId=4AD97E8B010B
    DECL_EXPORT_DLL int setColVal(int aColIdx, short aVal);

    //##ModelId=4AD97E8B010E
    DECL_EXPORT_DLL int setColVal(const char* aColName, short aVal);

    //##ModelId=4AD97E8B0119
    DECL_EXPORT_DLL int setColVal(int aColIdx, u_short aVal);

    //##ModelId=4AD97E8B011C
    DECL_EXPORT_DLL int setColVal(const char* aColName, u_short aVal);

    //##ModelId=4AD97E8B011F
    DECL_EXPORT_DLL int setColVal(int aColIdx, long aVal);

    //##ModelId=4AD97E8B0129
    DECL_EXPORT_DLL int setColVal(const char* aColName, long aVal);

    //##ModelId=4AD97E8B012C
    DECL_EXPORT_DLL int setColVal(int aColIdx, u_long aVal);

    //##ModelId=4AD97E8B0138
    DECL_EXPORT_DLL int setColVal(const char* aColName, u_long aVal);

    //##ModelId=4AD97E8B013B
    DECL_EXPORT_DLL int setColVal(int aColIdx, float aVal);

    //##ModelId=4AD97E8B013E
    DECL_EXPORT_DLL int setColVal(const char* aColName, float aVal);

    //##ModelId=4AD97E8B0149
    DECL_EXPORT_DLL int setColVal(int aColIdx, double aVal);

    //##ModelId=4AD97E8B014C
    DECL_EXPORT_DLL int setColVal(const char* aColName, double aVal);

    //##ModelId=4AD97E8B0157
    DECL_EXPORT_DLL int setColVal(int aColIdx, const char *aVal);

    //##ModelId=4AD97E8B015A
    DECL_EXPORT_DLL int setColVal(const char* aColName, const char* aVal);

    //##ModelId=4AD97E8B015D
    DECL_EXPORT_DLL int setFixWidthFmt(int aColNum, int aRowNum = -1, const char* aRD = "\n");

    //##ModelId=4AD97E8B0168
    DECL_EXPORT_DLL int setDlmtFmt(int aColNum, int aRowNum = -1, const char* aFD = "|", const char* aRD = "\n", const char* aQuote = "\"");

    //##ModelId=4AD97E8B0178
    DECL_EXPORT_DLL int setColDef(int aIdx, const char* aColName, int aColLen = 0);

    //##ModelId=4AD97E8B017C
    DECL_EXPORT_DLL int open(const char *aFileName, bool aWriteTitle = false);

    //##ModelId=4AD97E8B0186
    DECL_EXPORT_DLL void close();

    //##ModelId=4AD97E8B0187
    DECL_EXPORT_DLL int postTitle();

    //##ModelId=4AD97E8B0188
    DECL_EXPORT_DLL int postRow();

  private:
    // 定义数据集的总列数
    //##ModelId=4AD97E8B0189
    int setColNum(int aColNum);

    //##ModelId=4AD97E8B00AB
    FILE* Fp;

    //##ModelId=4AD97E8B00AC
    short ErrCode;

    //##ModelId=4AD97E8B00BB
    short* ColLens;

    //##ModelId=4AD97E8B00BC
    char* DataBuf;

    //##ModelId=4AD97E8B00BD
    short FreeOffset;

    //##ModelId=4AD97E8B00CB
    char FDStr[4];

    //##ModelId=4AD97E8B00CC
    int FDLen;

    //##ModelId=4AD97E8B00CD
    char QuoteStr[4];

    //##ModelId=4AD97E8B00CE
    char RDStr[4];

    //##ModelId=4AD97E8B00DA
    int RDLen;

    //##ModelId=4AD97E8B00DB
    int QuoteLen;

    //##ModelId=4AD97E8B00DC
    char FileFmt;

    //##ModelId=4AD97E8B00DD
    short ColNum;

    //##ModelId=4AD97E8B00EA
    int RowNum;

    //##ModelId=4AD97E8B00EB
    int CurRow;

    //##ModelId=4AD97E8B00AE
    pchar *ColNames;

    //##ModelId=4AD97E8B00B3
    pchar *ColValues;

};

//##ModelId=4AD97E8A038A
class CMemDsR
{
  public:
    //构造方法
    //##ModelId=4AD97E8B01C8
    DECL_EXPORT_DLL CMemDsR();

    //## 析构方法
    //##ModelId=4AD97E8B01D4
    DECL_EXPORT_DLL ~CMemDsR();

    //## 返回上次操作的出错码
    //##ModelId=4AD97E8B01D5
    DECL_EXPORT_DLL const char* getErrMsg();

    //##ModelId=4AD97E8B01D6
    DECL_EXPORT_DLL int getInt(int aFldNo);

    //##ModelId=4AD97E8B01D8
    DECL_EXPORT_DLL int getInt(const char* aFldName);

    //读取无符号整型字段
    //##ModelId=4AD97E8B01DA
    DECL_EXPORT_DLL u_int getUInt(int aFldNo);

    //##ModelId=4AD97E8B01DC
    DECL_EXPORT_DLL u_int getUInt(const char* aFldName);

    //##ModelId=4AD97E8B01E4
    DECL_EXPORT_DLL short getShort(int aFldNo);

    //##ModelId=4AD97E8B01E6
    DECL_EXPORT_DLL short getShort(const char* aFldName);

    //## 读取无符号短整型字段
    //##ModelId=4AD97E8B01E8
    DECL_EXPORT_DLL u_short getUShort(int aFldNo);

    //##ModelId=4AD97E8B01EA
    DECL_EXPORT_DLL u_short getUShort(const char* aFldName);

    //##ModelId=4AD97E8B01EC
    DECL_EXPORT_DLL long getLong(int aFldNo);

    //##ModelId=4AD97E8B01F4
    DECL_EXPORT_DLL long getLong(const char* aFldName);

    //## 读取无符号长整型字段
    //##ModelId=4AD97E8B01F6
    DECL_EXPORT_DLL u_long getULong(int aFldNo);

    //##ModelId=4AD97E8B01F8
    DECL_EXPORT_DLL u_long getULong(const char* aFldName);

    //##ModelId=4AD97E8B01FA
    DECL_EXPORT_DLL float getFloat(int aFldNo);

    //##ModelId=4AD97E8B01FC
    DECL_EXPORT_DLL float getFloat(const char* aFldName);

    //##ModelId=4AD97E8B0203
    DECL_EXPORT_DLL double getDouble(int aFldNo);

    //##ModelId=4AD97E8B0205
    DECL_EXPORT_DLL double getDouble(const char* aFldName);

    //## 读取字符串字段
    //##ModelId=4AD97E8B0207
    DECL_EXPORT_DLL const char* getString(int aFldNo);

    //##ModelId=4AD97E8B0209
    DECL_EXPORT_DLL const char* getString(const char* aFldName);

    //##ModelId=4AD97E8B0213
    DECL_EXPORT_DLL int open(const char *aBuf, int aLen);

    //##ModelId=4AD97E8B0216
    DECL_EXPORT_DLL int fetch();

    //##ModelId=4AD97E8B0217
    DECL_EXPORT_DLL const char* getColName(int pos) const;

    //##ModelId=4AD97E8B021A
    DECL_EXPORT_DLL const char getColType(int pos) const;

    //##ModelId=4AD97E8B021D
    DECL_EXPORT_DLL int getColLen(int pos) const;

    //##ModelId=4AD97E8B0222
    DECL_EXPORT_DLL int getRowNum() const;

    //##ModelId=4AD97E8B0224
    DECL_EXPORT_DLL int getColNum() const;

  private:
    //##ModelId=4AD97E8B018B
    short ErrCode;

    //##ModelId=4AD97E8B0196
    const char* SrcBuf;

    //##ModelId=4AD97E8B0197
    int SrcLen;

    //##ModelId=4AD97E8B0198
    const char* BufPtr;

    // 值分类：
    // 'S'   普通字符串
    // 'E'   加密字符串，读出时需先解密
    // 'X'   二进制字符串，读出时需先还原为二进制码
    //##ModelId=4AD97E8B01A5
    char* ColTypes;

    //##ModelId=4AD97E8B01A6
    short* ColLens;

    //##ModelId=4AD97E8B01A7
    char* NameBuf;

    //##ModelId=4AD97E8B01B5
    char* DataBuf;

    //##ModelId=4AD97E8B01B6
    char FDChar;

    //##ModelId=4AD97E8B01B7
    char RDChar;

    //##ModelId=4AD97E8B01B8
    int ColNum;

    //##ModelId=4AD97E8B01C5
    int RowNum;

    //##ModelId=4AD97E8B01C6
    int CurCol;

    //##ModelId=4AD97E8B01C7
    int CurRow;

    //##ModelId=4AD97E8B019A
    pchar *ColNames;

    //##ModelId=4AD97E8B019F
    pchar *ColValues;

};

//##ModelId=4AD97E8A0399
class CMemDsW
{
  public:
    // 构造方法
    //##ModelId=4AD97E8B0237
    DECL_EXPORT_DLL CMemDsW();

    // 析构方法
    //##ModelId=4AD97E8B0238
    DECL_EXPORT_DLL ~CMemDsW();

    // 返回上次操作的出错码
    //##ModelId=4AD97E8B0239
    DECL_EXPORT_DLL const char* getErrMsg();

    //##ModelId=4AD97E8B023A
    DECL_EXPORT_DLL int addInt(int val);

    //##ModelId=4AD97E8B023C
    DECL_EXPORT_DLL int addUInt(u_int val);

    //##ModelId=4AD97E8B023E
    DECL_EXPORT_DLL int addShort(short val);

    //##ModelId=4AD97E8B0243
    DECL_EXPORT_DLL int addUShort(u_short val);

    //##ModelId=4AD97E8B0245
    DECL_EXPORT_DLL int addLong(long val);

    //##ModelId=4AD97E8B0247
    DECL_EXPORT_DLL int addULong(u_long val);

    //##ModelId=4AD97E8B0249
    DECL_EXPORT_DLL int addFloat(float val);

    //##ModelId=4AD97E8B024B
    DECL_EXPORT_DLL int addDouble(double val);

    //##ModelId=4AD97E8B024D
    DECL_EXPORT_DLL int addString(const char *val);

    //## 读取最后生成的数据集内存块
    //##ModelId=4AD97E8B024F
    DECL_EXPORT_DLL const char* getBufData() const;

    //##ModelId=4AD97E8B0251
    DECL_EXPORT_DLL int getBufLen() const;

    //##ModelId=4AD97E8B0253
    DECL_EXPORT_DLL int open(char aFD = '\x01', char aRD = '\n');

    //##ModelId=4AD97E8B0256
    DECL_EXPORT_DLL int addColName(const char* aColName, const char aType = MD_T_ASCII, int aLen = 0);

    //##ModelId=4AD97E8B025A
    DECL_EXPORT_DLL int post();

  private:
    //##ModelId=4AD97E8B0226
    short ErrCode;

    //##ModelId=4AD97E8B0227
    char* BufPtr;

    //##ModelId=4AD97E8B0228
    char* DataBuf;

    //##ModelId=4AD97E8B0229
    char FDChar;

    //##ModelId=4AD97E8B0232
    char RDChar;

    //##ModelId=4AD97E8B0233
    int RowNum;

    //##ModelId=4AD97E8B0234
    int ColNum;

    //##ModelId=4AD97E8B0235
    int CurCol;

    //##ModelId=4AD97E8B0236
    int CurRow;

};

#endif /* INCLUDE_MYDS_H_HEADER_INCLUDED_B7D616C4 */
