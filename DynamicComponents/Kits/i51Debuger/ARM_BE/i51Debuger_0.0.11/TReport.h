
#ifndef THE_TReport_H
#define THE_TReport_H

#include "i51.h"
typedef enum {
	
	i51_CASE_ADAPTER = 0 ,
	i51_CASE_KERNEL,
	i51_CASE_BENCHMARK ,
	i51_CASE_INTER ,
	i51_CASE_ENVIR ,
} CASETYPE ;

typedef struct {
	
	iU16 type ;
	//	iTRUE、iFALSE
	iU16 results ;

} CASE ;

#if 0
#ifdef WIN32
#pragma pack(1)
#else
__align(1)
#endif
#endif

typedef struct {
	
	//	author : jelo
	//	since : 2012.4.10
	//	PKIG Tech. Co., Ltd.
	
	//	"TR"
       iCHAR Magic [2] ;
	iU32 Version ;
	iU32 TotallCases ;
	iU32 CaseEntry ;

	struct {
		
		//	报告生成时间

		iU16 Year ;
		iU16 Month ;
		iU16 Day ;
		iU16 Hour ;
		iU16 Minute ;
		iU16 Second ;

	} CreateTime ;

	struct {

		//	手机信息

		//	大小端
		iS8 Endian ;
		//	Kernel版本号
		iU32 KernelVersion ;
		//	Adapter版本号
		iU32 AdapterVersion ;
		//	项目id
		iU32 Projectid ;
              //  客户 id
              iU32 CustomerId;
		//	平台识别串
		iCHAR Platform[64] ;
              //  分辨率
              iCHAR Resolution[10];
		//	SN
		iU8 Sn[16] ;
		//	imei
		iCHAR IMEI[32] ;
		//	移植批号
		iU8 BatchCode[16] ;
		//	剩余磁盘空间
		iU32 DiskFreeSpace ;
		//	剩余内存空间	
		iU32 MemoryAvalible ;
	
	} PhoneInfo ;

	struct {

		//	联网SIM卡的各类信息
	
		//	imsi
		iCHAR IMSI[32] ;
		//	短信中心号
		iCHAR IMSC[32] ;
		//	基站id
		iU32 bid ;
		//	基站区域id
		iU32 zid ;

	} Sim ;

	struct {

		//	短信发送总次数
		iU32 sendsmstotall ;
		//	发送成功的次数
		iU32 succsmstotall ;
		//	短信接收条数
		iU32 hookedtotall ;
		//	短信拦截次数
		iU32 Intercepted ;
		//	短信拦截并回复的次数
		iU32 Interceptedsend ;

	} KitServerInfo ;

	struct {
		
		//	各项性能指标
		iU16 Integer ;
		iU16 Float ;
		iU16 FSO ;
		iU16 FsSeek ;
		iU16 FsWrite ;
		iU16 FsRead ;
		iU16 Memory ;
		iU16 Graphics ;
		iU16 Image ;
		iU16 Font ;
		iU16 Sound ;
		iU16 UPS ;
		iU16 TotallCost ;

	} Benchmark ;
    
    struct {
        
        //Dotpay 版本
        iU32 DotpayVersion ;
        //两次DotPay初始化的时间
        iU32 InitTime1;
        iU32 InitTime2;
    }DotpayInfo;

} TREPORT ;

//#pragma pack()

iBOOL i51KitTReportCreateHeader ( iFILE file );
iBOOL i51KitTReportAddCase ( iFILE file , iU32 casetype , iBOOL results );
iBOOL i51KitTReportReadHeader ( iFILE file , TREPORT* report );
void i51UpdataReport(iU8 para,TREPORT report);
#endif
