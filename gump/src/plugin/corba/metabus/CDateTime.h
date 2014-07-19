#ifndef INCLUDE_PUBLIC_CDATETIME_H_HEADER_INCLUDED_C15AA25E
#define INCLUDE_PUBLIC_CDATETIME_H_HEADER_INCLUDED_C15AA25E

#include <mypub.h>
#include <time.h>
#include <sys/timeb.h>

//##ModelId=4AD9804C01D4
class CDateTime
{
  public:
  	
    // 中文名称：
    //   缺省构造函数
    // 功能：
    //   缺省构造函数
    //##ModelId=4AD9804C0253
    DECL_EXPORT_DLL CDateTime();

    // 中文名称：
    //   构造函数
    // 功能：
    //   以复制另一个对象的方式构造对象
    //##ModelId=4AD9804C0254
    DECL_EXPORT_DLL CDateTime(CDateTime& right);

    // 中文名称：
    //   构造函数
    // 功能：
    //   以字符串时间构造对象，格式:
    //       1. YYYYMMDD:hhnnssmmm
    //       2. YYYYMMDD
    //       3. YYYY/MM/DD
    //       4. YYYY-MM-DD
    //##ModelId=4AD9804C0256
    DECL_EXPORT_DLL CDateTime(const char* str);

    // 中文名称：
    //   构造函数
    // 功能：
    //   以日期值和时间值构造对象，日期值为YYYYMMDD，时间值为hhnnssmmm"
    //##ModelId=4AD9804C0258
    DECL_EXPORT_DLL CDateTime(long date, long time);

	// 计算从指定日期到本日期之间的天数
    //##ModelId=4AD9804C025B
    DECL_EXPORT_DLL long calDaysFrom(CDateTime& from);

	// 计算从指定时间到本时间之间的毫秒数
	//##ModelId=4AD9804C025D
    DECL_EXPORT_DLL long calMSecsFrom(CDateTime& from);

	// 计算银行天数
    //##ModelId=4AD9804C025F
    DECL_EXPORT_DLL int calBankDays(CDateTime& from);

	// 计算指定年份的天数
    //##ModelId=4AD9804C0261
    DECL_EXPORT_DLL static int getDaysOfYear(int year);

	// 计算指定年月的天数
    //##ModelId=4AD9804C0264
    DECL_EXPORT_DLL static int getDaysOfMonth(int year, int month);

	// 判断指定日期是否闰年
    //##ModelId=4AD9804C0268
    DECL_EXPORT_DLL static bool isLeapYear(int aYear);

    // 中文名称：
    //   调整日期
    // 功能：
    //   将不合法的日期调整为合法日期
    //##ModelId=4AD9804C026B
    DECL_EXPORT_DLL CDateTime& toValidDate();

    // 中文名称：
    //     设定日期
    // 功能：
    //     以字符串设定日期，格式:
    //       1. YYYYMMDD
    //       2. YYYY/MM/DD
    //       3. YYYY-MM-DD
    //##ModelId=4AD9804C026C
    DECL_EXPORT_DLL CDateTime& setDateStr(const char* aStr);

    // 中文名称：
    //     设定日期
    // 功能：
    //     以字符串设定日期，格式: YYYYMMDD
    //##ModelId=4AD9804C026E
    DECL_EXPORT_DLL CDateTime& setDecDate(long aDate);

    // 中文名称：
    //     设定时间
    // 功能：
    //     以字符串设定时间，格式:
    //       1. HHNNSS
    //       2. HH:NN:SS
    //       3. HHNNSSMMM
    //       3. HH:NN:SS:MMM
    //##ModelId=4AD9804C0270
    DECL_EXPORT_DLL CDateTime& setTimeStr(const char* aStr);

    // 中文名称：
    //     设定时间
    // 功能：
    //     以字符串设定时间，格式: HHNNSS
    //##ModelId=4AD9804C0272
    DECL_EXPORT_DLL CDateTime& setDecTime(long aTime);

    // 中文名称：
    //   设为当前日期
    // 功能：
    //   将本对象设为当前机器日期和时间
    //##ModelId=4AD9804C0274
    DECL_EXPORT_DLL CDateTime& setToNow();

