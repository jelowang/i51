#ifndef _MW_I51PUBLIC_H_
#define _MW_I51PUBLIC_H_

#include "i51.h"
#include "i51KitAde.h"
#include "i51KitStd.h"
#include "i51KitG2.h"
#include "WheresMyWaterRes.h"
#include "LLPInput.h"
#include "i51Public.h"
#include "i51KitP2.h"
#include "i51KitG1.h"

#include "mwStateMain.h"
#include "mwStateSet.h"
#include "mwStateExit.h"
#include "mwStateMode.h"

#define PKIG_SCREEN_320X480

#if defined(PKIG_SCREEN_240X320)
	#define SCREEN_WIDTH	240
	#define SCREEN_HEIGHT	320
	#define SCREEN_WIDTH_HALF	120
	#define SCREEN_HEIGHT_HALF	160
#elif defined(PKIG_SCREEN_240X400)	
	#define SCREEN_WIDTH	240
	#define SCREEN_HEIGHT	400
	#define SCREEN_WIDTH_HALF	120
	#define SCREEN_HEIGHT_HALF	200
#elif defined(PKIG_SCREEN_320X480)	
	#define SCREEN_WIDTH	320
	#define SCREEN_HEIGHT	480
	#define SCREEN_WIDTH_HALF	160
	#define SCREEN_HEIGHT_HALF	240
	#define RETURN_BUTTON_X 19
	#define RETURN_BUTTON_Y 13
	#define RETURN_BUTTON_W 50
	#define RETURN_BUTTON_H 37
	// 当前屏幕分辨率类型
	#define CURR_SCREEN_TYPE	0
	// 水滴外形半径
	#define WATER_RADIUS	300
	// 水管喷出水滴外形半径
	#define SPRAY_RADIUS	500
	// 水滴弹性系数
	#define WATER_E 0.0001f
	// 水滴摩擦系数
	#define WATER_U 0.00001f
	// 轮廓弹性系数
	#define SHAPE_E 0.0001f
	// 轮廓摩擦系数
	#define SHAPE_U 0.0001f
	// 石头轮廓直线厚度
	#define ROCK_SEG_R	400
	// 浴缸轮廓直线厚度
	#define BATH_SEG_R	400
	//矩形轮廓直线厚度
	#define RECT_SEG_R	400
	// 水滴质量
	#define MASS_WATER	40
	
//	extern iS16 RectSegR[15];
//	extern iS16 RockSegR[15];
//	extern iS16 BathSegR[15];

//关卡或者功能未开放的提示持续时间
#define Prompt_Time  2000
//关卡或者功能未开放的提示文字坐标
#define PromptWord_X 64
#define PromptWord_Y 186
#define PromptWord_W 193
#define PromptWord_H 25
//关卡或者功能未开放的提示框坐标
#define PromptFrame_X 30
#define PromptFrame_Y 111
#define PromptFrame_W 256
#define PromptFrame_H 256

#define SideLength 10

// 增加的外形约束类型
// 不能删除
#define SHAPE_TYPE_NONDELETABLE	0
// 可以删除
#define SHAPE_TYPE_DELETABLE	1
// 红色机关
#define SHAPE_TYPE_ORGAN_RED	2
// 蓝色机关
#define SHAPE_TYPE_ORGAN_BLUE	3
// 绿色机关
#define SHAPE_TYPE_ORGAN_GREEN	4
// 机关已经更新
#define SHAPE_TYPE_UPDATA		5
// 关卡开始时提示文本的时间
#define PromptShowTime	1500

#define Gridding_MUD_No 	0X00
#define Gridding_MUD_Yes 	0X01
#define Gridding_MUD_Update 0X02
#define Gridding_MUD_FULL 	0X04

