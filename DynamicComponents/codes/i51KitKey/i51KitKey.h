
#ifndef THE_i51KitKey_H
#define THE_i51KitKey_H

#include "i51.h"
//	最终发布动态库时，将此宏打开
//#define RELEASE_i51KitKey

typedef enum 
{
	i51_KEY_DOWN = 1,
	i51_KEY_UP,
	i51_KEY_LONG_PRESS,
	i51_KEY_REPEAT,
	i51_KEY_EVENT_MAX
}i51KeyEventType;

typedef enum 
{
	i51_PEN_DOWN = 1, 
	i51_PEN_UP, 
	i51_PEN_LONG_PRESS,
	i51_PEN_REPEAT,
	i51_PEN_MOVE,
	i51_PEN_EVENT_MAX
}i51PenEventType;

typedef struct 
{
	i51KeyEventType type;   /*!< 按键类型 */
	i51KEYS value;  /*!< i51 键值 */
} i51PhoneKey;

typedef struct 
{
	iU16 x;
	iU16 y;
	i51PenEventType type;  /*!< 触摸类型 */
} i51PhonePen;

typedef enum 
{
	i51_INPUT_NUL = 0,
	i51_KEY_INPUT, 
	i51_PEN_INPUT
}i51InputType;

typedef struct
{
	i51InputType type;
	union
	{
		i51PhoneKey key;
		i51PhonePen pen;
	}u;
}i51InputStruct;

i51KIT_DYNAMIC_KIT_DEF_BEGIN

	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitKeyInputQueueCreat)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitKeyInputQueueIn)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitKeyInputQueueOut)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitKeyInputQueueEmpty)
	i51KIT_DYNAMIC_KIT_DEF_PORT(i51KitKeyInputQueueDestroy)

i51KIT_DYNAMIC_KIT_DEF_END


i51KIT_DYNAMIC_KIT_DEF_TYPE(iHANDLE, i51KitKeyInputQueueCreat,( void) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitKeyInputQueueIn,(iHANDLE Queue, iU32 message , void* parameter) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitKeyInputQueueOut,( iHANDLE Queue, i51InputStruct *InputData) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitKeyInputQueueEmpty,( iHANDLE Queue) )
i51KIT_DYNAMIC_KIT_DEF_TYPE(iBOOL, i51KitKeyInputQueueDestroy,( iHANDLE Queue ) )

#ifdef RELEASE_i51KitKey

#define i51KitKeyInputQueueCreat i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitKey,i51KitKeyInputQueueCreat)
#define i51KitKeyInputQueueIn i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitKey,i51KitKeyInputQueueIn)
#define i51KitKeyInputQueueOut i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitKey,i51KitKeyInputQueueOut)
#define i51KitKeyInputQueueEmpty i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitKey,i51KitKeyInputQueueEmpty)
#define i51KitKeyInputQueueDestroy i51KIT_DYNAMIC_KIT_DEF_CALL(i51KitKey,i51KitKeyInputQueueDestroy)

#endif

i51KIT_DYNAMIC_KIT_DEC(i51KitKey);

#endif

