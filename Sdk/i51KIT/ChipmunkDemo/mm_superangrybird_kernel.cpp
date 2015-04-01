//#include "../marmota/marmota.h"
#include "inc/mm_superangrybird_resource.h"
#include "mm_superangrybird.h"
#include "marmota_math.h"

extern mmBool mmSABCollision;

extern cpShape* mmSABBlueBirds[2];
static mmU32 mmSABBlueNumOfPathPoint[2];
mmPoint *mmSABBluePathPoint[2];
extern mmBool mmSABBlueCollision[2];

extern "C" void mmDbgPrint(char *fmt,...);





#ifdef MM_SUPERANGRYBIRD_RESOURCE_240X320
#define SAB_UI_PAUSE_X	(UI_device_width - 40)
#define SAB_UI_PAUSE_Y	5
#define SAB_CURRENT_SCORE_Y 80
#define SAB_SCORE_POINT 117
#define SAB_SCORE_GAP 12
#define SAB_CAMERA_OFFSET_MAX 316
#elif defined MM_SUPERANGRYBIRD_RESOURCE_240X400
#define SAB_UI_PAUSE_X	(UI_device_width - 40)
#define SAB_UI_PAUSE_Y	10
#define SAB_CURRENT_SCORE_Y 80
#define SAB_SCORE_POINT 117
#define SAB_SCORE_GAP 12
#define SAB_CAMERA_OFFSET_MAX 236
#else
#define SAB_UI_PAUSE_X	(UI_device_width - 60)
#define SAB_UI_PAUSE_Y	10
#define SAB_CURRENT_SCORE_Y 100
#define SAB_SCORE_POINT 160
#define SAB_SCORE_GAP 15
#define SAB_CAMERA_OFFSET_MAX 156
#endif

//#ifndef KKFUN_RES_OFFSET
	static char mmSABResFullPath[128] = {'\0'};
//#endif

mmBool mmSABIsOccurSpecialTech;
mmBool mmSABIsBackFromSuspend = mmFalse;

static mmBool mmSABReadyShootSound = mmFalse;

static mmU32 mmSABSavePreSuspendScore;//保存中断前得最高分

