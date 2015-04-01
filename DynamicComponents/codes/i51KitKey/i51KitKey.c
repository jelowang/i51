
#include "i51KitAde.h"
#include "i51KitKey.h"

i51KIT_STATIC_KIT_DEF(Adapter);

#define DEBUG_KIT_KEY	0
#define i51KITKEY_DEBUG	0

#define QUEUE_INPUT_MAX	32
#define TIME_LONGPRESS	800
#define TIME_REPEAT		400
#define PAD_JOGGLING_PIXEL	4

static iS32 TotalMoveX = 0;
static iS32 TotalMoveY = 0;
static iS32 MoveNumber = 0;

typedef struct {
	iU8  DownFlag;
	iU8  RepeatFlag;
	iS32 front ;
	iS32 rear ;
	iU32 LastTime;
	iU32 TotallElement ;

	i51InputStruct InputData [ QUEUE_INPUT_MAX ] ;

} i51INPUTQUEUE ;

#if DEBUG_KIT_KEY
i51INPUTQUEUE * Queue = iNULL;
#endif

iKIT iHANDLE i51KitKeyInputQueueCreat(void)
{
	//	author : Otto
	//	since : 2011.12.20
	//	(C) PKIG Tech. Co., Ltd.

	i51INPUTQUEUE * NewInputQueue = iNULL;
#if i51KITKEY_DEBUG
		iLog( "KitKeyInputQueueCreat S ") ;
#endif
	NewInputQueue = i51AdeOsMalloc((sizeof(i51INPUTQUEUE)), __FILE__ , __LINE__ );
	if( iNULL==NewInputQueue ) return iFALSE;

	memset ( NewInputQueue , 0 , sizeof(i51INPUTQUEUE) ) ;

#if i51KITKEY_DEBUG
			iLog( "KitKeyInputQueueCreat E ") ;
#endif
	return NewInputQueue;
	
}

