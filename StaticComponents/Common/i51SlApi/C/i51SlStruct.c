
/*

	Layer belong : 'Static Components'.
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#include "i51AdeOs.h"
#include "i51SlStruct.h"

#define i51SlDSQueueIsFull(queue) ( (( queue->rear + 1 ) % QueueSpaceMax ) == queue->front )

void i51SlDSListInit ( iHANDLE lt ) {

	//	author : Jelo Wang
	//	since : 20091123
	//	(C)TOK

	i51SlDSLIST* list = (i51SlDSLIST* ) lt ;
	
	list->element = 0 ;
	list->totall = 0 ;
	list->front = 0 ;
	list->head = 0 ;
	list->next = 0 ;

}

iBOOL i51SlDSListInsert ( iHANDLE lt , iHANDLE el ) {

	//	author : Jelo Wang
	//	since : 20091123
	//	(C)TOK

	i51SlDSLIST* list = (i51SlDSLIST* ) lt ;
	i51SlDSLIST* listnew = 0 ;

	if ( !list ) return iFALSE ;

	listnew = (i51SlDSLIST* ) i51MALLOC ( sizeof(i51SlDSLIST) ) ;

	if( !listnew ) return iFALSE ;

	memset ( listnew , 0 , sizeof(i51SlDSLIST) ) ;

	listnew->element = el ;
	listnew->next = 0 ;
	
	if ( 0 == list->head ) {
		list->head = listnew ;
		listnew->front = list ;
		list->next = listnew ;		
		list->totall = 1 ;
	} else {
		list->next->next = listnew ;
		listnew->front = list->next ;
		list->next = listnew ;
		list->totall ++ ;
 	}

	return iTRUE ;
	
}

iBOOL i51SlDSListEmpty ( iHANDLE lt ) {

	//	author : Jelo Wang
	//	since : 20100811
	//	(C)TOK

	i51SlDSLIST* list = (i51SlDSLIST* ) lt ;
	
	if ( !list ) return 1 ;

	if ( !list->head ) return 1 ;

	return 0 ;
	
}

iBOOL i51SlDSListSetIterator ( iHANDLE lt , iS32 position ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : set an iterator

	i51SlDSLIST* list = (i51SlDSLIST* ) lt ;
	i51SlDSLIST* walker = 0 ;

	if ( 0 == list ) return 0 ;
	
	if ( i51_SLLIST_SEEK_HEAD == position ) {
		//	将迭代器指向头节点
		list->next = list->head ; 
	} else if ( i51_SLLIST_SEEK_TAIL == position ) {
		//	将迭代器指向尾节点
		for ( walker = list->head ; walker && walker->next ; walker = walker->next ) ;
		if ( walker ) list->next = walker ;
	}

	return 1 ; 

}

iBOOL i51SlDSListIteratorPermit ( iHANDLE lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : 判断一个迭代器是否可以继续运行

	i51SlDSLIST* list = (i51SlDSLIST* ) lt ;
	i51SlDSLIST* walker = 0 ;

	if ( 0 == list ) return 0 ;
	if ( 0 == list->next ) return 0 ;

	return 1 ; 

}

iHANDLE i51SlDSListIteratorGetElement ( iHANDLE lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : get an element

	i51SlDSLIST* list = (i51SlDSLIST* ) lt ;

	if ( 0 == list ) return 0 ;
	if ( 0 == list->next ) return 0 ;

	return list->next->element ;

}

iBOOL i51SlDSListListIteratorNext ( iHANDLE lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : iterator next

	i51SlDSLIST* list = (i51SlDSLIST* ) lt ;

	if ( 0 == list ) return 0 ;
	if ( 0 == list->next ) return 0 ;

	list->next = list->next->next ; 

	return 1 ;

}

void i51SlDSListDestroy ( iHANDLE lt ) {

	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK

	i51SlDSLIST* list = (i51SlDSLIST* ) lt ;
	i51SlDSLIST* walker = list->head ;

	while ( walker ) {
		list->next = walker->next ;
		i51FREE ( walker ) ;
		walker = list->next ;
	}
	
}

iBOOL i51SlDSStackInit ( iHANDLE stk )
{

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.	

	memset ( stk , 0 , sizeof(i51SlDSSTACK) ) ; 
	
	return iTRUE ;

}

iBOOL i51SlDSStackClear ( iHANDLE stk )
{

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.

	i51SlDSSTACK* stack = (i51SlDSSTACK* ) stk ;
	i51SlDSSTACK* atom = stack->next ;
	
	while ( atom )
	{
		stack->next = atom->next ;
		i51FREE ( atom ) ;
		atom = stack->next ;
	}

	stack->next = 0 ;

}

iBOOL i51SlDSStackPush ( iHANDLE stk , iHANDLE element )
{

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.

	i51SlDSSTACK* atom = 0 ;
	i51SlDSSTACK* stack = (i51SlDSSTACK* ) stk ;
	
	atom = (i51SlDSSTACK* ) i51MALLOC ( sizeof(i51SlDSSTACK) ) ;

	if ( 0 == atom ) {
		i51AdeOsLog ( i51_LOG_SC , "KERNEL : STK PUSH F\r\n" ) ;
		return iFALSE ;
	}

	memset ( atom , 0 , sizeof(i51SlDSSTACK) ) ;
	
	atom->element = element ;	
	atom->next = stack->next ;
	stack->next = atom ;
	stack->deep ++ ;
	
}

iBOOL i51SlDSStackPop ( iHANDLE stk , void* buffer )
{

	//	author : Jelo 
	//	since : 2011.8.23
	// 	(C) PKIG Tech. Co., Ltd.		

	i51SlDSSTACK* atom = 0 ;
	i51SlDSSTACK* stack = (i51SlDSSTACK* ) stk ;
	
	if ( 0 == stack || stack->next )
	{
		return iFALSE ;
	}

	atom = stack->next ;

	memcpy ( buffer , atom , sizeof(i51SlDSSTACK) ) ;
	
	stack->next = atom->next ;

	i51FREE ( atom ) ;
	
	stack->deep -- ;

	return iTRUE ;
	
}

iBOOL i51SlDSQueueInit ( i51SlDSQUEUE* queue ) 
{
	
	//	author : Jelo
	//	since : 2011.10.10
	//	(C) PKIG Tech. Co., Ltd.
	
	if ( 0 == (iS32)queue ) 
	{
		return iFALSE ;
	}

	memset ( queue , 0 , sizeof(i51SlDSQUEUE) ) ;

	return iTRUE ;

}
iBOOL i51SlDSQueueClear ( i51SlDSQUEUE* queue )
{
	
	//	author : Jelo
	//	since : 2011.10.10
	//	(C) PKIG Tech. Co., Ltd.
	
	if ( 0 == queue ) 
	{
		return iFALSE ;
	}

	memset ( queue , 0 , sizeof(i51SlDSQUEUE) ) ;

	return iTRUE ;

}

iBOOL i51SlDSQueueIsEmpty  ( i51SlDSQUEUE* queue )
{

	//	author : Jelo
	//	since : 2011.10.10
	//	(C) PKIG Tech. Co., Ltd.

	if ( 0 == queue ) 
	{
		return iFALSE ;
	}
	
	if ( queue->front == queue->rear )
	{
		return iTRUE ;
	}

	return iFALSE ;
		
}

iU32 i51SlDSQueueGetTotall ( i51SlDSQUEUE* queue )
{
	
	//	author : Jelo
	//	since : 2011.10.10
	//	(C) PKIG Tech. Co., Ltd.

	if ( 0 == queue ) 
	{
		return iFALSE ;
	}

	return queue->TotallElement ;

}

iBOOL i51SlDSQueueIn ( i51SlDSQUEUE* queue , iHANDLE el )
{
	
	//	author : Jelo
	//	since : 2011.10.10
	//	(C) PKIG Tech. Co., Ltd.
	
	if ( 0 == queue ) 
	{
		return iFALSE ;
	}
	
	if ( iTRUE == i51SlDSQueueIsFull(queue) )
	{
		return iFALSE ;
	}
			
	queue->space [ queue->rear] = el ;
	queue->TotallElement ++ ;

	queue->rear = ( queue->rear + 1 ) % QueueSpaceMax ;

	return iTRUE ;

}

iBOOL i51SlDSQueueOut ( i51SlDSQUEUE* queue , iHANDLE* el )
{
	
	//	author : Jelo
	//	since : 2011.10.10
	//	(C) PKIG Tech. Co., Ltd.
	
	if ( 0 == queue ) 
	{
		return iFALSE ;
	}
	
	if ( iTRUE == i51SlDSQueueIsEmpty(queue))
	{
		return iFALSE ;
	}
			
	*el = queue->space [ queue->front] ;
	queue->TotallElement -- ;
	queue->front = ( queue->front + 1 ) % QueueSpaceMax ;

	return iTRUE ;

}

iU32 i51SlDSQueueSpaceMax () {

	//	author : Jelo
	//	since : 2012.03.01
	//	(C) PKIG Tech. Co., Ltd.

	return QueueSpaceMax ;
	
}

