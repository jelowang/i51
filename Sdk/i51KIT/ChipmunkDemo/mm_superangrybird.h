#ifndef __MM_SUPERANGRYBIRD_H__
#define __MM_SUPERANGRYBIRD_H__
#include "chipmunk.h"
//#include "../marmota.h"
#include "marmota_types.h"
#include "AngryBird.h"

//enum mm_concubine_state{MM_SUPERANGRYBIRD_MAINMENU, MM_SUPERANGRYBIRD_KERNEL, MM_SUPERANGRYBIRD_STATE_MAX};

#define MM_IMAGE_BASE_RANGE(name, count)    \
	IMG_ANGRYBIRD_##name,                   \
	IMG_ANGRYBIRD_##name##_END = IMG_ANGRYBIRD_##name + count - 1
	
#define MM_IMAGE_MATERIAL_RANGE(name)	\
	MM_IMAGE_BASE_RANGE(name##_MAX, 2), \
	MM_IMAGE_BASE_RANGE(name##_MID, 2), \
	MM_IMAGE_BASE_RANGE(name##_LIT, 2)
//	MM_IMAGE_BASE_RANGE(name##_TIN, 2),

typedef enum {
	MM_IMAGE_BASE_RANGE(BRACKET, 2),
	
	MM_IMAGE_BASE_RANGE(BIRD_RED, 4),
	MM_IMAGE_BASE_RANGE(BIRD_BLUE, 2),
	MM_IMAGE_BASE_RANGE(BIRD_YELLOW, 2),
	MM_IMAGE_BASE_RANGE(BIRD_BLACK, 2),
	
	MM_IMAGE_BASE_RANGE(PIG_SML, 2),
	MM_IMAGE_BASE_RANGE(PIG_BIG, 2),
	
	IMG_ANGRYBIRD_MONKEY,
	
	MM_IMAGE_BASE_RANGE(EFFECT, 9),
	
	MM_IMAGE_MATERIAL_RANGE(WOOD),
	MM_IMAGE_MATERIAL_RANGE(STONE),
	IMG_ANGRYBIRD_STONE_TIN,//毛石块
	IMG_ANGRYBIRD_STONE_TIN_END,
	MM_IMAGE_MATERIAL_RANGE(ICE),
	
	MM_IMAGE_BASE_RANGE(BIG_WOOD, 2),
	MM_IMAGE_BASE_RANGE(BIG_STONE, 2),
	MM_IMAGE_BASE_RANGE(STONE_TRI, 2),
	MM_IMAGE_BASE_RANGE(ICE_BOX, 2),
	
	IMG_ANGRYBIRD_BOMB_EFFECT0,
	
	IMG_ANGRYBIRD_GROUND,//地板
	IMG_ANGRYBIRD_LANTERN,//灯笼,自动往上找东西来绑
	IMG_ANGRYBIRD_LANTERN1,//灯笼1，不会自动往上找东西来绑
	IMG_ANGRYBIRD_MOONCAKE,//月饼
	
	IMG_ANGRYBIRD_CAP,//帽子
	
	IMG_ANGRYBIRD_BARRIER_TIN,
	IMG_ANGRYBIRD_BARRIER_LIT,
	IMG_ANGRYBIRD_BARRIER_MIN,
	IMG_ANGRYBIRD_BARRIER_MAX,
	
	
}mm_sab_imageid;

typedef enum{
	IMG_ANGRYBIRD_UI_WORDS_PASS,
	IMG_ANGRYBIRD_UI_WORDS_NOTPASS,
	IMG_ANGRYBIRD_UI_WORDS_STAGE,
	
	IMG_ANGRYBIRD_UI_NUMBER_0,
	IMG_ANGRYBIRD_UI_NUMBER_9 = IMG_ANGRYBIRD_UI_NUMBER_0 + 9,
	
	IMG_ANGRYBIRD_UI_AGAIN,
	IMG_ANGRYBIRD_UI_NEXT,
	IMG_ANGRYBIRD_UI_STAGE_SELECTOR,
	IMG_ANGRYBIRD_UI_MAINMENU,
	
	IMG_ANGRYBIRD_UI_SOUND_ON,
	IMG_ANGRYBIRD_UI_HELP,
	IMG_ANGRYBIRD_UI_CONTINUE,
	IMG_ANGRYBIRD_UI_PAUSE,
	IMG_ANGRYBIRD_UI_SOUND_OFF,
	
	IMG_ANGRYBIRD_UI_YELLOWSTAR,
	IMG_ANGRYBIRD_UI_BLACKSTAR,
	
	IMG_ANGRYBIRD_UI_WORD_CURRECORD,
	IMG_ANGRYBIRD_UI_WORD_STAGE,
	IMG_ANGRYBIRD_UI_WORD_MAXRECORD,
	
	IMG_ANGRYBIRD_UI_SCORENUMBER_0,//27
	IMG_ANGRYBIRD_UI_SCORENUMBER_9 = IMG_ANGRYBIRD_UI_SCORENUMBER_0 + 9,
	
	IMG_ANGRYBIRD_UI_SCORENUMBER_BIRD,
	IMG_ANGRYBIRD_UI_SCORENUMBER_BIRD_MAX = IMG_ANGRYBIRD_UI_SCORENUMBER_BIRD + 7,
	
	IMG_ANGRYBIRD_UI_SCORENUMBER_PIG,
	IMG_ANGRYBIRD_UI_SCORENUMBER_PIG_MAX,
	
	IMG_ANGRYBIRD_UI_SCORENUMBER_LANTERN,//0
	IMG_ANGRYBIRD_UI_SCORENUMBER_LANTERN_MAX,//2
	
	IMG_ANGRYBIRD_UI_PATH_POINT,
	IMG_ANGRYBIRD_UI_NEWRECORD,
	
	IMG_ANGRYBIRD_UI_LINE,//画地板的线
};

//extern const mmRectangle mm_sab_raw_image[];
extern const mmPoint BIRD_STARTPOSITION;
//extern gdi_handle mm_superangrybird_bglayers[];
extern mmS32 layer_offset_y;
extern void mm_sab_destoryspace(void);
extern void mm_sab_initspace(int stageId);
extern void mm_sab_runspace();
extern void mm_sab_setangrybird_v(mmFloat x, mmFloat y);

//extern mmCanvas *mmSABBaseCanva;
//extern mmCanvas *mmSABSceneCanva;
extern mmPoint birdPosition;
extern mmS32 cameraOffsetY;
extern mmS32 mm_sba_getbirdposition_y();


typedef struct mmSABStageObject{
	mmU32 objectType;
	cpVect pos;
	mmFloat a;
}mmSABStageObject;

typedef enum mmShapType{
	MM_CIRCLE_SHAPE,
	MM_BOX_SHAPE,
	MM_TRIANGLE_SHAPE,
	MM_STATIC_SHAPE,
	
	MM_JOINTER,
	
}mmShapType;

typedef struct mmSABObjectType{
	mmShapType shapetype;
	mmFloat m;
	mmFloat e;
	mmFloat u;
	mmU32 img_id;
	//mmU32 hp;
	mmS32 hp;
	mmU32 score;
}mmObjectType,mmSABObjectType;

typedef enum {
	ANGRYBIRD_BIRD_RED,
	ANGRYBIRD_BIRD_BLUE,
	ANGRYBIRD_BIRD_YELLOW,
	ANGRYBIRD_BIRD_BLACK,
	
	ANGRYBIRD_PIG,
	ANGRYBIRD_PIG_BIG,
	ANGRYBIRD_MONKEY,//猴子
	
	ANGRYBIRD_WOOD_MAX,
	ANGRYBIRD_WOOD_MID,
	ANGRYBIRD_WOOD_LIT,

	ANGRYBIRD_ICE_MAX,
	ANGRYBIRD_ICE_MID,
	ANGRYBIRD_ICE_LIT,

	ANGRYBIRD_STONE_MAX,
	ANGRYBIRD_STONE_MID,
	ANGRYBIRD_STONE_LIT,
	ANGRYBIRD_STONE_TIN,// = ANGRYBIRD_STONE_LIT,

	ANGRYBIRD_BIGWOOD,
	ANGRYBIRD_BIGSTONE,
	
	ANGRYBIRD_STONE_TRI,
	ANGRYBIRD_ICE_BOX,		//20*20的冰块
	
	/**TODO:到时删掉***/
	ANGRYBIRD_STATIC_MAX,
	ANGRYBIRD_STATIC_MID,
	ANGRYBIRD_STATIC_TIN,
	/******************/
	
	ANGRYBIRD_LANTERN,//灯笼,自动往上找东西来绑
	ANGRYBIRD_LANTERN1,//灯笼1，不会自动往上找东西来绑
	ANGRYBIRD_MOONCAKE,//月饼
	ANGRYBIRD_CAP,
	
	//静态障碍物
	ANGRYBIRD_BARRIER_TIN,
	ANGRYBIRD_BARRIER_LIT,
	ANGRYBIRD_BARRIER_MID,
	ANGRYBIRD_BARRIER_MAX,
	
	
	
	ANGRYBIRD_JOINT,
}ANGRYBIRD_OBECT_TYPE;

extern const mmU32 mmNumberOfStageObjects[];
extern const mmSABStageObject mmSABStageObjectsParam[];
extern const mmSABObjectType mmSABObjectsParam[];

typedef enum mmSABBirdAnimation{
	mmSABBirdAnimation_IDLE,
	mmSABBirdAnimation_BLINK,
	mmSABBirdAnimation_JUMP,
	mmSABBirdAnimation_SING,
	mmSABBirdAnimation_FLYING,
	mmSABBirdAnimation_DIZZY,
	mmSABBirdAnimation_MAX,
}mmSABBirdAnimation;

typedef enum mmSABBirdState{
	mmSABBirdState_IDLE,
	mmSABBirdState_JUMP,
	mmSABBirdState_ONLINE,
	mmSABBirdState_FLYING,
	mmSABBirdState_DIZZY,
}mmSABBirdState;

typedef struct mmSABBird;
#ifdef zq
typedef void (*mmSABSpecialCallBack)(mmSABBird *bird);
#else
typedef void (*mmSABSpecialCallBack)(void *bird);
#endif
typedef struct mmSABBird{
	cpBody *body;
	cpShape *shape;
	mmU32 birdtype;
	mmSABBirdState state; // state manage animation and switch staitc or acitive in physics mode.
	mmSABBirdState aimstate;

	mmSABSpecialCallBack specialSkill;
	mmSABBirdAnimation animid;
	mmU32 curframe;
	
	struct mmSABBird *next;
}mmSABBird;
extern mmSABBird* mmSABBirdListHead;
extern mmSABBird* mmSABOnlineBird;
extern mmSABBird* mmSABLastFlying;

typedef struct{
	mmS32 type_id;
	mmU32 effect_img_id;
	mmU32 img_id;
	mmS32 hp;
}mmSABBodyData;

typedef enum{
	ANGRYBIRD_COLLISION_BIRD,
	ANGRYBIRD_COLLISION_PIG,
	ANGRYBIRD_COLLISION_BARRIER,
	ANGRYBIRD_COLLISION_GROUND,
};

extern cpSpace *mmSABSpace;
extern mmBool mmSBAIsWatching;
extern mmS32 mmSABNumberOfBirds;
extern mmS32 mmSABNumberOfPigs;
extern mmU32 mmSABGameStageId;
extern void mmSABDrawUI(int x, int y, int id);
extern void mm_sab_mainmenu_draw_image(int x, int y, int img_id, mmBool transparentSign);
//extern void mmCreateLayerWithImage(mmPU8 img, gdi_handle *handle);

/******************菜单**************************/
typedef enum{
	/***/
	ANGRYBIRD_MAINMENU_BLACK_IMG,
	ANGRYBIRD_MAINMENU_YELLOW_IMG,
	ANGRYBIRD_MAINMENU_BLUE_IMG,
	ANGRYBIRD_MAINMENU_RED_IMG,


	ANGRYBIRD_MAINMENU_HELP_IMG,//小鸟帮助
	ANGRYBIRD_MAINMENU_HELP_TITLE_IMG,//帮助标题
	
	ANGRYBIRD_MAINMENU_LEVEL_TITLE_IMG,//关卡选择字
	
	ANGRYBIRD_MAINMENU_SOUND_OFF_IMG,//静音
	
	ANGRYBIRD_MAINMENU_LEVEL_UNLOCK_IMG,//未锁
	ANGRYBIRD_MAINMENU_LEVEL_LOCK_IMG,//锁
	
	ANGRYBIRD_MAINMENU_NUMBER0_IMG,//数字0-9
	ANGRYBIRD_MAINMENU_NUMBER1_IMG,
	ANGRYBIRD_MAINMENU_NUMBER2_IMG,
	ANGRYBIRD_MAINMENU_NUMBER3_IMG,
	ANGRYBIRD_MAINMENU_NUMBER4_IMG,
	ANGRYBIRD_MAINMENU_NUMBER5_IMG,
	ANGRYBIRD_MAINMENU_NUMBER6_IMG,
	ANGRYBIRD_MAINMENU_NUMBER7_IMG,
	ANGRYBIRD_MAINMENU_NUMBER8_IMG,
	ANGRYBIRD_MAINMENU_NUMBER9_IMG,
	
	
	ANGRYBIRD_MAINMENU_LITTLE_STAR_IMG,//小星星
	
	ANGRYBIRD_MAINMENU_QUESTION_IMG,
	ANGRYBIRD_MAINMENU_SOUND_IMG,
	ANGRYBIRD_MAINMENU_EXIT_IMG,
	ANGRYBIRD_MAINMENU_START_IMG,
	
};

typedef enum{
	ANGRYBIRD_MAINMENU_BEGIN,//开始游戏
	ANGRYBIRD_MAINMENU_EXIT,//退出
	ANGRYBIRD_MAINMENU_HELP,//问号
	ANGRYBIRD_MAINMENU_SOUND,//声音
	ANGRYBIRD_MAINMENU_LEAVE_HELP,//帮助退出
	
	ANGRYBIRD_MAINMENU_LEVEL_TITLE,
	ANGRYBIRD_MAINMENU_LEAVE_LEVEL,//关卡选项退出
	ANGRYBIRD_MAINMENU_LEVEL_ICON,//关卡图标起始位置
	ANGRYBIRD_MAINMENU_HELP_DES,//帮助介绍
	ANGRYBIRD_MAINMENU_HELP_TITLE,//帮助标题
	
};


//#define MMSAB_FILE_PATH		L"E:\\AngryBird_Data.dat"
//#define MMSAB_FILE_PATH		L"/CARD/AngryBird_Data.data"
#define MMSAB_FILE_PATH			"AngryBird_Data.dat"
#define MMSAB_LEVEL_SUM 21
#define MMSAB_MAX_PATHPOINT 60
#define MMSAB_SCREEN_HEIGHT 640
extern mmU32 mmSABSound;//0关1开
extern mmS32 mmSABMainMenu;//菜单界面0，1为关卡界面，2为帮助界面
extern void mmSABReadGameRestore();//将文件中数据读入mmSABLevelScore数组
extern void mmSABWriteGameRestore();//将数组写入文件

extern mmU32 mmSABLevelScore[MMSAB_LEVEL_SUM+1];//玩家已过关数及每关所得最高分



/******宏定义****/
//#define KKFUN_RES_OFFSET //标识资源没有移8个字节

#ifdef MAMOTA_BACKEND_MSTAR
	#define MMSAB_RES_PATH "/CARD/Res.dat"
#else
	#define MMSAB_RES_PATH  "E:\\Res.dat"
#endif

/****************/
/************************************************/
extern mmU32 mmGroundPointStart;
extern const mmPoint mmSABGroundPoints[];
extern const mmU32 mmSABNumberOfGroundPoint[];

extern const mmU32 mmSABNumberOfGroundPoint[];
//extern void mmRotateImageBuffer(mmCanvas * img_layer, int src_cent_x, int src_cent_y, int weight, int height, 
//	mmCanvas * dst_layer, int dst_cent_x, int dst_cent_y, mmFloat a);
	
typedef struct mmSABEffectObjectLNode{
	mmU32 img_id;
	mmU32 max_img_id;
	mmPoint p;
	mmFloat a;
	struct mmSABEffectObjectLNode *next;
}mmSABEffectObjectLNode;


typedef struct mmSABScoreObjectNode{
	int type_id;
	mmPoint pos;
	mmPoint end_pos;
	struct mmSABScoreObjectNode *next;
}mmSABScoreObjectNode;
extern mmSABEffectObjectLNode *mmSABEffectObjectListHead;
extern mmSABScoreObjectNode *mmSABScoreListHead;
extern void mmSABAddScoreObject(mmS32 x, mmS32 y, mmS32 type_id);
extern void mmSABBirdCalScore();
extern mmU32 mmSBAFullScoreOfStage;
extern mmU32 mmSBAScoreOfStage;

typedef enum{mmSABTooltip,mmSABShowScene,mmSABShowBirdDel,mmSABGotoHotare, mmSABUserOperation, mmSABPK, mmSABAuditScore, mmSABShowResult, mmSABGameOption,mmSABGameHelp}mmSABGameStaeType;
extern mmU32 mmSABGameState;

extern mmBool mmSABIsOccurSpecialTech;
extern mmBool mmSABIsInitial;
extern mmBool mmSABIsBackFromSuspend;
/***/
typedef struct mmSABirdFly{
		mmS32 type_id;
		mmPoint pos;
		mmS32 v_x;
		mmS32 v_y;
		float	     zoom;
		struct mmSABirdFly* next;
	}mmSABirdFly;
/****/	
/***2011-8-18****/
//typedef enum{
//	ANGRYBIRD_MAINMENU_RES_CANVA = 6,
//	ANGRYBIRD_MAINMENU_CANVA = 4,
//	ANGRYBIRD_MAINMENU_LEVEL_CANVA = 2,
//	ANGRYBIRD_SCENE_CANVA = 8,
//	ANGRYBIRD_UI_RES_CANVA = 9,
//	ANGRYBIRD_MOON_CANVA = 0,
//	ANGRYBIRD_STOREY_CANVA = 1,
//	ANGRYBIRD_RESULT_CANVA = 10,
//	ANGRYBIRD_PAUSE_CANVA = 7,
//	ANGRYBIRD_LOGO_CANVA = 3,
//	ANGRYBIRD_MAINMENU_LOGO_CANVA = 5,
//	
//};
/****************/
#endif