iKIT iBOOL i51KitKeyInputQueueIn(iHANDLE Queue, iU32 message , void* parameter)
{
	//	author : Otto
	//	since : 2011.12.20
	//	(C) PKIG Tech. Co., Ltd.

	iU8  AddFlag = iFALSE;
	iS32 LastFront = 0;
	i51INPUTQUEUE * CurrInputQueue = iNULL;

	if( iNULL==Queue ) return iFALSE;
#if i51KITKEY_DEBUG
			iLog( "i51KitKeyInputQueueIn S :%X,%X,%X ",Queue,message,parameter) ;
#endif

	CurrInputQueue = (i51INPUTQUEUE *)Queue;

	switch(message)
	{
		case i51_MSG_KEYUP:
			CurrInputQueue->DownFlag= iFALSE;
			CurrInputQueue->RepeatFlag = iFALSE;
			CurrInputQueue->InputData[CurrInputQueue->front].type = i51_KEY_INPUT;
			CurrInputQueue->InputData[CurrInputQueue->front].u.key.type = i51_KEY_UP;
			CurrInputQueue->InputData[CurrInputQueue->front].u.key.value = (iU32)parameter;
			AddFlag = iTRUE;
			break;
		case i51_MSG_KEYDOWN:
			CurrInputQueue->DownFlag= iTRUE;
			CurrInputQueue->RepeatFlag = iFALSE;
			CurrInputQueue->LastTime = i51AdeOsGetTick();
			CurrInputQueue->InputData[CurrInputQueue->front].type = i51_KEY_INPUT;
			CurrInputQueue->InputData[CurrInputQueue->front].u.key.type = i51_KEY_DOWN;
			CurrInputQueue->InputData[CurrInputQueue->front].u.key.value = (iU32)parameter;
			AddFlag = iTRUE;
			break;
		case i51_MSG_PADDOWN:
			TotalMoveX = (((iU32)parameter)>>16)&0XFFFF;
			TotalMoveY = (((iU32)parameter))&0XFFFF;
			MoveNumber = 1;
			CurrInputQueue->DownFlag= iTRUE;
			CurrInputQueue->RepeatFlag = iFALSE;
			CurrInputQueue->LastTime = i51AdeOsGetTick();
			CurrInputQueue->InputData[CurrInputQueue->front].type = i51_PEN_INPUT;
			CurrInputQueue->InputData[CurrInputQueue->front].u.pen.type = i51_PEN_DOWN;
			CurrInputQueue->InputData[CurrInputQueue->front].u.pen.x = (((iU32)parameter)>>16)&0XFFFF;
			CurrInputQueue->InputData[CurrInputQueue->front].u.pen.y = (((iU32)parameter))&0XFFFF;
			AddFlag = iTRUE;
			break;
		case i51_MSG_PADUP:
			TotalMoveX = 0;
			TotalMoveY = 0;
			MoveNumber = 0;
			CurrInputQueue->DownFlag= iFALSE;
			CurrInputQueue->RepeatFlag = iFALSE;
			CurrInputQueue->InputData[CurrInputQueue->front].type = i51_PEN_INPUT;
			CurrInputQueue->InputData[CurrInputQueue->front].u.pen.type = i51_PEN_UP;
			CurrInputQueue->InputData[CurrInputQueue->front].u.pen.x = (((iU32)parameter)>>16)&0XFFFF;
			CurrInputQueue->InputData[CurrInputQueue->front].u.pen.y = (((iU32)parameter))&0XFFFF;
			AddFlag = iTRUE;
			break;
		case i51_MSG_PADMOVE:
		{
			iS32 MoveX = ((((iU32)parameter)>>16)&0XFFFF);
			iS32 MoveY = ((((iU32)parameter))&0XFFFF);
			iS32 AverageMoveX = 0;
			iS32 AverageMoveY = 0;

			if(MoveNumber==0)
			{
				AverageMoveX = -10;
				AverageMoveY = -10;
			}
			else
			{
				AverageMoveX = TotalMoveX/MoveNumber;
				AverageMoveY = TotalMoveY/MoveNumber;
			}
			TotalMoveX += MoveX;
			TotalMoveY += MoveY;
			MoveNumber += 1;
			
			LastFront = (CurrInputQueue->front-1+QUEUE_INPUT_MAX)%QUEUE_INPUT_MAX;
			if( (MoveX>=(AverageMoveX-PAD_JOGGLING_PIXEL)) && (MoveX<=(AverageMoveX+PAD_JOGGLING_PIXEL))
			&&(MoveY>=(AverageMoveY-PAD_JOGGLING_PIXEL)) && (MoveY<=(AverageMoveY+PAD_JOGGLING_PIXEL)))
			{
//iLog("______________Move break!________________,DownFlag=%d",CurrInputQueue->DownFlag);
//				CurrInputQueue->DownFlag= iTRUE;
				break;
			}

			TotalMoveX = MoveX;
			TotalMoveY = MoveY;
			MoveNumber = 1;
			CurrInputQueue->LastTime = i51AdeOsGetTick();
			CurrInputQueue->DownFlag= iTRUE;
			CurrInputQueue->RepeatFlag = iFALSE;
			CurrInputQueue->InputData[CurrInputQueue->front].type = i51_PEN_INPUT;
			CurrInputQueue->InputData[CurrInputQueue->front].u.pen.type = i51_PEN_MOVE;
			CurrInputQueue->InputData[CurrInputQueue->front].u.pen.x = ((((iU32)parameter)>>16)&0XFFFF);
			CurrInputQueue->InputData[CurrInputQueue->front].u.pen.y = ((((iU32)parameter))&0XFFFF);
			AddFlag = iTRUE;
			break;
		}
		default :break;	
	}

	if( CurrInputQueue->DownFlag )
	{
		if( (i51AdeOsGetTick()-CurrInputQueue->LastTime) >= TIME_LONGPRESS)
		{
			CurrInputQueue->DownFlag = iFALSE;
			CurrInputQueue->RepeatFlag = iTRUE;
			CurrInputQueue->LastTime = i51AdeOsGetTick();
			
			LastFront = (CurrInputQueue->front-1+QUEUE_INPUT_MAX)%QUEUE_INPUT_MAX;
			if(CurrInputQueue->InputData[LastFront].type == i51_KEY_INPUT )
			{
				CurrInputQueue->InputData[CurrInputQueue->front].type = i51_KEY_INPUT;
				CurrInputQueue->InputData[CurrInputQueue->front].u.key.type = i51_KEY_LONG_PRESS;
				CurrInputQueue->InputData[CurrInputQueue->front].u.key.value = CurrInputQueue->InputData[LastFront].u.key.value;
			}
			else 
			{
				CurrInputQueue->InputData[CurrInputQueue->front].type = i51_PEN_INPUT;
				CurrInputQueue->InputData[CurrInputQueue->front].u.pen.type = i51_PEN_LONG_PRESS;
				CurrInputQueue->InputData[CurrInputQueue->front].u.pen.x= CurrInputQueue->InputData[LastFront].u.pen.x;
				CurrInputQueue->InputData[CurrInputQueue->front].u.pen.y= CurrInputQueue->InputData[LastFront].u.pen.y;
			}

			AddFlag = iTRUE;
		}
	}

	if( CurrInputQueue->RepeatFlag )
	{
		if( (i51AdeOsGetTick()-CurrInputQueue->LastTime) >= TIME_REPEAT )
		{
			CurrInputQueue->LastTime = i51AdeOsGetTick();
			LastFront = (CurrInputQueue->front-1+QUEUE_INPUT_MAX)%QUEUE_INPUT_MAX;
			if(CurrInputQueue->InputData[LastFront].type == i51_KEY_INPUT )
			{
				CurrInputQueue->InputData[CurrInputQueue->front].type = i51_KEY_INPUT;
				CurrInputQueue->InputData[CurrInputQueue->front].u.key.type = i51_KEY_REPEAT;
				CurrInputQueue->InputData[CurrInputQueue->front].u.key.value = CurrInputQueue->InputData[LastFront].u.key.value;
			}
			else 
			{
				CurrInputQueue->InputData[CurrInputQueue->front].type = i51_PEN_INPUT;
				CurrInputQueue->InputData[CurrInputQueue->front].u.pen.type = i51_PEN_REPEAT;
				CurrInputQueue->InputData[CurrInputQueue->front].u.pen.x= CurrInputQueue->InputData[LastFront].u.pen.x;
				CurrInputQueue->InputData[CurrInputQueue->front].u.pen.y= CurrInputQueue->InputData[LastFront].u.pen.y;
			}

			AddFlag = iTRUE;
		}
	}

	if(AddFlag)
	{
		CurrInputQueue->front = (CurrInputQueue->front+1)%QUEUE_INPUT_MAX;
		if(CurrInputQueue->front==CurrInputQueue->rear)
		{
			CurrInputQueue->rear = (CurrInputQueue->rear+1)%QUEUE_INPUT_MAX;
		}
		CurrInputQueue->TotallElement++;
		if( CurrInputQueue->TotallElement>=QUEUE_INPUT_MAX )
		{
			CurrInputQueue->TotallElement = QUEUE_INPUT_MAX;
		}
#if i51KITKEY_DEBUG
					iLog( "i51KitKeyInputQueueIn E :%X,%X,%X ",Queue,message,parameter) ;
#endif
		return iTRUE;
	}
	else
	{
#if i51KITKEY_DEBUG
	iLog( "i51KitKeyInputQueueIn E :%X,%X,%X ",Queue,message,parameter) ;
#endif
		return iFALSE;
	}
}