#define Gridding_TYPE_SPACE	0X0
#define Gridding_TYPE_TOP	0X1
#define Gridding_TYPE_RT		0X2
#define Gridding_TYPE_RIGHT	0X3
#define Gridding_TYPE_RB		0X4
#define Gridding_TYPE_BUTTOM	0X5
#define Gridding_TYPE_LB		0X6
#define Gridding_TYPE_LEFT	0X7
#define Gridding_TYPE_LT		0X8

#define Gridding_TYPE_TOP_3		0X9
#define Gridding_TYPE_BUTTOM_3	0XA
#define Gridding_TYPE_LEFT_3		0XB
#define Gridding_TYPE_RIGHT_3		0XC

// 世界管理器中各个区域的属性
//空
#define WORD_SPACE	0X00
// 泥巴
#define WORD_MUD	0X01
//泥巴边界
#define MUD_EDGE	0X04
// 石头
#define WORD_ROCK	0X02
//石头边界
#define ROCK_EDGE	0XF0
// 海藻
#define WORD_ALGAE	0X08
// 新增海藻
//#define ALGAE_ADD	0X18
// 无需绘制的海藻
//#define ALGAE_NOSHOW	0XF8
// 区域已经绘制过水滴
//#define WATER_DRAW	0X10


#endif

#define MAX_SCREEN_TYPE	3

#define SHAPE_TYPE_SPACE	0
#define SHAPE_TYPE_MUD		1

typedef enum
{
	// 空
	GAME_STATE_IDLE = 0,
	// 启动
	GAME_STATE_START,
	// 主界面
	GAME_STATE_MAIN,
	// 游戏模式选择
	GAME_STATE_MODE,
	// 游戏中
	GAME_STATE_PLAY,
	// 设置
	GAME_STATE_SET,
	// 退出
	GAME_STATE_EXIT

}mwGameState;

#define MW_CONTAIN_PATH (L"WheresMyWater\\wmw.contain")
#define MW_IMAGERE_PATH (L"WheresMyWater\\wmw.re")
#define MW_DATA_PATH (L"WheresMyWater\\GameData.re")

#define WORD_GRAVITY_X	0
#define WORD_GRAVITY_Y	1000

// 世界指针
extern iWorld * mwSpace;
// 屏幕指针
extern iU16* mwScreenBuffer;
// 按键队列
extern iHANDLE mwKeyQueue;
// 获得的按键数据
extern LLPInputStruct mwKeyData;
// 图片容器句柄
extern iHANDLE mwContainer;
// 游戏第一级状态机标记
extern mwGameState CurrGameState;
// 应用程序 ID
//extern iS32 AppID;

#define WaterSpray_MAX	40
#define WaterSpray_MIN	20

// 当前游戏中水滴总数
extern iU32 CurrTotalWaterNum;
// 释放允许龙头喷水的标记
extern iU32 WaterSprayFlag;

extern iS32 TimeCount;

// 水滴颜色
#define COLOR_WATER		0X07FF
// 水滴alpha值
extern iU8 COLOR_ALPHA;
// 水滴已经到达浴缸
extern iU8 WaterDrawAll;
// 绘制半透明圆
extern iBOOL mwFillCircle( iS32 X, iS32 Y, iU32 R, iCOLOR Color);


// 鸭子信息
typedef struct DuckData
{
	// 鸭子已经被水冲击的次数
	iU8 WaterNum;
	// 鸭子是否还存在的标记
	iU8 DuckExist;
	// 鸭子上一次被水冲击的时间
	iU32 OldRushTime;
	// 鸭子的坐标和区域
	iRECT Rect;
}DuckData;

// 鸭子管理器
typedef struct DuckManage
{
	// 总鸭子数量
	iU8 TotalDuckNum;
	// 剩余鸭子数量
	iU8 ResidueDuckNum;
	//  鸭子详情
	DuckData * DuckMess;
}DuckManage;

