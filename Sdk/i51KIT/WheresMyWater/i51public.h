#ifndef I51PUBLIC_H
#define I51PUBLIC_H

#include "i51.h"
#include "i51KitAde.h"
#include "i51KitStd.h"
#include "i51KitG2.h"

#include "LLPInput.h"
//#include "version.h"

///////////////////////////////////// 根据每个游戏不同 在此修改 //////////////////////////////////////////

#define PKIG_SCREEN_VERTICAL  //#define PKIG_SCREEN_HORIZONTAL  //横屏  PKIG_SCREEN_VERTICAL 竖屏  
//#define INSTALL_VERTICAL      //INSTALL_VERTICAL  安装进度条为竖屏。      INSTALL_HORIZONTAL  横屏
#define AUDIO_RESOURCE        //控制install是否出现解码声音进度

//#define MODE_LOCAL  //是否跑完整的单机版本，完整的游戏(和联网版失败后的单机版不同)

//#define LANGUAGE_ENGLISH  //英文版的宏

//#define GAME_LOGO   //是否需要公司LOGO的宏

//#define PKIG_SCREEN_240X400
#define INDEPENDENT_51PKPAY

#define AUDIO_PLAYER_MIDI_NUMS  (4)  //midi播放器的数目
#define AUDIO_PLAYER_WAV_NUMS   (0)  //wav播放器的数目    //注意midi_nums + wav_nums <= 5

#define GAME_DIR       "WheresMyWater"       //游戏目录
#define IMAGE_RESOURCE_VERSION  0x01   //图片版本

/* game */
#define GAME_KIT_VERSION (1)   //game.kit的版本号，每次更新需更改
#define GAME_KIT_NUM   1            //kit的个数
#define GAME_KIT       {"wmw.kit"}   //游戏kit
#define GAME_RESOURCE_NUM   1   //re的个数
#define GAME_RESOURCE  {"wmw.re"}    //游戏资源
#define  CONTAINER_NAMES      {"wmw.contain"}  //容器名字
#define  RESOURCE_IMAGE_NUMS  {186}   //每个资源文件的图片数量

#define GAME_PACKAGE_URL    "app://%s/%d/wmw/"    //服务器上下载的游戏url，需要修改最后的cf，改为你自己的包名

/* install */
#define INSTALL_KIT_VERSION  (1)
#define INSTALL_KIT_NAME  "install.kit"
#define INSTALL_RESOURCE_NUM  (2)  //re的个数，install.re和UI_Frame.re
#define INSTALL_RESOURCE  {"install.re", "UI_frame.re"}
#define INSTALL_CONTAINER {"install.container", "UI_frame.container"}

#define KIT_GAME_PATH      L"WheresMyWater\\wmw.kit"       //kit的路径
#define KIT_INSTALL_PATH   L"WheresMyWater\\install.kit"