iKIT iBOOL i51KitKeyInputQueueOut( iHANDLE Queue, i51InputStruct *InputData)
{
	//	author : Otto
	//	since : 2011.12.20
	//	(C) PKIG Tech. Co., Ltd.

	i51INPUTQUEUE * CurrInputQueue = iNULL;
	CurrInputQueue = (i51INPUTQUEUE *)Queue;

	if( iNULL==InputData || iNULL==CurrInputQueue ) return iFALSE;
#if i51KITKEY_DEBUG
				iLog( "i51KitKeyInputQueueOut S :%X,%X",Queue,InputData) ;
#endif

	if( 0==CurrInputQueue->TotallElement)
	{
		InputData->type = i51_INPUT_NUL;
#if i51KITKEY_DEBUG
	iLog( "i51KitKeyInputQueueOut E :%X,%X",Queue,InputData) ;
#endif
		return iFALSE;
	}

	i51AdeStdMemcpy16(InputData,&CurrInputQueue->InputData[CurrInputQueue->rear],sizeof(i51InputStruct));

	CurrInputQueue->TotallElement--;
	
	CurrInputQueue->rear = (CurrInputQueue->rear+1)%QUEUE_INPUT_MAX;

#if i51KITKEY_DEBUG
					iLog( "i51KitKeyInputQueueOut E :%X,%X",Queue,InputData) ;
#endif
	return iTRUE;
	
}

