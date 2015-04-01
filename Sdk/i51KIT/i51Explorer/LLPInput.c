/*********************************************************************************/
/**
 * Copyright (C) 2010, PKIG Tech. Co., Ltd.
 *
 * @file	LLPInput.c
 * @brief	输入模块实现
 * 
 * @author	ian, andy
 * @version	$Id: LLPInput.c 508 2010-10-30 02:36:35Z ian $
 * @date	$Date:: 2010-10-30 10:36:35#$
 *********************************************************************************/


/*********************************************************************************
 * 常量定义
 */
#include "LLPInput.h"

#define LLP_QUEUE_MAX    32

/*********************************************************************************
 * 数据结构
 */
typedef struct
{
	LLPInputStruct input[LLP_QUEUE_MAX];
	int front;
	int rear;
}LLPInputQueueStruct;

/*********************************************************************************
 * 变量定义
 */

static LLPInputQueueStruct InputQueue;

/*********************************************************************************
 * 内部函数
 */

/*********************************************************************************
 * 接口实现
 */


//用于解决编译警告
typedef void (*FuncPtr) (void);


//进队列
static int llp_input_append_queue(LLPInputStruct *pInput)
{
	if((InputQueue.rear + 1)%LLP_QUEUE_MAX == InputQueue.front)
	{
		return -1;  //队列已满
	}

	InputQueue.rear = (InputQueue.rear + 1)%LLP_QUEUE_MAX;
	memcpy(&InputQueue.input[InputQueue.rear], pInput, sizeof(LLPInputStruct));
	return 0;
}

//出队列
static int llp_input_delete_queue(LLPInputStruct *pInput)
{
	if(InputQueue.front == InputQueue.rear)
	{
		return -1;  //队列已空
	}

	InputQueue.front = (InputQueue.front + 1)%LLP_QUEUE_MAX;
	memcpy(pInput, &InputQueue.input[InputQueue.front], sizeof(LLPInputStruct));
	return 0;
}

static void llp_input_clear_queue(void)
{
	memset(&InputQueue, 0, sizeof(LLPInputQueueStruct));
}

void llp_input_key(LLPKeyEventType keyType, int KeyValue) 
{
	LLPInputStruct input;

	input.type        = LLP_KEY_INPUT;
	input.u.key.type  = keyType;
	input.u.key.value = KeyValue;
	llp_input_append_queue(&input);
}

void llp_input_pen(LLPPenEventType PenType, int value) 
{
	int x;
	int y;
	LLPInputStruct input;

	x = (value >> 16) & 0xFFFF;
	y = value  & 0xFFFF;

	input.type = LLP_PEN_INPUT;
	input.u.pen.type = PenType;
	input.u.pen.x = x;	
	input.u.pen.y = y;
	input.u.pen.time = i51AdeOsGetTick();
	llp_input_append_queue(&input);
}

/**
 * @fn	int llp_get_input(LLPInputStruct * input)
 * @brief	获取输入
 * 
 * @param	input 输入
 * 
 * @retval	 0 成功
 * @retval -1 失败
 */
int llp_get_input(LLPInputStruct * input)
{	
	memset(input, 0, sizeof(LLPInputStruct));
	return llp_input_delete_queue(input);
}


/**
 * @fn		void llp_input_init(void)
 * @brief	初始化输入模块
 * 
 * @note
 * @li	申请一个互斥量
 * @li	清空输入队列
 */
void llp_input_init(void)
{
	llp_input_clear_queue();
}