#ifdef GAME_LOGO
#define INSTALL_RESOURCE_IMAGE_NUMS  {4, 17}   //install.re里的图片数
#else
#define INSTALL_RESOURCE_IMAGE_NUMS  {4, 17}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////// 分辨率 ////////////////////////////////////////////////
#if defined(PKIG_SCREEN_240X320)
#define PK_SCREEN_WIDTH	 (240)
#define PK_SCREEN_HEIGHT (320)
#define SCREEN_OFFSET_Y (0)
#define SCREEN_OFFSET_X (0)
#elif defined(PKIG_SCREEN_240X400)
#define PK_SCREEN_WIDTH	 (240)
#define PK_SCREEN_HEIGHT (400)
#define SCREEN_OFFSET_Y (40)
#define SCREEN_OFFSET_X (0)
#elif defined(PKIG_SCREEN_320X400)
#define PK_SCREEN_WIDTH	(320)
#define PK_SCREEN_HEIGHT (400)
#define SCREEN_OFFSET_Y (0)
#define SCREEN_OFFSET_X (0)
#elif defined(PKIG_SCREEN_320X480)
#define PK_SCREEN_WIDTH	(320)
#define PK_SCREEN_HEIGHT (480)
#define SCREEN_OFFSET_Y (40)
#define SCREEN_OFFSET_X (0)
#elif defined(PKIG_SCREEN_320X240)
#define PK_SCREEN_WIDTH	(320)
#define PK_SCREEN_HEIGHT (240)
#define SCREEN_OFFSET_Y (0)
#define SCREEN_OFFSET_X (0)
#elif defined(PKIG_SCREEN_220X176)
#define PK_SCREEN_WIDTH	(220)
#define PK_SCREEN_HEIGHT (176)
#define SCREEN_OFFSET_Y (0)
#define SCREEN_OFFSET_X (0)
#elif defined(PKIG_SCREEN_272X480)
#define PK_SCREEN_WIDTH	(272)
#define PK_SCREEN_HEIGHT (480)
#define SCREEN_OFFSET_Y (40)
#define SCREEN_OFFSET_X (0)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* package */
#define GAME_PACKAGE     "game_package"     //游戏包名
#define UPDATE_PACKAGE   "update_package"   //升级包名

#define SCREEN_CONTENT_FILE   "screen.dat"   //屏幕数据文件
#define GAME_UPDATE_FILE  "update"

#define FEE_APP_NAME  L"51pkpay"    //计费应用名称

#define PK_FONT_HEIGHT   (12)
#define PEN_RECT_WORH    (80)
#define FONT_WHITE     0xffff  //字体白色

#define HTTP_PROTOCOL_VERSION   (0x01)    //协议版本

//#ifdef HTTP_SERVE_TEST
//#define HOST_NAME "sa.test.pkingame.net";
//#else
//#define HOST_NAME "sa.pkingame.net";
//#endif

typedef enum
{
	GAME_RUN_MODE_LOCAL = 0,
	GAME_RUN_MODE_NETWORK,
	GAME_RUN_MODE_LOCAL_NOT_LIMIT
};

typedef enum
{
	KIT_I51,
	KIT_INSTALL,
	KIT_GAME
};

typedef struct  
{
	iS16 x;
	iS16 y;
}iPoint;

enum
{
	SRC_IMAGE_WIDTH = 9999,
	SRC_IMAGE_HEIGHT
};

typedef struct 
{
	iHANDLE pContainer;
	iU32 id;
	iS32 w;
	iS32 h;
}public_image_struct;

///////////////////////////////////////////////////////////////////////////
//////////////////////声音部分//////////////////////////

#define CHANNEL_AUDIO_NUM   (5)  //每个通道最多共用的声音个数

typedef enum
{
	AUDIO_PLAYER_MIDI_0 = 0,
	AUDIO_PLAYER_MIDI_1,
	AUDIO_PLAYER_MIDI_2,
	AUDIO_PLAYER_MIDI_3,

	AUDIO_PLAYER_WAV_0,
	AUDIO_PLAYER_WAV_1,
	AUDIO_PLAYER_WAV_2,

	AUDIO_PLAYER_NUM_MAX
}audio_player_type;

typedef struct  
{
	void *p;   //声音的地址
	iU32 leng;   //声音的长度
	iU8  loop;  //播放模式
	iU8  playing;    //是否正在播放
	iU8  resume;     //是否需要恢复
	i51METYPE type;  //声音的类型，midi还是wav等
	i51MMIDEC mem_or_file;  //声音的数据形式，在内存还是文件
	iU32 priority;   //当前播放声音的优先级,值越小，优先值越高
}audio_struct;

///////////////////////////////////////////////////////////////////////////

typedef enum
{
	ERROR_DECODE_FALSE = 0,  //解码失败
	ERROR_LINK_FALSE,        //普通联网失败
	ERROR_SYSTEM,            //系统错误
	ERROR_GAME_LOCAL,        //单机版
}error_type_enum;