//在一定条件下，固定区域内的水滴速度恒定
typedef struct SpeedKeep
{
	// 是否开始恒速的标记
	iU8	KeepSpeedFlag;
	// X 方向速度
	iS32 SpeedX;
	// Y方向速度
	iS32 SpeedY;
	// X位移
	iS32 MoveX;
	// Y位移
	iS32 MoveY;
	// 速度恒定的区域
	iRECTEX Rect;
}SpeedKeep;

// 恒速区域管理器
typedef struct SpeedKeepManage
{
	// 恒速区域数量
	iU8 TotalSpeedKeepNum;
	// 当前活动的恒速区域数量
	iU8 ActiveSpeedKeepNum;
	//  鸭子详情
	SpeedKeep * SpeedKeepMess;
}SpeedKeepManage;

#define SpeedKeepNum 	3

//恒速区域 详情
extern SpeedKeep SpeedKeepMess[SpeedKeepNum];
// 恒速区域管理器
extern SpeedKeepManage SpeedKeep_Manage;

// 机关启动需要被水冲击次数
#define ORGAN_MAX_WATER	2
// 机关被水流冲击时两次冲击的时间间隔
#define ORGAN_TIME_DELAY	300

// 机关类型
#define ORGAN_TYPE_RED		1
#define ORGAN_TYPE_BLUE		2
#define ORGAN_TYPE_GREEN	3
// 机关状态
#define ORGAN_STATE_SRC		1
#define ORGAN_STATE_MOVE	2
#define ORGAN_STATE_DES		3

// 机关方向水平
#define ORGAN_DIR_LEVEL		1
// 机关方向垂直
#define ORGAN_DIR_VERTICAL		2

typedef struct OrganData
{
	// 机关类型，分红绿蓝三种
	iU8 OrganType;
	// 机关当前状态，分初始，移动和移动停止三种
	iU8 OrgneState;
	// 机关已经被水冲击的次数
	iU8 WaterNum;
	// 机关方向，有水平和垂直之分
	iU8 Direction;
	// 机关启动后X方向移动的增量
	iS32 OrganMulX;
	// 机关启动后Y方向移动的增量
	iS32 OrganMulY;
	// 机关上次被水冲击的时间
	iU32 OldTime;
	// 启动机关的区域
	iRECTEX KeyRect;
	// 机关未启动时原始区域
	iRECTEX OrganStartRect;
	// 机关启动后停止区域
	iRECTEX OrganEndRect;
	// 机器当前位置
	iRECTEX OrganCurrRect;
}OrganData;

// 机关管理器
typedef struct OrganManage
{
	// 总机关数量
	iU8 TotalOrganNum;
	// 剩余可活动关卡数量
	iU8 ResidueOrganNum;
	//  机关详情
	OrganData* OrganMess;
}OrganManage;


// 鸭子对水冲击的最小时间间隔
#define DUCK_RUSH_DELAY_TIME 300
// 鸭子最多杯水冲击次数
#define DUCK_MAX_RUSH_NUM	2
//每一关鸭子个数
#define DUCK_NUM	3
// 鸭子坐标
extern DuckData DuckMess[DUCK_NUM];
// 鸭子管理器
extern DuckManage Duck_Manage;

// 水滴经过水管后坐标不变
#define PipeOutX_HOLD	0X10FF
#define PipeOutY_HOLD	0X11FF

// 水滴经过一些转向水管后的速度损失
#define PipeSpeedWeight	4/5

// 水滴经过水滴后XY方向的速度分别不变、等于经过前的Y,等于0,取X反,取Y反
#define PipeSpeedX_HOLD	0X12FF
#define PipeSpeedXFromY	0X13FF
#define PipeSpeedXZeor	0X14FF
#define PipeSpeedX_INYX	0X15FF
#define PipeSpeedX_INYY	0X16FF

#define PipeSpeedY_HOLD	0X17FF
#define PipeSpeedYFromX	0X18FF
#define PipeSpeedYZeor	0X19FF
#define PipeSpeedY_INYY	0X1AFF
#define PipeSpeedY_INYX	0X1BFF


