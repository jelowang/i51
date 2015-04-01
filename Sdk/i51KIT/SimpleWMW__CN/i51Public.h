#ifndef I51PUBLIC_H
#define I51PUBLIC_H
#include "i51.h"
#include "i51KitAde.h"
#include "Image_Hex.h"
#include "LLPInput.h"
#include "localG2.h"
//#include "version.h"
#include "i51KitP2.h"
///////////////////////////////////// 根据每个游戏不同 在此修改 //////////////////////////////////////////

#define PKIG_SCREEN_VERTICAL  //#define PKIG_SCREEN_HORIZONTAL  //横屏  PKIG_SCREEN_VERTICAL 竖屏  
#define INSTALL_HORIZONTAL      //INSTALL_VERTICAL  安装进度条为竖屏。      INSTALL_HORIZONTAL  横屏
//#define MODE_LOCAL  //是否跑完整的单机版本，完整的游戏(和联网版失败后的单机版不同)
#define  LANGUAGE_CHINESE//中文版
//#define LANGUAGE_ENGLISH  //英文版的宏
//#define PKIG_SCREEN_320X480
//#define PKIG_SCREEN_240X400
#define AUDIO_PLAYER_MIDI_NUMS  (4)  //midi播放器的数目
#define AUDIO_PLAYER_WAV_NUMS   (0)  //wav播放器的数目    //注意midi_nums + wav_nums <= 5

#define GAME_DIR       "SimpleWMW"       //游戏目录
///* game */
//#define GAME_KIT_VERSION (5)   //game.kit的版本号，每次更新需更改
//#define GAME_KIT_NUM   1            //kit的个数
//#define GAME_KIT       {"DdzKit.kit"}   //游戏kit
//#define GAME_RESOURCE_NUM   1   //re的个数
//#define GAME_RESOURCE  {"DdzKit.re"}    //游戏资源
//#define  CONTAINER_NAMES      {"game.container"}  //容器名字
//#define  RESOURCE_IMAGE_NUMS  {76}   //每个资源文件的图片数量
//
//#define GAME_PACKAGE_URL    "app://%s/%d/DdzKit/"    //服务器上下载的游戏url，需要修改最后的cf，改为你自己的包名
//
///* install */
//#define INSTALL_KIT_VERSION  (1)
//#define INSTALL_KIT_NAME  "Dinstall.kit"
//#define INSTALL_RESOURCE_NUM  (2)  //re的个数，install.re和UI_Frame.re
//#define INSTALL_RESOURCE  {"Dinstall.re", "UI_frame.re"}
//#define INSTALL_CONTAINER {"install.container", "UI_frame.container"}
//
//#define KIT_GAME_PATH      L"DDZ\\DdzKit.kit"       //kit的路径
//#define KIT_INSTALL_PATH   L"DDZ\\install.kit"

//#ifdef GAME_LOGO
//#define INSTALL_RESOURCE_IMAGE_NUMS  {4, 17}   //install.re里的图片数
//#else
//#define INSTALL_RESOURCE_IMAGE_NUMS  {3, 17}
//#endif

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

#define i51KITG2_ALPHA_NONEED (0xFF) //xie

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

enum
{
	SRC_IMAGE_WIDTH = 9999,
	SRC_IMAGE_HEIGHT
};

typedef struct 
{
	iU32 srcData;
	iU32 pContainer;
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

	AUDIO_PLAYER_MP3,

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


//#define  __MODIS_DEBUG__

extern iBOOL find_aim_dir(char *dir);

extern iBOOL check_file_is_exist(char *dir, char *file);

extern void public_itoa(iS32 value, char *str);

extern void public_image_init(public_image_struct *image, iU32 image_id, iU32 image_data);

extern void public_image_destroy(public_image_struct *image);

extern iRECT *public_build_rect(iRECT *rect, iS16 x, iS16 y, iS16 width, iS16 height);

extern iPoint *public_build_point(iPoint *point, iS32 x, iS32 y);

extern iS32 public_draw_image(public_image_struct *image, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h);

extern iU8 public_pen_enabled_rect(iU16 pen_x, iU16 pen_y, iU16 min_x, iU16 max_x, iU16 min_y, iU16 max_y);

extern iS32 public_write_file(char *file_name, void *data, iU32 size);

extern iS32 public_read_file(char *file_name, void *data);

extern iS32 public_delect_file(char *dir, char *filename);

extern iBOOL public_audio_init(void (*cb)(iBOOL results));

extern void public_audio_all_exit(void);

extern void audio_resume(iBOOL results);

extern void audio_play_cb(void* handle);

extern iBOOL public_audio_play_x(iU32 priority, void *audio_data, iU32 audio_len, i51MMIDEC mem_or_file, iU32 channel, iBOOL loop);

extern iBOOL public_audio_stop(audio_player_type player_type);

extern iU8 save_screen(void);

extern iU8 resume_screen(void);

extern iBOOL time_is_second(void);

extern void public_draw_score(iU32 score, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h, public_image_struct *image);




extern public_image_struct iMageInfo[];

extern iU8 GameData[];

extern iBOOL iScreenFlag;

extern iS32 SWMW_i51KitG2DrawImage(public_image_struct *image, iRECT *dstRect, iRECT *srcRect);

extern void wmw_main_page();

extern void wmw_choose_page();

extern void wmw_image_destroy();

extern void wmw_partol_page();

extern void wmw_game_page();

extern void wmw_pause_page();

extern void wmw_win_page();

extern void wmw_result_page();

extern void wmw_destroy_win_page();

extern void wmw_destroy_result_page();

extern void portal_cb(iBOOL data);

extern void wmw_portal_ui();

extern unsigned char wmw_SoundChoose_mp3[];

extern unsigned int wmw_SoundChoose_mp3_len;

extern unsigned char wmw_SoundWin_mp3[];

extern unsigned int wmw_SoundWin_mp3_len;

extern iVECT INTERSECTION_P2S[10];

extern iS32 IntersectionNum;

extern iU32 CurrDirection;

extern iS32 DirectionNum;

extern iS16 se_TrackPointCount;

extern iS16 se_TrackPoint[6][2];


extern iU8 IsCompleten;

extern iU32 word_frame;

extern iU8 IsEntry;
//
////extern G1_EDGE_TABLE  G1_EdgeTable;
//
extern audio_struct audio[AUDIO_PLAYER_NUM_MAX];

extern iS8 ChooseErrFlag;

extern iU32 ChooseErrTime;

extern iU8 mwLevelWinFlag;


extern iU8 mwLevelResultFlag;

extern iU32 LevelErrStartTime;

extern iU8 LevelErrFlag;

extern iU8 DrawBlankAlpha;

extern iS16 OldGridding_X;

extern iS16 OldGridding_Y;

extern iS16 CurrGridding_X;

extern iS16 CurrGridding_Y;




#endif 