    // 中文名称：
    //   增加日数
    // 功能：
    //   在本日期的基础上增加指定的日数
    //##ModelId=4AD9804C0275
    DECL_EXPORT_DLL CDateTime& addDay(int days);

    // 中文名称：
    //   增加月数
    // 功能：
    //   在本日期的基础上增加指定的月数
    //##ModelId=4AD9804C0277
    DECL_EXPORT_DLL CDateTime& addMonth(int months);

    // 中文名称：
    //   增加年数
    // 功能：
    //   在本日期的基础上增加指定的年数
    //##ModelId=4AD9804C0279
    DECL_EXPORT_DLL CDateTime& addYear(int years);

    // 中文名称：
    //   增加小时数
    // 功能：
    //   在本日期的基础上增加指定的小时数
    //##ModelId=4AD9804C027B
    DECL_EXPORT_DLL CDateTime& addHour(int hours);

    // 中文名称：
    //   增加分钟数
    // 功能：
    //   在本日期的基础上增加指定的分钟数
    //##ModelId=4AD9804C027D
    DECL_EXPORT_DLL CDateTime& addMin(int minutes);

    // 中文名称：
    //   增加秒数
    // 功能：
    //   在本日期的基础上增加指定的秒数
    //##ModelId=4AD9804C027F
    DECL_EXPORT_DLL CDateTime& addSec(int seconds);

    // 中文名称：
    //   增加毫秒数
    // 功能：
    //   在本日期的基础上增加指定的毫秒数
    //##ModelId=4AD9804C0281
    DECL_EXPORT_DLL CDateTime& addMS(int mseconds);

    // 中文名称：
    //   设置年份号
    // 功能：
    //   设置本日期的年份值
    //##ModelId=4AD9804C0283
    DECL_EXPORT_DLL CDateTime& setYear(short left);

    // 中文名称：
    //   设置月份号
    // 功能：
    //   设置本日期的月份值
    //##ModelId=4AD9804C0285
    DECL_EXPORT_DLL CDateTime& setMonth(short left);

    // 中文名称：
    //   设置日号
    // 功能：
    //   设置本日期的日号值
    //##ModelId=4AD9804C0287
    DECL_EXPORT_DLL CDateTime& setDay(short left);

    // 中文名称：
    //   设置时点数
    // 功能：
    //   设置本时间的小时值
    //##ModelId=4AD9804C0289
    DECL_EXPORT_DLL CDateTime& setHour(short left);

    // 中文名称：
    //   设置分钟数
    // 功能：
    //   设置本时间的分钟值
    //##ModelId=4AD9804C028B
    DECL_EXPORT_DLL CDateTime& setMin(short left);

    // 中文名称：
    //   设置秒数
    // 功能：
    //   设置本时间的秒值
    //##ModelId=4AD9804C028D
    DECL_EXPORT_DLL CDateTime& setSec(short left);

    // 中文名称：
    //   设置毫秒数
    // 功能：
    //   设置本时间的毫秒值
    //##ModelId=4AD9804C028F
    DECL_EXPORT_DLL CDateTime& setMS(short left);

	// 判断本日期是否闰年
    //##ModelId=4AD9804C0291
    DECL_EXPORT_DLL bool isLeapYear();

    // 中文名称：
    //   获取日期数字值
    // 功能：
    //   获取本日期的数字值，格式为YYYYMMDD
    //##ModelId=4AD9804C0292
    DECL_EXPORT_DLL long getDecDate();

    // 中文名称：
    //   获取时间数字值
    // 功能：
    //   获取本时间的数字值，格式为hhnnssmmm
    //##ModelId=4AD9804C0293
    DECL_EXPORT_DLL long getDecTime();

    // 中文名称：
    //   获取日期字符串
    // 功能：
    //   获取本日期的字符串，格式为YYYYMMDD
    //##ModelId=4AD9804C0294
    DECL_EXPORT_DLL const char* getDateStr();

    // 中文名称：
    //   获取时间字符串
    // 功能：
    //   获取本时间的字符串，格式为hhnnssmmm
    //##ModelId=4AD9804C0295
    DECL_EXPORT_DLL const char* getTimeStr();

