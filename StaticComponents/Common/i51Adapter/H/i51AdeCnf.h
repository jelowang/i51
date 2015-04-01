 
/*

	The Implementing of 'Adapter' , Module : 'MMI'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Api.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#ifndef The_i51ADECONFIG_H
#define The_i51ADECONFIG_H

#include "i51.h"

//	设计版本号，当适配器的设计新加、删除、更新接口时才会变更该版本号。
//	最大值7.31.255
#define i51_ADE_CNF_VERSION "0.6."

#endif

extern iU32 i51AdeCnfGetType () ;
extern iU32 i51AdeCnfGetVersion () ;
extern iCHAR* i51AdeCnfGetVersionEx () ;
extern iU32 i51AdeCnfGetProjectId () ;
extern iU32 i51AdeCnfGetCustomerId () ;
extern iCHAR* i51AdeCnfGetPlatformId () ;
extern iU32 i51AdeCnfGetEndian () ;
extern iU32 i51AdeCnfGetCpu();
