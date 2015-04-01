#ifndef __LLPINPUT_H__
#define __LLPINPUT_H__

#include "i51.h"

typedef enum 
{
	VKEY_0 = i51_KEY_NUM0,    /*0*/
	VKEY_1 = i51_KEY_NUM1,    /*1*/
	VKEY_2 = i51_KEY_NUM2,    /*2*/
	VKEY_3 = i51_KEY_NUM3,    /*3*/
	VKEY_4 = i51_KEY_NUM4,    /*4*/
	VKEY_5 = i51_KEY_NUM5,    /*5*/
	VKEY_6 = i51_KEY_NUM6,    /*6*/
	VKEY_7 = i51_KEY_NUM7,    /*7*/
	VKEY_8 = i51_KEY_NUM8,    /*8*/
	VKEY_9 = i51_KEY_NUM9,    /*9*/
	VKEY_LSK = i51_KEY_LEFT,    /*lsk*/
	VKEY_RSK = i51_KEY_RIGHT,    /*rsk*/
	VKEY_VOL_UP = i51_KEY_VOL_UP,    /*vol up*/
	VKEY_VOL_DOWN = i51_KEY_VOL_DOWN,    /*vol down*/
	VKEY_ENTER = i51_KEY_ENTER,    /*enter*/
	VKEY_UP  = i51_KEY_ARROW_UP,    /*up arrow*/
	VKEY_DOWN = i51_KEY_ARROW_DOWN,    /*down arrow*/
	VKEY_LEFT = i51_KEY_ARROW_LEFT,    /*left arrow*/
	VKEY_RIGHT = i51_KEY_ARROW_RIGHT,    /*right arrow*/
	VKEY_STAR = i51_KEY_STAR,    /***/
	VKEY_POUND = i51_KEY_SHARP,    /*#*/
	VKEY_SEND = i51_KEY_CALL,    /*send*/
	VKEY_END = i51_KEY_END_CALL,    /*end*/

	VKEY_MAX_KEY
}LLPKeyType;

typedef enum 
{
	LLP_KEY_DOWN       = i51_MSG_KEYDOWN,
	LLP_KEY_UP         = i51_MSG_KEYUP,
//	LLP_KEY_LONG_PRESS = i51_MSG_KEYLONGPRESS,
//	LLP_KEY_REPEAT     = i51_MSG_KEYREPEAT,
	LLP_KEY_EVENT_MAX
}LLPKeyEventType;

typedef enum 
{
	LLP_PEN_DOWN = i51_MSG_PADDOWN, 
	LLP_PEN_UP   = i51_MSG_PADUP, 
	LLP_PEN_MOVE = i51_MSG_PADMOVE,
	LLP_PEN_EVENT_MAX
}LLPPenEventType;

typedef struct 
{
	LLPKeyEventType type;   /*!< 按键类型 */
	LLPKeyType         value;  /*!< llp 键值 */
} LLPPhoneKey;

typedef struct 
{
	short x;
	short y;
	LLPPenEventType type;  /*!< 触摸类型 */
} LLPPhonePen;

typedef enum 
{
	LLP_INPUT_NUL = 0,
	LLP_KEY_INPUT, 
	LLP_PEN_INPUT
}LLPInputType;

typedef struct
{
	LLPInputType type;
	union
	{
		LLPPhoneKey key;
		LLPPhonePen pen;
	}u;
}LLPInputStruct;

extern void llp_input_init(void);
extern void llp_input_key(LLPKeyEventType keyType, int KeyValue);
extern void llp_input_pen(LLPPenEventType PenType, int value); 
extern int llp_get_input(LLPInputStruct * input);

#endif