extern mmBool mmSABCanOccurSpecial;
const mmU32 mmNumberOfStageObjects[] = {
11, 12, 11, 12,
7, 13, 14, 14,
9, 11, 10, 13,
11, 13, 11, 10,
14, 20, 9, 12,
9,
};
const mmSABStageObject mmSABStageObjectsParam[] = {
	
//11-1

	{ANGRYBIRD_WOOD_MID,		{57.0f, 425.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MID,		{57.0f, 515.0f}, __MM_PIO2__},
	
	{ANGRYBIRD_WOOD_MAX,		{82.0f, 430.0f}, 0},
	{ANGRYBIRD_LANTERN,			{62.0f, 400.0f}, 0},
	
	{ANGRYBIRD_WOOD_MAX,		{82.0f, 510.0f}, 0},
	{ANGRYBIRD_LANTERN,			{62.0f, 540.0f}, 0},
	
	{ANGRYBIRD_WOOD_LIT,		{95.0f, 445.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_LIT,		{95.0f, 495.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{110.0f, 470.0f}, 0},
	{ANGRYBIRD_PIG,			{50.0f, 485.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{50.0f, 450.0f}, 0},

//12-2

	{ANGRYBIRD_STONE_LIT,		{55.0f, 455.0f}, __MM_PIO2__},
	{ANGRYBIRD_STONE_LIT,		{55.0f, 515.0f}, __MM_PIO2__},
	{ANGRYBIRD_STONE_MID,		{72.0f, 440.0f}, 0},
	{ANGRYBIRD_STONE_MID,		{72.0f, 530.0f}, 0},
	{ANGRYBIRD_ICE_MID,		{93.0f, 445.0f}, __MM_PIO2__},
	{ANGRYBIRD_ICE_MID,		{93.0f, 525.0f}, __MM_PIO2__},
	
	{ANGRYBIRD_WOOD_MAX,		{118.0f, 485.0f}, 0},
	{ANGRYBIRD_LANTERN,		{98.0f, 485.0f}, 0},
	
	{ANGRYBIRD_ICE_LIT,		{129.0f, 485.0f}, __MM_PIO2__},

	{ANGRYBIRD_PIG,			{30.0f, 485.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{91.0f, 423.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{91.0f, 547.0f}, 0},
//11-3

	{ANGRYBIRD_BIGSTONE,		{58.0f, 330.0f}, __MM_PIO2__},
	{ANGRYBIRD_BIGWOOD,		{78.0f, 450.0f}, __MM_PIO2__},

	{ANGRYBIRD_WOOD_MID,		{103.0f, 440.0f}, 0},
	{ANGRYBIRD_LANTERN,		{83.0f, 425.0f}, 0},
	
	{ANGRYBIRD_ICE_BOX,		{90.0f, 570.0f}, 0},
	{ANGRYBIRD_ICE_BOX,		{110.0f, 570.0f}, 0},
	{ANGRYBIRD_ICE_MID,		{125.0f, 570.0f}, 0},
	
	{ANGRYBIRD_MONKEY,		{99.0f, 330.0f}, 0},
	{ANGRYBIRD_PIG,			{118.0f, 450.0f}, 0},
	{ANGRYBIRD_PIG,			{140.0f, 560.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{142.0f, 585.0f}, 0},

//12-4

	{ANGRYBIRD_LANTERN1,		{104.0f, 405.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{121.0f, 444.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{126.0f, 484.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{117.0f, 522.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{103.0f, 564.0f}, 0},

	{ANGRYBIRD_BARRIER_MAX,		{133.0f, 558.0f}, 0.5236},
	{ANGRYBIRD_BARRIER_MAX,		{155.0f, 480.0f}, 0},
	{ANGRYBIRD_BARRIER_MID,		{146.0f, 419.0f}, 2.61799},
	
	{ANGRYBIRD_PIG,			{30.0f, 445.0f}, 0},
	{ANGRYBIRD_PIG_BIG,		{40.0f, 485.0f}, 0},
	{ANGRYBIRD_PIG,			{30.0f, 525.0f}, 0},
	{ANGRYBIRD_CAP,			{56.0f, 486.0f}, 0},

//7-5

	{ANGRYBIRD_BIGWOOD,		{40.0f, 500.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{100.0f, 505.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{146.0f, 490.0f}, 0},

	{ANGRYBIRD_BARRIER_MID,		{136.0f, 430.0f}, 0},
	
	{ANGRYBIRD_PIG_BIG,		{38.0f, 550.0f}, 0},
	{ANGRYBIRD_PIG,			{131.0f, 610.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{163.0f, 480.0f}, 0},

//13-6
	//{ANGRYBIRD_STONE_TRI,		{177.0f, 590.0f}, __MM_PIO2__},
	{ANGRYBIRD_STONE_TRI,		{45.0f, 528.0f}, 4.450589},
	{ANGRYBIRD_STONE_TRI,		 {46.0f, 462.0f}, 3.665194},
	{ANGRYBIRD_STONE_TRI,		{25.0f, 414.0f}, 2.0943951024},
	{ANGRYBIRD_STONE_TIN,		{61.0f, 550.0f}, 0},
	{ANGRYBIRD_STONE_TIN,		{25.0f, 442.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{125.0f, 400.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{215.0f, 460.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{185.0f, 525.0f}, 0},
	{ANGRYBIRD_BARRIER_MID,		{145.0f, 400.0f}, 0},
	{ANGRYBIRD_BARRIER_MID,		{235.0f, 460.0f}, 0},
	{ANGRYBIRD_BARRIER_MID,		{205.0f, 525.0f}, 0},
	{ANGRYBIRD_MONKEY,		{177.0f, 589.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{32.0f, 495.0f}, 0},

//14-7


	{ANGRYBIRD_WOOD_LIT,		{73.0f, 410.0f}, 0},
	{ANGRYBIRD_WOOD_MID,		{96.0f, 405.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{121.0f, 405.0f}, 0},
	{ANGRYBIRD_WOOD_LIT,		{134.0f, 405.0f}, __MM_PIO2__},

	{ANGRYBIRD_STONE_TIN,		{25.0f, 475.0f}, 0},
	{ANGRYBIRD_STONE_TIN,		{25.0f, 495.0f}, 0},
	{ANGRYBIRD_STONE_MID,		{35.0f, 485.0f}, 0},
	{ANGRYBIRD_STONE_LIT,		{28.0f, 545.0f}, __MM_PIO2__},
	{ANGRYBIRD_STONE_LIT,		{28.0f, 565.0f}, __MM_PIO2__},
	{ANGRYBIRD_STONE_MID,		{43.0f, 555.0f}, 0},

	{ANGRYBIRD_PIG,			{50.0f, 485.0f}, 0},
	{ANGRYBIRD_PIG_BIG,		{66.0f, 555.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{156.0f, 405.0f}, 0},
	{ANGRYBIRD_CAP,			{171.0f, 410.0f}, 0.4},

//14-8


	{ANGRYBIRD_WOOD_MID,		{40.0f, 320.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{60.0f, 410.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{60.0f, 490.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MID,		{40.0f, 580.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{52.0f, 299.0f}, 2.3561944902},
	{ANGRYBIRD_WOOD_MAX,		{95.0f, 366.0f}, 2.8797932658},
	{ANGRYBIRD_LANTERN,		{65.0f, 365.0f}, 0},
	{ANGRYBIRD_WOOD_MAX,		{105.0f, 450.0f}, 0},
	{ANGRYBIRD_LANTERN,		{80.0f, 450.0f}, 0},
	{ANGRYBIRD_WOOD_MAX,		{95.0f, 534.0f}, 0.2617993878},
	{ANGRYBIRD_WOOD_MAX,		{52.0f, 601.0f}, 0.7853981634},
	{ANGRYBIRD_LANTERN,		{65.0f, 535.0f}, 0},

	{ANGRYBIRD_PIG_BIG,		{120.0f, 380.0f}, 0},
	{ANGRYBIRD_PIG_BIG,		{120.0f, 520.0f}, 0},
//9-9


	{ANGRYBIRD_WOOD_MAX,		{98.0f, 408.0f}, 0},
	{ANGRYBIRD_WOOD_MAX,		{89.0f, 449.0f}, 2.356194},
	{ANGRYBIRD_WOOD_MAX,		{61.0f, 484.0f}, 0.785398},
	{ANGRYBIRD_WOOD_MAX,		{97.0f, 515.0f}, 2.356194},
	{ANGRYBIRD_LANTERN,		{100.0f, 545.0f}, 0},
	{ANGRYBIRD_WOOD_MAX,		{61.0f, 544.0f}, 0.785398},

	{ANGRYBIRD_PIG_BIG,		{55.0f, 450.0f}, 0},
	{ANGRYBIRD_PIG,			{55.0f, 517.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{79.0f, 400.0f}, 0},
//11-10

	{ANGRYBIRD_BARRIER_MID,		{52.0f, 444.0f}, 0.5236},
	{ANGRYBIRD_BARRIER_MID,		{52.0f, 515.0f}, 2.61799},
	{ANGRYBIRD_BARRIER_MID,		{86.0f, 425.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MID,		{126.0f, 425.0f}, __MM_PIO2__},
	{ANGRYBIRD_BARRIER_MAX,		{106.0f, 535.0f}, __MM_PIO2__},
	{ANGRYBIRD_BARRIER_MID,		{165.0f, 438.0f}, 2.61799},
	{ANGRYBIRD_BARRIER_MID,		{165.0f, 522.0f}, 0.5236},
	{ANGRYBIRD_BARRIER_MID,		{175.0f, 480.0f}, 0},
	{ANGRYBIRD_BARRIER_LIT,		{75.0f, 480.0f}, 0},

	{ANGRYBIRD_PIG_BIG,		{100.0f, 480.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{150.0f, 480.0f}, 0},

//10-11

	{ANGRYBIRD_BARRIER_MID,		{230.0f, 540.0f}, 0.78539},

	{ANGRYBIRD_ICE_MAX,		{60.0f, 475.0f}, __MM_PIO2__},
	{ANGRYBIRD_ICE_MAX,		{60.0f, 545.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{105.0f, 510.0f}, 0},

	{ANGRYBIRD_PIG_BIG,		{40.0f, 510.0f}, 0},
	{ANGRYBIRD_PIG,			{120.0f, 480.0f}, 0},
	{ANGRYBIRD_PIG,			{120.0f, 540.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{33.0f, 425.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{33.0f, 455.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{56.0f, 440.0f}, 0},



//13-12

	{ANGRYBIRD_BARRIER_MID,		{158.0f, 419.0f}, 2.3561945},
	{ANGRYBIRD_BARRIER_MID,		{158.0f, 531.0f}, 0.7853985},
	{ANGRYBIRD_ICE_MAX,		{178.0f, 475.0f}, 0},
	{ANGRYBIRD_BIGSTONE,		{193.0f, 475.0f}, 0},
	{ANGRYBIRD_BIGSTONE,		{213.0f, 475.0f}, 0},

	{ANGRYBIRD_ICE_BOX,		{30.0f, 475.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{50.0f, 475.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{70.0f, 475.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{90.0f, 475.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{110.0f, 475.0f}, 0},

	{ANGRYBIRD_PIG_BIG,		{40.0f, 435.0f}, 0},
	{ANGRYBIRD_PIG_BIG,		{40.0f, 515.0f}, 0},
	{ANGRYBIRD_PIG,			{130.0f, 475.0f}, 0},
//11-13

	{ANGRYBIRD_STONE_MAX,		{80.0f, 440.0f}, 0},
	{ANGRYBIRD_LANTERN,		{60.0f, 410.0f}, 0},
	{ANGRYBIRD_STONE_MAX,		{80.0f, 520.0f}, 0},
	{ANGRYBIRD_LANTERN,		{60.0f, 550.0f}, 0},
	{ANGRYBIRD_BARRIER_MAX,		{115.0f, 480.0f}, 0},
	{ANGRYBIRD_STONE_MAX,		{145.0f, 480.0f}, 0},
	{ANGRYBIRD_BIGSTONE,		{168.0f, 480.0f}, __MM_PIO2__},


	{ANGRYBIRD_PIG_BIG,		{57.0f, 445.0f}, 0},
	{ANGRYBIRD_PIG_BIG,		{57.0f, 515.0f}, 0},
	{ANGRYBIRD_PIG,			{130.0f, 460.0f}, 0},
	{ANGRYBIRD_PIG,			{130.0f, 500.0f}, 0},

//13-14

	{ANGRYBIRD_WOOD_MAX,		{89.0f, 390.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{89.0f, 442.0f}, __MM_PIO2__},
	{ANGRYBIRD_BARRIER_MID,		{80.0f, 415.0f}, 0},
	{ANGRYBIRD_BARRIER_MID,		{103.0f, 415.0f}, 0},
	{ANGRYBIRD_WOOD_MID,		{139.0f, 415.0f}, 0},

	{ANGRYBIRD_WOOD_MID,		{40.0f, 522.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MID,		{40.0f, 568.0f}, __MM_PIO2__},
	{ANGRYBIRD_STONE_TRI,		{60.0f, 545.0f}, 2.3561945},

	{ANGRYBIRD_BARRIER_TIN,		{70.0f, 500.0f}, 0},
	{ANGRYBIRD_BARRIER_TIN,		{70.0f, 590.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{50.0f, 500.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{50.0f, 590.0f}, 0},

	{ANGRYBIRD_PIG,			{30.0f, 545.0f}, 0},

//11-15

	{ANGRYBIRD_PIG,			{120.0f, 403.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{142.0f, 435.0f}, 0},
	{ANGRYBIRD_PIG,			{160.0f, 463.0f}, 0},
	{ANGRYBIRD_PIG_BIG,		{177.0f, 515.0f}, 0},
	{ANGRYBIRD_CAP,			{195.0f, 515.0f}, 0},

	{ANGRYBIRD_ICE_MAX,		{154.0f, 515.0f}, 0},
	{ANGRYBIRD_LANTERN,		{135.0f, 515.0f}, 0},

	{ANGRYBIRD_PIG_BIG,		{76.0f, 515.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{33.0f, 515.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{51.0f, 494.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{50.0f, 536.0f}, 0},

//10-16

	{ANGRYBIRD_WOOD_MAX,		{60.0f, 445.0f}, __MM_PIO2__},
	{ANGRYBIRD_ICE_MID,		{40.0f, 465.0f}, __MM_PIO2__},		
	{ANGRYBIRD_ICE_MID,		{40.0f, 495.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{60.0f, 515.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MID,		{124.0f, 460.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MID,		{124.0f, 500.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MID,		{64.0f, 480.0f}, 0},
	{ANGRYBIRD_WOOD_MAX,		{104.0f, 480.0f}, 0},
	{ANGRYBIRD_WOOD_MID,		{149.0f, 480.0f}, 0},
	{ANGRYBIRD_PIG,			{119.0f, 480.0f}, 0},


//14-17

	{ANGRYBIRD_WOOD_MAX,		{60.0f, 395.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{60.0f, 475.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{60.0f, 555.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{99.0f, 435.0f}, 0},
	{ANGRYBIRD_WOOD_MAX,		{99.0f, 515.0f}, 0},

	{ANGRYBIRD_BIGWOOD,		{120.0f, 400.0f}, __MM_PIO2__},
	{ANGRYBIRD_BIGWOOD,		{120.0f, 450.0f}, __MM_PIO2__},
	{ANGRYBIRD_BIGWOOD,		{120.0f, 500.0f}, __MM_PIO2__},
	{ANGRYBIRD_BIGWOOD,		{120.0f, 550.0f}, __MM_PIO2__},

	{ANGRYBIRD_MOONCAKE,		{117.0f, 425.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{117.0f, 475.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{117.0f, 525.0f}, 0},
	
	{ANGRYBIRD_PIG_BIG,		{38.0f, 435.0f}, 0},
	{ANGRYBIRD_PIG_BIG,		{38.0f, 515.0f}, 0},


//20-18


	{ANGRYBIRD_WOOD_LIT,		{28.0f, 375.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_LIT,		{28.0f, 405.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MID,		{43.0f, 390.0f}, 0},
	{ANGRYBIRD_WOOD_LIT,		{56.0f, 375.0f}, __MM_PIO2__},

	{ANGRYBIRD_WOOD_MID,		{40.0f, 445.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MID,		{40.0f, 505.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{65.0f, 475.0f}, 0},


	{ANGRYBIRD_WOOD_LIT,		{28.0f, 545.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_LIT,		{28.0f, 575.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MID,		{43.0f, 560.0f}, 0},
	{ANGRYBIRD_WOOD_LIT,		{56.0f, 575.0f}, __MM_PIO2__},

	{ANGRYBIRD_MOONCAKE,		{82.0f, 460.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{82.0f, 490.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{105.0f, 475.0f}, 0},

	{ANGRYBIRD_BARRIER_TIN,		{175.0f, 395.0f}, 0},
	{ANGRYBIRD_BARRIER_TIN,		{175.0f, 555.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{155.0f, 395.0f}, 0},
	{ANGRYBIRD_LANTERN1,		{155.0f, 555.0f}, 0},

	{ANGRYBIRD_PIG_BIG,		{68.0f, 400.0f}, 0},
	{ANGRYBIRD_PIG,			{58.0f, 555.0f}, 0},
//9-19

	{ANGRYBIRD_WOOD_MAX,		{130.0f, 386.0f}, 0.52359888},
	{ANGRYBIRD_LANTERN,		{120.0f, 355.0f}, 0},
	{ANGRYBIRD_LANTERN,		{90.0f, 355.0f}, 0},
	{ANGRYBIRD_LANTERN,		{60.0f, 355.0f}, 0},
	{ANGRYBIRD_WOOD_MID,		{115.0f, 533.0f}, 2.6179939},
	{ANGRYBIRD_LANTERN,		{100.0f, 550.0f}, 0},
	{ANGRYBIRD_LANTERN,		{70.0f, 550.0f}, 0},
	{ANGRYBIRD_PIG_BIG,		{110.0f, 440.0f}, 0},
	{ANGRYBIRD_PIG,			{100.0f, 505.0f}, 0},
//12-20

	{ANGRYBIRD_ICE_MAX,		{100.0f, 388.0f}, 2.3562},
	{ANGRYBIRD_WOOD_MID,		{90.0f, 414.0f}, 1.0472},
	{ANGRYBIRD_STONE_MAX,		{110.0f, 435.0f}, __MM_PIO2__},
	{ANGRYBIRD_STONE_MAX,		{110.0f, 505.0f}, __MM_PIO2__},
	{ANGRYBIRD_WOOD_MAX,		{164.0f, 461.0f}, 2.879793266},
	{ANGRYBIRD_LANTERN,		{135.0f, 470.0f}, 0},
	{ANGRYBIRD_WOOD_MAX,		{152.0f, 529.0f}, 0.261799388},

	{ANGRYBIRD_BARRIER_LIT,		{130.0f, 570.0f}, 0},

	{ANGRYBIRD_LANTERN1,		{115.0f, 570.0f}, 0},

	{ANGRYBIRD_MONKEY,		{40.0f, 575.0f}, 0},
	{ANGRYBIRD_PIG,			{60.0f, 390.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{91.0f, 470.0f}, 0},


//9-21

	{ANGRYBIRD_STONE_MAX,		{114.0f, 405.0f}, 0},
	{ANGRYBIRD_STONE_MID,		{125.0f, 440.0f}, 0},
	{ANGRYBIRD_BIGSTONE,		{127.0f, 530.0f}, __MM_PIO2__},
	{ANGRYBIRD_ICE_BOX,		{140.0f, 450.0f}, 0},
	{ANGRYBIRD_STONE_MAX,		{155.0f, 490.0f}, 0},
	
	{ANGRYBIRD_BARRIER_MID,		{105.0f, 500.0f}, 0},

	{ANGRYBIRD_PIG_BIG,		{129.0f, 495.0f}, 0},
	{ANGRYBIRD_MOONCAKE,		{82.0f, 455.0f}, 0},
	{ANGRYBIRD_PIG,			{81.0f, 505.0f}, 0},
	
	
};

const mmU32 mmSABNumberOfGroundPoint[] = {
	8, 17, 12, 8,
4, 6, 8, 2,
12, 6, 6, 10,
6, 8, 16, 2,
2, 2, 6, 14,
12,

	
};

const mmPoint mmSABGroundPoints[] = {
	
{20, 0}, {20, 360}, {30, 380}, {40, 380}, {40, 560}, {30, 560}, {20, 580}, {20, 680},
{20, 0}, {20, 330}, {90, 400}, {70,420}, {70, 430}, {60, 430}, {50, 440}, {50, 460}, {20, 460}, {20, 510}, {50, 510}, {50, 530}, {60, 540}, {70, 540}, {70, 550}, {90, 570}, {20, 680},
{20, 0}, {20, 220}, {37, 320}, {42, 320}, {42, 340}, {57, 440}, {62, 440}, {62, 460}, {77, 560}, {82, 560}, {82, 580}, {90, 680},
{20, 0}, {20, 580}, {30, 590}, {110, 590}, {110, 600}, {30, 600}, {20, 610}, {20, 680}, 

{20, 0}, {20, 580}, {120, 580}, {120, 680}, 
{20, 0}, {20, 550}, {160, 570}, {160, 610}, {240, 610}, {240, 680},
{20, 0}, {20, 380}, {40, 400}, {70,400}, {70, 420}, {40, 420}, {20, 440}, {20, 680},
{20, 0}, {20, 680},

{20, 0}, {20, 280}, {100, 360}, {60,400}, {70, 410}, {30, 450}, {60, 480}, {30, 510}, {60, 540}, {30, 570}, {60, 600}, {120, 680},
{20, 0}, {20, 380}, {40, 410}, {40, 550}, {20, 580}, {20, 680},
{20, 0}, {20, 390}, {190, 390}, {190, 410}, {20, 410}, {20, 680},
{20, 0}, {20, 390}, {160, 390}, {140, 410}, {20, 410}, {20, 540}, {140, 540}, {160, 560}, {20,560}, {20,680},

{20, 0}, {20, 380}, {40, 400}, {40, 560}, {20, 580}, {20, 680},
{20, 0}, {20, 370}, {40, 370}, {50, 380}, {50, 450}, {40, 460}, {20, 460}, {20, 680},
{20, 0}, {20, 360}, {90, 360}, {90, 390}, {110, 390}, {110, 420}, {130, 420}, {130, 450}, {150, 450}, {150, 480}, {20, 480}, {20, 550}, {150, 550}, {150, 610}, {20, 610}, {20, 680},
{20, 0}, {20, 680},
 
{20, 0}, {20, 680}, 
{20, 0}, {20, 680},
{20, 0}, {20, 380}, {90, 420}, {90, 520}, {20, 560}, {20, 680},
{20, 0}, {20, 320}, {80, 350}, {40, 390}, {70, 420}, {70, 440}, {80, 450}, {80, 490}, {70, 500}, {70, 510}, {60, 510}, {20, 550}, {20, 600}, {60, 680},

{20, 0}, {20, 380}, {70, 420}, {110, 420}, {110, 440}, {70,	440}, {70, 520}, {110, 520}, {110,540}, {70, 540}, {20, 590},  {20, 680},


};

mmU32 mmSABGameState, mmSBALastState, mmSBALastSuspendState = 0;
mmU32 mmSABGameStateTimer;
mmU32 mmSABGameStageId;
const mmPoint BIRD_STARTPOSITION = {84, 103};
const static mmRectangle hotarea = {50, 30, 80, 110};
static mmS32 pen_point_y;

mmPoint birdPosition;
mmBool mmSBAIsWatching;
mmBool mmSABIsInitial;
static mmBool mmSABDrawOption = mmFalse;
static mmBool isInitPhysicsNow;
mmBool isValidReady = mmFalse;// isReadyToShoot;
mmBool isReadyToShoot = mmFalse;
extern void
mm_sba_draw_image(int x, int y, int id, mmBool transparentSign);


mmS32 cameraOffsetY;
static mmS32 cameraMoveSign;
static const mmS32 CamerMoveSpeed = 32;
mmPoint *mmSABFlyPathPoints;
static mmU32 mmSBANumberOfPathPoint;

mmU32 mmSBAFullScoreOfStage;
mmU32 mmSBAScoreOfStage;

mmU32 mmGroundPointStart;
mmS32 mmSABIsEffectPoint;
mmPoint mmSABEffectPoint;
extern mmU8 mmSABDrawOnce;

static mmU32
getStageGroundPoint()
{
	int i = 0;
	mmGroundPointStart = 0;
	for(; i < mmSABGameStageId; i++)
	{
		mmGroundPointStart += mmSABNumberOfGroundPoint[i];
	}
	return 1;
}

void mmSABUIMessage(mmU16 x, mmU16 y){
	int i;
	mmBool isContinue = ((x > mmSBAUIHotAreas[0].x && x < mmSBAUIHotAreas[0].x + mmSBAUIHotAreas[0].width))&&
	((y > mmSBAUIHotAreas[0].y && y < mmSBAUIHotAreas[0].y + mmSBAUIHotAreas[0].height));
	
	if(!isContinue){
		mmSABGameState = mmSBALastState;
	}else{
		i = 1;
		for(; i <= 5; i++){
			isContinue = ((x > mmSBAUIHotAreas[i].x && x < mmSBAUIHotAreas[i].x + mmSBAUIHotAreas[i].width))&&
				((y > mmSBAUIHotAreas[i].y && y < mmSBAUIHotAreas[i].y + mmSBAUIHotAreas[i].height));
			
			if(isContinue){
				break;
			}
		}
		switch(i){
			case 1:
				mmSABDrawOption = mmTrue;
				if(mmSABSound == 1)
				{
				////	marmota->audio->stopAllAudio();
					mmSABSound = 0;
				}
				else
					mmSABSound = 1;	
				break;
			case 2:
				mmSABMainMenu=0;
//				mmEntryState(MM_SUPERANGRYBIRD_MAINMENU);
				break;
			case 3:
				mmSABMainMenu=1;
				mmSABDrawOnce=0;
//				mmEntryState(MM_SUPERANGRYBIRD_MAINMENU);
				break;
			case 4:
//				mmEntryState(MM_SUPERANGRYBIRD_KERNEL);
				break;
			case 5:
				mmSABGameState = mmSBALastState;
				break;
		}
	}
}
/*char mmSABLogBuf[256];
void mmWriteLogFile(mmU32 data_length){
	marmota->record->writeRecord("E:\\angrybird.log", mmSABLogBuf, strlen(mmSABLogBuf));
}*/

/*********************************************************************
 * FUNCTION
 *	mmSABRefreshGround
 * DESCRIPTION
 *	refresh ground.  
 * PARAMETERS
 *
 * RETURNS
*********************************************************************/
static void  mmSABRefreshGround()
{
	mmU32 startPoint = mmGroundPointStart, i;
	mmS32 x1, x0, y1, y0, x;
	mmS32 tanValue = 0;
	mmU32 offsetValue = 0;
	mmS32 sign = 1;
	
	for(; startPoint < mmSABNumberOfGroundPoint[mmSABGameStageId] + mmGroundPointStart - 1; startPoint++)
	{
		if(mmSABGroundPoints[startPoint + 1].y > cameraOffsetY)
			break;
	}
	
	for( i = cameraOffsetY; i < cameraOffsetY + UI_device_height; i++)
	{
		if(i == cameraOffsetY || i == mmSABGroundPoints[startPoint].y)
		{
		
			x1 = mmSABGroundPoints[startPoint + 1].x;
			y1 = mmSABGroundPoints[startPoint + 1].y;
			x0 = mmSABGroundPoints[startPoint].x;
			y0 = mmSABGroundPoints[startPoint].y;

			startPoint++;
			
			if(y1 == y0){
				i--;
				continue;
			}
			
			sign = x1 > x0 ? 1 : -1;
			
			tanValue = (x1 - x0) * sign;
			tanValue <<= 16;
			tanValue /= (y1 - y0);
			//tanValue = ((x1 - x0) << 16) / (y1 - y0);
			//KscDbgPrint("-- %d", tanValue);
		}
		
		x = (((i - y0) * tanValue) >> 16)*sign + x0;
		
		if(x > 20){
			//mmU16 *dst_buf = (mmPU16)((mmSABBaseCanva)->buf_ptr);
			//mmU16 *src_buf = (mmPU16)((mmSABUIResCanva)->buf_ptr);
		/*	mmU32 src_mem_offset = 163 +((offsetValue++) % 3);
			memcpy(dst_buf + (i - cameraOffsetY) * UI_device_width + 18, src_buf + (src_mem_offset) * 240, (x - 20)*2);*/
			mmU32 src_mem_offset = 162 +((offsetValue++) % 3);
			//411为图片的宽度
			//memcpy(dst_buf + (i - cameraOffsetY) * UI_device_width + 18, src_buf + (src_mem_offset) * mmSABUIResCanva->w, (x - 20)*2);
			//pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_UI_RES_CANVA], 18, i-cameraOffsetY, 0, src_mem_offset, x-20, 1);
//			draw_ground_buf_to_screen(18, i-cameraOffsetY, x-20, ((offsetValue++) % 3));
		}
	}
}
static void mmSbaRefreshBackground(){
	
	int i = 0,j = 0;
	int id = IMG_ANGRYBIRD_GROUND;
	
	pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_MOON_CANVA], 142, 0, 0, 0, 178, 480);
	
	cameraOffsetY = cameraOffsetY > 0 ? cameraOffsetY : 0;
	
	if(cameraOffsetY > SAB_CAMERA_OFFSET_MAX)
		cameraOffsetY = SAB_CAMERA_OFFSET_MAX;
	
	pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_STOREY_CANVA], 20, 0, 0, cameraOffsetY / 4, 122, 560);
	
	i = cameraOffsetY % 101;
	
	pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_SCENE_CANVA], 0, 0, 
		mm_sab_raw_image[id].x, mm_sab_raw_image[id].y+i, mm_sab_raw_image[id].width, mm_sab_raw_image[id].height-i);
	
	for(j = 0; j<= UI_device_height/101 + 1; j++)
	{
		pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_SCENE_CANVA], 0, mm_sab_ui_image[id].height-i + 101*j, 
			mm_sab_raw_image[id].x, mm_sab_raw_image[id].y, mm_sab_raw_image[id].width, mm_sab_raw_image[id].height);
	}
	
	mmSABRefreshGround();
	
}

static void initPhysicsEngine()
{
	mm_sab_setangrybird_v(birdPosition.x, birdPosition.y);
	birdPosition.x = BIRD_STARTPOSITION.x;
	birdPosition.y = BIRD_STARTPOSITION.y;
}

static mmBool mmSABCameraMoveLeft = mmFalse;
static mmBool mmSABCameraMoveRight = mmFalse;

static /*inline*/ mmBool mmSABCheckIsGetBorder(){
	if(cameraMoveSign )
	{
		cameraOffsetY += cameraMoveSign * CamerMoveSpeed;
		if(mmSABCameraMoveLeft == mmFalse && mmSABCameraMoveRight == mmFalse)
			cameraMoveSign = 0;

	}
	
	if(cameraOffsetY < 0)
	{
		cameraOffsetY = 0;
		mmSABCameraMoveLeft = mmFalse;
		mmSABCameraMoveRight = mmFalse;
		return mmTrue;
	}
	else if(cameraOffsetY > MMSAB_SCREEN_HEIGHT - UI_device_height)
	{
		cameraOffsetY = MMSAB_SCREEN_HEIGHT - UI_device_height;
		mmSABCameraMoveLeft = mmFalse;
		mmSABCameraMoveRight = mmFalse;
	}
	
	return mmFalse;
}

void mmSBAGamestateDispatchCenter(){
	switch(mmSABGameState){
		case mmSABShowScene:
			if(++mmSABGameStateTimer == 20){
				mmSABGameStateTimer = 0;
				mmSABGameState = mmSABGotoHotare;
			}
		break;
		
		case mmSABShowBirdDel:
			if(++mmSABGameStateTimer == 10){
				mmSABGameStateTimer = 0;
				mmSABGameState = mmSABGotoHotare;
			}
		break;
		case mmSABGotoHotare:
			if(mmSABCheckIsGetBorder() == mmTrue)
			{
				if(mmSABNumberOfPigs <= 0)
				{
					mmSABGameState = mmSABAuditScore;
					mmSABIsInitial = mmTrue;
				}
				else
				{
					mmSABGameState = mmSABUserOperation;
				}
			}
			else
			{
				cameraMoveSign = -1;
			}
			
		break;
		
		case mmSABUserOperation:
			if(mmTrue == isInitPhysicsNow)
			{
				initPhysicsEngine();
				if(1 == mmSABSound){
					/*if(mmSABLastFlying->birdtype == ANGRYBIRD_BIRD_RED){
						marmota->audio->stopAllAudio();
						marmota->audio->playRawData((mmAudioData*)sab_audio_bird01fly_resources, 0);
					}
					if(mmSABLastFlying->birdtype == ANGRYBIRD_BIRD_BLUE){
						marmota->audio->stopAllAudio();
						marmota->audio->playRawData((mmAudioData*)sab_audio_bird02fly_resources, 0);
					}
					if(mmSABLastFlying->birdtype == ANGRYBIRD_BIRD_YELLOW){
						marmota->audio->stopAllAudio();
						marmota->audio->playRawData((mmAudioData*)sab_audio_bird03fly_resources, 0);
					}
					if(mmSABLastFlying->birdtype == ANGRYBIRD_BIRD_BLACK){
						marmota->audio->stopAllAudio();
						marmota->audio->playRawData((mmAudioData*)sab_audio_bird04fly_resources, 0);
					}*/
				}
				isInitPhysicsNow = mmFalse;
				mmSABGameState = mmSABPK;
				mmSBAIsWatching = mmTrue;
				mmSABCollision = mmFalse;
				mmSABBlueCollision[0] = mmFalse;
				mmSABBlueCollision[1] = mmFalse;
			}
			
			mmSABCheckIsGetBorder();
			
			//if(mmSABNumberOfBirds == 0 || mmSABNumberOfPigs <= 0){
					if(mmSABNumberOfBirds == 0)
					{
						mmSABGameState = mmSABShowResult;
						mmSABIsInitial = mmTrue;
					}
				/*	else
					{
						//mmSABGameState = mmSABGotoHotare;
						mmSABGameState = mmSABShowScene;
					}
				*/
			//}
		if(mmSABNumberOfPigs <= 0){
			//	mmSABGameState = mmSABAuditScore;
			mmSABGameState = mmSABGotoHotare;
					mmSABIsInitial = mmTrue;
		}
		break;
		
		case mmSABPK:
			if(mmSBAIsWatching){
				if(mmSABLastFlying && mmSABLastFlying->body)
				{	
					if(mmSABLastFlying->body->p.y < 0){
						cameraOffsetY = -1;
					}
					else
					{
						mmS32 i = mmSABLastFlying->body->p.y - BIRD_STARTPOSITION.y;
						cameraOffsetY = i < 0 ? 0 : i;
					}
				}
			}
			
			else if(mmTrue == mmSABCheckIsGetBorder())
				{
				mmSABGameState = mmSABUserOperation;
				mmSBAIsWatching = mmFalse;
			}
		break;
		
		case mmSABAuditScore:
			if(mmSABIsInitial){
				mmSABBirdCalScore();
				mmSABIsInitial = mmTrue;
			}
			
			if(mmSABScoreListHead == null)
			{
				if(++mmSABGameStateTimer >= 10){
					mmSABGameStateTimer = 0;
					mmSABGameState = mmSABShowResult;
					
					if(mmSABNumberOfPigs <= 0 && 1 == mmSABSound){
						//marmota->audio->stopAllAudio();
						//marmota->audio->playRawData((mmAudioData*)sab_complete_logo_resources, 0);//
					}
					mmSABIsInitial = mmTrue;
				}
			}
		break;
	}
}


static void mmSABGetFullPath()
{
	/*KscMemset(mmSABResFullPath, 0, sizeof(mmSABResFullPath));

	KscStrcat(mmSABResFullPath, (char*)KscFileGetRevableDisk());
	
	#ifdef MAMOTA_BACKEND_MSTAR
	KscStrcat(mmSABResFullPath, "/funboxctrl/FB_Extr1.res");
	#else
	KscStrcat(mmSABResFullPath, "\\funboxctrl\\FB_Extr1.res");
	#endif*/
 
	
//	KscDbgPrint("%s", mmSABResFullPath);
}


void mm_superangrybird_kernel_initial(){
	mmSABNumberOfPigs = 0;
	mmSABGameState = mmSABShowScene;
	cameraOffsetY = MMSAB_SCREEN_HEIGHT - UI_device_height;
	mmSABGameStateTimer = 0;
	
	birdPosition.x = BIRD_STARTPOSITION.x;
	birdPosition.y = BIRD_STARTPOSITION.y;
	isInitPhysicsNow = mmFalse; isValidReady = mmFalse; isReadyToShoot = mmFalse;
	

	getStageGroundPoint();
	
	mmSBAFullScoreOfStage = 0;
	//MM_DBG_PRINT("\r\n mmSABGameStageId = %d",mmSABGameStageId);
	
	mmSABBirdListHead = NULL;
	
	mm_sab_initspace(mmSABGameStageId);
	
	mmSBANumberOfPathPoint = 0;
	mmSABBlueNumOfPathPoint[0] = 0;
	mmSABBlueNumOfPathPoint[1] = 0;
	
	mmSABFlyPathPoints = null;
	mmSABBluePathPoint[0] = null;
	mmSABBluePathPoint[1] = null;

	mmSBAScoreOfStage = 0;
	
	mmSABLastFlying = null;
		
	mmSABReadyShootSound = mmFalse;
	
	mmSABDrawOption = mmFalse;
	
	//MM_DBG_PRINT("\r\n mmSABLevelScore[0]111 = %d",mmSABLevelScore[0]);
	
	if(mmSABLevelScore[0] > MMSAB_LEVEL_SUM - 1)
		mmSABLevelScore[0] = MMSAB_LEVEL_SUM-1;
		
	//MM_DBG_PRINT("\r\n mmSABLevelScore[0] = %d",mmSABLevelScore[0]);
	#ifndef KKFUN_RES_OFFSET
		mmSABGetFullPath();
	#endif
	
	mmSABIsEffectPoint = 0;
	mmSABEffectPoint.x = 0;
	mmSABEffectPoint.y = 0;
	
	mmSABCameraMoveLeft = mmFalse;
	mmSABCameraMoveRight = mmFalse;
	
	if(1 == mmSABSound)
	{
		/*marmota->audio->stopAllAudio();
		marmota->audio->playRawData((mmAudioData*)sab_outro_resources, 0);*/
	}
	
}

void mm_superangrybird_kernel_destory(){
	if( mmSABFlyPathPoints )
	{	
		mmFree(mmSABFlyPathPoints);
		mmSABFlyPathPoints = null;
	}
	
	if( mmSABBluePathPoint[0] )
	{
		mmFree(mmSABBluePathPoint[0]);
		mmSABBluePathPoint[0] = null;
	}
	
	if( mmSABBluePathPoint[1] )
	{
		mmFree(mmSABBluePathPoint[1]);
		mmSABBluePathPoint[1] = null;
	}
	
	mm_sab_destoryspace();
	
	///marmota->audio->stopAllAudio();
	
	if(mmSABLevelScore[0] > MMSAB_LEVEL_SUM - 1)
		mmSABLevelScore[0] = MMSAB_LEVEL_SUM-1;
	mmSABWriteGameRestore();
	
}

static void 
mmSABUpdateFlyingPath(){
	int i,j;
	if(mmSABLastFlying && mmSABLastFlying->body && (mmS32)mmSABLastFlying->body->p.x > 35){
		if(mmSBANumberOfPathPoint < MMSAB_MAX_PATHPOINT - 1 && mmFalse == mmSABCollision){
			(*(mmSABFlyPathPoints + mmSBANumberOfPathPoint)).x = (mmS32)mmSABLastFlying->body->p.x;
			(*(mmSABFlyPathPoints + mmSBANumberOfPathPoint)).y = (mmS32)mmSABLastFlying->body->p.y;
			mmSBANumberOfPathPoint++;
		}
	}
	
	if(mmSBANumberOfPathPoint >= 0 && mmSBANumberOfPathPoint < MMSAB_MAX_PATHPOINT){
		for(i = 0; i < mmSBANumberOfPathPoint; i++){
			mmSABDrawUI((*(mmSABFlyPathPoints + i)).x, (*(mmSABFlyPathPoints + i)).y - cameraOffsetY, IMG_ANGRYBIRD_UI_PATH_POINT);
		}
	}
	
	for(i=0; i<2; i++)
	{
		if(mmSABBlueBirds[i] != null && mmSABBlueBirds[i]->body != null && (mmS32)mmSABBlueBirds[i]->body->p.x > 35)
		{
			if(mmSABBlueNumOfPathPoint[i] < MMSAB_MAX_PATHPOINT - 1/* - mmSBANumberOfPathPoint*/ && mmFalse == mmSABBlueCollision[i]){
				(*(mmSABBluePathPoint[i] + mmSABBlueNumOfPathPoint[i])).x = (mmS32)mmSABBlueBirds[i]->body->p.x;
				(*(mmSABBluePathPoint[i] + mmSABBlueNumOfPathPoint[i])).y = (mmS32)mmSABBlueBirds[i]->body->p.y;
				 mmSABBlueNumOfPathPoint[i]++;
			}
		}
	}
	
	for(i=0; i<2; i++)
	{
		if(mmSABBlueNumOfPathPoint[i] >= 0 && mmSABBlueNumOfPathPoint[i] < MMSAB_MAX_PATHPOINT)
		{
			for(j = 0; j < mmSABBlueNumOfPathPoint[i]; j++)
			{
				mmSABDrawUI((*(mmSABBluePathPoint[i] + j)).x, (*(mmSABBluePathPoint[i] + j)).y - cameraOffsetY, IMG_ANGRYBIRD_UI_PATH_POINT);
			}
		}
	}
}

static mmS32 mmSABGetScoreNum(mmU32 temp)
{	
	mmS32 j = 0;
	if(temp >= 0)
		j = 0;
	if(temp >= 10)
		j = 1;
	if(temp >= 100)	
		j = 2;
	if(temp >= 1000)
		j = 3;
	if(temp >= 10000)
		j = 4;
	return j;
}
static /*inline*/ void
mmSABDrawStageScore(){
	mmU32 x = UI_device_width - mm_sab_ui_image[IMG_ANGRYBIRD_UI_WORD_CURRECORD].width - 10;
	mmU32 y = UI_device_height - mm_sab_ui_image[IMG_ANGRYBIRD_UI_WORD_CURRECORD].height - SAB_CURRENT_SCORE_Y;
	mmU32 temp = mmSBAScoreOfStage;
	mmU32 temp1 = ((mmSABLevelScore[mmSABGameStageId+1] >> 8) & 0xffffff);
	int i=0,j=0,k=0;
	
	mmSABDrawUI(x+5,y+12,IMG_ANGRYBIRD_UI_WORD_MAXRECORD);
	mmSABDrawUI(x-12,y+12,IMG_ANGRYBIRD_UI_WORD_CURRECORD);
	
	k = mmSABGetScoreNum(temp1);
	for(i = 0; i < k+1; i++){
		mmSABDrawUI(x+7, y + SAB_SCORE_POINT- i * SAB_SCORE_GAP, IMG_ANGRYBIRD_UI_SCORENUMBER_0 + (temp1 % 10));
		temp1 /= 10;
	}

	j = mmSABGetScoreNum(temp);
	for(i=0;i<j+1;i++){
		mmSABDrawUI(x - 13, y+ SAB_SCORE_POINT - i * SAB_SCORE_GAP, IMG_ANGRYBIRD_UI_SCORENUMBER_0 + (temp % 10));
		temp /= 10;
	}
}

mmU8 mmSABGetNumberOfStar(mmU32 score)
{	mmU8 num = 0;
	
	score += 10000;
	
	if(score > 0)
		num = 1;
	if(score >= (mmSBAFullScoreOfStage * 1) / 2)
		num = 2;
	if(score >= (mmSBAFullScoreOfStage  * 2) / 3)
		num = 3;
	return num;
}

void mmTrace_Log(char* text)
{
/*	FS_HANDLE handle = -1;
	WCHAR ucs_filename[32] = {0}; 
	
	char* filename = "E:\\sablog.txt";
	
	mmi_chset_text_to_ucs2_str((kal_uint8 *)ucs_filename, 256, (kal_uint8 *)filename , GB2312_ENCODING_TYPE);
	
	
	handle = FS_Open(ucs_filename, FS_CREATE | FS_READ_WRITE );
	FS_Seek( handle, 0, FS_FILE_END );
	FS_Write(handle, (void*)"\r\n----------\r\n", strlen("\r\n----------\r\n"), NULL );
	FS_Write(handle, (void*)text, strlen(text), NULL );
	FS_Write(handle, (void*)"\r\n----------\r\n", strlen("\r\n----------\r\n"), NULL );
	FS_Close(handle);
*/
}

extern void mmSABUpdateScoreList();

void mm_superangrybird_kernel_run(){

	switch(mmSABGameState){
		case mmSABShowScene:
		case mmSABShowBirdDel:
		case mmSABGotoHotare:
		case mmSABUserOperation:
		case mmSABPK:
		case mmSABAuditScore:
		
			mmSbaRefreshBackground();
		
			mm_sba_draw_image(20, 100 - cameraOffsetY, (int)IMG_ANGRYBIRD_BRACKET_END, mmTrue);
			
			/* 树杈上弹簧 */
//			public_draw_line(84, 107 - cameraOffsetY, birdPosition.x-5, birdPosition.y - cameraOffsetY, 0x0);
//			public_draw_line(85, 107 - cameraOffsetY, birdPosition.x-5, birdPosition.y - cameraOffsetY, 0x0);
//			public_draw_line(86, 107 - cameraOffsetY, birdPosition.x-5, birdPosition.y - cameraOffsetY, 0x0);
//			public_draw_line(87, 107 - cameraOffsetY, birdPosition.x-5, birdPosition.y - cameraOffsetY, 0x0);
//			public_draw_line(88, 107 - cameraOffsetY, birdPosition.x-5, birdPosition.y - cameraOffsetY, 0x0);

			if(!mmSABFlyPathPoints)
				mmSABFlyPathPoints =  (mmPoint*)i51AdeOsMalloc(sizeof(mmPoint) * MMSAB_MAX_PATHPOINT, __FILE__, __LINE__);
			
			if(!mmSABBluePathPoint[0])
				mmSABBluePathPoint[0] =  (mmPoint*)i51AdeOsMalloc(sizeof(mmPoint) * MMSAB_MAX_PATHPOINT, __FILE__, __LINE__);
			
			if(!mmSABBluePathPoint[1])
				mmSABBluePathPoint[1] =  (mmPoint*)i51AdeOsMalloc(sizeof(mmPoint) * MMSAB_MAX_PATHPOINT, __FILE__, __LINE__);
			
			mmSABUpdateFlyingPath();//
			
			if(mmTrue == mmSABIsEffectPoint)
				mm_sba_draw_image(mmSABEffectPoint.x-2, mmSABEffectPoint.y - cameraOffsetY-3, (int)IMG_ANGRYBIRD_EFFECT, mmTrue);	
					
			mm_sab_runspace();
			
//			public_draw_line(86, 94 - cameraOffsetY, birdPosition.x-7, birdPosition.y - cameraOffsetY-5, 0x0);
//			public_draw_line(87, 94 - cameraOffsetY, birdPosition.x-6, birdPosition.y - cameraOffsetY-5, 0x0);
//			public_draw_line(88, 94 - cameraOffsetY, birdPosition.x-5, birdPosition.y - cameraOffsetY-5, 0x0);
//			public_draw_line(89, 94 - cameraOffsetY, birdPosition.x-4, birdPosition.y - cameraOffsetY-5, 0x0);
//			public_draw_line(90, 94 - cameraOffsetY, birdPosition.x-3, birdPosition.y - cameraOffsetY-5, 0x0);
			
			mm_sba_draw_image(47, 84 - cameraOffsetY, (int)IMG_ANGRYBIRD_BRACKET, mmTrue);
				
			if(mmSABScoreListHead != null)
			{
				mmSABUpdateScoreList();
			}
			
			mmSABDrawStageScore();
			if(!mmSABIsInitial)
				mmSABDrawUI(SAB_UI_PAUSE_X, SAB_UI_PAUSE_Y, IMG_ANGRYBIRD_UI_PAUSE);
				
			if(mmSABIsBackFromSuspend == mmFalse)
			{
				mmSBAGamestateDispatchCenter();
			}
			else
			{
				if(mmSBALastSuspendState == mmSABShowResult)
				{
					mmSABGameState = mmSABShowResult;
					mmSABIsInitial = mmTrue;
				}
				if(mmSBALastSuspendState == mmSABGameOption)
				{
					mmSABGameState = mmSABGameOption;
					mmSABIsInitial = mmTrue;
				}
				if(mmSABGameState == mmSABUserOperation){
					if(mmSABOnlineBird && mmSABOnlineBird->body
						&& mmSABOnlineBird->state == mmSABBirdState_ONLINE){
						mmSABOnlineBird->body->p.x = birdPosition.x;
						mmSABOnlineBird->body->p.y = birdPosition.y;
						mmSABIsBackFromSuspend = mmFalse;
						mmSABIsInitial = mmFalse;
					}
				}			
			}
			
		break;
		
		case mmSABShowResult:
			if(mmSABIsInitial){
				mmS32 i = 0,j = 0, k = 0; 
				mmU32 temp1 =0,temp2 = 0;
				
				if(mmSABIsBackFromSuspend == mmFalse)
					mmSABSavePreSuspendScore = (mmSABLevelScore[mmSABGameStageId+1] >> 8) & 0xffffff;
				
				mmSABIsInitial = mmFalse;
				pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_RESULT_CANVA], mmSABResultAreas[0].x, mmSABResultAreas[0].y, 0, 0, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);
				mmSABDrawUI(mmSABResultAreas[1].x, mmSABResultAreas[1].y, IMG_ANGRYBIRD_UI_WORDS_STAGE);
				/// max score
				mmSABDrawUI(mmSABResultAreas[2].x, mmSABResultAreas[2].y, IMG_ANGRYBIRD_UI_WORD_MAXRECORD);
				/// current score
				mmSABDrawUI(mmSABResultAreas[3].x, mmSABResultAreas[3].y, IMG_ANGRYBIRD_UI_WORD_CURRECORD);
				
				temp1 = mmSABSavePreSuspendScore;
				temp2 = mmSBAScoreOfStage;
				j = mmSABGetScoreNum(temp1);
				for(i = 0; i < 1 + j; i++)
				{
					mmSABDrawUI(mmSABResultAreas[2].x, mmSABResultAreas[2].y + SAB_SCORE_POINT - i * SAB_SCORE_GAP, IMG_ANGRYBIRD_UI_SCORENUMBER_0 + (temp1 % 10));
					temp1 /= 10;
				}
				k = mmSABGetScoreNum(temp2);
				for(i = 0; i < 1 + k; i++)
				{
					mmSABDrawUI(mmSABResultAreas[3].x, mmSABResultAreas[3].y + SAB_SCORE_POINT - i * SAB_SCORE_GAP, IMG_ANGRYBIRD_UI_SCORENUMBER_0 + (temp2 % 10));
					temp2 /= 10;
				}
			
				
				if(mmSABNumberOfPigs <= 0)
				{
					if(mmSABIsBackFromSuspend == mmFalse){
					
						if(mmSABLevelScore[0] == mmSABGameStageId &&  mmSABGameStageId < MMSAB_LEVEL_SUM-1)
							mmSABLevelScore[0]++;
						
						if(mmSABLevelScore[0] > MMSAB_LEVEL_SUM - 1)
							mmSABLevelScore[0] = MMSAB_LEVEL_SUM-1;

						if(((mmSABLevelScore[mmSABGameStageId+1] >> 8) & 0xffffff) < mmSBAScoreOfStage){		
								mmSABLevelScore[mmSABGameStageId+1] = mmSBAScoreOfStage;
								mmSABLevelScore[mmSABGameStageId+1] = (mmSABLevelScore[mmSABGameStageId+1] << 8) | mmSABGetNumberOfStar(mmSBAScoreOfStage);
							mmSABDrawUI(mmSABResultAreas[5].x, mmSABResultAreas[5].y, IMG_ANGRYBIRD_UI_NEWRECORD);
						}
					}
					else{
						if(mmSABSavePreSuspendScore < mmSBAScoreOfStage)
							mmSABDrawUI(mmSABResultAreas[5].x, mmSABResultAreas[5].y, IMG_ANGRYBIRD_UI_NEWRECORD);
						
					}
					mmSABDrawUI(mmSABResultAreas[6].x, mmSABResultAreas[6].y, IMG_ANGRYBIRD_UI_WORDS_PASS);
						/// stars
					for(i = 0; i < mmSABGetNumberOfStar(mmSBAScoreOfStage); i++)
					{
						#ifdef MM_SUPERANGRYBIRD_RESOURCE_320X480
						mmSABDrawUI(mmSABResultAreas[4].x, mmSABResultAreas[4].y + 39 * i, IMG_ANGRYBIRD_UI_YELLOWSTAR);
						#else
						mmSABDrawUI(mmSABResultAreas[4].x, mmSABResultAreas[4].y + 26 * i, IMG_ANGRYBIRD_UI_YELLOWSTAR);
						#endif
					}
					
					if(mmSABGameStageId < MMSAB_LEVEL_SUM-1)
						mmSABDrawUI(mmSABResultAreas[9].x, mmSABResultAreas[9].y, IMG_ANGRYBIRD_UI_NEXT);
				}
				
				else if(mmSABNumberOfBirds <= 0)
				{
					if(mmSABLevelScore[0] > mmSABGameStageId && mmSABGameStageId < MMSAB_LEVEL_SUM-1)
						mmSABDrawUI(mmSABResultAreas[9].x, mmSABResultAreas[9].y, IMG_ANGRYBIRD_UI_NEXT);
					mmSABDrawUI(mmSABResultAreas[6].x, mmSABResultAreas[6].y, IMG_ANGRYBIRD_UI_WORDS_NOTPASS);
					
					if(1 == mmSABSound)
					{
						/*marmota->audio->stopAllAudio();
						marmota->audio->playRawData((mmAudioData*)sab_pig_laugh, 0);*/
					}
				}
				
				mmSABDrawUI(mmSABResultAreas[7].x, mmSABResultAreas[7].y, IMG_ANGRYBIRD_UI_STAGE_SELECTOR);
				mmSABDrawUI(mmSABResultAreas[8].x, mmSABResultAreas[8].y, IMG_ANGRYBIRD_UI_AGAIN);
				
				if(mmSABIsBackFromSuspend == mmTrue)
					mmSABIsBackFromSuspend = mmFalse;
				
			}
		break; 
		
		case mmSABGameOption:
			/*if(mmSABIsInitial){
				mmSABIsInitial = mmFalse;*/
			if(mmSABDrawOption || mmSABIsInitial){
				pfunc->public_draw_image(&AngryBird.images[ANGRYBIRD_PAUSE_CANVA], 0, 0, 0, 0, SRC_IMAGE_WIDTH, SRC_IMAGE_HEIGHT);
				mmSABDrawOption = mmFalse;
				mmSABIsInitial = mmFalse;
			}
			
			if(mmSABSound == 0)
			{
				#ifdef MM_SUPERANGRYBIRD_RESOURCE_320X480
					mmSABDrawUI(mmSBAUIHotAreas[1].x+2, mmSBAUIHotAreas[1].y+4, IMG_ANGRYBIRD_UI_SOUND_OFF);
				#else	
					mmSABDrawUI(mmSBAUIHotAreas[1].x, mmSBAUIHotAreas[1].y-1, IMG_ANGRYBIRD_UI_SOUND_OFF);
				#endif
			}
			if(mmSABIsBackFromSuspend == mmTrue)
					mmSABIsBackFromSuspend = mmFalse;
		break;
	}
}

extern void setHeadBirdPosition(mmS32 x, mmS32 y);



void mmSABUserOperationEvent(mmU32 eventType, mmU16 wParam, mmU16 mParam){
	switch(eventType)
	{
	case LLP_KEY_UP:
		switch(wParam)
		{
			case VKEY_ENTER:
				break;
			case VKEY_UP:
				cameraMoveSign = -1;
				break;
			case VKEY_DOWN:
				cameraMoveSign = 1;
				break;
		}
		break;
	case LLP_KEY_DOWN:break;

	case LLP_PEN_DOWN:
		
		if((hotarea.y - cameraOffsetY) > 0){		
			if( ( wParam > hotarea.x && wParam < (hotarea.x + hotarea.width) ) &&
				( mParam > (hotarea.y - cameraOffsetY) && mParam < (hotarea.y - cameraOffsetY + hotarea.height) )   && mmSABOnlineBird != null)
			{
				isReadyToShoot = mmTrue;
				mmSABReadyShootSound = mmTrue;
			}
			else
			{
				isReadyToShoot = mmFalse;
				pen_point_y = wParam;
			}
		}
		else
		{
			isReadyToShoot = mmFalse;
			pen_point_y = wParam;
		}
		break;
	case LLP_PEN_UP:
		if(mmTrue == isValidReady){
			mmSBAIsWatching = mmTrue;
			isInitPhysicsNow = mmTrue;
			isReadyToShoot = mmFalse;
			isValidReady = mmFalse;
			mmSBANumberOfPathPoint = 0;
			mmSABBlueNumOfPathPoint[0] = 0;
			mmSABBlueNumOfPathPoint[1] = 0;
		}
		break;
	case LLP_PEN_MOVE:
		if(mmFalse == isReadyToShoot)
		{
			cameraMoveSign = pen_point_y > mParam ? 1 : -1;
			pen_point_y = mParam;
			//MM_DBG_PRINT("\r\n cameraMoveSign = %d",cameraMoveSign);
			//mmSABGameState = mmSABGotoHotare;//
			if(cameraMoveSign == 1)
				mmSABCameraMoveRight = mmTrue;
			if(cameraMoveSign == -1)
			{
				mmSABCameraMoveLeft = mmTrue;
				if(mmSABGameState	==	mmSABShowScene)
					{
						mmSABGameStateTimer = 0;
						mmSABGameState = mmSABGotoHotare;
					}
			}
		}
		else
		{
			if(mmSABGameState == mmSABUserOperation)
			{
			mmS32 w = wParam - BIRD_STARTPOSITION.x;
			mmS32 h = mParam - BIRD_STARTPOSITION.y + cameraOffsetY;
			mmU32 ds= w * w + h * h;
			
			if(1 == mmSABSound && mmSABReadyShootSound){
				/*marmota->audio->stopAllAudio();
				marmota->audio->playRawData((mmAudioData*)sab_slingshot_resources, 0);*/
				mmSABReadyShootSound = mmFalse;
			}
			
			if(ds > 400)
			{
				float distance = mmSqrtf(ds);

				if(distance > 40)
				{
					w = 50 * (w / distance);
					h = 50 * (h / distance);
				}

				birdPosition.x = BIRD_STARTPOSITION.x + w;
				birdPosition.y = BIRD_STARTPOSITION.y + h;
				isValidReady = mmTrue;
			}
			else
			{
				birdPosition.x = BIRD_STARTPOSITION.x;
				birdPosition.y = BIRD_STARTPOSITION.y;
				isValidReady = mmFalse;
			}

			if(mmSABOnlineBird && mmSABOnlineBird->body
				&& mmSABOnlineBird->state == mmSABBirdState_ONLINE){
				mmSABOnlineBird->body->p.x = birdPosition.x;
				mmSABOnlineBird->body->p.y = birdPosition.y;
				}
			}
		}
		break;
	}
}


static void mmSABResultEvent(mmU16 x, mmU16 y)
{
	mmBool isContinue = mmFalse;
	int i = 0;
	for(i=7; i<=9; i++){
		isContinue = (( x > (mmSABResultAreas[i].x - 5) && x < (mmSABResultAreas[i].x + mmSABResultAreas[i].width + 5) ))&&
				(( y > (mmSABResultAreas[i].y - 5) && y < (mmSABResultAreas[i].y + mmSABResultAreas[i].height +5) ));
		if(isContinue)
			break;
	}
	
	switch(i){
		case 7:
			mmSABDrawOnce = 0;
			mmSABMainMenu=1;
//			mmEntryState(MM_SUPERANGRYBIRD_MAINMENU);
			break;
		case 8:
//			mmEntryState(MM_SUPERANGRYBIRD_KERNEL);
			break;
		case 9:
			if( (mmSABNumberOfPigs <= 0 || mmSABLevelScore[0] > mmSABGameStageId) && mmSABGameStageId < MMSAB_LEVEL_SUM-1)
			{
				mmSABGameStageId++;
//				mmEntryState(MM_SUPERANGRYBIRD_KERNEL);//进入下一关
			}
			break;
		default:
			break;
	
	}
}

void mm_superangrybird_kernel_event(mmU32 eventType, mmU16 wParam, mmU16 mParam){
	if(mmSABGameState == mmSABShowBirdDel)
		return;
	if(mmSABGameState != mmSABGameOption && mmSABGameState != mmSABShowResult)
	{
		if(eventType ==  LLP_PEN_UP)
		{
			if((wParam > SAB_UI_PAUSE_X - 10 && wParam < (SAB_UI_PAUSE_X + 50 ))&&
				(mParam > SAB_UI_PAUSE_Y - 10 && mParam < (SAB_UI_PAUSE_Y +  50)))
			{
				mmSABIsInitial = mmTrue;
				mmSBALastState = mmSABGameState;
				mmSABGameState = mmSABGameOption;
				return;
			}
		}
	}
	else
	{	
		if(eventType ==  LLP_PEN_UP && mmSABGameState == mmSABGameOption)
		{
			mmSABUIMessage(wParam, mParam);
		}
	}
	
	switch(mmSABGameState){
		case mmSABGotoHotare:break;
			case mmSABShowScene:
		case mmSABUserOperation:
			mmSABUserOperationEvent(eventType, wParam, mParam);
		break;
		case mmSABAuditScore://2011-7-22
		case mmSABPK:
			switch(eventType)
			{
			case LLP_PEN_DOWN:
				if(mmSABLastFlying){
					if(mmSABCanOccurSpecial)
					{
						if(mmSABLastFlying->birdtype == ANGRYBIRD_BIRD_BLACK)
							mmSABIsOccurSpecialTech = mmTrue;
						else if(mmSABCollision == mmFalse && mmSABLastFlying->birdtype != ANGRYBIRD_BIRD_RED)
								{
									if(mmSABBlueBirds[0] || mmSABBlueBirds[1])
										return;
									mmSABIsOccurSpecialTech = mmTrue;
									mmSABIsEffectPoint = 1;
									mmSABEffectPoint.x = mmSABLastFlying->body->p.x - 5;
									mmSABEffectPoint.y = mmSABLastFlying->body->p.y - 5;
								}
							else if(mmSABCollision == mmFalse && mmSABLastFlying->birdtype == ANGRYBIRD_BIRD_YELLOW)
							{
								mmSABIsOccurSpecialTech = mmTrue;
								mmSABIsEffectPoint = 1;
								mmSABEffectPoint.x = mmSABLastFlying->body->p.x - 5;
								mmSABEffectPoint.y = mmSABLastFlying->body->p.y - 5;
							}			
							mmSABCanOccurSpecial = mmFalse;
						}
						
						if(mmSABCollision == mmTrue	 && mmSABLastFlying->next)
							{
								mmSABGameState = mmSABUserOperation;
								mmSBAIsWatching = mmFalse;
							}
				}
			break;
			}
		break;
		case mmSABShowResult:
			switch(eventType)
			{
			case LLP_PEN_UP:
				mmSABResultEvent(wParam, mParam);
			break;
			}
		break; 
	}
}