#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "i51Public.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DL_FILE_TEMP    "dl.package"       //当前下载的package保存的名字
#define PACKAGE_URL    "app://%s/%d/%s/"    //服务器上下载的游戏url

//进度
#define PROGRESS_NUM (1)
#define PROGRESS_START (500)  //最开始的起始进度
#define PROGRESS_EACH_PERCENT {9700} //各过程进度条百分比,扩大了100倍

#define  ERROR_MSG_LINK_FALSE  "   连接服务器失败，请重试！"

//解码出错
#define  ERROR_MSG_DECODE_FALSE  "    游戏启动失败，请检查存储卡状态或将手机关机后重启!           客服电话：0755-83232356"

enum
{
	DL_STATE_INIT = 0,
	DL_STATE_DOWNLOAD,
	DL_STATE_DECODE,
	DL_STATE_FINISH
};

enum  
{
	PROGRESS_DOWNLOAD = 0,
};

typedef struct  
{
	iS8   progress_state;
	iS8   IsError;
	iS8   error_type;
	iBOOL dl_create_ret;
	iU16 package_name[30];
	iU32 version;
	iS8   lsk_down;
	iS8   rsk_down;
	iS8   init;
	void (*FuncRender)(iU32);
	void (*cb)(iBOOL ret);
}download_struct;

typedef struct  
{
	iU16 current;  //进度条百分比
	iU16 each[PROGRESS_NUM];   //每个过程，所占的百分比，字体25%，游戏25%，计费SDK 10%，游戏解码20%
}percent_struct;

/* 
**下载资源并解码
** package_name         下载的资源包名字
** res_name             每个re的名字
** ctn_name             每个re解码后的容器文件名
** image_num            每个re里的图片数目
** resource_num         需要解码的re数目
** FuncRender(iU32)     进度条画图函数，参数为当前进度，总进度为10000
** callback(iBOOL)      回调函数，iBOOL = iTRUE下载成功， iFALSE下载失败
*/
//此函数需每个循环都调用。调用此函数后，游戏底图不能重绘。若需要重绘需特殊处理
extern void download_resource(iU16 *package_name, void (*FuncRender)(iU32), void (*callback)(iBOOL));

//在系统消息exit时调用
extern void download_exit(void);

//按键处理
extern void download_key_handle(LLPPhoneKey *key);

//触摸屏处理
extern void download_pen_handle(LLPPhonePen *pen);

extern void portal_entry(iU32 aid, void (*cb)(iBOOL));
extern void portal_exit(void);


#ifdef PKIG_SCREEN_320X480

#define LOADING_BAR_LEN  (258)

#ifdef PKIG_SCREEN_HORIZONTAL
#define BG_X   158
#define BG_Y   117
#define LOADING_BAR_X    96
#define LOADING_BAR_Y    110
#define LEFT_BUTTON_X    9
#define LEFT_BUTTON_Y    10
#define RIGHT_BUTTON_X   9
#define RIGHT_BUTTON_Y   421
#define PROGRESS_NUM_X   69
#define PROGRESS_NUM_Y   230
#define WORD_TIPS_X      121
#define WORD_TIPS_Y      189
#else
#define BG_X   37
#define BG_Y   85
#define LOADING_BAR_X    30
#define LOADING_BAR_Y    291
#define LEFT_BUTTON_X    14
#define LEFT_BUTTON_Y    450
#define RIGHT_BUTTON_X   256
#define RIGHT_BUTTON_Y   450
#define PROGRESS_NUM_X   150
#define PROGRESS_NUM_Y   322
#define WORD_TIPS_X      107
#define WORD_TIPS_Y      267
#endif

#elif defined(PKIG_SCREEN_240X320)

#define LOADING_BAR_LEN  (198)

#ifdef PKIG_SCREEN_HORIZONTAL
#define BG_X   123
#define BG_Y   70
#define LOADING_BAR_X    79
#define LOADING_BAR_Y    60
#define LEFT_BUTTON_X    6
#define LEFT_BUTTON_Y    8
#define RIGHT_BUTTON_X   7
#define RIGHT_BUTTON_Y   269
#define PROGRESS_NUM_X   61
#define PROGRESS_NUM_Y   149
#define WORD_TIPS_X      98
#define WORD_TIPS_Y      107
#else
#define BG_X   30
#define BG_Y   51
#define LOADING_BAR_X    20
#define LOADING_BAR_Y    195
#define LEFT_BUTTON_X    10
#define LEFT_BUTTON_Y    294
#define RIGHT_BUTTON_X   185
#define RIGHT_BUTTON_Y   295
#define PROGRESS_NUM_X   109
#define PROGRESS_NUM_Y   218
#define WORD_TIPS_X      67
#define WORD_TIPS_Y      175
#endif

#elif defined(PKIG_SCREEN_240X400) 

#define LOADING_BAR_LEN  (198)

#ifdef PKIG_SCREEN_HORIZONTAL
#define BG_X   123
#define BG_Y   109
#define LOADING_BAR_X    79
#define LOADING_BAR_Y    101
#define LEFT_BUTTON_X    6
#define LEFT_BUTTON_Y    8
#define RIGHT_BUTTON_X   7
#define RIGHT_BUTTON_Y   346
#define PROGRESS_NUM_X   62
#define PROGRESS_NUM_Y   190
#define WORD_TIPS_X      97
#define WORD_TIPS_Y      149
#else
#define BG_X   30
#define BG_Y   62
#define LOADING_BAR_X    20
#define LOADING_BAR_Y    216
#define LEFT_BUTTON_X    9
#define LEFT_BUTTON_Y    377
#define RIGHT_BUTTON_X   187
#define RIGHT_BUTTON_Y   378
#define PROGRESS_NUM_X   109
#define PROGRESS_NUM_Y   239
#define WORD_TIPS_X      67
#define WORD_TIPS_Y      196
#endif

#endif

#endif /* DOWNLOAD_H */