iKIT iBOOL i51KitKeyInputQueueEmpty(iHANDLE Queue)
{
	i51INPUTQUEUE * CurrInputQueue = iNULL;
#if i51KITKEY_DEBUG
	iLog( "i51KitKeyInputQueueEmpty S :%X",Queue) ;
#endif

	CurrInputQueue = (i51INPUTQUEUE *)Queue;

	if( iNULL==CurrInputQueue )
	{
#if i51KITKEY_DEBUG
	iLog( "i51KitKeyInputQueueEmpty E :%X",Queue) ;
#endif
		return iFALSE;
	}
	
	CurrInputQueue->front = 0;
	CurrInputQueue->rear = 0;
	CurrInputQueue->TotallElement = 0;

#if i51KITKEY_DEBUG
		iLog( "i51KitKeyInputQueueEmpty E :%X",Queue) ;
#endif
	return iTRUE;
	
}

iKIT iBOOL i51KitKeyInputQueueDestroy(iHANDLE Queue)
{
#if i51KITKEY_DEBUG
	iLog( "i51KitKeyInputQueueDestroy S:%X ",Queue) ;
#endif
	if( iNULL == Queue )
	{
#if i51KITKEY_DEBUG
				iLog( "i51KitKeyInputQueueDestroy E:%X ",Queue) ;
#endif
		return iFALSE;
	}

	i51AdeOsFree(Queue);
	Queue = iNULL;
#if i51KITKEY_DEBUG
	iLog( "i51KitKeyInputQueueDestroy E:%X ",Queue) ;
#endif
	return iTRUE;
}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	ÏûÏ¢Ñ­»·

#if DEBUG_KIT_KEY
	i51InputStruct InputData;
	if(Queue)
	{
		InputData.type=i51_INPUT_NUL;
		i51KitKeyInputQueueIn(Queue, message, parameter);
		i51KitKeyInputQueueOut(Queue, &InputData);
		if(InputData.type!=i51_INPUT_NUL)
		{
			if(InputData.type==i51_KEY_INPUT)
			{
				iLog("InputData.type=%d",InputData.type);
				iLog("InputData.u.key.type=%d",InputData.u.key.type);
				iLog("InputData.u.key.value=%d",InputData.u.key.value);
			}
			else
			{
				switch(InputData.u.pen.type)
				{
					case i51_PEN_DOWN:
						iLog("i51_PEN_DOWN");
						break;
					case i51_PEN_UP:
						iLog("i51_PEN_UP");
						break;
					case i51_PEN_LONG_PRESS:
						iLog("i51_PEN_LONG_PRESS");
						break;
					case i51_PEN_REPEAT:
						iLog("i51_PEN_REPEAT");
						break;
					case i51_PEN_MOVE:
						iLog("i51_PEN_MOVE");
						break;
				}
				iLog("InputData.u.pen.x=%d",InputData.u.pen.x);
				iLog("InputData.u.pen.y=%d",InputData.u.pen.y);
			}
		}
	}
#endif
	switch ( message )
	{
		case i51_MSG_INIT :
		{
	
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);
	
		}
		break ;
		
		case i51_MSG_RUN :
#if DEBUG_KIT_KEY
	Queue = i51KitKeyInputQueueCreat();
#endif

		break ;

		case i51_MSG_PAUSE :
		break ;

		case i51_MSG_EXIT :
		break ;
		

	}

	

	return iTRUE ;

}

iKIT iU32* i51KitPort ()
{

		//	TODO : Initialize API Port for external layer

		i51KIT_DYNAMIC_KIT_MAPING_BEGIN(i51KitKey)

		i51KIT_DYNAMIC_KIT_MAPING(i51KitKeyInputQueueCreat)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitKeyInputQueueIn)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitKeyInputQueueOut)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitKeyInputQueueEmpty)
		i51KIT_DYNAMIC_KIT_MAPING(i51KitKeyInputQueueDestroy)

		i51KIT_DYNAMIC_KIT_MAPING_END 

		return i51KIT_DYNAMIC_KIT_GET_MAPING(i51KitKey) ;

}
