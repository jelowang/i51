  
/*

	The Implementing of 'Adapter' , Module : 'MMI'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Api.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/
#include "i51.h"
#include "i51AdeCnf.h"
#include "i51Scripts.h"
#include "i51_define.h"

//	子版本号，MTK、MSTAR、SPRD、或其他平台小组用于跟踪问题的版本号。
//	该版本号的变化一搬发生在修改BUG后。
#define i51_ADE_CNF_IMP_VERSION "0"
#define i51_ADE_CNF_MAKE_VERSION(CNFV,MODV) _i51_ADE_CNF_MAKE_VERSION(CNFV,MODV)
#define _i51_ADE_CNF_MAKE_VERSION(CNFV,MODV) CNFV##MODV
//#define i51_ADE_CNF_MAKE_VERSION(CNFV,MODV) CNFV##MODV

iU32 i51AdeCnfGetType ()
{

	//	author : jelo
	//	since : 20111012
	//	PKIG Tech. Co., Ltd.

	//	获取适配器实现类型
	//	目前有三种类型：MTK、MSTAR、SPRD
	
	return i51_ADE_CNF_VER_SPRD ;
	
}

iU32 i51AdeCnfGetVersion ()
{

	//	author : jelo
	//	since : 20111017
	//	PKIG Tech. Co., Ltd.

	//	返回Adapter版本数值形式
	
	return i51KernelFormatVersion(i51_ADE_CNF_MAKE_VERSION(i51_ADE_CNF_VERSION,i51_ADE_CNF_IMP_VERSION)) ;

}

iCHAR* i51AdeCnfGetVersionEx ()
{

	//	author : jelo
	//	since : 20111018
	//	PKIG Tech. Co., Ltd.

	//	返回Adapter版本字符串形式
		
	return i51_ADE_CNF_MAKE_VERSION(i51_ADE_CNF_VERSION,i51_ADE_CNF_IMP_VERSION) ;

}

iU32 i51AdeCnfGetProjectId ()
{

	//	author : jelo
	//	since : 20111030
	//	PKIG Tech. Co., Ltd.

	//	获取项目ID
	
	return i51_ADE_CNF_PROJECT ;
	
}

iU32 i51AdeCnfGetCustomerId ()
{

	//	author : jelo
	//	since : 20111030
	//	PKIG Tech. Co., Ltd.

	//	获取客户ID
	
	return i51_ADE_CNF_CUSTOMER ;

}

iCHAR * i51AdeCnfGetPlatformId ()
{

	//	author : jelo
	//	since : 20111018
	//	PKIG Tech. Co., Ltd.

	//	获取平台识别号
		
	return i51_ADE_CNF_PLATFORM ;

}

iU32 i51AdeCnfGetEndian ()
{

	//	author : jelo
	//	since : 20111018
	//	PKIG Tech. Co., Ltd.

	//	返回系统大小端模式

	#ifndef SPRD_6530
	return i51_ADE_CNF_BIG_ENDIAN ;
	#else
	return i51_ADE_CNF_LITTEL_ENDIAN ;
	#endif

}

iU32 i51AdeCnfGetCpu()
{
	return i51_CPU_ARM ;
}




