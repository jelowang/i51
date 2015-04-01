
/*

	The Implementing of 'Adapter' , Module : 'SMS'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#ifndef i51ADESMS_H
#define i51ADESMS_H

#include "i51.h"

extern iS32* i51AdeSmsGetSims ( void ) ;
extern iCHAR* i51AdeSmsGetImsi ( iS32 sim ) ;
extern iCHAR* i51AdeSmsGetImei ( void ) ;
extern iCHAR* i51AdeSmsGetCenter ( iS32 sim ) ;
extern iU32 i51AdeSmsCreateDevice () ;
extern iBOOL i51AdeSmsSend ( iU32 device , iS32 sim , iCHAR* phone , iU16* contents, iBOOL save , void (*callback)( iBOOL results ) ) ;
extern iBOOL i51AdeSmsSendEx ( iU32 device , iS32 sim , iCHAR* phone , iCHAR* contents , iBOOL unicode , iBOOL save , void (*callback)( iBOOL results ) ) ;
extern iBOOL i51AdeSmsReleaseDevice ( iU32 device ) ;
extern iS32 i51AdeSmsCreateHook ( iS32 type ) ;
extern iBOOL i51AdeSmsSetHook ( iS32 hookid , i51SMSHOOKCALLBACK callback ) ;
extern iS32 i51AdeSmsRunHook (  iS32 sim ,  iCHAR* number , iU32 numlen , iU16* buffer , iU32 textlen ) ;
extern iBOOL i51AdeSmsReleaseHook (  iS32 hookid ) ;
extern iBOOL i51AdeSmsGetPhoneBook ( i51PHONEBOOK* phonebook , iU32* length ) ;
extern iBOOL i51AdeSmsCreate ( i51ADECALLBACK callback ) ;
extern iBOOL i51AdeSmsRelease ( void ) ;

#endif

