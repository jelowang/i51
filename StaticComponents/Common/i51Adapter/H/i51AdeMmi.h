 
/*

	The Implementing of 'Adapter' , Module : 'MMI'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#ifndef _i51ADEMMI_H
#define _i51ADEMMI_H

#include "i51.h"

extern iBOOL i51AdeMmiGetScreenScale ( iU16* width , iU16* height ) ;
extern  iU32* i51AdeMmiGetPanel (void) ;
extern iBOOL i51AdeMmiMediaPlayerCreate ( void (*callback) ( iBOOL results ) ) ;
extern iBOOL i51AdeMmiMediaPlayerRelease () ; 
extern iS32 i51AdeMmiMediaPlayerFormat () ;
extern iBOOL i51AdeMmiMediaPlayerPlay ( iU32 priority , void* handle , iU32 length , iU32 type , iU32 dec , iBOOL loop , void (*STOP)(void* handle)  ) ;
extern iBOOL i51AdeMmiMediaPlayerPause ( void* handle ) ;
extern iBOOL i51AdeMmiMediaPlayerResume ( void* handle ) ;
extern iBOOL i51AdeMmiMediaPlayerStop ( void* handle ) ;
extern iBOOL i51AdeMmiMediaPlayerSetVolume ( iU32 volume ) ;  
extern iU32 i51AdeMmiMediaPlayerGetVolume () ;
extern iS32 i51AdeMmiRecorderStart ( void* buffer , iU32 bufflen , void (*callback ) ( void* stream , iU32 length ) ) ;
//	Jelo Add 2012-7-6
extern iBOOL i51AdeMmiRecorderStartEx ( iU16* filename , void (*callback ) ( iBOOL finished ) ) ;
//	Jelo Add Finished
extern iBOOL i51AdeMmiRecorderStop ( void (*callback) ( iBOOL results ) ) ;
extern iBOOL i51AdeMmiRecoderPlay ( void* stream , iU32 length , void (*callback) ( iBOOL results ) ) ; 
extern iBOOL i51AdeMmiRecorderPlayStop (void) ;
extern void i51AdeMmiRecorderFormat ( iU32* samples , iU32* bitscale , iU32* chanels ) ;
extern void i51AdeMmiSetPanelUsed ( void* panel ) ;
extern void i51AdeMmiUpdateScreen (void) ;
extern void i51AdeMmiUpdateScreenEx ( iRECT* irect , iU32 recttotall ) ;
extern void i51AdeMmiGetTextAttr ( iU16* text , iU16* textwidth , iU16* textheight ) ;
extern void i51AdeMmiDrawText ( iU8* panel , iU16 width , iU16 height , const iU16* text , iS32 desx , iS32 desy , iCOLOR color ) ;
extern iBOOL i51AdeMmiCreate ( i51ADECALLBACK callback ) ;
extern iBOOL i51AdeMmiRelease ( void ) ;  

#endif

