
/*

	The Implementing of 'Adapter' , Module : 'SOC'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#ifndef i51ADESOC_H
#define i51ADESOC_H

#include "i51.h"

extern iU32 i51AdeSocHtonl ( iU32 value ) ;
extern iU16 i51AdeSocHtons ( iU16 value ) ;
extern iBOOL i51AdeSocInetAton ( iCHAR* ip , iU32* value ) ;
extern  iS32 i51AdeSocGetHostByName ( iCHAR* name  , i51DNSCALLBACK callback ) ;
extern iS32 i51AdeSocGet ( void ) ;
extern void i51AdeSocConnect ( iS32 socket , i51SOCADDR* address , i51SOCCONCBK callback ) ;
extern void i51AdeSocSend ( iS32 socket , void* data , iU32 length , i51SOCSENDCBK callback ) ;
extern void i51AdeSocReceive ( iS32 socket , void* data , iU32 length , i51SOCRECVCBK callback ) ;
extern iBOOL i51AdeSocClose ( iS32 socket ) ;
#ifdef i51PROFILINGS
extern iU32 i51AdeSocGetLive ( void ) ;
extern iU32 i51AdeSocGetRatio ( void ) ;
#endif
extern iS32* i51AdeSocGetLinkLayerMode (void) ;
extern void i51AdeSocGetLinkLayerStatus ( iBOOL* linked , iS32* simid , iS32* mode ) ; 

extern iU32 i51AdeSocCreateLinkLayer ( iS32 mode , iS32 sim ) ;
extern iBOOL i51AdeSocOpenLinkLayer ( iU32 aid , iU32 layerid , i51LLAYERCALLBACK callback  ) ;
extern iBOOL i51AdeSocCloseLinkLayer ( iU32 aid , iU32 layerid ) ;
extern iBOOL i51AdeSocReleaseLinkLayer ( iU32 layerid ) ;
extern iBOOL i51AdeSocCreate ( i51ADECALLBACK callback ) ;
extern iBOOL i51AdeSocRelease ( void ) ;

#endif