// 水管区域信息
typedef struct PipeData
{
	// 水管当前是否有效的标记
	iU8 ValidityFlag;
	// 进水管区域
	iRECT RectWaterIn;
	// 出水管区域
	iS16 WaterOutX;
	iS16 WaterOutY;
	iS16 SpeedX;
	iS16 SpeedY;
}PipeData;

// 水管管理器
typedef struct PipeManage
{
	// 当前水管数量
	iU8 PipeNum;
	// 水管具体信息
	PipeData * Data;
}PipeManage;

// 过关需要的水滴数
#define WIN_WATER	10
// 水管最多组数
#define MAX_PIPE	7
// 机关最多组数
#define MAX_ORGAN	3
// 水管具体信息
extern PipeData PipeMess[MAX_PIPE];
// 水管管理器
extern PipeManage Pipe_Manage;
// 浴缸中已有水滴数量
extern iS16 WaterInBathtub;
// 获得时间奖励的分数
extern iS16 CurrTimeWin;
// 获得鸭子奖励的分数
extern iS16 CurrDuckWin;
// 获得溢水奖励的分数
extern iS16 CurrWaterWin;
// 关卡获得的总分
extern iS16 CurrTotalWin;
// 鸭子是否减少的标记
extern iU8 DuckResidueFlag;
// 屏幕临时Buff需要更新的标记
extern iU8 TempScreenUpdataFlag;
// 每个网格中水滴数量
//extern iS8 GriddingWaterNum[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength];

// 选择时的背景音乐名字
#define MW_SOUND_NAME_CHOOSE	(L"WheresMyWater\\audio\\SoundChoose.wav")
// 选择时的背景音乐名字长度
#define MW_SOUND_LENGTH_CHOOSE	56

// 获胜时的背景音乐名字
#define MW_SOUND_NAME_WIN	(L"WheresMyWater\\audio\\SoundWin.wav")
// 获胜时的背景音乐名字长度
#define MW_SOUND_LENGTH_WIN	50
// 是否有声音的宏开关
#define SOUND_NEED	1
// 选择的音乐是否在播放的标记
extern iU8 SoundChoosePlay;
// 胜利的音乐是否在播放的标记
extern iU8 SoundWinPlay;

// 当前世界的标记，记录每一个像素点的属性
extern iU8 mwWorldAttribute[SCREEN_HEIGHT_HALF][SCREEN_WIDTH_HALF];

#define ANIMATION_MAX_FRAME	15
//图片动画结构
typedef struct AnimationStr
{
	// 图片所在容器文件句柄
	iHANDLE Container;
	// 播放图片的ID数组
	iU16 ImageId[ANIMATION_MAX_FRAME];
	// 图片宽度
	iS16 ImageWidth;
	// 图片高度
	iS16 ImageHeight;
	// 播放上一帧的时间
	iU32 OldPlayTime;
	// 当前播放的帧数
	iS16 CurrPlayFrame;
	// 图片总帧数
	iS16 ImageTotalFrame;
	// 两帧动画间的时间间隔
	iU32 FrameTimeMul;
	// 播放一帧的图片高度
	iU32 FrameHeight;
	// 图片显示的目的地区域
	iRECT DesRect;
	
}AnimationStr;

// i51 函数指针结构
extern i51Public_struct *pfunc;

extern iBOOL mwFillCircle( iS32 X, iS32 Y, iU32 R, iCOLOR Color);
extern iBOOL DrawWaterDrop(iBody *body, iShapeCircle *circle, iCOLOR Color);
extern iBOOL mwPublicAnimation( AnimationStr * AnimationMess );
extern iBOOL mwPublicAnimationEx( AnimationStr * AnimationMess );
extern iBOOL mwPublicDrawPrompt( iU32 DisPlayTime, iU32 StartTime, iU32 DisplayID );

#define WATER_MAX_SPEED 1200

#ifndef null
#define null 0//((void *)0)
#endif

#endif  /* i51Public.h */

