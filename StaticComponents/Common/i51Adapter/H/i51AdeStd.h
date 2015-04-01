
/*

	The Implementing of 'Adapter' , Module : 'STD'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#ifndef i51STD_H
#define i51STD_H

#include "i51.h"
extern iU32 i51AdeStdCrc32 ( iU8* buf , iS32 len , iU32** table ) ;
extern iBOOL i51AdeStdWSprintf ( iU16 *buffer , iU32 bytes , const iCHAR* format , ... ) ;
extern iBOOL i51AdeStdEntryEditor (  i51STDEDITOR* content ) ;
extern iU16* i51AdeStdAscii2Unicode ( const iCHAR* src , iU16* dst ) ;
extern iCHAR* i51AdeStdUnicode2Ascii ( const iU16* src , iCHAR* dst ) ;
extern void i51AdeStdMemset16 ( void* dest , iU16 value , iU32 length ) ;
extern void i51AdeStdMemcpy16 ( void* dest , void* src , iU32 length ) ;
extern iBOOL i51AdeStdUncompress ( iU8* dest , iU32* destlen , const iU8* source , iU32 srclen ) ;
extern iU32 i51AdeStdWStrlen ( iU16* S ) ; 
extern iU16* i51AdeStdWStrcpy ( iU16* T , iU16* S ) ;
extern iS32 i51AdeStdWStrcmp ( iU16* T , iU16* S ) ;
extern iU16* i51AdeStdWStrcat ( iU16* T , iU16* S ) ;
extern iS32 i51AdeStdAtoi (const iCHAR* buffer) ;
extern void i51AdeStdItoa (iCHAR **buf, iS32 integer , iS32 base ) ;
extern iBOOL i51AdeStdCreate ( i51ADECALLBACK callback ) ;
extern iBOOL i51AdeStdRelease ( void ) ;

#endif

