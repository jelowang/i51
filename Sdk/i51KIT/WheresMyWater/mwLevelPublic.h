
#ifndef _MW_LEVEL_PUBLIC_H_
#define _MW_LEVEL_PUBLIC_H_

#include "i51Public.h"
#include "mwPublic.h"
#include "mwLevelChoose.h"
#include "mwLevel001.h"
#include "mwLevel002.h"
#include "mwLevel003.h"
#include "mwLevel004.h"
#include "mwLevel005.h"
#include "mwLevel006.h"
#include "mwLevel007.h"
#include "mwLevel008.h"
#include "mwLevel009.h"
#include "mwLevel010.h"
#include "mwLevel011.h"
#include "mwLevel012.h"
#include "mwLevel013.h"
#include "mwLevel014.h"
#include "mwLevel015.h"

typedef enum
{
	MW_LEVEL_IDLE = 0,
	MW_LEVEL_INIT,
	MW_LEVEL_RUN,
	MW_LEVEL_WIN,
	MW_LEVEL_RESULT,
	MW_LEVEL_PAUSE,
	MW_LEVEL_RESUME,
	MW_LEVEL_EXIT	
}mwLevelState;

typedef enum
{
	MW_LEVEL_CHOOSE = 0,
	MW_LEVEL_001,
	MW_LEVEL_002,
	MW_LEVEL_003,
	MW_LEVEL_004,
	MW_LEVEL_005,
	MW_LEVEL_006,
	MW_LEVEL_007,
	MW_LEVEL_008,
	MW_LEVEL_009,
	MW_LEVEL_010,
	MW_LEVEL_011,
	MW_LEVEL_012,
	MW_LEVEL_013,
	MW_LEVEL_014,
	MW_LEVEL_015,
	MW_LEVEL_END
}mwLevelNum;

#define MW_LEVEL_MAX	MW_LEVEL_015

#define TOTAL_DUCK_NUM	45
#define TOTAL_LEVEL		15



// 当前是第几关的标记
extern mwLevelNum CurrLevelNum;
// 当前关卡内状态的标记
extern mwLevelState CurrLevelState;
// 当前世界的标记，记录每一个像素点的属性
extern iU8 mwWorldAttribute[SCREEN_HEIGHT_HALF][SCREEN_WIDTH_HALF];
// 每关得到的鸭子数
extern iU8 LevelDuckNum[TOTAL_LEVEL];
// 已经解锁的关卡数
extern iU32 UnlockMaxLevel;
// 当前总得到的鸭子数
extern iU32 CurrTotalDuck;
// 关卡暂停的标记
extern iU8 mwWordPause;
// 音乐开关的标记
extern iU8 MusicCloseFlag;
// 音效开关的标记
extern iU8 SoundCloseFlag;
// 关卡开始的时间
extern iU32 TimeLevelStart;
// 关卡当前的时间
extern iU32 TimeLevelCurr;
// 屏幕buff备份
extern iU16 TempScreenBuf[SCREEN_HEIGHT][SCREEN_WIDTH];
// 鳄鱼等待水来动画结构
extern AnimationStr CrocodileWait;
// 鳄鱼洗澡动画结构
extern AnimationStr CrocodileBathe;
// 海藻状态,为非零则有海藻为0则无海藻
//extern iU8  AlgaeMess[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength];
// 关卡内是否包含海藻的标记
extern iU8  AlgaeNeed ;
// 机关具体信息
extern OrganData OrganMess[MAX_ORGAN];
// 机关管理器
extern OrganManage Organ_Manage;

extern iBOOL mwStateGame( void );
extern iBOOL mwInitSpace( mwLevelNum level);
extern iBOOL mwLevelExit( mwLevelNum Level);
extern iBOOL mwLevelInit( mwLevelNum Level);
extern iBOOL mwLevelPause( mwLevelNum Level);
extern iBOOL mwLevelResume( mwLevelNum Level);
extern iBOOL mwLevelRun( mwLevelNum level);
extern iBOOL mwLevelWin( mwLevelNum level);
extern iBOOL mwLevelResult( mwLevelNum level);
extern iBOOL mwDrawNumStep( iHANDLE Container, iU32 ImageID, iRECT * DesRect, iU32 Number1, iU32 Number2 );

extern iBOOL mwLevelDrawRestart( void );
extern iBOOL mwLevelPressRestart( iU16 X, iU16 Y);
extern iBOOL mwLevelDrawPause( void );
extern iBOOL mwLevelPressPause( iU16 X, iU16 Y);
extern iBOOL mwLevelDrawBKwall( void );
extern iBOOL mwLevelDrawReturn( void );
extern iBOOL mwLevelPressReturn( iU16 X, iU16 Y);
extern iBOOL mwEarthCutting( mwLevelNum level );
extern iBOOL mwLevelWordInit( mwLevelNum level );
extern iBOOL mwLevelDuckInit(mwLevelNum level );
extern iBOOL mwLevelPipeInit( mwLevelNum level  );
extern iBOOL mwLevelDrawPrompt( mwLevelNum level );
extern iBOOL cpWordDraw( void );
extern iBOOL mwLevelRedrawBG( void );
extern iBOOL mwLevelRedrawFG( void );
extern iBOOL mwLevelDrawDuck( void  );
extern iBOOL mwTempScreenBufUpdate( mwLevelNum level );
extern iBOOL mwLevelWaitInit( mwLevelNum level );
extern iBOOL mwLevelBatheInit( mwLevelNum level );
extern iBOOL mwLevelAlgaeInit( mwLevelNum level );
extern iBOOL mwLevelWaterSpray( mwLevelNum level );
extern iBOOL mwLevelOrganInit( mwLevelNum level  );
extern iBOOL mwLevelDrawOrgan( void  );
extern iBOOL mwLevelSpeedKeepInit( mwLevelNum level );
extern iBOOL mwLevelSetImageMem( mwLevelNum level );
extern iBOOL mwLevelUnsetImageMem( mwLevelNum level );

#endif