    // 中文名称：
    //   获取日期时间字符串
    // 功能：
    //   获取本日期时间的字符串，格式为YYYYMMDD:hhnnssmmm
    //##ModelId=4AD9804C0296
    DECL_EXPORT_DLL const char* getDateTimeStr();

	// 返回日期的日号值
    //##ModelId=4AD9804C0297
    DECL_EXPORT_DLL short getDay();

	// 返回日期的月份值
    //##ModelId=4AD9804C0298
    DECL_EXPORT_DLL short getMonth();

	// 返回日期的年份值
    //##ModelId=4AD9804C0299
    DECL_EXPORT_DLL short getYear();

	// 返回时间的小时值
    //##ModelId=4AD9804C029A
    DECL_EXPORT_DLL short getHour();

	// 返回时间的分钟值
    //##ModelId=4AD9804C029B
    DECL_EXPORT_DLL short getMin();

	// 返回时间的秒值
    //##ModelId=4AD9804C029C
    DECL_EXPORT_DLL short getSec();

	// 返回时间的毫秒值
    //##ModelId=4AD9804C029D
    DECL_EXPORT_DLL short getMS();

	// 返回日期值，从1899/12/31开始的天数
    //##ModelId=4AD9804C029E
    DECL_EXPORT_DLL long getDateVal() const;

	// 返回时间值，从00:00:00:000开始的毫秒数
    //##ModelId=4AD9804C02A0
    DECL_EXPORT_DLL long getTimeVal() const;

	// 设定日期值，从1899/12/31开始的天数
    //##ModelId=4AD9804C02A2
    DECL_EXPORT_DLL CDateTime& setDateVal(long aDateVal);

	// 设定时间值，从00:00:00:000开始的毫秒数
    //##ModelId=4AD9804C02A4
    DECL_EXPORT_DLL CDateTime& setTimeVal(long aTimeVal);

	// 相等比较符
    //##ModelId=4AD9804C02A6
    DECL_EXPORT_DLL bool operator==(CDateTime& right);

	// 不等于比较符
    //##ModelId=4AD9804C02A8
    DECL_EXPORT_DLL bool operator!=(CDateTime& right);

	// 小于比较符
    //##ModelId=4AD9804C02AA
    DECL_EXPORT_DLL bool operator<(CDateTime& right);

	// 小于等于比较符
    //##ModelId=4AD9804C02AC
    DECL_EXPORT_DLL bool operator<=(CDateTime& right);

	// 大于比较符
    //##ModelId=4AD9804C02AE
    DECL_EXPORT_DLL bool operator>(CDateTime& right);

	// 大于等于比较符
    //##ModelId=4AD9804C02B0
    DECL_EXPORT_DLL bool operator>=(CDateTime& right);

	// 赋值操作符
    //##ModelId=4AD9804C02B2
    DECL_EXPORT_DLL CDateTime& operator=(CDateTime& right);

	// 返回操作系统时间值，从1970/1/1开始的秒数
    //##ModelId=4AD9804C02B4
    DECL_EXPORT_DLL long getOSTime();
    
	// 返回本日期的星期值
    DECL_EXPORT_DLL int getWeekDay() const;

  private:
  	
    //##ModelId=4AD9804C02B6
    void syncDateTime();

    //##ModelId=4AD9804C02B7
    void syncVal();

    //##ModelId=4AD9804C0232
    short Year;

    //##ModelId=4AD9804C0233
    short Month;

    //##ModelId=4AD9804C0234
    short Day;

    //##ModelId=4AD9804C0235
    short Hour;

    //##ModelId=4AD9804C0242
    short Min;

    //##ModelId=4AD9804C0243
    short Sec;

    //##ModelId=4AD9804C0244
    short Ms;

    //##ModelId=4AD9804C0245
    long DateVal;

    //##ModelId=4AD9804C0246
    long TimeVal;

    //##ModelId=4AD9804C0247
    char StrDT[19];

    //##ModelId=4AD9804C0248
    char StrDate[9];

    //##ModelId=4AD9804C0249
    char StrTime[10];

    //##ModelId=4AD9804C0251
    bool ValChanged;

    //##ModelId=4AD9804C0252
    short DstFlag;

};

#endif /* INCLUDE_PUBLIC_CDATETIME_H_HEADER_INCLUDED_C15AA25E */
