#ifndef INCL_MBADAPT_H_HEADER_INCLUDED_BDC5398A
#define INCL_MBADAPT_H_HEADER_INCLUDED_BDC5398A

#include <stdio.h>
#include <stdlib.h>
#include <mypub.h>

class CMbAdapt
{
  public:

	CMbAdapt(const char* aAdaptId);

    // 将适配器绑定到缺省工作流服务引擎
    void bindWFM(const char* aWfmId);

    // 装载适配器参数配置文件
    virtual int loadCfg() = 0;

    // 初始化对外网络通道
    virtual int initExtConn() = 0;

    // 往工作流发送请求并等待同步应答
    int sndReqToWfm(const char* aSvcName, void* aReqPkg, void* aRspPkg, 
					int aTimeout=0, const char* aWfmId=NULL);

    // 收到工作流请求后的自定义处理
    virtual int rcvReqFromWfm(void* aReqPkg, void* aRspPkg) = 0;

    // 往工作流发送异步应答
    int sndRspToWfm(const char* aSerialNo, const char* aActId, void* aRspMp);

    // 往外通道发送应答的自定义处理
    virtual int sndRspToOutter(void* aRspPkg) = 0;

	inline const char* getAdaptId() { return AdaptId; };

	inline const char* getWfmId() { return WfmId; };

  protected:

	char AdaptId[32];

	char WfmId[32];

};

#endif 