//提示框种类，主要是按钮不同
enum
{
	TIPS_NULL,         //无按钮
	TIPS_LINK,         //TIPS
	TIPS_REVIVED,      //复活，取消
	TIPS_EXIT,         //确定，退出
	TIPS_RETRY,        //重试，取消
	TIPS_NORMAL,       //确定，返回
	TIPS_RETRY_EXIT    //重试，退出
};

////////////////////////////////////////////////////////////////////////////

#define  unload_dynamic_kit(kit)\
{\
	if(i51KIT_DYNAMIC_KIT_PROTOTYPE(kit))\
{\
	i51KIT_DYNAMIC_KIT_UNLOAD(kit);\
	i51KIT_DYNAMIC_KIT_PROTOTYPE(kit) = iNULL;\
}\
}

typedef void (*FuncPtr) (void);

typedef struct  
{
	//字体初始化
	iFILE (*public_font_init)(iU16 *font_re);
	iBOOL (*public_font_close)(void);

	//画单行文字
	iBOOL (*public_draw_text)(char *text, iS16 dst_x, iS16 dst_y, iU16 length, iCOLOR color);

	//画多行文字
	iBOOL (*public_draw_string_text)(char *text, iS16 dst_x, iS16 dst_y, iS16 width, iS16 height, iCOLOR color, iU16 spacing);

	//以x坐标为中心画一行文字
	iBOOL (*public_draw_text_x_middle)(char *text, iS16 middle_x, iS16 dst_y, iCOLOR color);

	//以y坐标为中心画一行文字
	iBOOL (*public_draw_text_y_middle)(char *text, iS16 dst_x, iS16 middle_y, iCOLOR color);

	//以y坐标为中心画多行文字
	iBOOL (*public_draw_string_text_y_middle)(char *text, iS16 dst_x, iS16 middle_y, iCOLOR color, iU16 spacing);

	//画单行文字(旋转90度)
	iBOOL (*public_draw_text_rotate_90)(char *text, iS16 dst_x, iS16 dst_y, iU16 length, iCOLOR color);

	//画多行文字(旋转90度)
	iBOOL (*public_draw_string_text_rotate_90)(char *text, iS16 dst_x, iS16 dst_y, iS16 width, iS16 height, iCOLOR color, iU16 spacing);

	//以y坐标为中心画一行文字(旋转90度)
	iBOOL (*public_draw_text_y_middle_rotate_90)(char *text, iS16 dst_x, iS16 dst_y, iCOLOR color);

	//以x坐标为中心画多行文字(旋转90度)
	void (*public_draw_string_text_x_middle_rotate_90)(char *text, iS16 middle_x, iS16 dst_y, iU32 color, iU32 spacing);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//填充矩形
	void (*public_fill_rect)(iS32 x, iS32 y, iS32 width, iS32 height, iCOLOR color);

	//画带边框的填充矩形
	void (*public_draw_frame_rect)(iS32 x, iS32 y, iS32 width, iS32 height, iCOLOR frame_color, iCOLOR fill_color);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//图像结构初始化
	void (*public_image_init)(public_image_struct *image, iHANDLE pContainer, iU32 image_id);

	//画图
	iS32 (*public_draw_image)(public_image_struct *image, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h);

	//画分数
	void (*public_draw_score)(iU32 score, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h, public_image_struct *image);

	//画左右按钮
	void (*public_draw_button)(char *lsk_word, char *rsk_word, public_image_struct *image);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//整形转换为字符串
	void (*public_itoa)(iS32 value, char *str);

	//矩形区域赋值
	iRECT *(*public_build_rect)(iRECT *rect, iS16 x, iS16 y, iS16 width, iS16 height);

	//坐标赋值
	iPoint *(*public_build_point)(iPoint *point, iS32 x, iS32 y);

	//矩形四个顶点赋值
	void (*public_build_rect_vertex)(iS32 point[][2], iS32 x, iS32 y, iS32 width, iS32 height);

	//判断触摸点是否在矩形内
	iU8 (*public_pen_enabled_rect)(iU16 pen_x, iU16 pen_y, iU16 min_x, iU16 max_x, iU16 min_y, iU16 ma1x_y);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//写文件
	iS32 (*public_write_file)(char *file_name, void *data, iU32 size);

	//读文件
	iS32 (*public_read_file)(char *file_name, void *data);
	iS32 (*public_delect_file)(char *dir, char *file_name);

	iBOOL (*public_audio_init)(void (*cb)(iBOOL results));
	iBOOL (*public_audio_play)(iU32 priority, void *audio_data, iU32 audio_len, i51MMIDEC mem_or_file, audio_player_type channel, iBOOL loop);
	iBOOL (*public_audio_stop)(audio_player_type player_type);
	void (*public_audio_all_exit)(void);

	iBOOL (*find_aim_dir)(char *dir);
	iBOOL (*check_file_is_exist)(char *dir, char *file);

	//UI frame
	void (*UI_image_init)(void);
	void (*UI_frame_msg_init)(iU8 type, char *msg);
	void (*UI_frame_init)(iU8 type, char *msg);
	iS32 (*UI_frame_render)(void);
	void (*UI_frame_input_key)(LLPPhoneKey *key, FuncPtr lsk_func, FuncPtr rsk_func);
	void (*UI_frame_input_pen)(LLPPhonePen *pen, FuncPtr lsk_func, FuncPtr rsk_func);

	int (*llp_get_input)(LLPInputStruct * input);

	iS32 (*load_fee_app)(void);

	///* HTTP */
	//const char *(*hostname)(void);
	//unsigned short (*probuf_pop_short)(ProBuffer *buf);
	//unsigned char (*probuf_pop_byte)(ProBuffer *buf);
	//unsigned long (*probuf_pop_long)(ProBuffer *buf);
	//char *(*probuf_pop_string)(ProBuffer *buf, char s[], int len);
	//ProBuffer *(*probuf_init)(ProBuffer *buf, int capacity, void *pbuf, int siz);
	//ProBuffer *(*probuf_push)(ProBuffer *buf, const void *data, int size);
	//ProBuffer *(*probuf_push_short)(ProBuffer *buf, unsigned short val);
	//ProBuffer *(*probuf_push_long)(ProBuffer *buf, unsigned long val);
	//ProBuffer *(*probuf_push_string)(ProBuffer *buf, const char* s);
	//void *(*pkig_httpc_postbuf)(Pkig_Httpc *q, const char *host, const char *path, const ProBuffer *buf, Pkig_Httpc_Callback fp);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void (*unload_kit_msg)(iU8 state);
	void (*kernel_send_request)(iU32 cmd); //发消息给kernel(如i51_REQ_EXIT，退出)
	iU8 (*set_game_run_mode)(iU8 mode);  //设置游戏模式，单机或联网版,对install.kit
	iU8 (*get_game_run_mode)(void);      //获取游戏模式,对game.kit
	void (*update_check)(char *game, void (*cb)(void));      //更新游戏版本
	void (*set_sound_state)(iBOOL music, iBOOL effects);    //设置是否开启声音
	void (*get_sound_state)(iBOOL *music, iBOOL *effects);  //获取当前是否开启声音

	/*fee_point 计费节点
	**title     "复活"或"购买道具"
	**cb        回调函数，返回值为： 
	**          <0 计费失败（联网失败,系统错误等）
	**          =0 取消计费(主动取消)
	**          0< 计费成功
	*/
	void (*fee_pay)(char *fee_point, char *title, void (*cb)(iS32));
	void (*fee_node_init)(char num, char *point, iU32 money, char *msg);   //install.kit用
	iS32 (*fee_node_save)(void);  
}i51Public_struct;


#endif  /* i51Public.h */