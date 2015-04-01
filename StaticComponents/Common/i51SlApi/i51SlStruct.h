
/*

	Layer belong : 'Static Components'.
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#ifndef The_i51STRUCT_H
#define The_i51STRUCT_H

#include "i51.h"

#define QueueSpaceMax 32

typedef enum {

	//	author : Jelo Wang
	//	since : 20100609
 
	//	notes : seek

	i51_SLLIST_SEEK_HEAD ,
	i51_SLLIST_SEEK_TAIL ,

} i51SlDSLISTSEEK ;

typedef struct ILN {

	//	author : Jelo Wang
	//	since : 20091123
	
	iS32 eltype ;
	iS32 totall ;

	void* element ;
	
	struct ILN* head ;
	struct ILN* front ;
	struct ILN* next ;
	
} i51SlDSLIST ;

typedef struct STKATOM
{

	//	author : Jelo	
	//	since : 2011.8.23
	//	(C) PKIG Tech. Co., Ltd.

	//	”¶”√≥Ã–Ú’ª

	iU32 deep ;
	iHANDLE element ;
	
	struct STKATOM* next ;
	
} i51SlDSSTACK ;

typedef struct {
	
	iS32 front ;
	iS32 rear ;
	
	iU32 TotallElement ;

	iHANDLE space [ QueueSpaceMax ] ;

} i51SlDSQUEUE ;

#endif


extern void i51SlDSListInit ( iHANDLE list ) ;
extern iBOOL i51SlDSListInsert ( iHANDLE list , iHANDLE el ) ; 
extern iBOOL i51SlDSListEmpty ( iHANDLE list ) ;
extern iBOOL i51SlDSListSetIterator ( iHANDLE lt , iS32 position ) ;
extern iBOOL i51SlDSListIteratorPermit ( iHANDLE lt ) ;
extern iHANDLE i51SlDSListIteratorGetElement ( iHANDLE lt ) ;
extern iBOOL i51SlDSListListIteratorNext ( iHANDLE lt ) ;
extern void i51SlDSListDestroy ( iHANDLE list ) ;

extern iBOOL i51SlDSStackInit ( iHANDLE stk ) ;
extern iBOOL i51SlDSStackClear ( iHANDLE stk ) ;
extern iBOOL i51SlDSStackPush ( iHANDLE stk , iHANDLE element ) ;
extern iBOOL i51SlDSStackPop ( iHANDLE stk , iHANDLE buffer ) ;

extern iBOOL i51SlDSQueueInit ( i51SlDSQUEUE* queue ) ;
extern iBOOL i51SlDSQueueClear ( i51SlDSQUEUE* queue ) ;
extern iBOOL i51SlDSQueueIsEmpty  ( i51SlDSQUEUE* queue ) ;
extern iU32 i51SlDSQueueGetTotall ( i51SlDSQUEUE* queue ) ;
extern iBOOL i51SlDSQueueIn ( i51SlDSQUEUE* queue , iHANDLE el ) ;
extern iBOOL i51SlDSQueueOut ( i51SlDSQUEUE* queue , iHANDLE* el ) ;
extern iU32 i51SlDSQueueSpaceMax () ;

