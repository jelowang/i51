

#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#define GAME_RESTART_X	151
#define GAME_RESTART_Y	5
#define GAME_RESTART_W 23
#define GAME_RESTART_H	23
#define GAME_PAUSE_X	184
#define GAME_PAUSE_Y	5
#define GAME_PAUSE_W	46
#define GAME_PAUSE_H	23
#define GAME_RETURN_X	19
#define GAME_RETURN_Y	8
#define GAME_RETURN_W	41
#define GAME_RETURN_H	31
#define PAUSE_CONTINUE_X	72
#define PAUSE_CONTINUE_Y	103
#define PAUSE_CONTINUE_W	97
#define PAUSE_CONTINUE_H	27
#define PAUSE_CHOOSE_X	72
#define PAUSE_CHOOSE_Y	140
#define PAUSE_CHOOSE_W	97
#define PAUSE_CHOOSE_H	27
#define PAUSE_MAIN_X	72
#define PAUSE_MAIN_Y	178
#define PAUSE_MAIN_W	97
#define PAUSE_MAIN_H	27
#define MAX_POINT 22
#define HALF_POINT 11
#define MIN_POINT	7
#define LESS_POINT	3
// 格子中直线方式定义
#define BLOCK_LEFT	0
#define BLOCK_RIGHT	1
#define BLOCK_TOP	2
#define BLOCK_BUTTOM	3
#define BLOCK_LB	4
#define BLOCK_RB	5
#define BLOCK_RT	6
#define BLOCK_LT	7
#define BLOCK_ALL	8

#define RESULT_AGAIN_X	16
#define RESULT_AGAIN_Y	228
#define RESULT_AGAIN_W  97
#define RESULT_AGAIN_H  27
#define RESULT_NEXT_X	129
#define RESULT_NEXT_Y	228
#define RESULT_NEXT_W   97
#define RESULT_NEXT_H   27
#define RESULT_CHOOSE_X	72
#define RESULT_CHOOSE_Y	271
#define RESULT_CHOOSE_W   97
#define RESULT_CHOOSE_H   27


// 鸭子图片的宽度
#define DUCK_WIDTH 26
// 鸭子图片的高度
#define DUCK_HEIGHT	23

#define Organ_Key_W 24
#define Organ_Key_H 24

#elif defined(PKIG_SCREEN_240X400)
#define GAME_RESTART_X		142
#define GAME_RESTART_Y		10
#define GAME_RESTART_W		28
#define GAME_RESTART_H		28
#define GAME_PAUSE_X		179
#define GAME_PAUSE_Y		11
#define GAME_PAUSE_W		53
#define GAME_PAUSE_H		27
#define GAME_RETURN_X		19
#define GAME_RETURN_Y		13
#define GAME_RETURN_W		38
#define GAME_RETURN_H		32
#define PAUSE_CONTINUE_X	72
#define PAUSE_CONTINUE_Y	123
#define PAUSE_CONTINUE_W	97
#define PAUSE_CONTINUE_H	27
#define PAUSE_CHOOSE_X		72
#define PAUSE_CHOOSE_Y		170
#define PAUSE_CHOOSE_W	97
#define PAUSE_CHOOSE_H		27
#define PAUSE_MAIN_X		72
#define PAUSE_MAIN_Y		218
#define PAUSE_MAIN_W		97
#define PAUSE_MAIN_H		27
#define MAX_POINT			22
#define HALF_POINT			11
#define MIN_POINT			7
#define LESS_POINT			3
// 格子中直线方式定义
#define BLOCK_LEFT	0
#define BLOCK_RIGHT	1
#define BLOCK_TOP	2
#define BLOCK_BUTTOM	3
#define BLOCK_LB	4
#define BLOCK_RB	5
#define BLOCK_RT	6
#define BLOCK_LT	7
#define BLOCK_ALL	8

#define RESULT_AGAIN_X		16
#define RESULT_AGAIN_Y		228
#define RESULT_AGAIN_W  	97
#define RESULT_AGAIN_H  	27
#define RESULT_NEXT_X		129
#define RESULT_NEXT_Y		228
#define RESULT_NEXT_W   	97
#define RESULT_NEXT_H   		27
#define RESULT_CHOOSE_X	72
#define RESULT_CHOOSE_Y	271
#define RESULT_CHOOSE_W   	97
#define RESULT_CHOOSE_H   	27


// 鸭子图片的宽度
#define DUCK_WIDTH 26
// 鸭子图片的高度
#define DUCK_HEIGHT	23

#define Organ_Key_W 26
#define Organ_Key_H 26

#elif defined(PKIG_SCREEN_320X480)	
#define GAME_RESTART_X	219
#define GAME_RESTART_Y	7
#define GAME_RESTART_W	28
#define GAME_RESTART_H	28
#define GAME_PAUSE_X	255
#define GAME_PAUSE_Y	7
#define GAME_PAUSE_W	53
#define GAME_PAUSE_H	27
#define GAME_RETURN_X	19
#define GAME_RETURN_Y	13
#define GAME_RETURN_W	50
#define GAME_RETURN_H	37
#define PAUSE_CONTINUE_X	93
#define PAUSE_CONTINUE_Y	115
#define PAUSE_CONTINUE_W	128
#define PAUSE_CONTINUE_H	41
#define PAUSE_CHOOSE_X	93
#define PAUSE_CHOOSE_Y	170
#define PAUSE_CHOOSE_W	128
#define PAUSE_CHOOSE_H	41
#define PAUSE_MAIN_X	93
#define PAUSE_MAIN_Y	227
#define PAUSE_MAIN_W	128
#define PAUSE_MAIN_H	41
#define MAX_POINT 22
#define HALF_POINT 11
#define MIN_POINT	7
#define LESS_POINT	3
// 格子中直线方式定义
#define BLOCK_LEFT	0
#define BLOCK_RIGHT	1
#define BLOCK_TOP	2
#define BLOCK_BUTTOM	3
#define BLOCK_LB	4
#define BLOCK_RB	5
#define BLOCK_RT	6
#define BLOCK_LT	7
#define BLOCK_ALL	8

#define RESULT_AGAIN_X	21
#define RESULT_AGAIN_Y	330
#define RESULT_NEXT_X	167
#define RESULT_NEXT_Y	330
#define RESULT_CHOOSE_X	96
#define RESULT_CHOOSE_Y	394

// 鸭子图片的宽度
#define DUCK_WIDTH 26
// 鸭子图片的高度
#define DUCK_HEIGHT	23

#define RESULT_AGAIN_W  128
#define RESULT_AGAIN_H  41
#define RESULT_NEXT_W   128
#define RESULT_NEXT_H   41
#define RESULT_CHOOSE_W   128
#define RESULT_CHOOSE_H   41

#define Organ_Key_W 32
#define Organ_Key_H 32

#endif

//恒速区域 详情
SpeedKeep SpeedKeepMess[SpeedKeepNum];
// 恒速区域管理器
SpeedKeepManage SpeedKeep_Manage;

// 鸭子详情
DuckData DuckMess[DUCK_NUM];
// 鸭子管理器
DuckManage Duck_Manage;

// 机关具体信息
OrganData OrganMess[MAX_ORGAN];
// 机关管理器
OrganManage Organ_Manage;

// 水管具体信息
PipeData PipeMess[MAX_PIPE];
// 水管管理器
PipeManage Pipe_Manage;
// 浴缸中已有水滴数量
iS16 WaterInBathtub = 0;
// 获得时间奖励的分数
iS16 CurrTimeWin = 0;
// 获得鸭子奖励的分数
iS16 CurrDuckWin = 0;
// 获得溢水奖励的分数
iS16 CurrWaterWin = 0;
// 关卡获得的总分
iS16 CurrTotalWin = 0;

// 当前是第几关的标记
mwLevelNum CurrLevelNum = MW_LEVEL_CHOOSE;
// 当前关卡内状态的标记
mwLevelState CurrLevelState = MW_LEVEL_INIT;
// 当前世界的标记，记录每一个像素点的属性
//iU8 mwWorldAttribute[SCREEN_HEIGHT_HALF][SCREEN_WIDTH_HALF] = {0};
iU8 ** mwWorldAttribute = iNULL;
// 关卡暂停的标记
iU8 mwWordPause = iFALSE;
// 音效开关的标记
iU8 SoundCloseFlag = iFALSE;
// 音乐开关的标记
iU8 MusicCloseFlag = iFALSE;
// 关卡开始的时间
iU32 TimeLevelStart = 0;
// 关卡当前的时间
iU32 TimeLevelCurr = 0;
// 屏幕buff备份
iU16 *TempScreenBuf = iNULL;

#define DRAW_LINE	0

//#define Organ_Key_W 32
//#define Organ_Key_H 32 

// 屏幕网格管理器
 iU8  GriddingMud[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength] = {0};
// 屏幕网格状态
 iU8  GriddingType[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength] = {0};
// 屏幕网格是否需要更新的标记
 iU8  GriddingUpdataFlag[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength] = {0};
// 屏幕网格是否可更改信息，如果为1则不允许更新只能删除
//static iU8  GriddingAdd[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength] = {0};
// 海藻状态,为非零则有海藻为0则无海藻
//iU8  AlgaeMess[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength] = {0};
// 关卡内是否包含海藻的标记
iU8  AlgaeNeed = iFALSE;

// 鳄鱼等待水来动画结构
AnimationStr CrocodileWait;
// 鳄鱼洗澡动画结构
AnimationStr CrocodileBathe;

iS32 TimeCount = 0;

static void update(int ticks)
{
	i51KitP2Step(mwSpace);
}

void drawSegmentShape(iBody *body, iShapeSeg *seg, iWorld *space)
{
	iVECT a;// = seg->ta;
	iVECT b;// = seg->tb;
	iS32 i=0;
	iS32 StartX = 0;
	iS32 StartY = 0;
	iS32 EndX = 0;
	iS32 EndY = 0;
	iVECT MovePos;
	iS32 Point[2][2] = {0};// = {(iS32)a.x,(iS32)a.y,(iS32)b.x,(iS32)b.y};

	if( seg->Shape.ShapeGroup)
	{
		for( i=0; i<Organ_Manage.TotalOrganNum; i++ )
		{
			if(	OrganMess[i].OrgneState == ORGAN_STATE_MOVE
			&&	seg->Shape.ShapeGroup == OrganMess[i].OrganType )
			{
				if( OrganMess[i].OrgneState == ORGAN_STATE_MOVE )
				{
					MovePos.X = OrganMess[i].OrganMulX;
					MovePos.Y = OrganMess[i].OrganMulY;
					i51KitP2ShapePosMove((iHANDLE)(seg), &MovePos);
				}
			}
		}
	}
#if DRAW_LINE


	iVectInit1(&a, seg->p1);
	iVectInit1(&b, seg->p2);
	
	Point[0][0]= (iS32)(a.X)/COORDINATE_RATE;
	Point[0][1] = (iS32)(a.Y)/COORDINATE_RATE;
	Point[1][0]= (iS32)(b.X)/COORDINATE_RATE;
	Point[1][1]= (iS32)(b.Y)/COORDINATE_RATE;

	i51KitG2DrawLineEx( Point, 0XFF00);
#endif	
}

//iS32 CurrX = 0;
//iS32 CurrY = 0;

static void
drawPolyShape( iBody *body, iShapePoly *poly, iWorld *space)
{

}

static void
drawObject(iShape *shape, iWorld*space)
{
	iBody *body = shape->Body;
	
	switch(shape->ShapeType){
		case I51_CIRCLE_SHAPE:
		//	iLog("11111 DrawWaterDrop");

			DrawWaterDrop(body, (iShapeCircle *)shape, 0XFF0);

		//	iLog("22222 DrawWaterDrop");

			break;
		case I51_SEGMENT_SHAPE:
		//	iLog("33333 drawSegmentShape");

			drawSegmentShape(body, (iShapeSeg*)shape, space);

		//	iLog("44444 drawSegmentShape");
			break;
		case I51_POLY_SHAPE:
			drawPolyShape(body, (iShapePoly*)shape, space);
			break;
		default:
			break;
	}
}

void
drawSpace(iHANDLE space)
{
	iWorld * World = (iWorld*)space;
	NodeContainer * ShapeContainer = &(World->ShapeContainer);
	Node * NodeHandle = ShapeContainer->NodeHandle;
	iS32 MaxNode = ShapeContainer->MaxNode;
	iS32 UseNode = ShapeContainer->UseNode;
	iS32 Temp = 0;
	iS32 CurrUse = 0;
	iShape * Shape = iNULL;
	iS32 X= 0, Y = 0;

	for( Temp = 0; Temp<MaxNode; Temp++)
	{
		if( NodeHandle[Temp].Flag)
		{
			Shape = (iShape*)(NodeHandle[Temp].Handle);
		//	iLog("11111 drawObject");
			drawObject(Shape, World);
		//	iLog("22222 drawObject");

			CurrUse ++;
			if( CurrUse==UseNode ) break;
		}
	}
	if( CurrUse!=UseNode )
	{
		iLog("drawSpace CurrUse:%d,UseNode:%d",CurrUse,UseNode);
	}

	for( Y = 0 ; Y<SCREEN_HEIGHT_HALF; Y++ )
	{
		for( X = 0; X <SCREEN_WIDTH_HALF; X ++)
		{
			if( (mwWorldAttribute[Y][X]&WORD_ALGAE)==0 )
			{
				mwWorldAttribute[Y][X] &= 0X0F;
			}
		}
	}
	
}

static void cpWorldUpdata(void)
{
// 物理引擎管理的世界更新
//iLog("Start cpWorldUpdata");
	update(0);
//iLog("End cpWorldUpdata");
}

iBOOL cpWordDraw( void )
{
// 绘制物理世界中的点线多边形等等
//	memset(GriddingWaterNum, 0X00, SCREEN_HEIGHT/SideLength*SCREEN_WIDTH/SideLength);
	//iU32 OldTime, NextTime, CurrTime ;
	CurrTotalWaterNum = 0;

	//iLog("111 cpWordDraw");
	//OldTime = i51AdeOsGetTick();
	drawSpace(mwSpace);
	//iLog("222 cpWordDraw");
	//NextTime = i51AdeOsGetTick();

//iLog("NextTime-OldTime:%d",NextTime-OldTime);

	mwLevelDrawOrgan();
	//iLog("333 cpWordDraw");

	//CurrTime  = i51AdeOsGetTick();
//iLog("dCurrTime-NextTime:%d",CurrTime-NextTime);

	if( CurrTotalWaterNum>=WaterSpray_MAX)
	{
		WaterSprayFlag = iFALSE;
	}
	else if( CurrTotalWaterNum<=WaterSpray_MIN)
	{
		WaterSprayFlag = iTRUE;
	}
//iLog("End cpWordDraw");
	return iTRUE;
}

iBOOL mwInitSpace( mwLevelNum level)
{
	iVECT Gravity;
	
	Gravity.X = 0;
	Gravity.Y = 300;

	if(mwSpace)
	{
		i51KitP2ReleaseWorld(mwSpace);
		mwSpace = iNULL;
	}
	mwSpace = i51KitP2CreateWorld(iNULL, &Gravity);
	i51KitP2ResetWorldTimeStamp(mwSpace, 3, 7);
//	i51KitP2ResetWorldTimeStamp(mwSpace, 3, 8);

	
	mwWordPause = iFALSE;
	AlgaeNeed = iFALSE;
	TempScreenUpdataFlag = iFALSE;
	WaterSprayFlag = iTRUE;

	return iTRUE;
	
}

iBOOL mwStateGame( void )
{
//iLog("CurrLevelState=%d",CurrLevelState);
	switch(CurrLevelState)
	{
		case MW_LEVEL_INIT: 
			mwLevelInit(CurrLevelNum);
			break;
		case MW_LEVEL_RUN:
			mwLevelRun(CurrLevelNum);
			break;
		case MW_LEVEL_PAUSE:
			mwLevelPause(CurrLevelNum);
			break;
		case MW_LEVEL_WIN:
			mwLevelWin(CurrLevelNum);
			break;
		case MW_LEVEL_RESULT:
			mwLevelResult(CurrLevelNum);
			break;
		default :
			break;
			
	}
//iLog("End mwStateGame");
	return iTRUE;
}

iBOOL mwLevelExit( mwLevelNum level)
{
	i51KitP2ReleaseWorld(mwSpace);
	mwSpace = iNULL;
	return iTRUE;
}

iBOOL mwLevelInit( mwLevelNum level)
{
	// 清除关卡暂停标记
	mwWordPause = iFALSE;
	TimeCount = 0;
//iLog("level=%d",level);
	switch(level)
	{
		case MW_LEVEL_CHOOSE:
			if(mwInitLevelChoose())
			{
				CurrLevelState = MW_LEVEL_RUN;
			}
			break;
		case MW_LEVEL_001:
			mwInitLevel001();
			CurrLevelState = MW_LEVEL_RUN;
			//iLog("mwInitLevel001 OK");

			break;
		case MW_LEVEL_002:
			mwInitLevel002();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_003:
			mwInitLevel003();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_004:
			mwInitLevel004();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_005:
			//mwInitLevel005();
			mwInitLevel006();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_006:
			//mwInitLevel006();
			mwInitLevel009();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_007:
			//mwInitLevel007();
			mwInitLevel011();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_008:
			//mwInitLevel008();
			mwInitLevel013();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_009:
			mwInitLevel009();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_010:
			mwInitLevel010();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_011:
			mwInitLevel011();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_012:
			mwInitLevel012();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_013:
			mwInitLevel013();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_014:
			mwInitLevel014();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_015:
			mwInitLevel015();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		default:
			CurrLevelNum = MW_LEVEL_001;
			break;
	}

	return iTRUE;
}

extern mwLevelChooseInitDraw();

iBOOL mwLevelRun( mwLevelNum level)
{
	//iU32 t1, t2, t3, t4;
	if(!mwWordPause)	
	{
		mwEarthCutting(CurrLevelNum);

	}
	switch(level)
	{
		case MW_LEVEL_CHOOSE:
			mwLevelChooseInitDraw();
			mwRunLevelChoose();
			break;
		case MW_LEVEL_001:
			mwRunLevel001();
			break;
		case MW_LEVEL_002:
			mwRunLevel002();
			break;
		case MW_LEVEL_003:
			mwRunLevel003();
			break;
		case MW_LEVEL_004:
			mwRunLevel004();
			break;
		case MW_LEVEL_005:
			//mwRunLevel005();
			mwRunLevel006();
			break;
		case MW_LEVEL_006:
			//mwRunLevel006();
			mwRunLevel009();
			break;
		case MW_LEVEL_007:
			//mwRunLevel007();
			mwRunLevel011();
			break;
		case MW_LEVEL_008:
			//mwRunLevel008();
			mwRunLevel013();
		case MW_LEVEL_009:
			mwRunLevel009();
			break;
		case MW_LEVEL_010:
			mwRunLevel010();
			break;
		case MW_LEVEL_011:
			mwRunLevel011();
			break;
		case MW_LEVEL_012:
			mwRunLevel012();
			break;
		case MW_LEVEL_013:
			mwRunLevel013();
			break;
		case MW_LEVEL_014:
			mwRunLevel014();
			break;
		case MW_LEVEL_015:
			mwRunLevel015();
			break;
		default:
			//iLog("mwLevelRun:%d",level);
			break;
	}

if(!mwWordPause)	
{
	cpWorldUpdata();
}

	return iTRUE;
}

iU8 InitFlag = iFALSE;

iBOOL mwLevelPause( mwLevelNum level)
{
	iRECT DesRect;
	iRECT SrcRect;
	iU16 PenDownX = 0;
	iU16 PenDownY = 0;

	if( iFALSE == InitFlag )
	{
		InitFlag = iTRUE;
		mwLevelDrawPause();
#if 1		
		//i51KitG2SetAlpha(160);
		LocalG2AlphaSys = 160;
		//i51KitG2CleanScreen(0X0, iNULL);
		AlphaNeed = iTRUE;
		AlphaSrc = 160;

		i51KitG2CleanScreenEx(0X0, iNULL);
		//i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
		LocalG2AlphaSys = i51KITG2_ALPHA_NONEED;
#endif
		DesRect.X = PAUSE_CONTINUE_X;
		DesRect.Y = PAUSE_CONTINUE_Y;
		DesRect.Width= PAUSE_CONTINUE_W;
		DesRect.Height= PAUSE_CONTINUE_H;
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = PAUSE_CONTINUE_W;
		SrcRect.Height = PAUSE_CONTINUE_H;

		SWMW_i51KitG2DrawImage(&iMageInfo[PsMain], &DesRect, &SrcRect);
		DesRect.X = PAUSE_CHOOSE_X;
		DesRect.Y = PAUSE_CHOOSE_Y;
		DesRect.Width= PAUSE_CHOOSE_W;
		DesRect.Height= PAUSE_CHOOSE_H;

		SrcRect.X = 0;
		SrcRect.Y = PAUSE_CHOOSE_H * 2;
		SrcRect.Width = PAUSE_CHOOSE_W;
		SrcRect.Height = PAUSE_CHOOSE_H;

		//SWMW_i51KitG2DrawImage(mwContainer, PsMain, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[PsMain], &DesRect, &SrcRect);

		DesRect.X = PAUSE_MAIN_X;
		DesRect.Y = PAUSE_MAIN_Y;
		DesRect.Width= PAUSE_MAIN_W;
		DesRect.Height= PAUSE_MAIN_H;

		SrcRect.X = 0;
		SrcRect.Y = PAUSE_MAIN_H * 4;
		SrcRect.Width = PAUSE_MAIN_W;
		SrcRect.Height = PAUSE_MAIN_H;

		//SWMW_i51KitG2DrawImage(mwContainer, PsMain, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[PsMain], &DesRect, &SrcRect);

	}
	
	if(mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
	{
		PenDownX = mwKeyData.u.pen.x;
		PenDownY = mwKeyData.u.pen.y;

		if( PenDownX>=PAUSE_CONTINUE_X&&PenDownX<(PAUSE_CONTINUE_X+PAUSE_CONTINUE_W)
		&&PenDownY>=PAUSE_CONTINUE_Y&&PenDownY<(PAUSE_CONTINUE_Y+PAUSE_CONTINUE_H))
		{
			DesRect.X = PAUSE_CONTINUE_X;
			DesRect.Y = PAUSE_CONTINUE_Y;
			DesRect.Width= PAUSE_CONTINUE_W;
			DesRect.Height= PAUSE_CONTINUE_H;
			SrcRect.X = 0;
			SrcRect.Y = PAUSE_CONTINUE_H;
			SrcRect.Width = PAUSE_CONTINUE_W;
			SrcRect.Height = PAUSE_CONTINUE_H;

			//SWMW_i51KitG2DrawImage(mwContainer, PsMain, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[PsMain], &DesRect, &SrcRect);


			mwWordPause = iFALSE;
			CurrLevelState = MW_LEVEL_RUN;
			InitFlag = iFALSE;
		}
		else if( PenDownX>=PAUSE_CHOOSE_X&&PenDownX<(PAUSE_CHOOSE_X+PAUSE_CHOOSE_W)
		&&PenDownY>=PAUSE_CHOOSE_Y&&PenDownY<(PAUSE_CHOOSE_Y+PAUSE_CHOOSE_H))
		{
			DesRect.X = PAUSE_CHOOSE_X;
			DesRect.Y = PAUSE_CHOOSE_Y;
			DesRect.Width= PAUSE_CHOOSE_W;
			DesRect.Height= PAUSE_CHOOSE_H;
			////SWMW_i51KitG2DrawImage(mwContainer, PsChsPrs, &DesRect, iNULL);
			SrcRect.X = 0;
			SrcRect.Y = PAUSE_CHOOSE_H * 3;
			SrcRect.Width = PAUSE_CHOOSE_W;
			SrcRect.Height = PAUSE_CHOOSE_H;

			//SWMW_i51KitG2DrawImage(mwContainer, PsMain, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[PsMain], &DesRect, &SrcRect);


			mwWordPause = iTRUE;
			InitFlag = iFALSE;
			//mwLevelUnsetImageMem(level);
			CurrLevelState = MW_LEVEL_INIT;
			//xie
			//i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL );
			wmw_image_destroy();
			wmw_choose_page();
			CurrLevelNum = MW_LEVEL_CHOOSE;
		}
		else if( PenDownX>=PAUSE_MAIN_X&&PenDownX<(PAUSE_MAIN_X+PAUSE_MAIN_W)
		&&PenDownY>=PAUSE_MAIN_Y&&PenDownY<(PAUSE_MAIN_Y+PAUSE_MAIN_H))
		{
			DesRect.X = PAUSE_MAIN_X;
			DesRect.Y = PAUSE_MAIN_Y;
			DesRect.Width= PAUSE_MAIN_W;
			DesRect.Height= PAUSE_MAIN_H;
			SrcRect.X = 0;
			SrcRect.Y = PAUSE_MAIN_H * 5;
			SrcRect.Width = PAUSE_MAIN_W;
			SrcRect.Height = PAUSE_MAIN_H;

			//SWMW_i51KitG2DrawImage(mwContainer, PsMain, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[PsMain], &DesRect, &SrcRect);


			mwWordPause = iTRUE;
			CurrGameState = GAME_STATE_MAIN;
			wmw_image_destroy();
			wmw_main_page();
			//i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL );
			InitFlag = iFALSE;
			mwLevelExit(level);		
		}
	}	
	
	return iTRUE;
}

iBOOL mwLevelResume( mwLevelNum level)
{
	return iTRUE;
}

iBOOL mwLevelShowNum( iHANDLE Handle,iU32 ImageID, iS16 Num, iS16 X, iS16 Y)
{
	iU32 ImageWidth = 0;
	iU32 ImageHeight = 0;
	iRECT DesRect;
	iRECT SrcRect;
	
	if (ImageID == NumSmall)
	{
		i51KitG2GetImageAttrEx((iU32)NumSmall_Hex, &ImageWidth, &ImageHeight);
	}
	else if (ImageID == NumBig)
	{
		i51KitG2GetImageAttrEx((iU32)NumBig_Hex, &ImageWidth, &ImageHeight);
	}
	DesRect.X = X;
	DesRect.Y = Y;
	DesRect.Width = ImageWidth;
	DesRect.Height = ImageHeight/12;

	SrcRect.X = 0;
	SrcRect.Y = 0;
	SrcRect.Width = ImageWidth;
	SrcRect.Height= DesRect.Height;
	if(Num==0)
	{
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &DesRect, &SrcRect);
		return iTRUE;
	}
	
	while(Num>0)
	{
		SrcRect.Y = (Num%10)*DesRect.Height;
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &DesRect, &SrcRect);
		DesRect.X -= DesRect.Width;
		Num /= 10;
	}

	return iTRUE;
	
}
 iU8	mwLevelWinFlag = iFALSE;
 
iBOOL mwLevelWin( mwLevelNum level)
{
	iS32 TimeMul = 20000 - TimeLevelCurr + TimeLevelStart;
	//static iU8 Flag = iFALSE;
	int i = 0;

#if SOUND_NEED
	if(iFALSE==SoundCloseFlag && SoundWinPlay== iFALSE)
	{
		SoundWinPlay = iTRUE;
		//pfunc->public_audio_play(
		//	0, 
		//	MW_SOUND_NAME_WIN, 
		//	i51AdeStdWStrlen((iU16*)MW_SOUND_NAME_WIN), 
		//	i51_MMI_DEC_FILE_BASED,
		//	AUDIO_PLAYER_WAV_0, 
		//	iFALSE
		//	);

		//pfunc->public_audio_play(0, SD_WIN, AUDIO_PLAYER_MP3, iFALSE);
		public_audio_play_x(0, wmw_SoundWin_mp3, wmw_SoundWin_mp3_len,i51_MMI_DEC_MEM_BASED, AUDIO_PLAYER_MP3, iFALSE);

	}
#endif

	if( !mwLevelWinFlag)
	{
		mwLevelWinFlag = iTRUE;
		mwLevelBatheInit( level );
		if( TimeMul>0 ) CurrTimeWin = TimeMul/1000*10;
		else CurrTimeWin = 0;

		CurrDuckWin = (Duck_Manage.TotalDuckNum - Duck_Manage.ResidueDuckNum) * 100;

		TimeLevelStart = i51AdeOsGetTick();
	}
	else if( i51AdeOsGetTick() - TimeLevelStart > 3000 )
	{
	 	mwLevelWinFlag = iFALSE;
		if( WaterInBathtub>WIN_WATER ) CurrWaterWin = (WaterInBathtub-WIN_WATER)*40;
		if( CurrWaterWin>100 ) CurrWaterWin = 100;

		CurrTotalWin = CurrTimeWin + CurrDuckWin + CurrWaterWin;

		WaterInBathtub = 0;
		
		CurrLevelState = MW_LEVEL_RESULT;

		iScreenFlag = iTRUE;

		wmw_image_destroy();

		wmw_result_page();

		mwWordPause = iTRUE;


		if( LevelDuckNum[CurrLevelNum-1]<Duck_Manage.TotalDuckNum - Duck_Manage.ResidueDuckNum )
		{
			CurrTotalDuck -= LevelDuckNum[CurrLevelNum-1];
			LevelDuckNum[CurrLevelNum-1] = Duck_Manage.TotalDuckNum - Duck_Manage.ResidueDuckNum;
			CurrTotalDuck += LevelDuckNum[CurrLevelNum-1];
		}
		
		if( UnlockMaxLevel<TOTAL_LEVEL && CurrLevelNum==UnlockMaxLevel )
		{
			UnlockMaxLevel++;
		}
		
	}

	if( TempScreenUpdataFlag )
	{
		TempScreenUpdataFlag = iFALSE;
		mwTempScreenBufUpdate(CurrLevelNum);
		cpWordDraw();
	}

	mwLevelWaterSpray(CurrLevelNum);
	cpWorldUpdata();
	mwEarthCutting(CurrLevelNum);
#ifdef __MODIS_DEBUG__
	for( i=0;i<SCREEN_HEIGHT; i++)
	{
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+i*320),
			(void *)(TempScreenBuf + i*SCREEN_WIDTH), 
			SCREEN_WIDTH*2);
	}
#else
	i51AdeStdMemcpy16( (void *)(mwScreenBuffer),
		(void *)(TempScreenBuf), 
		SCREEN_WIDTH*SCREEN_HEIGHT*2);

#endif
	mwPublicAnimation(&CrocodileBathe);
	mwLevelDrawDuck();
	cpWordDraw();

//	mwLevelDrawOrgan();

	return iTRUE;
}

	 iU8 mwLevelResultFlag = iFALSE;
	 iU32 LevelErrStartTime = 0;
	 iU8 LevelErrFlag = iFALSE;
	 iU8 DrawBlankAlpha = iFALSE;

iBOOL mwLevelResult( mwLevelNum level)
{/*
	static iU8 Flag = iFALSE;
	static iU32 LevelErrStartTime = 0;
	static iU8 LevelErrFlag = iFALSE;
	static iU8 DrawBlankAlpha = iFALSE;
*/
	iRECT DesRect;
	iRECT SrcRect;
	iU32 ImageWidth = 0;
	iU32 ImageHeight = 0;

	iU16 PenDownX = 0;
	iU16 PenDownY = 0;

#ifdef PKIG_SCREEN_240X320
	iU16 WordOrg_X = 29; 
	iU16 WordOrg_Y = 29; 
	iU16 WinDuck_X = 15;
	iU16 WinDuck_Y = 57;
	iU16 Offset_X = 30;

	iS16 TimeAward_X = 196;
	iS16 TimeAward_Y = 18;
	iS16 DuckAward_X = 196;
	iS16 DuckAward_Y = 32;
	iS16 WaterAward_X = 196;
	iS16 WaterAward_Y = 46;
	iS16 Mark_X = 196;
	iS16 Mark_Y = 69;


#elif defined PKIG_SCREEN_240X400
	iU16 WordOrg_X = 10; 
	iU16 WordOrg_Y = 31; 
	iU16 WinDuck_X = 3;
	iU16 WinDuck_Y = 62;
	iU16 Offset_X = 30;

	iS16 TimeAward_X = 204;
	iS16 TimeAward_Y = 32;
	iS16 DuckAward_X = 204;
	iS16 DuckAward_Y = 47;
	iS16 WaterAward_X = 204;
	iS16 WaterAward_Y = 62;
	iS16 Mark_X = 200;
	iS16 Mark_Y = 80;


#elif defined PKIG_SCREEN_320X480
	iU16 WordOrg_X = 37; 
	iU16 WordOrg_Y = 38; 
	iU16 WinDuck_X = 13;
	iU16 WinDuck_Y = 78;
	iU16 Offset_X = 41;

	iS16 TimeAward_X = 283;
	iS16 TimeAward_Y = 40;
	iS16 DuckAward_X = 283;
	iS16 DuckAward_Y = 58;
	iS16 WaterAward_X = 283;
	iS16 WaterAward_Y = 76;
	iS16 Mark_X = 279;
	iS16 Mark_Y = 99;

#endif				

	iFILE File = iNULL;

	iU32 Read;


	if( !mwLevelResultFlag)
	{
		mwLevelResultFlag = iTRUE;

		File = i51AdeOsFileOpen((iU16 *)MW_DATA_PATH, i51_OS_FL_WRITE|i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ );
		if (File > 0)
		{
			//i51AdeOsFileSetSeek(File, 10, i51_OS_FL_SEEK_HEAD);
			i51AdeOsFileWriteInt32(File, &CurrTotalDuck );
			i51AdeOsFileWriteInt32(File, &UnlockMaxLevel  );
			i51AdeOsFileWrite(File, LevelDuckNum, TOTAL_LEVEL, &Read);
			i51AdeOsFileClose(File);
			//UnlockMaxLevel = 15; //xie

		}
	}

	if (iScreenFlag == iTRUE)
	{

		iScreenFlag = iFALSE;

		SWMW_i51KitG2DrawImage(&iMageInfo[WinCrocodile], iNULL, iNULL);

		DesRect.X = RESULT_AGAIN_X;
		DesRect.Y = RESULT_AGAIN_Y;
		DesRect.Width= RESULT_AGAIN_W;
		DesRect.Height= RESULT_AGAIN_H;

		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = RESULT_AGAIN_W;
		SrcRect.Height = RESULT_AGAIN_H;
		SWMW_i51KitG2DrawImage(&iMageInfo[WinAgain], &DesRect, &SrcRect);

		DesRect.X = RESULT_NEXT_X;
		DesRect.Y = RESULT_NEXT_Y;
		DesRect.Width= RESULT_NEXT_W;
		DesRect.Height= RESULT_NEXT_H;

		SrcRect.X = 0;
		SrcRect.Y = RESULT_NEXT_H * 2;
		SrcRect.Width = RESULT_NEXT_W;
		SrcRect.Height = RESULT_NEXT_H;

		//SWMW_i51KitG2DrawImage(mwContainer, WinAgain, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[WinAgain], &DesRect, &SrcRect);

		DesRect.X = RESULT_CHOOSE_X;
		DesRect.Y = RESULT_CHOOSE_Y;
		DesRect.Width= RESULT_CHOOSE_W;
		DesRect.Height= RESULT_CHOOSE_H;

		SrcRect.X = 0;
		SrcRect.Y = RESULT_CHOOSE_H * 2;
		SrcRect.Width = RESULT_CHOOSE_W;
		SrcRect.Height = RESULT_CHOOSE_H;
		//SWMW_i51KitG2DrawImage(mwContainer, PsMain, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[PsMain], &DesRect, &SrcRect);


		switch(Duck_Manage.ResidueDuckNum)
		{
		case 0:
			i51KitG2GetImageAttrEx((iU32)WinWord_Hex, &ImageWidth, &ImageHeight);
			DesRect.X = WordOrg_X;
			DesRect.Y = WordOrg_Y;

			DesRect.Width = ImageWidth;
			DesRect.Height = ImageHeight / 3;

			SrcRect.X = 0;
			SrcRect.Y = 0;
			SrcRect.Width = ImageWidth;
			SrcRect.Height = ImageHeight / 3;

			//SWMW_i51KitG2DrawImage(mwContainer, WinWord, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[WinWord], &DesRect, &SrcRect);


			////i51KitG2GetImageAttr(mwContainer, WinDuckOK, &ImageWidth, &ImageHeight, iNULL);
			//DesRect.X = 13;
			//DesRect.Y = 78;
			//DesRect.Width = ImageWidth;
			//DesRect.Height = ImageHeight;

			i51KitG2GetImageAttrEx((iU32)WinDuck_Hex, &ImageWidth, &ImageHeight);

			DesRect.X = WinDuck_X;
			DesRect.Y = WinDuck_Y;

			DesRect.Width = ImageWidth;
			DesRect.Height = ImageHeight / 2;

			SrcRect.X = 0;
			SrcRect.Y = 0;
			SrcRect.Width = ImageWidth;
			SrcRect.Height = ImageHeight / 2;


			//SWMW_i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[WinDuck], &DesRect, &SrcRect);

			DesRect.X += Offset_X;
			//SWMW_i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[WinDuck], &DesRect, &SrcRect);

			DesRect.X += Offset_X;
			//SWMW_i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[WinDuck], &DesRect, &SrcRect);


			break;
		case 1:


			i51KitG2GetImageAttrEx((iU32)WinWord_Hex, &ImageWidth, &ImageHeight);

			DesRect.X = WordOrg_X;
			DesRect.Y = WordOrg_Y;
			DesRect.Width = ImageWidth;
			DesRect.Height = ImageHeight/3;

			SrcRect.X = 0;
			SrcRect.Y = ImageHeight/3;
			SrcRect.Width = ImageWidth;
			SrcRect.Height = ImageHeight / 3;

			//SWMW_i51KitG2DrawImage(mwContainer, WinWord, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[WinWord], &DesRect, &SrcRect);


			i51KitG2GetImageAttrEx((iU32)WinDuck_Hex, &ImageWidth, &ImageHeight);
			DesRect.X = WinDuck_X;
			DesRect.Y = WinDuck_Y;
			DesRect.Width = ImageWidth;
			DesRect.Height = ImageHeight;

			SrcRect.X = 0;
			SrcRect.Y = 0;
			SrcRect.Width = ImageWidth;
			SrcRect.Height = ImageHeight / 2;

			//SWMW_i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[WinDuck], &DesRect, &SrcRect);

			DesRect.X += Offset_X;
			//SWMW_i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[WinDuck], &DesRect, &SrcRect);

			DesRect.X += Offset_X;

			SrcRect.X = 0;
			SrcRect.Y = ImageHeight / 2;
			SrcRect.Width = ImageWidth;
			SrcRect.Height = ImageHeight / 2;

			//SWMW_i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[WinDuck], &DesRect, &SrcRect);


			break;
		case 2:
			////i51KitG2GetImageAttr(mwContainer, WinOK, &ImageWidth, &ImageHeight, iNULL);
			//DesRect.X = 37;
			//DesRect.Y = 38;
			//DesRect.Width = ImageWidth;
			//DesRect.Height = ImageHeight;

			////i51KitG2GetImageAttr(mwContainer, WinDuckOK, &ImageWidth, &ImageHeight, iNULL);
			//DesRect.X = 13;
			//DesRect.Y = 78;
			//DesRect.Width = ImageWidth;
			//DesRect.Height = ImageHeight;
			//DesRect.X += 41;
			//DesRect.X += 41;

			i51KitG2GetImageAttrEx((iU32)WinWord_Hex, &ImageWidth, &ImageHeight);
			DesRect.X = WordOrg_X;
			DesRect.Y = WordOrg_Y;
			DesRect.Width = ImageWidth;
			DesRect.Height = ImageHeight / 3;

			SrcRect.X = 0;
			SrcRect.Y = ImageHeight * 2 / 3;
			SrcRect.Width = ImageWidth;
			SrcRect.Height = ImageHeight / 2;

			//SWMW_i51KitG2DrawImage(mwContainer, WinWord, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[WinWord], &DesRect, &SrcRect);

			i51KitG2GetImageAttrEx((iU32)WinDuck_Hex, &ImageWidth, &ImageHeight);
			DesRect.X = WinDuck_X;
			DesRect.Y = WinDuck_Y;
			DesRect.Width = ImageWidth;
			DesRect.Height = ImageHeight / 2;

			SrcRect.X = 0;
			SrcRect.Y = 0;
			SrcRect.Width = ImageWidth;
			SrcRect.Height = ImageHeight / 2;

			//SWMW_i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[WinDuck], &DesRect, &SrcRect);


			DesRect.X += Offset_X;

			SrcRect.X = 0;
			SrcRect.Y = ImageHeight / 2;
			SrcRect.Width = ImageWidth;
			SrcRect.Height = ImageHeight / 2;

			//SWMW_i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, &SrcRect);		
			SWMW_i51KitG2DrawImage(&iMageInfo[WinDuck], &DesRect, &SrcRect);

			DesRect.X += Offset_X;
			//SWMW_i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, &SrcRect);				
			SWMW_i51KitG2DrawImage(&iMageInfo[WinDuck], &DesRect, &SrcRect);
			break;
		default:
			////i51KitG2GetImageAttr(mwContainer, WinDuckErr, &ImageWidth, &ImageHeight, iNULL);
			//DesRect.X = 13;
			//DesRect.Y = 78;
			//DesRect.Width = ImageWidth;
			//DesRect.Height = ImageHeight;
			//DesRect.X += 41;
			//DesRect.X += 41;

			i51KitG2GetImageAttrEx((iU32)WinDuck_Hex, &ImageWidth, &ImageHeight);
			DesRect.X = WinDuck_X;
			DesRect.Y = WinDuck_Y;
			DesRect.Width = ImageWidth;
			DesRect.Height = ImageHeight / 2;

			SrcRect.X = 0;
			SrcRect.Y = ImageHeight / 2;
			SrcRect.Width = ImageWidth;
			SrcRect.Height = ImageHeight / 2;

			//SWMW_i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, &SrcRect);	
			SWMW_i51KitG2DrawImage(&iMageInfo[WinDuck], &DesRect, &SrcRect);
			DesRect.X += Offset_X;
			//SWMW_i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, &SrcRect);	
			SWMW_i51KitG2DrawImage(&iMageInfo[WinDuck], &DesRect, &SrcRect);
			DesRect.X += Offset_X;
			//SWMW_i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, &SrcRect);		
			SWMW_i51KitG2DrawImage(&iMageInfo[WinDuck], &DesRect, &SrcRect);

			break;				
		}

		////i51KitG2GetImageAttr(mwContainer, WinTime, &ImageWidth, &ImageHeight, iNULL);
		//DesRect.X = 162;
		//DesRect.Y = 37;
		//DesRect.Width = ImageWidth;
		//DesRect.Height = ImageHeight;
		//mwLevelShowNum(mwContainer,NumSmall,CurrTimeWin,283,40);
		mwLevelShowNum(mwContainer,NumSmall,CurrTimeWin,TimeAward_X,TimeAward_Y);

		////i51KitG2GetImageAttr(mwContainer, WinDuck, &ImageWidth, &ImageHeight, iNULL);
		//DesRect.X = 162;
		//DesRect.Y = 55;
		//DesRect.Width = ImageWidth;
		//DesRect.Height = ImageHeight;
		//mwLevelShowNum(mwContainer,NumSmall,CurrDuckWin,283,58);
		mwLevelShowNum(mwContainer,NumSmall,CurrDuckWin,DuckAward_X,DuckAward_Y);


		////i51KitG2GetImageAttr(mwContainer, WinWater, &ImageWidth, &ImageHeight, iNULL);
		//DesRect.X = 162;
		//DesRect.Y = 73;
		//DesRect.Width = ImageWidth;
		//DesRect.Height = ImageHeight;
		//mwLevelShowNum(mwContainer,NumSmall,CurrWaterWin,283,76);
		mwLevelShowNum(mwContainer,NumSmall,CurrWaterWin,WaterAward_X,WaterAward_Y);

		////i51KitG2GetImageAttr(mwContainer, WinLine, &ImageWidth, &ImageHeight, iNULL);
		//DesRect.X = 160;
		//DesRect.Y = 91;
		//DesRect.Width = ImageWidth;
		//DesRect.Height = ImageHeight;

		////i51KitG2GetImageAttr(mwContainer, WinNun, &ImageWidth, &ImageHeight, iNULL);
		//DesRect.X = 167;
		//DesRect.Y = 97;
		//DesRect.Width = ImageWidth;
		//DesRect.Height = ImageHeight;
		//mwLevelShowNum(mwContainer,NumBig,CurrTotalWin,279,99);
		mwLevelShowNum(mwContainer,NumBig,CurrTotalWin,Mark_X,Mark_Y);
	}
	////i51KitG2GetImageAttr(mwContainer, WinCrocodile, &ImageWidth, &ImageHeight, iNULL);
	//DesRect.X = 0;
	//DesRect.Y = 255;
	//DesRect.Width = ImageWidth;
	//DesRect.Height = ImageHeight;


	//if( LevelErrFlag )
	//{
	//	if( iFALSE == DrawBlankAlpha)
	//	{
	//		DrawBlankAlpha = iTRUE;
	//		//i51KitG2SetAlpha(128);
	//		LocalG2AlphaSys = 128;
	//		//i51KitG2CleanScreen(0X0, iNULL);
	//		AlphaNeed = iTRUE;
	//		AlphaSrc = 128;

	//		i51KitG2CleanScreenEx(0X0, iNULL);
	//		//i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);

	//		LocalG2AlphaSys = i51KITG2_ALPHA_NONEED;
	//	}
	//	if( mwPublicDrawPrompt(LevelErrStartTime, Prompt_Time, 0) )
	//	{
	//		LevelErrStartTime = 0;
	//		LevelErrFlag = iFALSE;
	//		Flag = iFALSE;
	//		DrawBlankAlpha = iFALSE;
	//	}
	//	return iTRUE;
	//}


	if(mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
	{
		PenDownX = mwKeyData.u.pen.x;
		PenDownY = mwKeyData.u.pen.y;

		////i51KitG2GetImageAttr(mwContainer, WinAgain, &ImageWidth, &ImageHeight, iNULL);

		ImageWidth = RESULT_AGAIN_W;
		ImageHeight = RESULT_AGAIN_H;
		if( PenDownX>=RESULT_AGAIN_X&&PenDownX<(RESULT_AGAIN_X+ImageWidth)
			&&PenDownY>=RESULT_AGAIN_Y&&PenDownY<(RESULT_AGAIN_Y+ImageHeight))
		{
			DesRect.X = RESULT_AGAIN_X;
			DesRect.Y = RESULT_AGAIN_Y;
			DesRect.Width= ImageWidth;
			DesRect.Height= ImageHeight;
			SrcRect.X = 0;
			SrcRect.Y = RESULT_AGAIN_H;
			SrcRect.Width = RESULT_AGAIN_W;
			SrcRect.Height = RESULT_AGAIN_H;
			SWMW_i51KitG2DrawImage(&iMageInfo[WinAgain], &DesRect, &SrcRect);

			mwWordPause = iFALSE;
			CurrLevelState = MW_LEVEL_INIT;
			wmw_destroy_result_page();
			mwLevelResultFlag = iFALSE;
			iScreenFlag = iTRUE;
			public_audio_stop(AUDIO_PLAYER_MP3);
			SoundWinPlay = iFALSE;
		}

		ImageWidth = RESULT_NEXT_W;
		ImageHeight = RESULT_NEXT_H;

		////i51KitG2GetImageAttr(mwContainer, WinNext, &ImageWidth, &ImageHeight, iNULL);
		if( PenDownX>=RESULT_NEXT_X&&PenDownX<(RESULT_NEXT_X+ImageWidth)
			&&PenDownY>=RESULT_NEXT_Y&&PenDownY<(RESULT_NEXT_Y+ImageHeight))
		{
			DesRect.X = RESULT_NEXT_X;
			DesRect.Y = RESULT_NEXT_Y;
			DesRect.Width= ImageWidth;
			DesRect.Height= ImageHeight;
			SrcRect.X = 0;
			SrcRect.Y = RESULT_NEXT_H * 3;
			SrcRect.Width = RESULT_NEXT_W;
			SrcRect.Height = RESULT_NEXT_H;

			SWMW_i51KitG2DrawImage(&iMageInfo[WinAgain], &DesRect, &SrcRect);

			mwLevelResultFlag = iFALSE;

			iScreenFlag= iTRUE;
			if( CurrLevelNum>=MW_LEVEL_MAX)
				//			if( CurrLevelNum>=UnlockMaxLevel)
			{

				wmw_portal_ui();

				////DrawBlankAlpha = iTRUE;
				////i51KitG2SetAlpha(128);
				//LocalG2AlphaSys = 128;
				////i51KitG2CleanScreen(0X0, iNULL);
				//AlphaNeed = iTRUE;
				//AlphaSrc = 128;

				//i51KitG2CleanScreenEx(0X0, iNULL);
				////i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);

				//LocalG2AlphaSys = i51KITG2_ALPHA_NONEED;

				//wmw_image_destroy();
				//wmw_partol_page();
				//CurrGameState = GAME_STATE_PARTOL;

				//LevelErrStartTime = i51AdeOsGetTick();
				//LevelErrFlag = iTRUE;
			}
			else
			{
				mwLevelUnsetImageMem(level);
				mwWordPause = iFALSE;
				CurrLevelState = MW_LEVEL_INIT;
				wmw_destroy_result_page();
				CurrLevelNum = (mwLevelNum)((int)CurrLevelNum + 1);
				public_audio_stop(AUDIO_PLAYER_MP3);
				SoundWinPlay = iFALSE;
			}
		}
		ImageWidth = RESULT_CHOOSE_W;
		ImageHeight = RESULT_CHOOSE_H;

		////i51KitG2GetImageAttr(mwContainer, WinChoose, &ImageWidth, &ImageHeight, iNULL);
		if( PenDownX>=RESULT_CHOOSE_X&&PenDownX<(RESULT_CHOOSE_X+ImageWidth)
			&&PenDownY>=RESULT_CHOOSE_Y&&PenDownY<(RESULT_CHOOSE_Y+ImageHeight))
		{
			DesRect.X = RESULT_CHOOSE_X;
			DesRect.Y = RESULT_CHOOSE_Y;
			DesRect.Width= ImageWidth;
			DesRect.Height= ImageHeight;
			SrcRect.X = 0;
			SrcRect.Y = RESULT_CHOOSE_H * 3;
			SrcRect.Width = RESULT_CHOOSE_W;
			SrcRect.Height = RESULT_CHOOSE_H;

			SWMW_i51KitG2DrawImage(&iMageInfo[PsMain], &DesRect, &SrcRect);

			mwWordPause = iTRUE;
			////xie
			//i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL );
			wmw_image_destroy();
			wmw_choose_page();
			CurrLevelState = MW_LEVEL_INIT;
			CurrLevelNum = MW_LEVEL_CHOOSE;
			mwLevelExit(level);
			//i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL);
			mwLevelResultFlag = iFALSE;
			iScreenFlag = iTRUE;
			public_audio_stop(AUDIO_PLAYER_MP3);
			SoundWinPlay = iFALSE;
		}
	}
	return iTRUE;
}

iBOOL mwDrawNumStep( iHANDLE Container, iU32 ImageID, iRECT * DesRect, iU32 Number1, iU32 Number2 )
{
	iU32 ImageWidth = 0;
	iU32 ImageHeight = 0;
	iU32 OneNumHeight = 0;
	iU32 TempNum = 1;
	iRECT TempDesRect;
	iRECT TempSrcRect;


	if( Number1>999 || Number2>999 || iNULL==DesRect) return iFALSE;

	SWMW_i51KitG2DrawImage(&iMageInfo[NumFrame], DesRect, iNULL);
	
	//i51KitG2GetImageAttr(Container, ImageID, &ImageWidth, &ImageHeight, iNULL);

	if (ImageID == NumBig)
	{
		i51KitG2GetImageAttrEx((iU32)NumBig_Hex, &ImageWidth, &ImageHeight);
	}
	else if (ImageID == NumSmall)
	{
		i51KitG2GetImageAttrEx((iU32)NumSmall_Hex, &ImageWidth, &ImageHeight);
	}
	OneNumHeight = ImageHeight/12;

	TempSrcRect.X = 0;
	TempSrcRect.Y = 0;
	TempSrcRect.Width = ImageWidth;
	TempSrcRect.Height= OneNumHeight;

	TempDesRect.X = DesRect->X+8;
	TempDesRect.Y = DesRect->Y+8;
	TempDesRect.Width= DesRect->Width;
	TempDesRect.Height= DesRect->Height;

	if( Number1<10 )
	{
		TempSrcRect.Y = OneNumHeight*Number1;
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);
		TempDesRect.X += ImageWidth;
	}
	else if(Number1<100 )
	{
//iLog("Number1:%d",Number1);
//iLog("TempSrcRect:X=%d,Y=%d,W=%d,H=%d",TempSrcRect.X,TempSrcRect.Y,TempSrcRect.Width,TempSrcRect.Height);
		TempSrcRect.Y = OneNumHeight*(Number1/10);
//iLog("TempSrcRect:X=%d,Y=%d,W=%d,H=%d",TempSrcRect.X,TempSrcRect.Y,TempSrcRect.Width,TempSrcRect.Height);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);
		TempDesRect.X += ImageWidth;
		TempSrcRect.Y = OneNumHeight*(Number1%10);
//iLog("TempSrcRect:X=%d,Y=%d,W=%d,H=%d",TempSrcRect.X,TempSrcRect.Y,TempSrcRect.Width,TempSrcRect.Height);
		//SWMW_i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);

		TempDesRect.X += ImageWidth;
	}
	else
	{
//iLog("Number1:%d",Number1);
		TempSrcRect.Y = OneNumHeight*(Number1/100);
		//SWMW_i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);

		TempDesRect.X += ImageWidth;
		TempSrcRect.Y = OneNumHeight*((Number1%100)/10);
		//SWMW_i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);

		TempDesRect.X += ImageWidth;
		TempSrcRect.Y = OneNumHeight*((Number1%10));
		//SWMW_i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);

		TempDesRect.X += ImageWidth;
	}
//iLog("Draw Num1");

	//	画反斜杠
	TempSrcRect.Y = OneNumHeight*10;
	//SWMW_i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
	SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);

	TempDesRect.X += ImageWidth;
//iLog("Draw //");

	if( Number2<10 )
	{
		TempSrcRect.Y = OneNumHeight*Number2;
		//SWMW_i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);

	}
	else if(Number2<100 )
	{
		TempSrcRect.Y = OneNumHeight*(Number2/10);
		//SWMW_i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);

		TempDesRect.X += ImageWidth;
		TempSrcRect.Y = OneNumHeight*(Number2%10);
		//SWMW_i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);

	}
	else
	{
		TempSrcRect.Y = OneNumHeight*(Number2/100);
		//SWMW_i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);

		TempDesRect.X += ImageWidth;
		TempSrcRect.Y = OneNumHeight*((Number2%100)/10);
		//SWMW_i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);

		TempDesRect.X += ImageWidth;
		TempSrcRect.Y = OneNumHeight*((Number2%10));
		//SWMW_i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], &TempDesRect, &TempSrcRect);

	}

	return iTRUE;
}

iBOOL mwLevelDrawRestart( void )
{
	iRECT DesRect;
	iRECT SrcRect;

	DesRect.X = GAME_RESTART_X;
	DesRect.Y = GAME_RESTART_Y;
	DesRect.Width= GAME_RESTART_W;
	DesRect.Height= GAME_RESTART_H;
	SrcRect.X = 0;
	SrcRect.Y = 0;
	SrcRect.Width = GAME_RESTART_W;
	SrcRect.Height = GAME_RESTART_W;
	SWMW_i51KitG2DrawImage(&iMageInfo[Restart], &DesRect, &SrcRect);
	return iTRUE;
}

iBOOL mwLevelPressRestart( iU16 X, iU16 Y)
{
	iRECT DesRect;
	iRECT SrcRect;

	if( X>=GAME_RESTART_X&&X<(GAME_RESTART_X+GAME_RESTART_W)
	&&Y>=GAME_RESTART_Y&&Y<(GAME_RESTART_Y+GAME_RESTART_H))
	{
		DesRect.X = GAME_RESTART_X;
		DesRect.Y = GAME_RESTART_Y;
		DesRect.Width= GAME_RESTART_W;
		DesRect.Height= GAME_RESTART_H;
		SrcRect.X = 0;
		SrcRect.Y = GAME_RESTART_H;
		SrcRect.Width = GAME_RESTART_W;
		SrcRect.Height = GAME_RESTART_H;

		SWMW_i51KitG2DrawImage(&iMageInfo[Restart], &DesRect, &SrcRect);

		WaterInBathtub = 0;
		return iTRUE;
	}
	return iFALSE;
}

iBOOL mwLevelDrawPause( void )
{
	iRECT DesRect;
	iRECT SrcRect;

	DesRect.X = GAME_PAUSE_X;
	DesRect.Y = GAME_PAUSE_Y;
	DesRect.Width= GAME_PAUSE_W;
	DesRect.Height= GAME_PAUSE_H;

	SrcRect.X = 0;
	SrcRect.Y = 0;
	SrcRect.Width = GAME_PAUSE_W;
	SrcRect.Height = GAME_PAUSE_H;
	SWMW_i51KitG2DrawImage(&iMageInfo[Pause], &DesRect, &SrcRect);

	return iTRUE;
}

iBOOL mwLevelPressPause( iU16 X, iU16 Y)
{
	iRECT DesRect;
	iRECT SrcRect;

	if( X>=GAME_PAUSE_X&&X<(GAME_PAUSE_X+GAME_PAUSE_W)
	&&Y>=GAME_PAUSE_Y&&Y<(GAME_PAUSE_Y+GAME_PAUSE_H))
	{
		DesRect.X = GAME_PAUSE_X;
		DesRect.Y = GAME_PAUSE_Y;
		DesRect.Width= GAME_PAUSE_W;
		DesRect.Height= GAME_PAUSE_H;
		SrcRect.X = 0;
		SrcRect.Y = GAME_PAUSE_H;
		SrcRect.Width = GAME_PAUSE_W;
		SrcRect.Height = GAME_PAUSE_H;

		SWMW_i51KitG2DrawImage(&iMageInfo[Pause], &DesRect, &SrcRect);

		return iTRUE;
	}
	return iFALSE;
}

iBOOL mwLevelDrawBKwall( void )
{
#if defined(PKIG_SCREEN_240X320)
	iS16 BKwall_H = 40;
#elif defined(PKIG_SCREEN_240X400)	
	iS16 BKwall_H = 50;
#elif defined(PKIG_SCREEN_320X480)	
	iS16 BKwall_H = 60;
#endif
	iRECT DesRect;
	DesRect.X = 0;
	DesRect.Y =0;
	DesRect.Width = SCREEN_WIDTH;
	DesRect.Height = BKwall_H;

	while(DesRect.Y<SCREEN_HEIGHT)
	{
		SWMW_i51KitG2DrawImage(&iMageInfo[BKwall], &DesRect, iNULL);
		DesRect.Y += BKwall_H;
	}

	return iTRUE;
}
iBOOL mwLevelDrawReturn( void )
{
	iRECT DesRect;
	iRECT SrcRect;
	DesRect.X = GAME_RETURN_X;
	DesRect.Y = GAME_RETURN_Y;
	DesRect.Width= GAME_RETURN_W;
	DesRect.Height= GAME_RETURN_H;

	SrcRect.X = 0;
	SrcRect.Y = 0;
	SrcRect.Width = GAME_RETURN_W;
	SrcRect.Height = GAME_RETURN_H;

	//SWMW_i51KitG2DrawImage(mwContainer, Return, &DesRect, &SrcRect);

	SWMW_i51KitG2DrawImage(&iMageInfo[Return], &DesRect, &SrcRect);

	return iTRUE;
}

iBOOL mwLevelPressReturn( iU16 X, iU16 Y)
{
	iRECT DesRect;
	iRECT SrcRect;
	if( X>=GAME_RETURN_X&&X<(GAME_RETURN_X+GAME_RETURN_W)
		&&Y>=GAME_RETURN_Y&&Y<(GAME_RETURN_Y+GAME_RETURN_H))
	{
		DesRect.X = GAME_RETURN_X;
		DesRect.Y = GAME_RETURN_Y;
		DesRect.Width= GAME_RETURN_W;
		DesRect.Height= GAME_RETURN_H;

		SrcRect.X = 0;
		SrcRect.Y = GAME_RETURN_H;
		SrcRect.Width = GAME_RETURN_W;
		SrcRect.Height = GAME_RETURN_H;

		//SWMW_i51KitG2DrawImage(mwContainer, Return, &DesRect, iNULL);

		SWMW_i51KitG2DrawImage(&iMageInfo[Return], &DesRect, &SrcRect);

		return iTRUE;
	}
	return iFALSE;
}

#ifdef  PKIG_SCREEN_240X320
	#define WORD_LEVEL_WIDTH	160
	#define WORD_LEVEL_HEIGHT	17
	#define STED_NUM_WIDTH	13
	#define STED_NUM_HEIGHT	13

#elif defined PKIG_SCREEN_240X400

	#define WORD_LEVEL_WIDTH	129
	#define WORD_LEVEL_HEIGHT	17
	#define STED_NUM_WIDTH	13
	#define STED_NUM_HEIGHT	13

#elif defined PKIG_SCREEN_320X480

#ifdef  LANGUAGE_ENGLISH
	#define WORD_LEVEL_WIDTH	240

#else
#define WORD_LEVEL_WIDTH	164

#endif

#define WORD_LEVEL_HEIGHT	22
#define STED_NUM_WIDTH	16
#define STED_NUM_HEIGHT	16

#endif

//#ifdef  LANGUAGE_ENGLISH
//#define WORD_LEVEL_WIDTH	240
//
//#else
//#define WORD_LEVEL_WIDTH	164
//
//#endif
//#define WORD_LEVEL_HEIGHT	22
//#define STED_NUM_WIDTH	16
//#define STED_NUM_HEIGHT	16

iBOOL mwLevelDrawPrompt( mwLevelNum level )
{
	iRECT RectDesLevel;
	iRECT RectDesPrompt;
	iRECT RectDesStep;
	iRECT RectDesBK;
	iRECT RectSrc;
	//iU32 WordLevelNum[15] = {
	//		WordLevel001,WordLevel002,WordLevel003,WordLevel004,WordLevel005,
	//		WordLevel006,WordLevel007,WordLevel008,WordLevel009,WordLevel010,
	//		WordLevel011,WordLevel012,WordLevel013,WordLevel014,WordLevel015,
	//	};

#ifdef PKIG_SCREEN_240X320
	RectDesLevel.X = 5;
	RectDesLevel.Y = 298;
	RectDesLevel.Width = WORD_LEVEL_WIDTH;
	RectDesLevel.Height = WORD_LEVEL_HEIGHT;

	RectDesStep.X = 70;
	RectDesStep.Y = 300;
	RectDesStep.Width = 16;
	RectDesStep.Height = 16;

	RectDesPrompt.X = 40;
	RectDesPrompt.Y = 125;
	RectDesPrompt.Width = WORD_LEVEL_WIDTH;
	RectDesPrompt.Height = WORD_LEVEL_HEIGHT;

	RectDesBK.X = 15;
	RectDesBK.Y = 117;
	RectDesBK.Width = 212;
	RectDesBK.Height = 34;

#elif defined  PKIG_SCREEN_240X400
	RectDesLevel.X = 5;
	RectDesLevel.Y = 373;
	RectDesLevel.Width = WORD_LEVEL_WIDTH;
	RectDesLevel.Height = WORD_LEVEL_HEIGHT;

	RectDesStep.X = 70;
	RectDesStep.Y = 373;
	RectDesStep.Width = 16;
	RectDesStep.Height = 19;

	RectDesPrompt.X = 56;
	RectDesPrompt.Y = 152;
	RectDesPrompt.Width = WORD_LEVEL_WIDTH;
	RectDesPrompt.Height = WORD_LEVEL_HEIGHT;

	RectDesBK.X = 14;
	RectDesBK.Y = 146;
	RectDesBK.Width = 212;
	RectDesBK.Height = 34;

#elif defined PKIG_SCREEN_320X480
	RectDesLevel.X = 0;
	RectDesLevel.Y = 449;
	RectDesLevel.Width = WORD_LEVEL_WIDTH;
	RectDesLevel.Height = WORD_LEVEL_HEIGHT;

	RectDesStep.X = 70;
	RectDesStep.Y = 455;
	RectDesStep.Width = 16;
	RectDesStep.Height = 19;
#ifdef LANGUAGE_ENGLISH
	RectDesPrompt.X = 40;
#else
	RectDesPrompt.X = 78;
#endif
	RectDesPrompt.Y = 180;
	RectDesPrompt.Width = WORD_LEVEL_WIDTH;
	RectDesPrompt.Height = WORD_LEVEL_HEIGHT;

	RectDesBK.X = 24;
	RectDesBK.Y = 168;
	RectDesBK.Width = 272;
	RectDesBK.Height = 48;

#endif
	//i51KitG2SetAlpha(128);
	LocalG2AlphaSys = 128;

	SWMW_i51KitG2DrawImage(&iMageInfo[BlackBK], &RectDesBK, iNULL);
	//i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
	LocalG2AlphaSys = i51KITG2_ALPHA_NONEED;

	
	RectSrc.X = 0;
	RectSrc.Y = 0;
	RectSrc.Width = WORD_LEVEL_WIDTH;
	RectSrc.Height = WORD_LEVEL_HEIGHT;
	SWMW_i51KitG2DrawImage(&iMageInfo[WordLevel], &RectDesLevel, iNULL);


	RectSrc.Width = STED_NUM_WIDTH;
	RectSrc.Height = STED_NUM_HEIGHT;
	RectSrc.Y = 0;
	RectSrc.X = STED_NUM_WIDTH;
	SWMW_i51KitG2DrawImage(&iMageInfo[LevelNum], &RectDesStep, &RectSrc);

	RectSrc.X = 0;
	RectSrc.Y = WORD_LEVEL_HEIGHT * level;
	RectSrc.Width = WORD_LEVEL_WIDTH;
	RectSrc.Height = WORD_LEVEL_HEIGHT;

	SWMW_i51KitG2DrawImage(&iMageInfo[WordLevel], &RectDesPrompt, &RectSrc);

	RectDesStep.X += STED_NUM_WIDTH;
	
	RectSrc.X = STED_NUM_WIDTH*10;
	RectSrc.Y = 0;
	RectSrc.Width = STED_NUM_WIDTH;
	RectSrc.Height = STED_NUM_HEIGHT;

	SWMW_i51KitG2DrawImage(&iMageInfo[LevelNum], &RectDesStep, &RectSrc);
	RectDesStep.X += STED_NUM_WIDTH;

	if(level>9)
	{
		RectSrc.X = STED_NUM_WIDTH;
		SWMW_i51KitG2DrawImage(&iMageInfo[LevelNum], &RectDesStep, &RectSrc);
		RectDesStep.X += STED_NUM_WIDTH;
		RectSrc.X = STED_NUM_WIDTH*(level-10);
		SWMW_i51KitG2DrawImage(&iMageInfo[LevelNum], &RectDesStep, &RectSrc);
	}
	else
	{
		RectSrc.X = STED_NUM_WIDTH*level;
		SWMW_i51KitG2DrawImage(&iMageInfo[LevelNum], &RectDesStep, &RectSrc);
	}
	return iTRUE;
}

iBOOL mwLevelDuckInit(mwLevelNum level )
{
#if defined(PKIG_SCREEN_240X320)
	iS16 Duck_X[MW_LEVEL_MAX][DUCK_NUM] = {
		{106, 106, 106},
		{112, 116, 117},
		{32, 92, 120},
		{131, 164, 199},
		//{191, 198, 196},	
		{56, 104, 152},
		//{10, 25, 124},	
		//{7, 26, 136},	
		{37, 149, 170},
		//{22, 68, 187},	
		{19, 29, 183},	
		//{71,103, 147},
		{19, 102, 197},
		//{74, 84, 157},	
		//{24, 151, 181}
	};
	iS16 Duck_Y[MW_LEVEL_MAX][DUCK_NUM] = { 
		{65, 101, 132},
		{94, 143, 192},
		{185, 185, 223},
		{140, 164, 198},
		//{56, 111, 184},
		{195, 129, 195},
		//{248, 133, 152},
		//{129, 230, 157},
		{155, 202, 94},
		//{253, 195, 32},
		{179, 52, 179},
		//{30, 195, 30},
		{247, 151, 110},
		//{72, 207, 149},
		//{172, 47, 98}
	};

	iS16 Duck_W = 36;
	iS16 Duck_H = 36;

#elif defined(PKIG_SCREEN_240X400)	
	iS16 Duck_X[MW_LEVEL_MAX][DUCK_NUM] = {
		{111, 112, 114},
		{119, 122, 125},
		{39, 98, 126},
		{133, 180, 209},	
		//{203, 210, 209},
		{65, 111, 161},
		//{15, 31, 131},
		//{11, 33, 142},	
		{42, 157, 180},
		//{27, 77, 194},	
		{26, 32, 189},	
		//{77, 110, 155},
		{24, 115, 205},	
		//{79, 89, 163},
		//{31, 156, 188}
	};
	iS16 Duck_Y[MW_LEVEL_MAX][DUCK_NUM] = { 
		{92, 135, 176},
		{126, 191, 251},
		{243, 243, 290},
		{195, 200, 259},
		//{81, 152, 240},
		{255, 167, 255},
		//{321, 176, 202},
		//{170, 300, 204},
		{202, 270, 128},
		//{325, 251, 48},
		{232, 84, 232},
		//{47, 254, 47},
		{320, 198, 149},	
		//01, 269, 197},
		//25, 77, 132}
	};
	iS16 Duck_W = 21;
	iS16 Duck_H = 21;

#elif defined(PKIG_SCREEN_320X480)
	iS16 Duck_X[MW_LEVEL_MAX][DUCK_NUM] = {
			{137,139,137},
			{147,151,154},
			{33,122,165},
			{166,227,264},	
			//{251,259,259},
			{136,75,204},
			//{30,10,163},
			//{6,32,177}, 
			{46,199,234},
			//{246,94,29},
			{26,244,32},
			//{92,196,136},
			{22,134,260},
			//{95,207,109},
			//{199,240,30}
		};
	iS16 Duck_Y[MW_LEVEL_MAX][DUCK_NUM] = { 
			{100,149,198},
			{139,212,284},
			{264,264,329},
			{221,210,297},
			//{81,163,273},	
			{188,294,293},
			//{198,368,227},
			//{190,345,231},
			{229,310,148},
			//{47,290,378},
			{273,270,101},
			//{42,42,293},
			{370,226,166},
			//{107,219,307},
			//{68,146,258}
		};

	iS16 Duck_W = 53;
	iS16 Duck_H = 53;
#endif

	int  i = 0;

	for( i=0; i<DUCK_NUM; i++ )
	{
		DuckMess[i].WaterNum = 0;
		DuckMess[i].OldRushTime = 0;
		DuckMess[i].DuckExist = iTRUE;
		DuckMess[i].Rect.X = Duck_X[level-1][i];
		DuckMess[i].Rect.Y = Duck_Y[level-1][i];
		DuckMess[i].Rect.Width= Duck_W;
		DuckMess[i].Rect.Height= Duck_H;
	}

	Duck_Manage.ResidueDuckNum = DUCK_NUM;
	Duck_Manage.TotalDuckNum = DUCK_NUM;
	Duck_Manage.DuckMess = (DuckData * )(&DuckMess);

	return iTRUE;
}

iBOOL mwLevelPipeInit( mwLevelNum level  )
{
#if defined(PKIG_SCREEN_240X320)
	if( level==MW_LEVEL_001 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 110;
		PipeMess[0].RectWaterIn.Y = 175 ;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 106;
		PipeMess[0].WaterOutY =244;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_002 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 120;
		PipeMess[0].RectWaterIn.Y = 235;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 39;
		PipeMess[0].WaterOutY = 253;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_003 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 30;
		PipeMess[0].RectWaterIn.Y = 268;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 177;
		PipeMess[0].WaterOutY = 252;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_004 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 210;
		PipeMess[0].RectWaterIn.Y = 250;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 37;
		PipeMess[0].WaterOutY = 250;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_005 )
	{
	/*
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 77;
		PipeMess[0].RectWaterIn.Y = 261;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 39;
		PipeMess[0].WaterOutY = 178;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 66;
		PipeMess[1].RectWaterIn.Y = 25;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 40;
		PipeMess[1].WaterOutX = PipeOutX_HOLD;
		PipeMess[1].WaterOutY = PipeOutY_HOLD;
		PipeMess[1].SpeedX = PipeSpeedX_HOLD;
		PipeMess[1].SpeedY = PipeSpeedY_HOLD;

		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	*/

		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 100;
		PipeMess[0].RectWaterIn.Y = 285;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 175;
		PipeMess[0].WaterOutY = 256;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);

	}
	else if( level==MW_LEVEL_006 )
	{
	/*
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 100;
		PipeMess[0].RectWaterIn.Y = 285;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 175;
		PipeMess[0].WaterOutY = 256;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	*/
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 160;
		PipeMess[0].RectWaterIn.Y = 278;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 72;
		PipeMess[0].WaterOutY = 253;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);

	}		
	else if( level==MW_LEVEL_007 )
	{
	/*
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 190;
		PipeMess[0].RectWaterIn.Y = 175;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height =20;
		PipeMess[0].WaterOutX = 155;
		PipeMess[0].WaterOutY = 255;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	*/

		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 110;
		PipeMess[0].RectWaterIn.Y = 205;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 106;
		PipeMess[0].WaterOutY = 257;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 55;
		PipeMess[1].RectWaterIn.Y = 165;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 100;
		PipeMess[1].WaterOutY = 144;
		PipeMess[1].SpeedX = 23*COORDINATE_RATE;
		PipeMess[1].SpeedY = -1*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 140;
		PipeMess[2].RectWaterIn.Y = 138;
		PipeMess[2].RectWaterIn.Width = 20;
		PipeMess[2].RectWaterIn.Height = 20;
		PipeMess[2].WaterOutX = 178;
		PipeMess[2].WaterOutY = 174;
		PipeMess[2].SpeedX = 23*COORDINATE_RATE;
		PipeMess[2].SpeedY = 0*COORDINATE_RATE;

		PipeMess[3].ValidityFlag = iTRUE;
		PipeMess[3].RectWaterIn.X = 215;
		PipeMess[3].RectWaterIn.Y = 165;
		PipeMess[3].RectWaterIn.Width = 20;
		PipeMess[3].RectWaterIn.Height = 20;
		PipeMess[3].WaterOutX = 500;
		PipeMess[3].WaterOutY = 500;
		PipeMess[3].SpeedX = 23*COORDINATE_RATE;
		PipeMess[3].SpeedY = -2*COORDINATE_RATE;

		PipeMess[4].ValidityFlag = iTRUE;
		PipeMess[4].RectWaterIn.X = 135;
		PipeMess[4].RectWaterIn.Y = 120;
		PipeMess[4].RectWaterIn.Width = 20;
		PipeMess[4].RectWaterIn.Height = 10;
		PipeMess[4].WaterOutX = 70;
		PipeMess[4].WaterOutY = 90;
		PipeMess[4].SpeedX = -4*COORDINATE_RATE;
		PipeMess[4].SpeedY = -8*COORDINATE_RATE;

		PipeMess[5].ValidityFlag = iTRUE;
		PipeMess[5].RectWaterIn.X = 20;
		PipeMess[5].RectWaterIn.Y = 245;
		PipeMess[5].RectWaterIn.Width = 20;
		PipeMess[5].RectWaterIn.Height = 20;
		PipeMess[5].WaterOutX = 100;
		PipeMess[5].WaterOutY = 200;
		PipeMess[5].SpeedX = 10*COORDINATE_RATE;
		PipeMess[5].SpeedY = -25*COORDINATE_RATE;

		PipeMess[6].ValidityFlag = iTRUE;
		PipeMess[6].RectWaterIn.X = 190;
		PipeMess[6].RectWaterIn.Y = 240;
		PipeMess[6].RectWaterIn.Width = 20;
		PipeMess[6].RectWaterIn.Height = 20;
		PipeMess[6].WaterOutX = 138;
		PipeMess[6].WaterOutY = 187;
		PipeMess[6].SpeedX = -8*COORDINATE_RATE;
		PipeMess[6].SpeedY = -25*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 7;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);

	}
	else if( level==MW_LEVEL_008 )
	{
	/*
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 60;
		PipeMess[0].RectWaterIn.Y = 280;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 174;
		PipeMess[0].WaterOutY = 253;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	*/
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 205;
		PipeMess[0].RectWaterIn.Y = 265;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 109;
		PipeMess[0].WaterOutY = 250;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 184;
		PipeMess[1].RectWaterIn.Y = 130;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 118;
		PipeMess[1].WaterOutY = 160;
		PipeMess[1].SpeedX = 0;
		PipeMess[1].SpeedY = 20*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 70;
		PipeMess[2].RectWaterIn.Y = 145;
		PipeMess[2].RectWaterIn.Width = 30;
		PipeMess[2].RectWaterIn.Height = 50;
		PipeMess[2].WaterOutX = PipeOutX_HOLD;
		PipeMess[2].WaterOutY = PipeOutY_HOLD;
		PipeMess[2].SpeedX = 12*COORDINATE_RATE;
		PipeMess[2].SpeedY = PipeSpeedY_HOLD;
		Pipe_Manage.PipeNum = 3;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);	

	}
	else if( level==MW_LEVEL_009 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 160;
		PipeMess[0].RectWaterIn.Y = 278;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 72;
		PipeMess[0].WaterOutY = 253;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_010 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 198;
		PipeMess[0].RectWaterIn.Y = 266;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 175;
		PipeMess[0].WaterOutY = 116;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_011 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 110;
		PipeMess[0].RectWaterIn.Y = 205;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 106;
		PipeMess[0].WaterOutY = 257;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 55;
		PipeMess[1].RectWaterIn.Y = 165;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 100;
		PipeMess[1].WaterOutY = 144;
		PipeMess[1].SpeedX = 23*COORDINATE_RATE;
		PipeMess[1].SpeedY = -1*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 140;
		PipeMess[2].RectWaterIn.Y = 138;
		PipeMess[2].RectWaterIn.Width = 20;
		PipeMess[2].RectWaterIn.Height = 20;
		PipeMess[2].WaterOutX = 178;
		PipeMess[2].WaterOutY = 174;
		PipeMess[2].SpeedX = 23*COORDINATE_RATE;
		PipeMess[2].SpeedY = 0*COORDINATE_RATE;

		PipeMess[3].ValidityFlag = iTRUE;
		PipeMess[3].RectWaterIn.X = 215;
		PipeMess[3].RectWaterIn.Y = 165;
		PipeMess[3].RectWaterIn.Width = 20;
		PipeMess[3].RectWaterIn.Height = 20;
		PipeMess[3].WaterOutX = 500;
		PipeMess[3].WaterOutY = 500;
		PipeMess[3].SpeedX = 23*COORDINATE_RATE;
		PipeMess[3].SpeedY = -2*COORDINATE_RATE;

		PipeMess[4].ValidityFlag = iTRUE;
		PipeMess[4].RectWaterIn.X = 135;
		PipeMess[4].RectWaterIn.Y = 120;
		PipeMess[4].RectWaterIn.Width = 20;
		PipeMess[4].RectWaterIn.Height = 10;
		PipeMess[4].WaterOutX = 70;
		PipeMess[4].WaterOutY = 90;
		PipeMess[4].SpeedX = -4*COORDINATE_RATE;
		PipeMess[4].SpeedY = -8*COORDINATE_RATE;

		PipeMess[5].ValidityFlag = iTRUE;
		PipeMess[5].RectWaterIn.X = 20;
		PipeMess[5].RectWaterIn.Y = 245;
		PipeMess[5].RectWaterIn.Width = 20;
		PipeMess[5].RectWaterIn.Height = 20;
		PipeMess[5].WaterOutX = 100;
		PipeMess[5].WaterOutY = 200;
		PipeMess[5].SpeedX = 10*COORDINATE_RATE;
		PipeMess[5].SpeedY = -25*COORDINATE_RATE;

		PipeMess[6].ValidityFlag = iTRUE;
		PipeMess[6].RectWaterIn.X = 190;
		PipeMess[6].RectWaterIn.Y = 240;
		PipeMess[6].RectWaterIn.Width = 20;
		PipeMess[6].RectWaterIn.Height = 20;
		PipeMess[6].WaterOutX = 138;
		PipeMess[6].WaterOutY = 187;
		PipeMess[6].SpeedX = -8*COORDINATE_RATE;
		PipeMess[6].SpeedY = -25*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 7;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_012 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 16;
		PipeMess[0].RectWaterIn.Y = 205;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 40;
		PipeMess[0].WaterOutY = 252;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 212;
		PipeMess[1].RectWaterIn.Y = 108;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 211;
		PipeMess[1].WaterOutY = 31;
		PipeMess[1].SpeedX = -10*COORDINATE_RATE;
		PipeMess[0].SpeedY = -2*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 2;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);	
	}
	else if( level==MW_LEVEL_013 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 205;
		PipeMess[0].RectWaterIn.Y = 265;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 109;
		PipeMess[0].WaterOutY = 250;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 184;
		PipeMess[1].RectWaterIn.Y = 135;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 115;
		PipeMess[1].WaterOutY = 150;
		PipeMess[1].SpeedX = 0;
		PipeMess[1].SpeedY = 20*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 70;
		PipeMess[2].RectWaterIn.Y = 145;
		PipeMess[2].RectWaterIn.Width = 30;
		PipeMess[2].RectWaterIn.Height = 50;
		PipeMess[2].WaterOutX = PipeOutX_HOLD;
		PipeMess[2].WaterOutY = PipeOutY_HOLD;
		PipeMess[2].SpeedX = 12*COORDINATE_RATE;
		PipeMess[2].SpeedY = PipeSpeedY_HOLD;
		Pipe_Manage.PipeNum = 3;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);	
	}
	else if( level==MW_LEVEL_014 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 175;
		PipeMess[0].RectWaterIn.Y = 115;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 175;
		PipeMess[0].WaterOutY = 47;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 43;
		PipeMess[1].RectWaterIn.Y = 297;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 130;
		PipeMess[1].WaterOutY = 220;
		PipeMess[1].SpeedX = -15*COORDINATE_RATE;
		PipeMess[1].SpeedY = -1*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 49;
		PipeMess[2].RectWaterIn.Y = 218;
		PipeMess[2].RectWaterIn.Width = 20;
		PipeMess[2].RectWaterIn.Height = 20;
		PipeMess[2].WaterOutX = 121;
		PipeMess[2].WaterOutY = 152;
		PipeMess[2].SpeedX = 15*COORDINATE_RATE;
		PipeMess[2].SpeedY = -3*COORDINATE_RATE;

		PipeMess[3].ValidityFlag = iTRUE;
		PipeMess[3].RectWaterIn.X = 195;
		PipeMess[3].RectWaterIn.Y = 165;
		PipeMess[3].RectWaterIn.Width = 20;
		PipeMess[3].RectWaterIn.Height = 20;
		PipeMess[3].WaterOutX = 21;
		PipeMess[3].WaterOutY = 75;
		PipeMess[3].SpeedX = 15*COORDINATE_RATE;
		PipeMess[3].SpeedY = -3*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 4;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_015 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 180;
		PipeMess[0].RectWaterIn.Y = 140;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 123;
		PipeMess[0].WaterOutY = 210;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 20;
		PipeMess[1].RectWaterIn.Y = 205;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 34;
		PipeMess[1].WaterOutY = 271;
		PipeMess[1].SpeedX = 0;
		PipeMess[1].SpeedY = 1*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 190;
		PipeMess[2].RectWaterIn.Y = 188;
		PipeMess[2].RectWaterIn.Width = 20;
		PipeMess[2].RectWaterIn.Height = 20;
		PipeMess[2].WaterOutX = 145;
		PipeMess[2].WaterOutY = 90;
		PipeMess[2].SpeedX = 3*COORDINATE_RATE;
		PipeMess[2].SpeedY = -15*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 3;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}

#elif defined(PKIG_SCREEN_240X400)	
	if( level==MW_LEVEL_001 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 110;
		PipeMess[0].RectWaterIn.Y = 225;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 106;
		PipeMess[0].WaterOutY =305;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_002 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 121;
		PipeMess[0].RectWaterIn.Y = 306;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 40;
		PipeMess[0].WaterOutY = 317;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_003 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 33;
		PipeMess[0].RectWaterIn.Y = 345;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 178;
		PipeMess[0].WaterOutY = 316;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_004 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 212;
		PipeMess[0].RectWaterIn.Y = 321;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 36;
		PipeMess[0].WaterOutY = 313;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_005 )
	{
	/*
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 70;
		PipeMess[0].RectWaterIn.Y = 324;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 41;
		PipeMess[0].WaterOutY = 223;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 65;
		PipeMess[1].RectWaterIn.Y = 37;
		PipeMess[1].RectWaterIn.Width = 30;
		PipeMess[1].RectWaterIn.Height = 30;
		PipeMess[1].WaterOutX = PipeOutX_HOLD;
		PipeMess[1].WaterOutY = PipeOutY_HOLD;
		PipeMess[1].SpeedX = PipeSpeedX_HOLD;
		PipeMess[1].SpeedY = PipeSpeedY_HOLD;
		Pipe_Manage.PipeNum = 2;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	*/

		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 98;
		PipeMess[0].RectWaterIn.Y = 364;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 175;
		PipeMess[0].WaterOutY = 325;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);

	}
	else if( level==MW_LEVEL_006 )
	{
	/*
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 98;
		PipeMess[0].RectWaterIn.Y = 364;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 175;
		PipeMess[0].WaterOutY = 325;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	*/
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 156;
		PipeMess[0].RectWaterIn.Y = 345;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 72;
		PipeMess[0].WaterOutY = 316;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);

	}		
	else if( level==MW_LEVEL_007 )
	{
	/*
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 193;
		PipeMess[0].RectWaterIn.Y = 219;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 155;
		PipeMess[0].WaterOutY = 282;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	*/
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 107;
		PipeMess[0].RectWaterIn.Y = 260;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 106;
		PipeMess[0].WaterOutY = 321;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 60;
		PipeMess[1].RectWaterIn.Y = 210;
		PipeMess[1].RectWaterIn.Width = 15;
		PipeMess[1].RectWaterIn.Height = 15;
		PipeMess[1].WaterOutX = 94;
		PipeMess[1].WaterOutY = 181;
		PipeMess[1].SpeedX = 23*COORDINATE_RATE;
		PipeMess[1].SpeedY = -2*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 140;
		PipeMess[2].RectWaterIn.Y = 176;
		PipeMess[2].RectWaterIn.Width = 15;
		PipeMess[2].RectWaterIn.Height = 15;
		PipeMess[2].WaterOutX = 175;
		PipeMess[2].WaterOutY = 218;
		PipeMess[2].SpeedX = 23*COORDINATE_RATE;
		PipeMess[2].SpeedY = -2*COORDINATE_RATE;

		PipeMess[3].ValidityFlag = iTRUE;
		PipeMess[3].RectWaterIn.X = 220;
		PipeMess[3].RectWaterIn.Y = 215;
		PipeMess[3].RectWaterIn.Width = 15;
		PipeMess[3].RectWaterIn.Height = 15;
		PipeMess[3].WaterOutX = 500;
		PipeMess[3].WaterOutY = 500;
		PipeMess[3].SpeedX = 23*COORDINATE_RATE;
		PipeMess[3].SpeedY = -2*COORDINATE_RATE;

		PipeMess[4].ValidityFlag = iTRUE;
		PipeMess[4].RectWaterIn.X = 130;
		PipeMess[4].RectWaterIn.Y = 150;
		PipeMess[4].RectWaterIn.Width = 15;
		PipeMess[4].RectWaterIn.Height = 15;
		PipeMess[4].WaterOutX = 70;
		PipeMess[4].WaterOutY = 120;
		PipeMess[4].SpeedX = -4*COORDINATE_RATE;
		PipeMess[4].SpeedY = -8*COORDINATE_RATE;

		PipeMess[5].ValidityFlag = iTRUE;
		PipeMess[5].RectWaterIn.X = 25;
		PipeMess[5].RectWaterIn.Y = 310;
		PipeMess[5].RectWaterIn.Width = 20;
		PipeMess[5].RectWaterIn.Height = 20;
		PipeMess[5].WaterOutX = 100;
		PipeMess[5].WaterOutY = 250;
		PipeMess[5].SpeedX = 7*COORDINATE_RATE;
		PipeMess[5].SpeedY = -25*COORDINATE_RATE;

		PipeMess[6].ValidityFlag = iTRUE;
		PipeMess[6].RectWaterIn.X = 190;
		PipeMess[6].RectWaterIn.Y = 310;
		PipeMess[6].RectWaterIn.Width = 20;
		PipeMess[6].RectWaterIn.Height = 20;
		PipeMess[6].WaterOutX = 140;
		PipeMess[6].WaterOutY = 248;
		PipeMess[6].SpeedX = -10*COORDINATE_RATE;
		PipeMess[6].SpeedY = -25*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 7;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);

	}
	else if( level==MW_LEVEL_008 )
	{
	/*
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 55;
		PipeMess[0].RectWaterIn.Y = 362;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 176;
		PipeMess[0].WaterOutY = 317;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	*/

		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 200;
		PipeMess[0].RectWaterIn.Y = 325;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 110;
		PipeMess[0].WaterOutY = 313;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 180;
		PipeMess[1].RectWaterIn.Y = 167;
		PipeMess[1].RectWaterIn.Width = 30;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 122;
		PipeMess[1].WaterOutY = 200;
		PipeMess[1].SpeedX = 0;
		PipeMess[1].SpeedY = 20;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 67;
		PipeMess[2].RectWaterIn.Y = 186;
		PipeMess[2].RectWaterIn.Width = 40;
		PipeMess[2].RectWaterIn.Height = 60;
		PipeMess[2].WaterOutX = PipeOutX_HOLD;
		PipeMess[2].WaterOutY = PipeOutY_HOLD;
		PipeMess[2].SpeedX = 200;
		PipeMess[2].SpeedY = PipeSpeedY_HOLD;
		Pipe_Manage.PipeNum = 3;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);	
 
	}
	else if( level==MW_LEVEL_009 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 156;
		PipeMess[0].RectWaterIn.Y = 345;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 72;
		PipeMess[0].WaterOutY = 316;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_010 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 200;
		PipeMess[0].RectWaterIn.Y = 330;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 175;
		PipeMess[0].WaterOutY = 145;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_011 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 107;
		PipeMess[0].RectWaterIn.Y = 260;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 106;
		PipeMess[0].WaterOutY = 321;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 60;
		PipeMess[1].RectWaterIn.Y = 210;
		PipeMess[1].RectWaterIn.Width = 15;
		PipeMess[1].RectWaterIn.Height = 15;
		PipeMess[1].WaterOutX = 94;
		PipeMess[1].WaterOutY = 181;
		PipeMess[1].SpeedX = 23*COORDINATE_RATE;
		PipeMess[1].SpeedY = -2*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 140;
		PipeMess[2].RectWaterIn.Y = 176;
		PipeMess[2].RectWaterIn.Width = 15;
		PipeMess[2].RectWaterIn.Height = 15;
		PipeMess[2].WaterOutX = 175;
		PipeMess[2].WaterOutY = 218;
		PipeMess[2].SpeedX = 23*COORDINATE_RATE;
		PipeMess[2].SpeedY = -2*COORDINATE_RATE;

		PipeMess[3].ValidityFlag = iTRUE;
		PipeMess[3].RectWaterIn.X = 220;
		PipeMess[3].RectWaterIn.Y = 215;
		PipeMess[3].RectWaterIn.Width = 15;
		PipeMess[3].RectWaterIn.Height = 15;
		PipeMess[3].WaterOutX = 500;
		PipeMess[3].WaterOutY = 500;
		PipeMess[3].SpeedX = 23*COORDINATE_RATE;
		PipeMess[3].SpeedY = -2*COORDINATE_RATE;

		PipeMess[4].ValidityFlag = iTRUE;
		PipeMess[4].RectWaterIn.X = 130;
		PipeMess[4].RectWaterIn.Y = 150;
		PipeMess[4].RectWaterIn.Width = 15;
		PipeMess[4].RectWaterIn.Height = 15;
		PipeMess[4].WaterOutX = 70;
		PipeMess[4].WaterOutY = 120;
		PipeMess[4].SpeedX = -4*COORDINATE_RATE;
		PipeMess[4].SpeedY = -8*COORDINATE_RATE;

		PipeMess[5].ValidityFlag = iTRUE;
		PipeMess[5].RectWaterIn.X = 25;
		PipeMess[5].RectWaterIn.Y = 310;
		PipeMess[5].RectWaterIn.Width = 20;
		PipeMess[5].RectWaterIn.Height = 20;
		PipeMess[5].WaterOutX = 100;
		PipeMess[5].WaterOutY = 250;
		PipeMess[5].SpeedX = 7*COORDINATE_RATE;
		PipeMess[5].SpeedY = -25*COORDINATE_RATE;

		PipeMess[6].ValidityFlag = iTRUE;
		PipeMess[6].RectWaterIn.X = 190;
		PipeMess[6].RectWaterIn.Y = 310;
		PipeMess[6].RectWaterIn.Width = 20;
		PipeMess[6].RectWaterIn.Height = 20;
		PipeMess[6].WaterOutX = 140;
		PipeMess[6].WaterOutY = 248;
		PipeMess[6].SpeedX = -10*COORDINATE_RATE;
		PipeMess[6].SpeedY = -25*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 7;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_012 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 12;
		PipeMess[0].RectWaterIn.Y = 255;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 31;
		PipeMess[0].WaterOutY = 309;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 215;
		PipeMess[1].RectWaterIn.Y = 130;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 212;
		PipeMess[1].WaterOutY = 39;
		PipeMess[1].SpeedX = -10*COORDINATE_RATE;
		PipeMess[1].SpeedY = -2*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 2;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);	
	}
	else if( level==MW_LEVEL_013 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 200;
		PipeMess[0].RectWaterIn.Y = 325;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 110;
		PipeMess[0].WaterOutY = 313;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 180;
		PipeMess[1].RectWaterIn.Y = 167;
		PipeMess[1].RectWaterIn.Width = 30;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 120;
		PipeMess[1].WaterOutY = 194;
		PipeMess[1].SpeedX = 0;
		PipeMess[1].SpeedY = 20;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 67;
		PipeMess[2].RectWaterIn.Y = 186;
		PipeMess[2].RectWaterIn.Width = 40;
		PipeMess[2].RectWaterIn.Height = 60;
		PipeMess[2].WaterOutX = PipeOutX_HOLD;
		PipeMess[2].WaterOutY = PipeOutY_HOLD;
		PipeMess[2].SpeedX = 200;
		PipeMess[2].SpeedY = PipeSpeedY_HOLD;
		Pipe_Manage.PipeNum = 3;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);	
	}
	else if( level==MW_LEVEL_014 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 180;
		PipeMess[0].RectWaterIn.Y = 154;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 175;
		PipeMess[0].WaterOutY = 59;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 30;
		PipeMess[1].RectWaterIn.Y = 368;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 131;
		PipeMess[1].WaterOutY = 275;
		PipeMess[1].SpeedX = -15*COORDINATE_RATE;
		PipeMess[1].SpeedY = -1*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 40;
		PipeMess[2].RectWaterIn.Y = 270;
		PipeMess[2].RectWaterIn.Width = 20;
		PipeMess[2].RectWaterIn.Height = 20;
		PipeMess[2].WaterOutX = 122;
		PipeMess[2].WaterOutY = 186;
		PipeMess[2].SpeedX = 15*COORDINATE_RATE;
		PipeMess[2].SpeedY = -3*COORDINATE_RATE;

		PipeMess[3].ValidityFlag = iTRUE;
		PipeMess[3].RectWaterIn.X = 200;
		PipeMess[3].RectWaterIn.Y = 213;
		PipeMess[3].RectWaterIn.Width = 20;
		PipeMess[3].RectWaterIn.Height = 20;
		PipeMess[3].WaterOutX = 19;
		PipeMess[3].WaterOutY = 94;
		PipeMess[3].SpeedX = 15*COORDINATE_RATE;
		PipeMess[3].SpeedY = -3*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 4;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_015 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 178;
		PipeMess[0].RectWaterIn.Y = 170;
		PipeMess[0].RectWaterIn.Width = 20;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 124;
		PipeMess[0].WaterOutY = 262;
		PipeMess[0].SpeedX = 1*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 18;
		PipeMess[1].RectWaterIn.Y = 262;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 35;
		PipeMess[1].WaterOutY = 340;
		PipeMess[1].SpeedX = 0;
		PipeMess[1].SpeedY = 1*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 192;
		PipeMess[2].RectWaterIn.Y = 239;
		PipeMess[2].RectWaterIn.Width = 30;
		PipeMess[2].RectWaterIn.Height = 20;
		PipeMess[2].WaterOutX = 146;
		PipeMess[2].WaterOutY = 125;
		PipeMess[2].SpeedX = 4*COORDINATE_RATE;
		PipeMess[2].SpeedY = -15*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 3;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
#elif defined(PKIG_SCREEN_320X480)
	Pipe_Manage.PipeNum = 0;
	if( level==MW_LEVEL_001 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 145;
		PipeMess[0].RectWaterIn.Y = 275;
		PipeMess[0].RectWaterIn.Width = 36;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 145;
		PipeMess[0].WaterOutY =375;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_002 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 150;
		PipeMess[0].RectWaterIn.Y = 360;
		PipeMess[0].RectWaterIn.Width = 40;
		PipeMess[0].RectWaterIn.Height = 25;
		PipeMess[0].WaterOutX = 54;
		PipeMess[0].WaterOutY = 382;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
 		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_003 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 40;
		PipeMess[0].RectWaterIn.Y = 410;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 238;
		PipeMess[0].WaterOutY = 382;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_004 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 275;
		PipeMess[0].RectWaterIn.Y = 385;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 50;
		PipeMess[0].WaterOutY = 376;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_005 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 150;
		PipeMess[0].RectWaterIn.Y = 440;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 233;
		PipeMess[0].WaterOutY = 386;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);

	}
	else if( level==MW_LEVEL_006 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 209;
		PipeMess[0].RectWaterIn.Y = 411;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 97;
		PipeMess[0].WaterOutY = 379;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);

	}		
	else if( level==MW_LEVEL_007 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 144;
		PipeMess[0].RectWaterIn.Y = 316;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 141;
		PipeMess[0].WaterOutY = 387;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 80;
		PipeMess[1].RectWaterIn.Y = 252;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 130;
		PipeMess[1].WaterOutY = 217;
		PipeMess[1].SpeedX = 23*COORDINATE_RATE;
		PipeMess[1].SpeedY = -2*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 188;
		PipeMess[2].RectWaterIn.Y = 210;
		PipeMess[2].RectWaterIn.Width = 20;
		PipeMess[2].RectWaterIn.Height = 20;
		PipeMess[2].WaterOutX = 238;
		PipeMess[2].WaterOutY = 262;
		PipeMess[2].SpeedX = 23*COORDINATE_RATE;
		PipeMess[2].SpeedY = -2*COORDINATE_RATE;

		PipeMess[3].ValidityFlag = iTRUE;
		PipeMess[3].RectWaterIn.X = 298;
		PipeMess[3].RectWaterIn.Y = 256;
		PipeMess[3].RectWaterIn.Width = 20;
		PipeMess[3].RectWaterIn.Height = 20;
		PipeMess[3].WaterOutX = 500;
		PipeMess[3].WaterOutY = 500;
		PipeMess[3].SpeedX = 23*COORDINATE_RATE;
		PipeMess[3].SpeedY = -2*COORDINATE_RATE;
 
		PipeMess[4].ValidityFlag = iTRUE;
		PipeMess[4].RectWaterIn.X = 180;
		PipeMess[4].RectWaterIn.Y = 184;
		PipeMess[4].RectWaterIn.Width = 20;
		PipeMess[4].RectWaterIn.Height = 20;
		PipeMess[4].WaterOutX = 95;
		PipeMess[4].WaterOutY = 150;
		PipeMess[4].SpeedX = -6*COORDINATE_RATE;
		PipeMess[4].SpeedY = -8*COORDINATE_RATE;

		PipeMess[5].ValidityFlag = iTRUE;
		PipeMess[5].RectWaterIn.X = 37;
		PipeMess[5].RectWaterIn.Y = 374;
		PipeMess[5].RectWaterIn.Width = 20;
		PipeMess[5].RectWaterIn.Height = 20;
		PipeMess[5].WaterOutX = 131;
		PipeMess[5].WaterOutY = 298;
		PipeMess[5].SpeedX = 6*COORDINATE_RATE;
		PipeMess[5].SpeedY = -25*COORDINATE_RATE;

		PipeMess[6].ValidityFlag = iTRUE;
		PipeMess[6].RectWaterIn.X = 257;
		PipeMess[6].RectWaterIn.Y = 374;
		PipeMess[6].RectWaterIn.Width = 20;
		PipeMess[6].RectWaterIn.Height = 20;
		PipeMess[6].WaterOutX = 187;
		PipeMess[6].WaterOutY = 288;
		PipeMess[6].SpeedX = -8*COORDINATE_RATE;
		PipeMess[6].SpeedY = -25*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 7;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_008 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 267;
		PipeMess[0].RectWaterIn.Y = 387;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 148;
		PipeMess[0].WaterOutY = 378;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 240;
		PipeMess[1].RectWaterIn.Y = 208;
		PipeMess[1].RectWaterIn.Width = 30;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 158;
		PipeMess[1].WaterOutY = 232;
		PipeMess[1].SpeedX = 0;
		PipeMess[1].SpeedY = 20;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 100;
		PipeMess[2].RectWaterIn.Y = 222;
		PipeMess[2].RectWaterIn.Width = 50;
		PipeMess[2].RectWaterIn.Height = 80;
		PipeMess[2].WaterOutX = PipeOutX_HOLD;
		PipeMess[2].WaterOutY = PipeOutY_HOLD;
		PipeMess[2].SpeedX = 12*COORDINATE_RATE;
		PipeMess[2].SpeedY = PipeSpeedY_HOLD;
		Pipe_Manage.PipeNum = 3;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);	
	}
	else if( level==MW_LEVEL_009 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 209;
		PipeMess[0].RectWaterIn.Y = 411;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 97;
		PipeMess[0].WaterOutY = 379;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_010 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 270;
		PipeMess[0].RectWaterIn.Y = 395;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 233;
		PipeMess[0].WaterOutY = 175;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_011 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 144;
		PipeMess[0].RectWaterIn.Y = 316;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 141;
		PipeMess[0].WaterOutY = 387;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 80;
		PipeMess[1].RectWaterIn.Y = 252;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 130;
		PipeMess[1].WaterOutY = 217;
		PipeMess[1].SpeedX = 23*COORDINATE_RATE;
		PipeMess[1].SpeedY = -2*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 188;
		PipeMess[2].RectWaterIn.Y = 210;
		PipeMess[2].RectWaterIn.Width = 20;
		PipeMess[2].RectWaterIn.Height = 20;
		PipeMess[2].WaterOutX = 238;
		PipeMess[2].WaterOutY = 262;
		PipeMess[2].SpeedX = 23*COORDINATE_RATE;
		PipeMess[2].SpeedY = -2*COORDINATE_RATE;

		PipeMess[3].ValidityFlag = iTRUE;
		PipeMess[3].RectWaterIn.X = 298;
		PipeMess[3].RectWaterIn.Y = 256;
		PipeMess[3].RectWaterIn.Width = 20;
		PipeMess[3].RectWaterIn.Height = 20;
		PipeMess[3].WaterOutX = 500;
		PipeMess[3].WaterOutY = 500;
		PipeMess[3].SpeedX = 23*COORDINATE_RATE;
		PipeMess[3].SpeedY = -2*COORDINATE_RATE;
 
		PipeMess[4].ValidityFlag = iTRUE;
		PipeMess[4].RectWaterIn.X = 180;
		PipeMess[4].RectWaterIn.Y = 184;
		PipeMess[4].RectWaterIn.Width = 20;
		PipeMess[4].RectWaterIn.Height = 20;
		PipeMess[4].WaterOutX = 95;
		PipeMess[4].WaterOutY = 150;
		PipeMess[4].SpeedX = -6*COORDINATE_RATE;
		PipeMess[4].SpeedY = -8*COORDINATE_RATE;

		PipeMess[5].ValidityFlag = iTRUE;
		PipeMess[5].RectWaterIn.X = 37;
		PipeMess[5].RectWaterIn.Y = 374;
		PipeMess[5].RectWaterIn.Width = 20;
		PipeMess[5].RectWaterIn.Height = 20;
		PipeMess[5].WaterOutX = 131;
		PipeMess[5].WaterOutY = 298;
		PipeMess[5].SpeedX = 6*COORDINATE_RATE;
		PipeMess[5].SpeedY = -25*COORDINATE_RATE;

		PipeMess[6].ValidityFlag = iTRUE;
		PipeMess[6].RectWaterIn.X = 257;
		PipeMess[6].RectWaterIn.Y = 374;
		PipeMess[6].RectWaterIn.Width = 20;
		PipeMess[6].RectWaterIn.Height = 20;
		PipeMess[6].WaterOutX = 187;
		PipeMess[6].WaterOutY = 288;
		PipeMess[6].SpeedX = -8*COORDINATE_RATE;
		PipeMess[6].SpeedY = -25*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 7;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_012 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 9;
		PipeMess[0].RectWaterIn.Y = 307;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 53;
		PipeMess[0].WaterOutY = 379;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 275;
		PipeMess[1].RectWaterIn.Y = 150;
		PipeMess[1].RectWaterIn.Width = 30;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 284;
		PipeMess[1].WaterOutY = 47;
		PipeMess[1].SpeedX = -10*COORDINATE_RATE;
		PipeMess[1].SpeedY = -2*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 2;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);	
	}
	else if( level==MW_LEVEL_013 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 267;
		PipeMess[0].RectWaterIn.Y = 387;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 148;
		PipeMess[0].WaterOutY = 378;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 240;
		PipeMess[1].RectWaterIn.Y = 208;
		PipeMess[1].RectWaterIn.Width = 30;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 158;
		PipeMess[1].WaterOutY = 232;
		PipeMess[1].SpeedX = 0;
		PipeMess[1].SpeedY = 20;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 100;
		PipeMess[2].RectWaterIn.Y = 222;
		PipeMess[2].RectWaterIn.Width = 50;
		PipeMess[2].RectWaterIn.Height = 80;
		PipeMess[2].WaterOutX = PipeOutX_HOLD;
		PipeMess[2].WaterOutY = PipeOutY_HOLD;
		PipeMess[2].SpeedX = 12*COORDINATE_RATE;
		PipeMess[2].SpeedY = PipeSpeedY_HOLD;
		Pipe_Manage.PipeNum = 3;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);	
	}
	else if( level==MW_LEVEL_014 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 244;
		PipeMess[0].RectWaterIn.Y = 176;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 40;
		PipeMess[0].WaterOutX = 235;
		PipeMess[0].WaterOutY = 75;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 33;
		PipeMess[1].RectWaterIn.Y = 435;
		PipeMess[1].RectWaterIn.Width = 30;
		PipeMess[1].RectWaterIn.Height = 30;
		PipeMess[1].WaterOutX = 170;
		PipeMess[1].WaterOutY = 329;
		PipeMess[1].SpeedX = -15*COORDINATE_RATE;
		PipeMess[1].SpeedY = -1*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 42;
		PipeMess[2].RectWaterIn.Y = 318;
		PipeMess[2].RectWaterIn.Width = 30;
		PipeMess[2].RectWaterIn.Height = 30;
		PipeMess[2].WaterOutX = 167;
		PipeMess[2].WaterOutY = 227;
		PipeMess[2].SpeedX = 15*COORDINATE_RATE;
		PipeMess[2].SpeedY = -3*COORDINATE_RATE;

		PipeMess[3].ValidityFlag = iTRUE;
		PipeMess[3].RectWaterIn.X = 260;
		PipeMess[3].RectWaterIn.Y = 255;
		PipeMess[3].RectWaterIn.Width = 50;
		PipeMess[3].RectWaterIn.Height = 50;
		PipeMess[3].WaterOutX = 40;
		PipeMess[3].WaterOutY = 105;
		PipeMess[3].SpeedX = 15*COORDINATE_RATE;
		PipeMess[3].SpeedY = -3*COORDINATE_RATE;

		Pipe_Manage.PipeNum = 4;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_015 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 237;
		PipeMess[0].RectWaterIn.Y = 208;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 166;
		PipeMess[0].WaterOutY = 316;
		PipeMess[0].SpeedX = 2*COORDINATE_RATE;
		PipeMess[0].SpeedY = 0;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 31;
		PipeMess[1].RectWaterIn.Y = 306;
		PipeMess[1].RectWaterIn.Width = 30;
		PipeMess[1].RectWaterIn.Height = 30;
		PipeMess[1].WaterOutX = 45;
		PipeMess[1].WaterOutY = 422;
		PipeMess[1].SpeedX = 0;
		PipeMess[1].SpeedY = 1*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 253;
		PipeMess[2].RectWaterIn.Y = 279;
		PipeMess[2].RectWaterIn.Width = 40;
		PipeMess[2].RectWaterIn.Height = 30;
		PipeMess[2].WaterOutX = 195;
		PipeMess[2].WaterOutY = 145;
		PipeMess[2].SpeedX = 2*COORDINATE_RATE;
		PipeMess[2].SpeedY = -15*COORDINATE_RATE;
		
		Pipe_Manage.PipeNum = 3;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
#endif
	
	return iTRUE;

}

iBOOL mwLevelSpeedKeepInit( mwLevelNum level  )
{ 
	SpeedKeep_Manage.TotalSpeedKeepNum = 0;
	SpeedKeep_Manage.ActiveSpeedKeepNum = 0;

#if defined(PKIG_SCREEN_240X320)
	if( level==MW_LEVEL_004 )
	{
		SpeedKeep_Manage.TotalSpeedKeepNum = 3;
		SpeedKeep_Manage.ActiveSpeedKeepNum = 3;
		SpeedKeep_Manage.SpeedKeepMess = (SpeedKeep *)(&SpeedKeepMess);
		SpeedKeepMess[0].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[0].MoveX = 0;
		SpeedKeepMess[0].MoveY = 0;
		SpeedKeepMess[0].SpeedX = 220; 
		SpeedKeepMess[0].SpeedY = 220;
		SpeedKeepMess[0].Rect.X = 4;
		SpeedKeepMess[0].Rect.Y= 165;
		SpeedKeepMess[0].Rect.Width = 36;
		SpeedKeepMess[0].Rect.Height = 36;

		SpeedKeepMess[1].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[1].MoveX = 0;
		SpeedKeepMess[1].MoveY = 0;
		SpeedKeepMess[1].SpeedX = 300; 
		SpeedKeepMess[1].SpeedY = 20;
		SpeedKeepMess[1].Rect.X = 40;
		SpeedKeepMess[1].Rect.Y= 188;
		SpeedKeepMess[1].Rect.Width = 46;
		SpeedKeepMess[1].Rect.Height = 12;

		SpeedKeepMess[2].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[2].MoveX = 0;
		SpeedKeepMess[2].MoveY = 0;
		SpeedKeepMess[2].SpeedX = 220; 
		SpeedKeepMess[2].SpeedY = -150;
		SpeedKeepMess[2].Rect.X = 92;
		SpeedKeepMess[2].Rect.Y= 186;
		SpeedKeepMess[2].Rect.Width = 15;
		SpeedKeepMess[2].Rect.Height = 20;

	}
	else if( level==MW_LEVEL_012 )
	{
		SpeedKeep_Manage.TotalSpeedKeepNum = 1;
		SpeedKeep_Manage.ActiveSpeedKeepNum = 0;
		SpeedKeep_Manage.SpeedKeepMess = (SpeedKeep *)(&SpeedKeepMess);
		SpeedKeepMess[0].KeepSpeedFlag = iFALSE;
		SpeedKeepMess[0].MoveX = 0;
		SpeedKeepMess[0].MoveY = -10;
		SpeedKeepMess[0].SpeedX = 0; 
		SpeedKeepMess[0].SpeedY = -300;
	}
	else if( level==MW_LEVEL_015 )
	{
		SpeedKeep_Manage.TotalSpeedKeepNum = 1;
		SpeedKeep_Manage.ActiveSpeedKeepNum = 0;
		SpeedKeep_Manage.SpeedKeepMess = (SpeedKeep *)(&SpeedKeepMess);
		SpeedKeepMess[0].KeepSpeedFlag = iFALSE;
		SpeedKeepMess[0].MoveX = 9;
		SpeedKeepMess[0].MoveY = 0;
		SpeedKeepMess[0].SpeedX = 3; 
		SpeedKeepMess[0].SpeedY = 0;
	}	

#elif defined(PKIG_SCREEN_240X400)	

	if( level==MW_LEVEL_004 )
	{
		SpeedKeep_Manage.TotalSpeedKeepNum = 3;
		SpeedKeep_Manage.ActiveSpeedKeepNum = 3;
		SpeedKeep_Manage.SpeedKeepMess = (SpeedKeep *)(&SpeedKeepMess);
		SpeedKeepMess[0].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[0].MoveX = 0;
		SpeedKeepMess[0].MoveY = 0;
		SpeedKeepMess[0].SpeedX = 220; 
		SpeedKeepMess[0].SpeedY = 220;
		SpeedKeepMess[0].Rect.X = 8;
		SpeedKeepMess[0].Rect.Y= 225;
		SpeedKeepMess[0].Rect.Width = 20;
		SpeedKeepMess[0].Rect.Height = 20;

		SpeedKeepMess[1].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[1].MoveX = 0;
		SpeedKeepMess[1].MoveY = 0;
		SpeedKeepMess[1].SpeedX = 300; 
		SpeedKeepMess[1].SpeedY = 25;
		SpeedKeepMess[1].Rect.X = 30;
		SpeedKeepMess[1].Rect.Y= 225;
		SpeedKeepMess[1].Rect.Width = 20;
		SpeedKeepMess[1].Rect.Height = 20;

		SpeedKeepMess[2].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[2].MoveX = 0;
		SpeedKeepMess[2].MoveY = 0;
		SpeedKeepMess[2].SpeedX = 220; 
		SpeedKeepMess[2].SpeedY = -150;
		SpeedKeepMess[2].Rect.X = 85;
		SpeedKeepMess[2].Rect.Y= 235;
		SpeedKeepMess[2].Rect.Width = 21;
		SpeedKeepMess[2].Rect.Height = 30;

	}
	else if( level==MW_LEVEL_012 )
	{
		SpeedKeep_Manage.TotalSpeedKeepNum = 1;
		SpeedKeep_Manage.ActiveSpeedKeepNum = 0;
		SpeedKeep_Manage.SpeedKeepMess = (SpeedKeep *)(&SpeedKeepMess);
		SpeedKeepMess[0].KeepSpeedFlag = iFALSE;
		SpeedKeepMess[0].MoveX = 0;
		SpeedKeepMess[0].MoveY = -10;
		SpeedKeepMess[0].SpeedX = 0; 
		SpeedKeepMess[0].SpeedY = -300;
	}
	else if( level==MW_LEVEL_008)
	{
		SpeedKeep_Manage.TotalSpeedKeepNum = 1;
		SpeedKeep_Manage.ActiveSpeedKeepNum = 1;
		SpeedKeep_Manage.SpeedKeepMess = (SpeedKeep *)(&SpeedKeepMess);
		SpeedKeepMess[0].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[0].MoveX = 0;
		SpeedKeepMess[0].MoveY = 0;
		SpeedKeepMess[0].SpeedX = 10; 
		SpeedKeepMess[0].SpeedY = 0;
		SpeedKeepMess[0].Rect.X = 60;
		SpeedKeepMess[0].Rect.Y= 190;
		SpeedKeepMess[0].Rect.Width = 45;
		SpeedKeepMess[0].Rect.Height = 15;

	}
	else if( level==MW_LEVEL_015 )
	{
		SpeedKeep_Manage.TotalSpeedKeepNum = 1;
		SpeedKeep_Manage.ActiveSpeedKeepNum = 0;
		SpeedKeep_Manage.SpeedKeepMess = (SpeedKeep *)(&SpeedKeepMess);
		SpeedKeepMess[0].KeepSpeedFlag = iFALSE;
		SpeedKeepMess[0].MoveX = 5;
		SpeedKeepMess[0].MoveY = 0;
		SpeedKeepMess[0].SpeedX = 300; 
		SpeedKeepMess[0].SpeedY = -2;
	}	

#elif defined(PKIG_SCREEN_320X480)

	if( level==MW_LEVEL_004 )
	{
		SpeedKeep_Manage.TotalSpeedKeepNum = 3;
		SpeedKeep_Manage.ActiveSpeedKeepNum = 3;
		SpeedKeep_Manage.SpeedKeepMess = (SpeedKeep *)(&SpeedKeepMess);
		SpeedKeepMess[0].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[0].MoveX = 0;
		SpeedKeepMess[0].MoveY = 0;
		SpeedKeepMess[0].SpeedX = 12; 
		SpeedKeepMess[0].SpeedY = 12;
		SpeedKeepMess[0].Rect.X = 8;
		SpeedKeepMess[0].Rect.Y= 265;
		SpeedKeepMess[0].Rect.Width = 44;
		SpeedKeepMess[0].Rect.Height = 44;

		SpeedKeepMess[1].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[1].MoveX = 0;
		SpeedKeepMess[1].MoveY = 0;
		SpeedKeepMess[1].SpeedX = 12; 
		SpeedKeepMess[1].SpeedY = 0;
		SpeedKeepMess[1].Rect.X = 52;
		SpeedKeepMess[1].Rect.Y= 288;
		SpeedKeepMess[1].Rect.Width = 67;
		SpeedKeepMess[1].Rect.Height = 20;

		SpeedKeepMess[2].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[2].MoveX = 0;
		SpeedKeepMess[2].MoveY = 0;
		SpeedKeepMess[2].SpeedX = 12; 
		SpeedKeepMess[2].SpeedY = -12;
		SpeedKeepMess[2].Rect.X = 120;
		SpeedKeepMess[2].Rect.Y= 276;
		SpeedKeepMess[2].Rect.Width = 21;
		SpeedKeepMess[2].Rect.Height = 30;

	}
	else if( level==MW_LEVEL_005 )
	{
	/*
		SpeedKeep_Manage.TotalSpeedKeepNum = 1;
		SpeedKeep_Manage.ActiveSpeedKeepNum = 1;
		SpeedKeep_Manage.SpeedKeepMess = (SpeedKeep *)(&SpeedKeepMess);
		SpeedKeepMess[0].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[0].MoveX = 0;
		SpeedKeepMess[0].MoveY = 0;
		SpeedKeepMess[0].SpeedX = -4; 
		SpeedKeepMess[0].SpeedY = 1;
		SpeedKeepMess[0].Rect.X = 0;
		SpeedKeepMess[0].Rect.Y= 100;
		SpeedKeepMess[0].Rect.Width = 60;
		SpeedKeepMess[0].Rect.Height = 40;
		*/

	}
	else if( level==MW_LEVEL_012 )
	{
		SpeedKeep_Manage.TotalSpeedKeepNum = 1;
		SpeedKeep_Manage.ActiveSpeedKeepNum = 0;
		SpeedKeep_Manage.SpeedKeepMess = (SpeedKeep *)(&SpeedKeepMess);
		SpeedKeepMess[0].KeepSpeedFlag = iFALSE;
		SpeedKeepMess[0].MoveX = 0;
		SpeedKeepMess[0].MoveY = -15;
		SpeedKeepMess[0].SpeedX = 0; 
		SpeedKeepMess[0].SpeedY = -20;
	}
	else if( level==MW_LEVEL_015 )
	{
		SpeedKeep_Manage.TotalSpeedKeepNum = 1;
		SpeedKeep_Manage.ActiveSpeedKeepNum = 0;
		SpeedKeep_Manage.SpeedKeepMess = (SpeedKeep *)(&SpeedKeepMess);
		SpeedKeepMess[0].KeepSpeedFlag = iFALSE;
		SpeedKeepMess[0].MoveX = 9;
		SpeedKeepMess[0].MoveY = 0;
		SpeedKeepMess[0].SpeedX = 3; 
		SpeedKeepMess[0].SpeedY = 0;
	}	
#endif

	return iTRUE;

}

iBOOL mwLevelOrganInit( mwLevelNum level  )
{
	int i=0; 
	iS32 StartX = 0;
	iS32 StartY = 0;
	iS32 EndX = 0;
	iS32 EndY = 0;
	iVECT tris[4];
	iBody * staticBody = iNULL;
	iShape * shape = iNULL;

	Organ_Manage.ResidueOrganNum = 0;
	Organ_Manage.TotalOrganNum = 0;
	Organ_Manage.OrganMess = iNULL;

#if defined(PKIG_SCREEN_240X320)
	switch(level)
	{
	case MW_LEVEL_001:
	case MW_LEVEL_002:
	case MW_LEVEL_003:
	case MW_LEVEL_004:
	case MW_LEVEL_005:
		break;
	case MW_LEVEL_006:
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = 5;
		OrganMess[0].OrganMulY = 0;
		OrganMess[0].KeyRect.X = 205;
		OrganMess[0].KeyRect.Y = 273;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = 140;
		OrganMess[0].OrganStartRect.Y = 230;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[0].OrganStartRect.Height= Organ_Key_H;
		OrganMess[0].Direction = ORGAN_DIR_LEVEL;
		OrganMess[0].OrganEndRect.X = 188;
		OrganMess[0].OrganEndRect.Y = 230;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
		OrganMess[1].OrgneState = ORGAN_STATE_SRC;
		OrganMess[1].WaterNum = 0;
		OrganMess[1].OldTime= 0;
		OrganMess[1].OrganMulX = 5;
		OrganMess[1].OrganMulY = 0;
		OrganMess[1].KeyRect.X = 8;
		OrganMess[1].KeyRect.Y = 275;
		OrganMess[1].KeyRect.Width = 20;
		OrganMess[1].KeyRect.Height= 20;
		OrganMess[1].OrganStartRect.X = 140;
		OrganMess[1].OrganStartRect.Y = 257;
		OrganMess[1].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[1].OrganStartRect.Height= Organ_Key_H;
		OrganMess[1].Direction = ORGAN_DIR_LEVEL;
		OrganMess[1].OrganEndRect.X = 188;
		OrganMess[1].OrganEndRect.Y = 257;
		OrganMess[1].OrganCurrRect.X = OrganMess[1].OrganStartRect.X;
		OrganMess[1].OrganCurrRect.Y = OrganMess[1].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 2;
		Organ_Manage.TotalOrganNum = 2;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);

		break;
	case MW_LEVEL_007:
		/*
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = -5;
		OrganMess[0].OrganMulY = 0;
		OrganMess[0].KeyRect.X = 55;
		OrganMess[0].KeyRect.Y = 187;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = 154;
		OrganMess[0].OrganStartRect.Y = 121;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[0].OrganStartRect.Height= Organ_Key_H;
		OrganMess[0].Direction = ORGAN_DIR_LEVEL;
		OrganMess[0].OrganEndRect.X = 87;
		OrganMess[0].OrganEndRect.Y = 121;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 1;
		Organ_Manage.TotalOrganNum = 1;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
		*/
		break;
		
	case MW_LEVEL_008:
		/*
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = -5;
		OrganMess[0].OrganMulY = 0;
		OrganMess[0].KeyRect.X = 167;
		OrganMess[0].KeyRect.Y = 75;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = -Organ_Key_W;
		OrganMess[0].OrganStartRect.Y = 105;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[0].OrganStartRect.Height= Organ_Key_H;
		OrganMess[0].Direction = ORGAN_DIR_LEVEL;
		OrganMess[0].OrganEndRect.X = -48-Organ_Key_W;
		OrganMess[0].OrganEndRect.Y = 105;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
		OrganMess[1].OrgneState = ORGAN_STATE_SRC;
		OrganMess[1].WaterNum = 0;
		OrganMess[1].OldTime= 0;
		OrganMess[1].OrganMulX = 5;
		OrganMess[1].OrganMulY = 0;
		OrganMess[1].KeyRect.X = 80;
		OrganMess[1].KeyRect.Y = 155;
		OrganMess[1].KeyRect.Width = 20;
		OrganMess[1].KeyRect.Height= 20;
		OrganMess[1].OrganStartRect.X = 135;
		OrganMess[1].OrganStartRect.Y = 180;
		OrganMess[1].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[1].OrganStartRect.Height= Organ_Key_H;
		OrganMess[1].Direction = ORGAN_DIR_LEVEL;
		OrganMess[1].OrganEndRect.X = 195;
		OrganMess[1].OrganEndRect.Y = 180;
		OrganMess[1].OrganCurrRect.X = OrganMess[1].OrganStartRect.X;
		OrganMess[1].OrganCurrRect.Y = OrganMess[1].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 2;
		Organ_Manage.TotalOrganNum = 2;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
		*/
		break;

	case MW_LEVEL_009:
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = 5;
		OrganMess[0].OrganMulY = 0;
		OrganMess[0].KeyRect.X = 205;
		OrganMess[0].KeyRect.Y = 273;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = 140;
		OrganMess[0].OrganStartRect.Y = 230;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[0].OrganStartRect.Height= Organ_Key_H;
		OrganMess[0].Direction = ORGAN_DIR_LEVEL;
		OrganMess[0].OrganEndRect.X = 188;
		OrganMess[0].OrganEndRect.Y = 230;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
		OrganMess[1].OrgneState = ORGAN_STATE_SRC;
		OrganMess[1].WaterNum = 0;
		OrganMess[1].OldTime= 0;
		OrganMess[1].OrganMulX = 5;
		OrganMess[1].OrganMulY = 0;
		OrganMess[1].KeyRect.X = 8;
		OrganMess[1].KeyRect.Y = 275;
		OrganMess[1].KeyRect.Width = 20;
		OrganMess[1].KeyRect.Height= 20;
		OrganMess[1].OrganStartRect.X = 140;
		OrganMess[1].OrganStartRect.Y = 257;
		OrganMess[1].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[1].OrganStartRect.Height= Organ_Key_H;
		OrganMess[1].Direction = ORGAN_DIR_LEVEL;
		OrganMess[1].OrganEndRect.X = 188;
		OrganMess[1].OrganEndRect.Y = 257;
		OrganMess[1].OrganCurrRect.X = OrganMess[1].OrganStartRect.X;
		OrganMess[1].OrganCurrRect.Y = OrganMess[1].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 2;
		Organ_Manage.TotalOrganNum = 2;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
		break;

	case MW_LEVEL_010:
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = 5;
		OrganMess[0].OrganMulY = 0;
		OrganMess[0].KeyRect.X = 20;
		OrganMess[0].KeyRect.Y = 75;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = 4;
		OrganMess[0].OrganStartRect.Y = 104;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[0].OrganStartRect.Height = Organ_Key_H;
		OrganMess[0].Direction = ORGAN_DIR_LEVEL;
		OrganMess[0].OrganEndRect.X = 76;
		OrganMess[0].OrganEndRect.Y = 104;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
		OrganMess[1].OrgneState = ORGAN_STATE_SRC;
		OrganMess[1].WaterNum = 0;
		OrganMess[1].OldTime= 0;
		OrganMess[1].OrganMulX = 0;
		OrganMess[1].OrganMulY = -5;
		OrganMess[1].KeyRect.X = 95;
		OrganMess[1].KeyRect.Y = 130;
		OrganMess[1].KeyRect.Width = 20;
		OrganMess[1].KeyRect.Height= 20;
		OrganMess[1].OrganStartRect.X = 70;
		OrganMess[1].OrganStartRect.Y = 274;
		OrganMess[1].OrganStartRect.Width= Organ_Key_W ;
		OrganMess[1].OrganStartRect.Height= Organ_Key_H * 3;
		OrganMess[1].Direction = ORGAN_DIR_VERTICAL;
		OrganMess[1].OrganEndRect.X = 70;
		OrganMess[1].OrganEndRect.Y = 240;
		OrganMess[1].OrganCurrRect.X = OrganMess[1].OrganStartRect.X;
		OrganMess[1].OrganCurrRect.Y = OrganMess[1].OrganStartRect.Y;

		OrganMess[2].OrganType = SHAPE_TYPE_ORGAN_BLUE;
		OrganMess[2].OrgneState = ORGAN_STATE_SRC;
		OrganMess[2].WaterNum = 0;
		OrganMess[2].OldTime= 0;
		OrganMess[2].OrganMulX = 5;
		OrganMess[2].OrganMulY = 0;
		OrganMess[2].KeyRect.X = 15;
		OrganMess[2].KeyRect.Y = 180;
		OrganMess[2].KeyRect.Width = 20;
		OrganMess[2].KeyRect.Height= 20;
		OrganMess[2].OrganStartRect.X = 114;
		OrganMess[2].OrganStartRect.Y = 280;
		OrganMess[2].OrganStartRect.Width= Organ_Key_W* 3;
		OrganMess[2].OrganStartRect.Height= Organ_Key_H;
		OrganMess[2].Direction = ORGAN_DIR_LEVEL;
		OrganMess[2].OrganEndRect.X = 140;
		OrganMess[2].OrganEndRect.Y = 280;
		OrganMess[2].OrganCurrRect.X = OrganMess[2].OrganStartRect.X;
		OrganMess[2].OrganCurrRect.Y = OrganMess[2].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 3;
		Organ_Manage.TotalOrganNum = 3;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
		break;
	case MW_LEVEL_012:
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = 0;
		OrganMess[0].OrganMulY = -8;
		OrganMess[0].KeyRect.X = 165;
		OrganMess[0].KeyRect.Y = 280;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = 205;
		OrganMess[0].OrganStartRect.Y = 225;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W;
		OrganMess[0].OrganStartRect.Height= Organ_Key_H*3;
		OrganMess[0].Direction = ORGAN_DIR_VERTICAL;
		OrganMess[0].OrganEndRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganEndRect.Y = 175;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 1;
		Organ_Manage.TotalOrganNum = 1;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
		break;
	case MW_LEVEL_015:
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = 8;
		OrganMess[0].OrganMulY = 0;
		OrganMess[0].KeyRect.X = 65;
		OrganMess[0].KeyRect.Y = 85;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = 0;
		OrganMess[0].OrganStartRect.Y = SCREEN_HEIGHT-Organ_Key_H*2;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W;
		OrganMess[0].OrganStartRect.Height= Organ_Key_H*3;
		OrganMess[0].Direction = ORGAN_DIR_VERTICAL;
		OrganMess[0].OrganEndRect.X = 170;
		OrganMess[0].OrganEndRect.Y = OrganMess[0].OrganStartRect.Y;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 1;
		Organ_Manage.TotalOrganNum = 1;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
		break;

	default:
		break;
	}

#elif defined(PKIG_SCREEN_240X400)	

	switch(level)
	{
	case MW_LEVEL_001:
	case MW_LEVEL_002:
	case MW_LEVEL_003:
	case MW_LEVEL_004:
	case MW_LEVEL_005:
		break;
	case MW_LEVEL_006:
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = 5;
		OrganMess[0].OrganMulY = 0;
		OrganMess[0].KeyRect.X = 208;
		OrganMess[0].KeyRect.Y = 344;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = 130;
		OrganMess[0].OrganStartRect.Y = 292;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[0].OrganStartRect.Height= Organ_Key_H;
		OrganMess[0].Direction = ORGAN_DIR_LEVEL;
		OrganMess[0].OrganEndRect.X = 190;
		OrganMess[0].OrganEndRect.Y = 292;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
		OrganMess[1].OrgneState = ORGAN_STATE_SRC;
		OrganMess[1].WaterNum = 0;
		OrganMess[1].OldTime= 0;
		OrganMess[1].OrganMulX = 5;
		OrganMess[1].OrganMulY = 0;
		OrganMess[1].KeyRect.X = 7;
		OrganMess[1].KeyRect.Y = 344;
		OrganMess[1].KeyRect.Width = 20;
		OrganMess[1].KeyRect.Height= 20;
		OrganMess[1].OrganStartRect.X = 130;
		OrganMess[1].OrganStartRect.Y = 325;
		OrganMess[1].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[1].OrganStartRect.Height= Organ_Key_H;
		OrganMess[1].Direction = ORGAN_DIR_LEVEL;
		OrganMess[1].OrganEndRect.X = 190;
		OrganMess[1].OrganEndRect.Y = 325;
		OrganMess[1].OrganCurrRect.X = OrganMess[1].OrganStartRect.X;
		OrganMess[1].OrganCurrRect.Y = OrganMess[1].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 2;
		Organ_Manage.TotalOrganNum = 2;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
		break;

		break;
	case MW_LEVEL_007:
		/*
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = -5;
		OrganMess[0].OrganMulY = 0;
		OrganMess[0].KeyRect.X = 50;
		OrganMess[0].KeyRect.Y = 242;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = 155;
		OrganMess[0].OrganStartRect.Y = 155;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[0].OrganStartRect.Height= Organ_Key_H;
		OrganMess[0].Direction = ORGAN_DIR_LEVEL;
		OrganMess[0].OrganEndRect.X = 108;
		OrganMess[0].OrganEndRect.Y = 155;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 1;
		Organ_Manage.TotalOrganNum = 1;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
	*/
		break;
	case MW_LEVEL_008:
		/*
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = -5;
		OrganMess[0].OrganMulY = 0;
		OrganMess[0].KeyRect.X = 170;
		OrganMess[0].KeyRect.Y = 95;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = -Organ_Key_W;
		OrganMess[0].OrganStartRect.Y = 130;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[0].OrganStartRect.Height= Organ_Key_H;
		OrganMess[0].Direction = ORGAN_DIR_LEVEL;
		OrganMess[0].OrganEndRect.X = -48-Organ_Key_W;
		OrganMess[0].OrganEndRect.Y = 130;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
		OrganMess[1].OrgneState = ORGAN_STATE_SRC;
		OrganMess[1].WaterNum = 0;
		OrganMess[1].OldTime= 0;
		OrganMess[1].OrganMulX = 5;
		OrganMess[1].OrganMulY = 0;
		OrganMess[1].KeyRect.X = 75;
		OrganMess[1].KeyRect.Y = 200;
		OrganMess[1].KeyRect.Width = 20;
		OrganMess[1].KeyRect.Height= 20;
		OrganMess[1].OrganStartRect.X = 138;
		OrganMess[1].OrganStartRect.Y = 230;
		OrganMess[1].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[1].OrganStartRect.Height= Organ_Key_H;
		OrganMess[1].Direction = ORGAN_DIR_LEVEL;
		OrganMess[1].OrganEndRect.X = 188;
		OrganMess[1].OrganEndRect.Y = 230;
		OrganMess[1].OrganCurrRect.X = OrganMess[1].OrganStartRect.X;
		OrganMess[1].OrganCurrRect.Y = OrganMess[1].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 2;
		Organ_Manage.TotalOrganNum = 2;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
*/
		break;

	case MW_LEVEL_009:
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = 5;
		OrganMess[0].OrganMulY = 0;
		OrganMess[0].KeyRect.X = 208;
		OrganMess[0].KeyRect.Y = 344;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = 130;
		OrganMess[0].OrganStartRect.Y = 292;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[0].OrganStartRect.Height= Organ_Key_H;
		OrganMess[0].Direction = ORGAN_DIR_LEVEL;
		OrganMess[0].OrganEndRect.X = 190;
		OrganMess[0].OrganEndRect.Y = 292;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
		OrganMess[1].OrgneState = ORGAN_STATE_SRC;
		OrganMess[1].WaterNum = 0;
		OrganMess[1].OldTime= 0;
		OrganMess[1].OrganMulX = 5;
		OrganMess[1].OrganMulY = 0;
		OrganMess[1].KeyRect.X = 7;
		OrganMess[1].KeyRect.Y = 344;
		OrganMess[1].KeyRect.Width = 20;
		OrganMess[1].KeyRect.Height= 20;
		OrganMess[1].OrganStartRect.X = 130;
		OrganMess[1].OrganStartRect.Y = 325;
		OrganMess[1].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[1].OrganStartRect.Height= Organ_Key_H;
		OrganMess[1].Direction = ORGAN_DIR_LEVEL;
		OrganMess[1].OrganEndRect.X = 190;
		OrganMess[1].OrganEndRect.Y = 325;
		OrganMess[1].OrganCurrRect.X = OrganMess[1].OrganStartRect.X;
		OrganMess[1].OrganCurrRect.Y = OrganMess[1].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 2;
		Organ_Manage.TotalOrganNum = 2;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
		break;

	case MW_LEVEL_010:
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = 5;
		OrganMess[0].OrganMulY = 0;
		OrganMess[0].KeyRect.X = 20;
		OrganMess[0].KeyRect.Y = 90;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = 2;
		OrganMess[0].OrganStartRect.Y = 130;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
		OrganMess[0].OrganStartRect.Height = Organ_Key_H;
		OrganMess[0].Direction = ORGAN_DIR_LEVEL;
		OrganMess[0].OrganEndRect.X = 75;
		OrganMess[0].OrganEndRect.Y = 130;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
		OrganMess[1].OrgneState = ORGAN_STATE_SRC;
		OrganMess[1].WaterNum = 0;
		OrganMess[1].OldTime= 0;
		OrganMess[1].OrganMulX = 0;
		OrganMess[1].OrganMulY = -5;
		OrganMess[1].KeyRect.X = 90;
		OrganMess[1].KeyRect.Y = 165;
		OrganMess[1].KeyRect.Width = 20;
		OrganMess[1].KeyRect.Height= 20;
		OrganMess[1].OrganStartRect.X = 68;
		OrganMess[1].OrganStartRect.Y = 357;
		OrganMess[1].OrganStartRect.Width= Organ_Key_W ;
		OrganMess[1].OrganStartRect.Height= Organ_Key_H * 3;
		OrganMess[1].Direction = ORGAN_DIR_VERTICAL;
		OrganMess[1].OrganEndRect.X = 68;
		OrganMess[1].OrganEndRect.Y = 310;
		OrganMess[1].OrganCurrRect.X = OrganMess[1].OrganStartRect.X;
		OrganMess[1].OrganCurrRect.Y = OrganMess[1].OrganStartRect.Y;

		OrganMess[2].OrganType = SHAPE_TYPE_ORGAN_BLUE;
		OrganMess[2].OrgneState = ORGAN_STATE_SRC;
		OrganMess[2].WaterNum = 0;
		OrganMess[2].OldTime= 0;
		OrganMess[2].OrganMulX = 5;
		OrganMess[2].OrganMulY = 0;
		OrganMess[2].KeyRect.X = 9;
		OrganMess[2].KeyRect.Y = 225;
		OrganMess[2].KeyRect.Width = 20;
		OrganMess[2].KeyRect.Height= 20;
		OrganMess[2].OrganStartRect.X = 99;
		OrganMess[2].OrganStartRect.Y = 338;
		OrganMess[2].OrganStartRect.Width= Organ_Key_W* 3;
		OrganMess[2].OrganStartRect.Height= Organ_Key_H;
		OrganMess[2].Direction = ORGAN_DIR_LEVEL;
		OrganMess[2].OrganEndRect.X = 125;
		OrganMess[2].OrganEndRect.Y = 338;
		OrganMess[2].OrganCurrRect.X = OrganMess[2].OrganStartRect.X;
		OrganMess[2].OrganCurrRect.Y = OrganMess[2].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 3;
		Organ_Manage.TotalOrganNum = 3;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
		break;
	case MW_LEVEL_012:
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = 0;
		OrganMess[0].OrganMulY = -4;
		OrganMess[0].KeyRect.X = 165;
		OrganMess[0].KeyRect.Y = 346;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = 203;
		OrganMess[0].OrganStartRect.Y = 298;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W;
		OrganMess[0].OrganStartRect.Height= Organ_Key_H*3;
		OrganMess[0].Direction = ORGAN_DIR_VERTICAL;
		OrganMess[0].OrganEndRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganEndRect.Y = 250;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 1;
		Organ_Manage.TotalOrganNum = 1;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
		break;
	case MW_LEVEL_015:
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
		OrganMess[0].OrgneState = ORGAN_STATE_SRC;
		OrganMess[0].WaterNum = 0;
		OrganMess[0].OldTime= 0;
		OrganMess[0].OrganMulX = 8;
		OrganMess[0].OrganMulY = 0;
		OrganMess[0].KeyRect.X = 65;
		OrganMess[0].KeyRect.Y = 104;
		OrganMess[0].KeyRect.Width = 20;
		OrganMess[0].KeyRect.Height= 20;
		OrganMess[0].OrganStartRect.X = 0;
		OrganMess[0].OrganStartRect.Y = SCREEN_HEIGHT-Organ_Key_H*2;
		OrganMess[0].OrganStartRect.Width= Organ_Key_W;
		OrganMess[0].OrganStartRect.Height= Organ_Key_H*3;
		OrganMess[0].Direction = ORGAN_DIR_VERTICAL;
		OrganMess[0].OrganEndRect.X = 175;
		OrganMess[0].OrganEndRect.Y = OrganMess[0].OrganStartRect.Y;
		OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
		OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

		Organ_Manage.ResidueOrganNum = 1;
		Organ_Manage.TotalOrganNum = 1;
		Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
		break;

	default:
		break;
	}

#elif defined(PKIG_SCREEN_320X480)

	switch(level)
	{
		case MW_LEVEL_001:
		case MW_LEVEL_002:
		case MW_LEVEL_003:
		case MW_LEVEL_004:
		case MW_LEVEL_005:
			break;
		case MW_LEVEL_006:
			OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
			OrganMess[0].OrgneState = ORGAN_STATE_SRC;
			OrganMess[0].WaterNum = 0;
			OrganMess[0].OldTime= 0;
			OrganMess[0].OrganMulX = 5;
			OrganMess[0].OrganMulY = 0;
			OrganMess[0].KeyRect.X = 275;
			OrganMess[0].KeyRect.Y = 420;
			OrganMess[0].KeyRect.Width = 40;
			OrganMess[0].KeyRect.Height= 40;
			OrganMess[0].OrganStartRect.X = 188;
			OrganMess[0].OrganStartRect.Y = 352;
			OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
			OrganMess[0].OrganStartRect.Height= Organ_Key_H;
			OrganMess[0].Direction = ORGAN_DIR_LEVEL;
			OrganMess[0].OrganEndRect.X = 248;
			OrganMess[0].OrganEndRect.Y = 352;
			OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
			OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

			OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
			OrganMess[1].OrgneState = ORGAN_STATE_SRC;
			OrganMess[1].WaterNum = 0;
			OrganMess[1].OldTime= 0;
			OrganMess[1].OrganMulX = 5;
			OrganMess[1].OrganMulY = 0;
			OrganMess[1].KeyRect.X = 14;
			OrganMess[1].KeyRect.Y = 421;
			OrganMess[1].KeyRect.Width = 70;
			OrganMess[1].KeyRect.Height= 40;
			OrganMess[1].OrganStartRect.X = 188;
			OrganMess[1].OrganStartRect.Y = 394;
			OrganMess[1].OrganStartRect.Width= Organ_Key_W*3;
			OrganMess[1].OrganStartRect.Height= Organ_Key_H;
			OrganMess[1].Direction = ORGAN_DIR_LEVEL;
			OrganMess[1].OrganEndRect.X = 248;
			OrganMess[1].OrganEndRect.Y = 394;
			OrganMess[1].OrganCurrRect.X = OrganMess[1].OrganStartRect.X;
			OrganMess[1].OrganCurrRect.Y = OrganMess[1].OrganStartRect.Y;

			Organ_Manage.ResidueOrganNum = 2;
			Organ_Manage.TotalOrganNum = 2;
			Organ_Manage.OrganMess = (OrganData*)(&OrganMess);

			break;
		case MW_LEVEL_007:
			/*
			OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
			OrganMess[0].OrgneState = ORGAN_STATE_SRC;
			OrganMess[0].WaterNum = 0;
			OrganMess[0].OldTime= 0;
			OrganMess[0].OrganMulX = -5;
			OrganMess[0].OrganMulY = 0;
			OrganMess[0].KeyRect.X = 76;
			OrganMess[0].KeyRect.Y = 283;
			OrganMess[0].KeyRect.Width = 50;
			OrganMess[0].KeyRect.Height= 60;
			OrganMess[0].OrganStartRect.X = 208;
			OrganMess[0].OrganStartRect.Y = 188;
			OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
			OrganMess[0].OrganStartRect.Height= Organ_Key_H;
			OrganMess[0].Direction = ORGAN_DIR_LEVEL;
			OrganMess[0].OrganEndRect.X = 129;
			OrganMess[0].OrganEndRect.Y = 188;
			OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
			OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

			Organ_Manage.ResidueOrganNum = 1;
			Organ_Manage.TotalOrganNum = 1;
			Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
			*/
			break;
		case MW_LEVEL_008:
			/*
			OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
			OrganMess[0].OrgneState = ORGAN_STATE_SRC;
			OrganMess[0].WaterNum = 0;
			OrganMess[0].OldTime= 0;
			OrganMess[0].OrganMulX = -5;
			OrganMess[0].OrganMulY = 0;
			OrganMess[0].KeyRect.X = 228;
			OrganMess[0].KeyRect.Y = 109;
			OrganMess[0].KeyRect.Width = 40;
			OrganMess[0].KeyRect.Height= 40;
			OrganMess[0].OrganStartRect.X = -Organ_Key_W;
			OrganMess[0].OrganStartRect.Y = 159;
			OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
			OrganMess[0].OrganStartRect.Height= Organ_Key_H;
			OrganMess[0].Direction = ORGAN_DIR_LEVEL;
			OrganMess[0].OrganEndRect.X = -48-Organ_Key_W;
			OrganMess[0].OrganEndRect.Y = 159;
			OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
			OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

			OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
			OrganMess[1].OrgneState = ORGAN_STATE_SRC;
			OrganMess[1].WaterNum = 0;
			OrganMess[1].OldTime= 0;
			OrganMess[1].OrganMulX = 5;
			OrganMess[1].OrganMulY = 0;
			OrganMess[1].KeyRect.X = 100;
			OrganMess[1].KeyRect.Y = 228;
			OrganMess[1].KeyRect.Width = 40;
			OrganMess[1].KeyRect.Height= 40;
			OrganMess[1].OrganStartRect.X = 191;
			OrganMess[1].OrganStartRect.Y = 273;
			OrganMess[1].OrganStartRect.Width= Organ_Key_W*3;
			OrganMess[1].OrganStartRect.Height= Organ_Key_H;
			OrganMess[1].Direction = ORGAN_DIR_LEVEL;
			OrganMess[1].OrganEndRect.X = 245;
			OrganMess[1].OrganEndRect.Y = 273;
			OrganMess[1].OrganCurrRect.X = OrganMess[1].OrganStartRect.X;
			OrganMess[1].OrganCurrRect.Y = OrganMess[1].OrganStartRect.Y;

			Organ_Manage.ResidueOrganNum = 2;
			Organ_Manage.TotalOrganNum = 2;
			Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
*/
			break;

		case MW_LEVEL_009:
			OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
			OrganMess[0].OrgneState = ORGAN_STATE_SRC;
			OrganMess[0].WaterNum = 0;
			OrganMess[0].OldTime= 0;
			OrganMess[0].OrganMulX = 5;
			OrganMess[0].OrganMulY = 0;
			OrganMess[0].KeyRect.X = 275;
			OrganMess[0].KeyRect.Y = 420;
			OrganMess[0].KeyRect.Width = 40;
			OrganMess[0].KeyRect.Height= 40;
			OrganMess[0].OrganStartRect.X = 188;
			OrganMess[0].OrganStartRect.Y = 352;
			OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
			OrganMess[0].OrganStartRect.Height= Organ_Key_H;
			OrganMess[0].Direction = ORGAN_DIR_LEVEL;
			OrganMess[0].OrganEndRect.X = 248;
			OrganMess[0].OrganEndRect.Y = 352;
			OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
			OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

			OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
			OrganMess[1].OrgneState = ORGAN_STATE_SRC;
			OrganMess[1].WaterNum = 0;
			OrganMess[1].OldTime= 0;
			OrganMess[1].OrganMulX = 5;
			OrganMess[1].OrganMulY = 0;
			OrganMess[1].KeyRect.X = 14;
			OrganMess[1].KeyRect.Y = 421;
			OrganMess[1].KeyRect.Width = 70;
			OrganMess[1].KeyRect.Height= 40;
			OrganMess[1].OrganStartRect.X = 188;
			OrganMess[1].OrganStartRect.Y = 394;
			OrganMess[1].OrganStartRect.Width= Organ_Key_W*3;
			OrganMess[1].OrganStartRect.Height= Organ_Key_H;
			OrganMess[1].Direction = ORGAN_DIR_LEVEL;
			OrganMess[1].OrganEndRect.X = 248;
			OrganMess[1].OrganEndRect.Y = 394;
			OrganMess[1].OrganCurrRect.X = OrganMess[1].OrganStartRect.X;
			OrganMess[1].OrganCurrRect.Y = OrganMess[1].OrganStartRect.Y;

			Organ_Manage.ResidueOrganNum = 2;
			Organ_Manage.TotalOrganNum = 2;
			Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
			break;

		case MW_LEVEL_010:
			OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
			OrganMess[0].OrgneState = ORGAN_STATE_SRC;
			OrganMess[0].WaterNum = 0;
			OrganMess[0].OldTime= 0;
			OrganMess[0].OrganMulX = 5;
			OrganMess[0].OrganMulY = 0;
			OrganMess[0].KeyRect.X = 20;
			OrganMess[0].KeyRect.Y = 110;
			OrganMess[0].KeyRect.Width = 30;
			OrganMess[0].KeyRect.Height= 30;
			OrganMess[0].OrganStartRect.X = 9;
			OrganMess[0].OrganStartRect.Y = 160;
			OrganMess[0].OrganStartRect.Width= Organ_Key_W*3;
			OrganMess[0].OrganStartRect.Height = Organ_Key_H;
			OrganMess[0].Direction = ORGAN_DIR_LEVEL;
			OrganMess[0].OrganEndRect.X = 100;
			OrganMess[0].OrganEndRect.Y = 160;
			OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
			OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

			OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
			OrganMess[1].OrgneState = ORGAN_STATE_SRC;
			OrganMess[1].WaterNum = 0;
			OrganMess[1].OldTime= 0;
			OrganMess[1].OrganMulX = 0;
			OrganMess[1].OrganMulY = -5;
			OrganMess[1].KeyRect.X = 130;
			OrganMess[1].KeyRect.Y = 200;
			OrganMess[1].KeyRect.Width = 40;
			OrganMess[1].KeyRect.Height= 40;
			OrganMess[1].OrganStartRect.X = 98;
			OrganMess[1].OrganStartRect.Y = 430;
			OrganMess[1].OrganStartRect.Width= Organ_Key_W;
			OrganMess[1].OrganStartRect.Height= Organ_Key_H*3;
			OrganMess[1].Direction = ORGAN_DIR_VERTICAL;
			OrganMess[1].OrganEndRect.X = 98;
			OrganMess[1].OrganEndRect.Y = 372;
			OrganMess[1].OrganCurrRect.X = OrganMess[1].OrganStartRect.X;
			OrganMess[1].OrganCurrRect.Y = OrganMess[1].OrganStartRect.Y;

			OrganMess[2].OrganType = SHAPE_TYPE_ORGAN_BLUE;
			OrganMess[2].OrgneState = ORGAN_STATE_SRC;
			OrganMess[2].WaterNum = 0;
			OrganMess[2].OldTime= 0;
			OrganMess[2].OrganMulX = 5;
			OrganMess[2].OrganMulY = 0;
			OrganMess[2].KeyRect.X = 10;
			OrganMess[2].KeyRect.Y = 270;
			OrganMess[2].KeyRect.Width = 40;
			OrganMess[2].KeyRect.Height= 40;
			OrganMess[2].OrganStartRect.X = 138;
			OrganMess[2].OrganStartRect.Y = 410;
			OrganMess[2].OrganStartRect.Width= Organ_Key_W*3;
			OrganMess[2].OrganStartRect.Height= Organ_Key_H;
			OrganMess[2].Direction = ORGAN_DIR_LEVEL;
			OrganMess[2].OrganEndRect.X = 186;
			OrganMess[2].OrganEndRect.Y = 410;
			OrganMess[2].OrganCurrRect.X = OrganMess[2].OrganStartRect.X;
			OrganMess[2].OrganCurrRect.Y = OrganMess[2].OrganStartRect.Y;

			Organ_Manage.ResidueOrganNum = 3;
			Organ_Manage.TotalOrganNum = 3;
			Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
			break;
		case MW_LEVEL_012:
			OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
			OrganMess[0].OrgneState = ORGAN_STATE_SRC;
			OrganMess[0].WaterNum = 0;
			OrganMess[0].OldTime= 0;
			OrganMess[0].OrganMulX = 0;
			OrganMess[0].OrganMulY = -8;
			OrganMess[0].KeyRect.X = 225;
			OrganMess[0].KeyRect.Y = 421;
			OrganMess[0].KeyRect.Width = 30;
			OrganMess[0].KeyRect.Height= 30;
			OrganMess[0].OrganStartRect.X = 275;
			OrganMess[0].OrganStartRect.Y = 357;
			OrganMess[0].OrganStartRect.Width= Organ_Key_W;
			OrganMess[0].OrganStartRect.Height= Organ_Key_H*3;
			OrganMess[0].Direction = ORGAN_DIR_VERTICAL;
			OrganMess[0].OrganEndRect.X = OrganMess[0].OrganStartRect.X;
			OrganMess[0].OrganEndRect.Y = 280;
			OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
			OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;

			Organ_Manage.ResidueOrganNum = 1;
			Organ_Manage.TotalOrganNum = 1;
			Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
			break;
		case MW_LEVEL_015:
			OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
			OrganMess[0].OrgneState = ORGAN_STATE_SRC;
			OrganMess[0].WaterNum = 0;
			OrganMess[0].OldTime= 0;
			OrganMess[0].OrganMulX = 8;
			OrganMess[0].OrganMulY = 0;
			OrganMess[0].KeyRect.X = 88;
			OrganMess[0].KeyRect.Y = 130;
			OrganMess[0].KeyRect.Width = 30;
			OrganMess[0].KeyRect.Height= 30;
			OrganMess[0].OrganStartRect.X = 0;
			OrganMess[0].OrganStartRect.Y = SCREEN_HEIGHT-Organ_Key_H*2;
			OrganMess[0].OrganStartRect.Width= Organ_Key_W;
			OrganMess[0].OrganStartRect.Height= Organ_Key_H*3;
			OrganMess[0].Direction = ORGAN_DIR_VERTICAL;
			OrganMess[0].OrganEndRect.X = 232;
			OrganMess[0].OrganEndRect.Y = OrganMess[0].OrganStartRect.Y;
			OrganMess[0].OrganCurrRect.X = OrganMess[0].OrganStartRect.X;
			OrganMess[0].OrganCurrRect.Y = OrganMess[0].OrganStartRect.Y;
		
			Organ_Manage.ResidueOrganNum = 1;
			Organ_Manage.TotalOrganNum = 1;
			Organ_Manage.OrganMess = (OrganData*)(&OrganMess);
			break;

		default:
			break;
	}
#endif

#if 1
	for(  i=0; i<Organ_Manage.TotalOrganNum; i++ )
	{
	//xie
		iS32 TempOrganX = 0, TempOrganY = 0;
		iS32 StartX = 0, EndX = 0;
		iS32 StartY = 0, EndY = 0;
		StartX = OrganMess[i].OrganStartRect.X;
		StartY = OrganMess[i].OrganStartRect.Y;
		EndX = OrganMess[i].OrganStartRect.X+ OrganMess[i].OrganStartRect.Width;
		EndY = OrganMess[i].OrganStartRect.Y+ OrganMess[i].OrganStartRect.Height;
		if( StartX<0 ) StartX = 0;
		if( StartY<0 ) StartY = 0;
		if( EndX>SCREEN_WIDTH ) EndX = SCREEN_WIDTH;
		if( EndY>SCREEN_HEIGHT ) EndY = SCREEN_HEIGHT;
		
		for(TempOrganY = StartY; TempOrganY<EndY; TempOrganY+=2 )
		{
			for(TempOrganX = StartX; TempOrganX<EndX; TempOrganX+=2 )
			{
				if( mwWorldAttribute[TempOrganY/2][TempOrganX/2] == WORD_SPACE )
				{
					mwWorldAttribute[TempOrganY/2][TempOrganX/2] = WORD_ORGAN;
				}
			}
		}
		//end
		OrganMess[i].OrganMulX *= COORDINATE_RATE;
		OrganMess[i].OrganMulY *= COORDINATE_RATE;
		OrganMess[i].KeyRect.X *= COORDINATE_RATE;
		OrganMess[i].KeyRect.Y *= COORDINATE_RATE;
		OrganMess[i].KeyRect.Width *= COORDINATE_RATE;
		OrganMess[i].KeyRect.Height *= COORDINATE_RATE;
		OrganMess[i].OrganStartRect.X *= COORDINATE_RATE;
		OrganMess[i].OrganStartRect.Y *= COORDINATE_RATE;
		OrganMess[i].OrganStartRect.Width *= COORDINATE_RATE;
		OrganMess[i].OrganStartRect.Height *= COORDINATE_RATE;
		OrganMess[i].OrganEndRect.X *= COORDINATE_RATE;
		OrganMess[i].OrganEndRect.Y *= COORDINATE_RATE;
		OrganMess[i].OrganCurrRect.X *= COORDINATE_RATE;
		OrganMess[i].OrganCurrRect.Y *= COORDINATE_RATE;

		StartX = OrganMess[i].OrganStartRect.X;
		StartY = OrganMess[i].OrganStartRect.Y;
		EndX = StartX + OrganMess[i].OrganStartRect.Width;
		EndY = StartY + OrganMess[i].OrganStartRect.Height;

		OrganMess[i].OrganCurrRect.X = OrganMess[i].OrganStartRect.X;
		OrganMess[i].OrganCurrRect.Y = OrganMess[i].OrganStartRect.Y;
		OrganMess[i].OrganCurrRect.Width = OrganMess[i].OrganStartRect.Width;
		OrganMess[i].OrganCurrRect.Height= OrganMess[i].OrganStartRect.Height;
		OrganMess[i].OrganEndRect.Width = OrganMess[0].OrganStartRect.Width;
		OrganMess[i].OrganEndRect.Height= OrganMess[0].OrganStartRect.Height;

		iVectInit2(&tris[0],StartX,StartY);
		iVectInit2(&tris[1],StartX,EndY);
		iVectInit2(&tris[2],EndX,EndY);
		iVectInit2(&tris[3],EndX,StartY);

		staticBody = (iBody*)(&(mwSpace->StaticBody));
		shape = iNULL;
		shape = (iShape*)i51KitP2CreateShapeSeg((iHANDLE)mwSpace, staticBody, ROCK_SEG_R, tris[0],tris[1]);
		shape->ShapeGroup = OrganMess[i].OrganType;
//		shape = (iShape*)i51KitP2CreateShapeSeg((iHANDLE)mwSpace, staticBody, ROCK_SEG_R, tris[1],tris[2]);
//		shape->ShapeGroup = OrganMess[i].OrganType;
		shape = (iShape*)i51KitP2CreateShapeSeg((iHANDLE)mwSpace, staticBody, ROCK_SEG_R, tris[2],tris[3]);
		shape->ShapeGroup = OrganMess[i].OrganType;
		shape = (iShape*)i51KitP2CreateShapeSeg((iHANDLE)mwSpace, staticBody, ROCK_SEG_R, tris[3],tris[0]);
		shape->ShapeGroup = OrganMess[i].OrganType;
//		shape->block.ShapeType = OrganMess[i].OrganType;
	}
#endif	
	return iTRUE;
}

static iBOOL mwLevelShapeDelet (iShape * shape)
{
	if( iNULL != shape )
	{
		i51KitP2ReleaseShape(mwSpace, shape);
		shape = iNULL;
	}
	return iTRUE;
}

static iBOOL mwLevelWordUpdate( void )
{
	iU16 CurrShapeX = 0;
	iU16 CurrShapeY = 0;
	iWorld * World = (iWorld*)mwSpace;
	NodeContainer * ShapeContainer = &(World->ShapeContainer);
	Node * NodeHandle = ShapeContainer->NodeHandle;
	iS32 MaxNode = ShapeContainer->MaxNode;
	iS32 UseNode = ShapeContainer->UseNode;
	iS32 Temp = 0;
	iS32 CurrUse = 0;
	iShape * shape = iNULL;

	for( Temp = 0; Temp<MaxNode; Temp++)
	{
		if( NodeHandle[Temp].Flag) CurrUse++;
		else continue;

		shape = (iShape *)(NodeHandle[Temp].Handle);
		
		CurrShapeX = shape->block.BlockX;
		CurrShapeY = shape->block.BlockY;
		if( shape->block.ShapeType == SHAPE_TYPE_DELETABLE		
			&&(shape->block.BlockType  != GriddingType[CurrShapeY][CurrShapeX]
			|| GriddingMud[CurrShapeY][CurrShapeX]==Gridding_MUD_No 
			|| (GriddingMud[CurrShapeY][CurrShapeX]&Gridding_MUD_Update)==Gridding_MUD_Update
			|| GriddingUpdataFlag[CurrShapeY][CurrShapeX])
		)
		{
			shape->block.BlockType == Gridding_TYPE_SPACE;
			mwLevelShapeDelet(shape);
		}

		if(CurrUse==UseNode) break;
		
	}

	if( CurrUse!=UseNode )
	{
		iLog("mwLevelWordUpdate CurrUse:%d,UseNode:%d",CurrUse,UseNode);
	}
	
	return iTRUE;
	
}

static iBOOL mwLevelShapeAdd (iS16 ShapeX, iS16 ShapeY, iU8 ShapeType )
{
	iS32 StartX = (ShapeX)*SideLength*COORDINATE_RATE;
	iS32 EndX = (ShapeX+1)*SideLength*COORDINATE_RATE;
	iS32 StartY = (ShapeY)*SideLength*COORDINATE_RATE;
	iS32 EndY = (ShapeY+1)*SideLength*COORDINATE_RATE;
	iS32 EndX2 = StartX + SideLength/2*COORDINATE_RATE;
	iS32 EndY2 = StartY + SideLength/2*COORDINATE_RATE;
	iVECT v1,v2;
	iBody *staticBody = &(mwSpace->StaticBody);
	iShape *shape = iNULL;

	if( ShapeX<0 || ShapeX>=SCREEN_WIDTH/SideLength *COORDINATE_RATE
	||	ShapeY<0 || ShapeY>=SCREEN_HEIGHT/SideLength *COORDINATE_RATE
	||	ShapeType>Gridding_TYPE_RIGHT_3 || ShapeType==Gridding_TYPE_SPACE )
	{
		iLog("mwLevelShapeAdd err:CurrLevelNum:%d,ShapeX:%d,ShapeY:%d,ShapeType:%d",CurrLevelNum,ShapeX,ShapeY,ShapeType);
		return iFALSE;
	}
	
	switch( ShapeType )
	{
		case Gridding_TYPE_TOP:
			iVectInit2(&v1,StartX,StartY);
			iVectInit2(&v2,EndX,StartY);
			break;		
		case Gridding_TYPE_BUTTOM:
			iVectInit2(&v1,StartX,EndY);
			iVectInit2(&v2,EndX,EndY);
			break;		
		case Gridding_TYPE_LEFT:
			 iVectInit2(&v1,StartX,StartY);
			 iVectInit2(&v2,StartX,EndY);
			break;		
		case Gridding_TYPE_RIGHT:
			iVectInit2(&v1,EndX,StartY);
			iVectInit2(&v2,EndX,EndY);
			break;		
		case Gridding_TYPE_LT:
		case Gridding_TYPE_RB:
			iVectInit2(&v1,StartX,EndY);
			iVectInit2(&v2,EndX,StartY);
			break;		
		case Gridding_TYPE_LB:
		case Gridding_TYPE_RT:
			iVectInit2(&v1,StartX,StartY);
			iVectInit2(&v2,EndX,EndY);
			break;		
		case Gridding_TYPE_TOP_3:
			iVectInit2(&v1,StartX,StartY);
			iVectInit2(&v2,EndX2,EndY2);
			shape = i51KitP2CreateShapeSeg(mwSpace, staticBody, RECT_SEG_R, v1, v2);
			shape->block.BlockType = ShapeType;
			shape->block.BlockX = ShapeX;
			shape->block.BlockY = ShapeY;
			shape->block.ShapeType = SHAPE_TYPE_DELETABLE;
			iVectInit2(&v1,EndX,StartY);
			iVectInit2(&v2,EndX2,EndY2);
			break;		
		case Gridding_TYPE_BUTTOM_3:
			iVectInit2(&v1,StartX,EndY);
			iVectInit2(&v2,EndX2,EndY2);
			shape = i51KitP2CreateShapeSeg(mwSpace, staticBody, RECT_SEG_R, v1, v2);
			shape->block.BlockType = ShapeType;
			shape->block.BlockX = ShapeX;
			shape->block.BlockY = ShapeY;
			shape->block.ShapeType = SHAPE_TYPE_DELETABLE;
			iVectInit2(&v1,EndX,EndY);
			iVectInit2(&v2,EndX2,EndY2);
			break;		
		case Gridding_TYPE_LEFT_3:
			iVectInit2(&v1,StartX,StartY);
			iVectInit2(&v2,EndX2,EndY2);
			shape = i51KitP2CreateShapeSeg(mwSpace, staticBody, RECT_SEG_R, v1, v2);
			shape->block.BlockType = ShapeType;
			shape->block.BlockX = ShapeX;
			shape->block.BlockY = ShapeY;
			shape->block.ShapeType = SHAPE_TYPE_DELETABLE;
			iVectInit2(&v1,StartX,EndY);
			iVectInit2(&v2,EndX2,EndY2);
			break;		
		case Gridding_TYPE_RIGHT_3:
			iVectInit2(&v1,EndX,StartY);
			iVectInit2(&v2,EndX2,EndY2);
			shape = i51KitP2CreateShapeSeg(mwSpace, staticBody, RECT_SEG_R, v1, v2);
			shape->block.BlockType = ShapeType;
			shape->block.BlockX = ShapeX;
			shape->block.BlockY = ShapeY;
			shape->block.ShapeType = SHAPE_TYPE_DELETABLE;
			iVectInit2(&v1,EndX,EndY);
			iVectInit2(&v2,EndX2,EndY2);
			break;		
		default:
			iLog("shape type:Level:%d,X:%d,Y:%d,T:%d",CurrLevelNum,ShapeX,ShapeX,ShapeType);
			return iFALSE;
			break;
	}

	shape = i51KitP2CreateShapeSeg(mwSpace, staticBody, RECT_SEG_R, v1, v2);
	shape->block.BlockType = ShapeType;
	shape->block.BlockX = ShapeX;
	shape->block.BlockY = ShapeY;
	shape->block.ShapeType = SHAPE_TYPE_DELETABLE;

	return iTRUE;
	
}

static iBOOL mwLevelShapeLoop( iS16 LoopStartX, iS16 LoopStartY )
{
	iS16 ShapeX[8];
	iS16 ShapeY[8] ;
	iS16 ShapeType[8] = {1,2,3,4,5,6,7,8};

	int i = 0;

	ShapeX[0] = LoopStartX;
	ShapeX[1] = LoopStartX+1;
	ShapeX[2] = LoopStartX+1;
	ShapeX[3] = LoopStartX+1;
	ShapeX[4] = LoopStartX;
	ShapeX[5] = LoopStartX-1;
	ShapeX[6] = LoopStartX-1;
	ShapeX[7] = LoopStartX-1;

	ShapeY[0] = LoopStartY-1;
	ShapeY[1] = LoopStartY-1;
	ShapeY[2] = LoopStartY;
	ShapeY[3] = LoopStartY+1;
	ShapeY[4] = LoopStartY+1;
	ShapeY[5] = LoopStartY+1;
	ShapeY[6] = LoopStartY;
	ShapeY[7] = LoopStartY-1;

	for( i=0; i<8; i++ )
	{
		GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
		mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
	}

	return iTRUE;
}

iBOOL mwLevelGriddingUpdate( iS16 GriddingX, iS16 GriddingY )
{

//	iS16 StartX = GriddingX*SideLength;
//	iS16 StartY = GriddingY*SideLength;
	iS16 CurrGriddingX = 0;
	iS16 CurrGriddingY = 0;
	int CurrGridding = 0;
	iU8 CurrGriddingNum = 0;
	iU8 ContinueFlag = iFALSE;
	iU8 CurrType = 0;

	iU8 AddShapeTypeTable[8][12] =
	{
		// 分别是上，右上，右，右下，下，左下，左，左上
		// 八个方向的新旧表格内外型类型转换表
		{1,1,0,0,0,0,0,1,1,0,0,0},
		{1,2,3,Gridding_TYPE_RIGHT_3,0,0,0,Gridding_TYPE_TOP_3,Gridding_TYPE_TOP_3,0,0,Gridding_TYPE_RIGHT_3},
		{0,3,3,3,0,0,0,0,0,0,0,3},
		{0,Gridding_TYPE_RIGHT_3,3,4,5,Gridding_TYPE_BUTTOM_3,0,0,0,Gridding_TYPE_BUTTOM_3,0,Gridding_TYPE_RIGHT_3},
		{0,0,0,5,5,5,0,0,0,5,0,0},
		{0,0,0,Gridding_TYPE_BUTTOM_3,5,6,7,Gridding_TYPE_LEFT_3,0,Gridding_TYPE_BUTTOM_3,Gridding_TYPE_LEFT_3,0},
		{0,0,0,0,0,7,7,7,0,0,7,0},
		{1,Gridding_TYPE_TOP_3,0,0,0,Gridding_TYPE_LEFT_3,7,8,Gridding_TYPE_TOP_3,0,Gridding_TYPE_LEFT_3,0}
	};

	iU8 GriddingNum[8] = {
	 Gridding_TYPE_TOP,
	 Gridding_TYPE_RT,
	 Gridding_TYPE_RIGHT,
	 Gridding_TYPE_RB,
	 Gridding_TYPE_BUTTOM,
	 Gridding_TYPE_LB,
	 Gridding_TYPE_LEFT,
	 Gridding_TYPE_LT
	};

	if( GriddingX<0 || GriddingX>=SCREEN_WIDTH/SideLength || GriddingY<=0 || GriddingY>=SCREEN_HEIGHT/SideLength )
	{
		// 越界
		return iFALSE;
	}
	
	GriddingType[GriddingY][GriddingX] = Gridding_TYPE_SPACE;
	GriddingMud[GriddingY][GriddingX] = Gridding_MUD_No;
	GriddingUpdataFlag[GriddingY][GriddingX] = 0;



	for( CurrGridding = 0; CurrGridding < 8 ; CurrGridding++ )
	{
		CurrGriddingNum = GriddingNum[CurrGridding];
		switch(CurrGridding)
		{
			// 中心区上方格子
			case 0:	CurrGriddingX = GriddingX; CurrGriddingY = GriddingY - 1; break;
			// 中心区右上方格子
			case 1:	CurrGriddingX = GriddingX+1; CurrGriddingY = GriddingY - 1; break;
			// 中心区右方格子
			case 2:	CurrGriddingX = GriddingX+1; CurrGriddingY = GriddingY; break;
			// 中心区右下方格子
			case 3:	CurrGriddingX = GriddingX+1; CurrGriddingY = GriddingY + 1; break;
			// 中心区下方格子
			case 4:	CurrGriddingX = GriddingX; CurrGriddingY = GriddingY + 1; break;
			// 中心区左下方格子
			case 5:	CurrGriddingX = GriddingX-1; CurrGriddingY = GriddingY + 1; break;
			// 中心区左方格子
			case 6:	CurrGriddingX = GriddingX-1; CurrGriddingY = GriddingY ; break;
			// 中心区左上方格子
			case 7:	CurrGriddingX = GriddingX-1; CurrGriddingY = GriddingY - 1; break;
			default : break;
		}

		if( CurrGriddingX<0 || CurrGriddingX>=SCREEN_WIDTH/SideLength || CurrGriddingY<=0 || CurrGriddingY>=SCREEN_HEIGHT/SideLength )
		{
			continue;
		}

		// 如果此区域已经没有泥土则返回
		if( GriddingMud[CurrGriddingY][CurrGriddingX]==Gridding_MUD_No )
		{
			GriddingUpdataFlag[CurrGriddingY][CurrGriddingX] = 0;
			continue;
		}
		
		ContinueFlag = iFALSE;
		// 如果此区域是上下左右四个区域之一，则检测与他相邻的一个区域
		switch(CurrGriddingNum)
		{
			case Gridding_TYPE_TOP:
				if(	(CurrGriddingY>0) 
				&&	(GriddingType[CurrGriddingY-1][CurrGriddingX]==Gridding_TYPE_BUTTOM
					))//||GriddingMud[CurrGriddingY-1][CurrGriddingX]==Gridding_MUD_No) )
				{
					GriddingMud[CurrGriddingY][CurrGriddingX] = Gridding_MUD_No;
					GriddingType[CurrGriddingY][CurrGriddingX] = Gridding_TYPE_SPACE;
					GriddingMud[CurrGriddingY-1][CurrGriddingX] = Gridding_MUD_No;
					GriddingType[CurrGriddingY-1][CurrGriddingX] = Gridding_TYPE_SPACE;
					GriddingUpdataFlag[CurrGriddingY][CurrGriddingX] = 0;
					GriddingUpdataFlag[CurrGriddingY-1][CurrGriddingX] = 0;
					ContinueFlag = iTRUE;
				}
				break;
				
			case Gridding_TYPE_BUTTOM:
				if( (CurrGriddingY<SCREEN_HEIGHT/SideLength-1)
					&&	(GriddingType[CurrGriddingY+1][CurrGriddingX]==Gridding_TYPE_TOP
						||GriddingMud[CurrGriddingY+1][CurrGriddingX]==Gridding_MUD_No) )
				
				{
					GriddingMud[CurrGriddingY][CurrGriddingX] = Gridding_MUD_No;
					GriddingType[CurrGriddingY][CurrGriddingX] = Gridding_TYPE_SPACE;
					GriddingMud[CurrGriddingY+1][CurrGriddingX] = Gridding_MUD_No;
					GriddingType[CurrGriddingY+1][CurrGriddingX] = Gridding_TYPE_SPACE;
					GriddingUpdataFlag[CurrGriddingY][CurrGriddingX] = 0;
					GriddingUpdataFlag[CurrGriddingY+1][CurrGriddingX] = 0;
					ContinueFlag = iTRUE;
				}
				break;
			case Gridding_TYPE_RIGHT:
				if( (CurrGriddingX<SCREEN_WIDTH/SideLength-1)
					&&	(GriddingType[CurrGriddingY][CurrGriddingX+1]==Gridding_TYPE_LEFT
						))//||GriddingMud[CurrGriddingY][CurrGriddingX+1]==Gridding_MUD_No) )
				
				{
					GriddingMud[CurrGriddingY][CurrGriddingX] = Gridding_MUD_No;
					GriddingType[CurrGriddingY][CurrGriddingX] = Gridding_TYPE_SPACE;
					GriddingMud[CurrGriddingY][CurrGriddingX+1] = Gridding_MUD_No;
					GriddingType[CurrGriddingY][CurrGriddingX+1] = Gridding_TYPE_SPACE;
					GriddingUpdataFlag[CurrGriddingY][CurrGriddingX] = 0;
					GriddingUpdataFlag[CurrGriddingY][CurrGriddingX+1] = 0;
					ContinueFlag = iTRUE;
				}
				break;
			case Gridding_TYPE_LEFT:
				if( (CurrGriddingX>0)
					&&	(GriddingType[CurrGriddingY][CurrGriddingX-1]==Gridding_TYPE_RIGHT
						))//||GriddingMud[CurrGriddingY][CurrGriddingX-1]==Gridding_MUD_No) )
				
				{
					GriddingMud[CurrGriddingY][CurrGriddingX] = Gridding_MUD_No;
					GriddingType[CurrGriddingY][CurrGriddingX] = Gridding_TYPE_SPACE;
					GriddingMud[CurrGriddingY][CurrGriddingX-1] = Gridding_MUD_No;
					GriddingType[CurrGriddingY][CurrGriddingX-1] = Gridding_TYPE_SPACE;
					GriddingUpdataFlag[CurrGriddingY][CurrGriddingX] = 0;
					GriddingUpdataFlag[CurrGriddingY][CurrGriddingX-1] = 0;
					ContinueFlag = iTRUE;
				}
				break;
			default:
				break;
		}

		if( ContinueFlag ) continue;

		// 如果此区域泥土还是满的
		if( (GriddingMud[CurrGriddingY][CurrGriddingX]&Gridding_MUD_FULL) == Gridding_MUD_FULL )
		{
			GriddingMud[CurrGriddingY][CurrGriddingX] = Gridding_MUD_Yes;
			GriddingType[CurrGriddingY][CurrGriddingX] = CurrGriddingNum;
			GriddingUpdataFlag[CurrGriddingY][CurrGriddingX] = 1;
//			mwLevelShapeAdd(CurrGriddingX, CurrGriddingY, CurrGriddingNum);
			continue;
		}

		if( GriddingType[CurrGriddingY][CurrGriddingX] == Gridding_TYPE_SPACE
		|| GriddingType[CurrGriddingY][CurrGriddingX] > Gridding_TYPE_RIGHT_3)
		{
//			iLog("ERR:GriddingType[CurrGriddingY][CurrGriddingX] =%d",GriddingType[CurrGriddingY][CurrGriddingX]);
			GriddingUpdataFlag[CurrGriddingY][CurrGriddingX] = 0;
			continue;
		}

		
		CurrType = GriddingType[CurrGriddingY][CurrGriddingX]-1;
		GriddingType[CurrGriddingY][CurrGriddingX] = AddShapeTypeTable[CurrGridding][CurrType];
		if( GriddingType[CurrGriddingY][CurrGriddingX] != Gridding_TYPE_SPACE )
		{
//			mwLevelShapeAdd(CurrGriddingX, CurrGriddingY, AddShapeTypeTable[CurrGridding][CurrType] );
			GriddingUpdataFlag[CurrGriddingY][CurrGriddingX] = 1;
			GriddingMud[CurrGriddingY][CurrGriddingX] = Gridding_MUD_Yes;
		}
		else 
		{
			GriddingUpdataFlag[CurrGriddingY][CurrGriddingX] = 0;
			GriddingMud[CurrGriddingY][CurrGriddingX] = Gridding_MUD_No;
		}
		
	}

//	mwLevelWordUpdate();
	
	return iTRUE;

}

iBOOL mwLevelGriddingInit( void )
{
	int GriddingNumW = SCREEN_WIDTH/SideLength;
	int GriddingNumH = SCREEN_HEIGHT/SideLength;
	int X = 0, Y = 0;
	int StartX = 0;
	int StartY = 0;
	int EndX = 0;
	int EndY = 0;
	iS16 TotalRockPoint = 0;
	iS16 TotalSpacePoint = 0;
	int TempY = 0;
	int TempX = 0;
	
	for( Y = 0; Y < GriddingNumH; Y++ )
	{
		for( X = 0; X < GriddingNumW; X++)
		{
			StartX = (X)*SideLength/2;
			StartY = (Y)*SideLength/2;
			EndX = (X+1)*SideLength/2;
			EndY = (Y+1)*SideLength/2;
			TotalRockPoint = 0;
			TotalSpacePoint = 0;
			for(TempY = StartY; TempY< EndY; TempY++ )
			{
				for(TempX = StartX; TempX< EndX; TempX++ )
				{
					if( (mwWorldAttribute[TempY][TempX]&WORD_ROCK) == WORD_ROCK )
					{
						TotalRockPoint++;
					}
					else if( mwWorldAttribute[TempY][TempX] == WORD_SPACE)
					{
						TotalSpacePoint++;
					}
				}
			}
			
			if( GriddingType[Y][X] != Gridding_TYPE_SPACE)
			{
				GriddingMud[Y][X] = Gridding_MUD_Yes;
			}
			else if( (TotalRockPoint+TotalSpacePoint)>= 20 )
			{
				GriddingMud[Y][X] = Gridding_MUD_No;
			}
		}
	}
	
	return iTRUE;
}

#if defined(PKIG_SCREEN_240X320)

const iS16 BathX[MW_LEVEL_MAX][4] = {
	{92, 100, 142, 151},
	{25, 34, 74, 86},
	{164, 173, 213, 223},
	{24, 31, 72, 82}, 
	//{26, 34, 75, 84},
	{161, 170, 210, 220},
	//{139, 148, 191, 199},
	//{160, 171, 208, 221},
	{59, 68, 109, 118},
	//{160, 172, 208, 220},
	{90, 98, 142, 152},	
	//{24, 33, 74, 84},
	{94, 104, 144, 155},
	//{161, 170, 212, 220},
	//{109, 118, 161, 168}
};
const iS16 BathY[MW_LEVEL_MAX][4] = {
	{273, 293, 293, 273},
	{280, 301, 301, 280},
	{281, 301, 301, 281}, 
	{278, 298, 298, 278},
	//{207, 228, 228, 207},
	{283, 302, 302, 283},
	//{253, 273, 273, 253},
	//{280, 302, 302, 280},
	{280, 300, 300, 280},
	//{144, 165, 165, 144},
	{284, 304, 304, 284},
	//{280, 301, 301, 280},
	{277, 298, 298, 277},
	//{75, 99, 99, 75},
	//{234, 255, 255, 234}
};

#elif defined(PKIG_SCREEN_240X400)	

const iS16 BathX[MW_LEVEL_MAX][4] = {
	{91, 100, 140, 152},
	{25, 33, 75, 84},
	{163, 169, 216, 223},
	{23, 31, 73, 82}, 
	//{27, 34, 76, 86},
	{161, 166, 211, 219},
	//{139, 148, 189, 199},
	//{161, 168, 211, 221},
	{57, 67, 109, 119},
	//{158, 167, 209, 220},
	{90, 97, 143, 152},
	//{24, 32, 75, 87},
	{96, 103, 144, 156},
	//{158, 167, 210, 220},
	//{109, 115, 160, 169}
};
const iS16 BathY[MW_LEVEL_MAX][4] = {
	{340, 362, 362, 340},
	{352, 375, 375, 352},
	{350, 372, 372, 350}, 
	{346, 370, 370, 346},
	//{259, 280, 280, 259},
	{353, 374, 374, 353},
	//{317, 339, 339, 317},
	//{351, 373, 373, 351},
	{351, 373, 373, 351},
	//{180, 202, 202, 180},
	{354, 375, 375, 354},
	//{349, 372, 372, 349},
	{347, 369, 369, 347},
	//{93, 116, 116, 93},	
	//{293, 314, 314, 293}
};

#elif defined(PKIG_SCREEN_320X480)	

const iS16 BathX[MW_LEVEL_MAX][4] = {
	{120,132,190,207},	
	{28,43,101,120},
	{213,228,287,301},
	{23,39,99,115}, 
	//{35,50,102,115},
	{215,226,281,293},
	//{186,197,252,268},
	//{214,226,283,294},
	{76,90,144,159},
	//{215,227,281,294},
	{121,134,189,203},	
	//{32,46,99,114},
	{127,140,193,206},	
	//{214,219,279,295},
	//{146,157,213,225}
};
const iS16 BathY[MW_LEVEL_MAX][4] = {
	{410,438,438,410},
	{419,450,450,419},
	{420,451,451,420}, 
	{415,449,449,415},
	//{310,343,343,310},
	{424,453,453,424},
	//{382,409,409,382},	
	//{422,449,449,422},
	{423,451,451,422},
	//{217,246,246,217},	
	{424,454,454,424},
	//{421,451,451,422},
	{418,447,447,419},	
	//{114,143,143,114},	
	//{351,380,380,351}
};

#endif


iBOOL mwLevelWordInit( mwLevelNum level)
{
	iU32 ReadDataLength = 0;
	iFILE File = iNULL;
	iS32 MulsX = 0;
	iS32 MulsY = 0;
	iShape *shape = iNULL;
	iBody *staticBody = iNULL;
	int Circle = 0;
	int Line = 0;
	int X=0,Y=0;
	int TempX = 0;
	int TempY = 0;
	int Count = 0;
	int TenthX = 0;
	int TenthY = 0;
	iS32 Point[2][2];
	int CurrX = 0;
	int CurrY = 0;
	int A,B,C;
	int CalculateY = 0;
	// 点到直线距离的分子
	int Numerator = 0;
	// 点到直线距离的分母
	int Denominator = 0;
	// 点到直线的距离的平方
	int Space = 0;

	int tmp = 0; //xie
	iU32 MapDataOffset = 0;
	iU32 MapDataLength = 0;
	iU32 ShapeNum = 0;
	iU32 LoopNum = 0;
	iU8 * ShapeX = iNULL;
	iU8 * ShapeY = iNULL;
	iU8 * ShapeT = iNULL;
	iU8 * LoopX = iNULL;
	iU8 * LoopY = iNULL;
	iU16 i = 0;
	iU8 Flag = iFALSE;
	
	iVECT v1,v2;
	
	ReadDataLength = 0;
	//File = iNULL;
	//File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\wmwdata.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
	//if( iNULL==File )
	//{	
	//	iLog("open wmwdata.re err");
	//	return iFALSE;
	//}
	//
	//Flag = i51AdeOsFileSetSeek(File, ((iU16)level+1)*4, i51_OS_FL_SEEK_HEAD);
	//if( iNULL==Flag )
	//{
	//	iLog("Init set seek err!");
	//	return iFALSE;
	//}

	MapDataOffset = GameData[((iU16)level+1)*4] + (GameData[((iU16)level+1)*4 + 1] << 8)
					+ (GameData[((iU16)level+1)*4 + 2] << 16) + (GameData[((iU16)level+1)*4 + 3] << 24);
	
 //   Flag = i51AdeOsFileReadInt32( File, &MapDataOffset);   
	//if( iNULL==Flag )
	//{
	//	iLog("read map data offset err!");
	//	return iFALSE;
	//}
	

	//Flag = i51AdeOsFileSetSeek(File, MapDataOffset, i51_OS_FL_SEEK_HEAD);
	//if( iNULL==Flag )
	//{
	//	iLog("Init set seek err!");
	//	return iFALSE;
	//}
	
	
	MapDataLength = GameData[MapDataOffset]
					+ (GameData[MapDataOffset + 1] << 8)
					+ (GameData[MapDataOffset + 2] << 16)
					+ (GameData[MapDataOffset + 3] << 24);

 //   Flag = i51AdeOsFileReadInt32( File, &MapDataLength);   
	//if( iNULL==Flag )
	//{
	//	iLog("read length err!");
	//	return iFALSE;
	//}

	ShapeNum = GameData[4 + MapDataOffset]
					+ (GameData[4 + MapDataOffset + 1] << 8)
					+ (GameData[4 + MapDataOffset + 2] << 16)
					+ (GameData[4 + MapDataOffset + 3] << 24);

 //   Flag = i51AdeOsFileReadInt32( File, &ShapeNum);   
	//if( iNULL==Flag )
	//{
	//	iLog("read shape num err!");
	//	return iFALSE;
	//}
	
	LoopNum = GameData[2*4 + MapDataOffset]
				+ (GameData[2*4 + MapDataOffset + 1] << 8)
				+ (GameData[2*4 + MapDataOffset + 2] << 16)
				+ (GameData[2*4 + MapDataOffset + 3] << 24);


				tmp = 3*4 + MapDataOffset;
 //   Flag = i51AdeOsFileReadInt32( File, &LoopNum);   
	//if( iNULL==Flag )
	//{
	//	iLog("read loop num err!");
	//	return iFALSE;
	//}

	if(ShapeNum) ShapeX = i51KitP2Malloc( ShapeNum*3);
	if(LoopNum) LoopX = i51KitP2Malloc( LoopNum*2);

	if( (iNULL==ShapeX&&ShapeNum) || (iNULL==LoopX&&LoopNum) )
	{
		iLog("ShapeNum:%d,LoopNum:%d",ShapeNum, LoopNum );
		iLog("Malloc err:ShapeX:%X,LoopX:%X,%d,%d",ShapeNum*3,LoopNum*2);
		return iFALSE;
	}

	//iU8 mwWorldAttribute[SCREEN_HEIGHT_HALF][SCREEN_WIDTH_HALF] = {0};

	for (i = 0;i < SCREEN_HEIGHT_HALF; i++)
	{
		memcpy(&mwWorldAttribute[i][0], GameData + tmp + SCREEN_WIDTH_HALF * i, SCREEN_WIDTH_HALF);
	}

	//memcpy(mwWorldAttribute, GameData + tmp, MapDataLength);

	//Flag = i51AdeOsFileRead(File, mwWorldAttribute, MapDataLength, &ReadDataLength);
	//if( iNULL==Flag )
	//{
	//	iLog("read map data err!");
	//	return iFALSE;
	//}

	tmp += MapDataLength;
	if(ShapeNum)
	{
		memcpy(ShapeX, GameData + tmp, ShapeNum*3);

		tmp += ShapeNum*3;

		//Flag = i51AdeOsFileRead(File, ShapeX, ShapeNum*3, &ReadDataLength);
		//if( iNULL==Flag )
		//{
		//	iLog("read shape err!");
		//	return iFALSE;
		//}
	}

	if(LoopNum)
	{
		memcpy(LoopX, GameData + tmp, LoopNum*2);

		//Flag = i51AdeOsFileRead(File, LoopX, LoopNum*2, &ReadDataLength);
		//if( iNULL==Flag )
		//{
		//	iLog("read loop err!");
		//	return iFALSE;
		//}
	}

	ShapeY = ShapeX + ShapeNum;
	ShapeT = ShapeY + ShapeNum;

	LoopY = LoopX + LoopNum;
	
	//i51AdeOsFileClose(File);

	memset(GriddingMud,Gridding_MUD_FULL,SCREEN_HEIGHT*SCREEN_WIDTH/SideLength/SideLength);
	memset(GriddingType,Gridding_TYPE_SPACE,SCREEN_HEIGHT*SCREEN_WIDTH/SideLength/SideLength);
	memset(GriddingUpdataFlag,Gridding_TYPE_SPACE,SCREEN_HEIGHT*SCREEN_WIDTH/SideLength/SideLength);

	for( i=0; i<ShapeNum; i++ )
	{
		GriddingType[ShapeY[i]][ShapeX[i]] = ShapeT[i];
		mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeT[i]);
	}

	for( i = 0; i< LoopNum; i++ )
	{
		mwLevelShapeLoop(LoopX[i],LoopY[i]);
	}

	if(ShapeX) i51AdeOsFree( ShapeX );
	if(LoopX) i51AdeOsFree( LoopX );
	
	mwLevelGriddingInit();
	
	MulsX = SCREEN_WIDTH/SideLength;
	MulsY = SCREEN_HEIGHT/SideLength;
	shape = iNULL;
	staticBody = &(mwSpace->StaticBody);

#if 1
	Circle = 0;
	Line = 0;
	for( Y = 0 ; Y<SCREEN_HEIGHT_HALF; Y++ )
	{
		for( X = 0; X <SCREEN_WIDTH_HALF; X ++)
		{
		//if( (mwWorldAttribute[Y][X]&0XF0) > 0) i51KitG2DrawPoint(X, Y, 0XF800);
		//continue;
			// 边缘点已经计算过着需要跳过此区域
			if( mwWorldAttribute[Y][X]&0X08 )
			{
				CurrX = X;
				for( CurrX = X; CurrX < SCREEN_WIDTH_HALF; CurrX++ )
				{
					if( (mwWorldAttribute[Y][CurrX]&0XF0)&& (!(mwWorldAttribute[Y][CurrX]&0X08)) ) break;
				}

				if( CurrX == SCREEN_WIDTH_HALF) 
				{
					X = SCREEN_WIDTH_HALF- 1;
					continue;
				}
				else
				{
					X = CurrX;
				}
			}

			// 找到一个未被标记计算的边缘点
			if( (mwWorldAttribute[Y][X]&0XF0)&& (!(mwWorldAttribute[Y][X]&0X08)) )
			{
				// 计算包括找到边缘点的一个环
				TempX = X;
				TempY = Y;
				X = 0;
				Circle++;
				Line = 0;
				// 环结束条件是与边缘点相邻的边缘点已经被标记
				while( !(mwWorldAttribute[TempY][TempX]&0X08) )
				{
					Count = 0;
					TenthX = TempX;
					TenthY = TempY;
					//沿着此边缘点开始查找与他相邻的第五个边缘点
					while( Count < 5 && !(mwWorldAttribute[TenthY][TenthX]&0X08) )
					{
						switch( mwWorldAttribute[TenthY][TenthX]&0XF0 )
						{
							case 0X10 : TenthX ++; break;
							case 0X20 : TenthX ++; TenthY ++; break;
							case 0X30 : TenthY ++; break;
							case 0X40 : TenthX --; TenthY ++; break;
							case 0X50 : TenthX --; break;
							case 0X60 : TenthX --; TenthY --; break;
							case 0X70 : TenthY --; break;
							case 0X80 : TenthX ++; TenthY --; break;
							default:
//								iLog("level:%d.\nWord border error: mwWorldAttribute[%d][%d]:0X%X",CurrLevelNum,TenthY, TenthX,mwWorldAttribute[TenthY][TenthX]);
								break;
						}
						Count ++;
					}

					// 计算组成环的直线
					// 直线的组成原理是边缘点在直线附近三个像素折认为是在直线内，
					// 否则认为是个拐点
					
					CurrX = TempX;
					CurrY = TempY;
					Point[0][0] = TempX;
					Point[0][1] = TempY;
		//			i51KitG2DrawPoint(TempX, TempY, 0X001F);
					mwWorldAttribute[CurrY][CurrX] |= 0X08;
					// 经过边缘点和边缘点相邻的第十个点的直线Ax+By+C = 0 的系数
					CalculateY = TempY;
					if( TempY == TenthY) 
					{
						A = 0;
						B = 1;
						C = -TempY;
					}
					else if( TempX==TenthX )
					{
						A = 1;
						B = 0;
						C = -TempX;
					}
					else 
					{
						A = TenthY - TempY;
						B = TempX - TenthX;
						C = TenthX*TempY - TempX*TenthY;
						
					}
					
					while(1)
					{
						Count ++ ;
						switch( mwWorldAttribute[CurrY][CurrX]&0XF0 )
						{
							case 0X10 : CurrX ++; break;
							case 0X20 : CurrX ++; CurrY ++; break;
							case 0X30 : CurrY ++; break;
							case 0X40 : CurrX --; CurrY ++; break;
							case 0X50 : CurrX --; break;
							case 0X60 : CurrX --; CurrY --; break;
							case 0X70 : CurrY --; break;
							case 0X80 : CurrX ++; CurrY --; break;
							default:
//								iLog("level:%d.\nWord border error: mwWorldAttribute[%d][%d]:0X%X",CurrLevelNum,CurrY, CurrX,mwWorldAttribute[CurrY][CurrX]);
								break;
						}

						// 点到直线距离的分子
						Numerator = A*CurrX + B*CurrY + C;
						// 点到直线距离的分母
						Denominator = A*A + B*B;
						// 点到直线的距离的平方
						Space = Numerator*Numerator/Denominator;

						if( Space>3 || (mwWorldAttribute[CurrY][CurrX]&0X08) ) 
						{
							break;
						}
						else
						{
//							i51KitG2DrawPoint(CurrX, CurrY, 0X001F);
							mwWorldAttribute[CurrY][CurrX] |= 0X08;
						}
					}
					Point[1][0] = CurrX;
					Point[1][1] = CurrY;
					TempX = CurrX;
					TempY = CurrY;
					//	i51KitG2DrawLine(Point, 0XF000);
					if( (Point[0][0]<3 || Point[0][0]>(SCREEN_WIDTH_HALF-3) || Point[1][0]<3 || Point[1][0]>(SCREEN_WIDTH_HALF-3) )
					&&	(Point[0][1]<3 || Point[0][1]>(SCREEN_HEIGHT_HALF-3) || Point[1][1]<3 || Point[1][1]>(SCREEN_HEIGHT_HALF-3))
					&&	( (AbsInt(Point[0][0]-Point[1][0])<3) || (AbsInt(Point[0][1]-Point[1][1])<3)) )
					{
						continue;
					}
					iVectInit2(&v1,Point[0][0]*2*COORDINATE_RATE,Point[0][1]*2*COORDINATE_RATE);
					iVectInit2(&v2,Point[1][0]*2*COORDINATE_RATE,Point[1][1]*2*COORDINATE_RATE);
					shape = i51KitP2CreateShapeSeg(mwSpace, staticBody, RECT_SEG_R, v1, v2);
					Line ++;
				}
			}
		}
	}
//	iLog("创建石头边缘");
#endif

	for( Y = 0 ; Y<SCREEN_HEIGHT_HALF; Y++ )
	{
		for( X = 0; X <SCREEN_WIDTH_HALF; X ++)
		{
			mwWorldAttribute[Y][X] &= 0X07;
		}
	}

	iVectInit2(&v1,BathX[level-1][0]*COORDINATE_RATE,BathY[level-1][0]*COORDINATE_RATE);
	iVectInit2(&v2,BathX[level-1][1]*COORDINATE_RATE,BathY[level-1][1]*COORDINATE_RATE);
	shape = i51KitP2CreateShapeSeg(mwSpace, staticBody, BATH_SEG_R, v1, v2);

	iVectInit2(&v1,BathX[level-1][1]*COORDINATE_RATE,BathY[level-1][1]*COORDINATE_RATE);
	iVectInit2(&v2,BathX[level-1][2]*COORDINATE_RATE,BathY[level-1][2]*COORDINATE_RATE);
	shape = i51KitP2CreateShapeSeg(mwSpace, staticBody, BATH_SEG_R, v1, v2);

	iVectInit2(&v1,BathX[level-1][2]*COORDINATE_RATE,BathY[level-1][2]*COORDINATE_RATE);
	iVectInit2(&v2,BathX[level-1][3]*COORDINATE_RATE,BathY[level-1][3]*COORDINATE_RATE);
	shape = i51KitP2CreateShapeSeg(mwSpace, staticBody, BATH_SEG_R, v1, v2);

	mwLevelWordUpdate();

	mwLevelSetImageMem(level); //xie
	
	mwLevelSpeedKeepInit(CurrLevelNum);

	// 清空按键队列
	while(1)
	{
		llp_get_input(&mwKeyData);
		if( mwKeyData.type==iNULL )
		{
			break;
		}
	};

	return iTRUE;
	
}

iBOOL mwLevelAlgaeInit( mwLevelNum level )
{
	int Y,X;
	int MaxX = SCREEN_WIDTH/2;
	int MaxY = SCREEN_HEIGHT/2;

#if defined(PKIG_SCREEN_240X320)
	switch(level)
	{
	case MW_LEVEL_004:
		AlgaeNeed = iTRUE;
		for( Y = 98; Y<MaxY; Y++ )
		{
			for( X = 75; X<85; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}
		break;
	case MW_LEVEL_005:
		/*
		AlgaeNeed = iTRUE;
		for( Y = 136; Y<MaxY; Y++ )
		{
			for( X = 75; X<92; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}
		*/
		break;
	case MW_LEVEL_008:
/*
		AlgaeNeed = iTRUE;
		for( Y = 102; Y<145; Y++ )
		{
			for( X = 95-(Y-102); X<95-(Y-102)+8; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}
*/
		AlgaeNeed = iTRUE;
		for( Y = 95; Y<100; Y++ )
		{
			for( X = 50; X<64; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}

		break;
	case MW_LEVEL_010:
		AlgaeNeed = iTRUE;
		for( Y = 125; Y<MaxY; Y++ )
		{
			for( X = 35; X<47; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}
		for( Y = 140; Y<145; Y++ )
		{
			for( X = 93; X<106; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}
		break;
	case MW_LEVEL_013:
		AlgaeNeed = iTRUE;
		for( Y = 95; Y<100; Y++ )
		{
			for( X = 50; X<64; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}
		break;
	default : 
		break;

	}

#elif defined(PKIG_SCREEN_240X400)	

	switch(level)
	{
	case MW_LEVEL_004:
		AlgaeNeed = iTRUE;
		for( Y = 123; Y<MaxY; Y++ )
		{
			for( X = 75; X<85; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}
		break;
	case MW_LEVEL_005:
		/*
		AlgaeNeed = iTRUE;
		for( Y = 170; Y<MaxY; Y++ )
		{
			for( X = 78; X<93; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}
		*/
		break;
	case MW_LEVEL_008:
		AlgaeNeed = iTRUE;
		for( Y = 121; Y<125; Y++ )
		{
			for( X = 52; X<66; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}
		break;
	case MW_LEVEL_010:
		AlgaeNeed = iTRUE;
		for( Y = 157; Y<MaxY; Y++ )
		{
			for( X = 34; X<47; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}
		for( Y = 178; Y<183; Y++ )
		{
			for( X = 75; X<105; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}
		break;
	case MW_LEVEL_013:
		AlgaeNeed = iTRUE;
		for( Y = 121; Y<125; Y++ )
		{
			for( X = 52; X<66; X++)
			{
				mwWorldAttribute[Y][X] = WORD_ALGAE;
			}
		}
		break;
	default : 
		break;

	}

#elif defined(PKIG_SCREEN_320X480)

	switch(level)
	{
		case MW_LEVEL_004:
			AlgaeNeed = iTRUE;
			for( Y = 147; Y<MaxY; Y++ )
			{
				for( X = 100; X<113; X++)
				{
					mwWorldAttribute[Y][X] = WORD_ALGAE;
				}
			}
			break;
		case MW_LEVEL_005:
			/*
			AlgaeNeed = iTRUE;
			for( Y = 210; Y<216; Y++ )
			{
				for( X = 104; X<126; X++)
				{
					mwWorldAttribute[Y][X] = WORD_ALGAE;
				}
			}
			*/
			break;
		case MW_LEVEL_008:
			AlgaeNeed = iTRUE;
			for( Y = 145; Y<155; Y++ )
			{
				for( X = 70; X<90; X++)
				{
					mwWorldAttribute[Y][X] = WORD_ALGAE;
				}
			}
			break;
		case MW_LEVEL_010:
			AlgaeNeed = iTRUE;
			for( Y = 195; Y<222; Y++ )
			{
				for( X = 50; X<65; X++)
				{
					mwWorldAttribute[Y][X] = WORD_ALGAE;
				}
			}
			for( Y = 215; Y<225; Y++ )
			{
				for( X = 95; X<145; X++)
				{
					mwWorldAttribute[Y][X] = WORD_ALGAE;
				}
			}
			break;
		case MW_LEVEL_013:
			AlgaeNeed = iTRUE;
			for( Y = 145; Y<155; Y++ )
			{
				for( X = 70; X<90; X++)
				{
					mwWorldAttribute[Y][X] = WORD_ALGAE;
				}
			}
			break;
		default : 
			break;
			
	}
#endif

	return iTRUE;	

}


iBOOL mwLevelWaterSpray( mwLevelNum level )
{
	static iU32 OldTime = 0;
	iBody *body = iNULL;
	iShape*shape = iNULL;
	iS32 radius = SPRAY_RADIUS;
	iVECT pos ;

#if defined(PKIG_SCREEN_240X320)
	switch(level)
	{
	case MW_LEVEL_005:
/*
		if( i51AdeOsGetTick()-OldTime >= 150 && WaterSprayFlag == iTRUE )
		{
			iVectInit2(&mwSpace->WorldGravity,0,100); 
			OldTime = i51AdeOsGetTick();

			iVectInit2(&pos,35*COORDINATE_RATE,38*COORDINATE_RATE);
			body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
			iVectInit2(&(body->V),1400,-300);
			i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);
		}
*/
		break;
	case MW_LEVEL_006:
		break;
	case MW_LEVEL_007:
		//			mwSpace->gravity = cpv(0,150); 
		if( i51AdeOsGetTick()-OldTime >= 150 && WaterSprayFlag == iTRUE )
		{
			OldTime = i51AdeOsGetTick();

			iVectInit2(&pos,16*COORDINATE_RATE,172*COORDINATE_RATE);
			body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
			iVectInit2(&(body->V),16*COORDINATE_RATE,-2*COORDINATE_RATE);
			i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);
		}
		break;			
	case MW_LEVEL_014:

		if( i51AdeOsGetTick()-OldTime >= 150 && WaterSprayFlag == iTRUE )
		{
			OldTime = i51AdeOsGetTick();

			iVectInit2(&pos,126*COORDINATE_RATE,275*COORDINATE_RATE);
			body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
			iVectInit2(&(body->V),-18*COORDINATE_RATE,0);
			i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);
		}
		break;

	default:
		break;
	}

#elif defined(PKIG_SCREEN_240X400)	

	switch(level)
	{
	case MW_LEVEL_005:
	/*
		if( i51AdeOsGetTick()-OldTime >= 150 && WaterSprayFlag == iTRUE )
		{
			iVectInit2(&mwSpace->WorldGravity,0,100); 
			OldTime = i51AdeOsGetTick();

			iVectInit2(&pos,35*COORDINATE_RATE,50*COORDINATE_RATE);
			body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
			iVectInit2(&(body->V),1400,-300);
			i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);
		}
	*/
		break;
	case MW_LEVEL_006:
		break;
	case MW_LEVEL_007:
		//			mwSpace->gravity = cpv(0,150); 
		if( i51AdeOsGetTick()-OldTime >= 150 && WaterSprayFlag == iTRUE )
		{
			OldTime = i51AdeOsGetTick();

			iVectInit2(&pos,20*COORDINATE_RATE,215*COORDINATE_RATE);
			body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
			iVectInit2(&(body->V),16*COORDINATE_RATE,-2*COORDINATE_RATE);
			i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);
		}
		break;			
	case MW_LEVEL_014:

		//			mwSpace->gravity = cpv(0,50); 

		if( i51AdeOsGetTick()-OldTime >= 150 && WaterSprayFlag == iTRUE )
		{
			OldTime = i51AdeOsGetTick();

			iVectInit2(&pos,128*COORDINATE_RATE,343*COORDINATE_RATE);
			body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
			iVectInit2(&(body->V),-18*COORDINATE_RATE,0);
			i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);
		}
		break;

	default:
		break;
	}

#elif defined(PKIG_SCREEN_320X480)


	switch(level)
	{
/*
		case MW_LEVEL_005:
			if( i51AdeOsGetTick()-OldTime >= 150 && WaterSprayFlag == iTRUE )
			{
				iVectInit2(&mwSpace->WorldGravity,0,100); 
				OldTime = i51AdeOsGetTick();

				iVectInit2(&pos,32*COORDINATE_RATE,61*COORDINATE_RATE);
				body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
				iVectInit2(&(body->V),1400,-300);
				i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);
			}
			break;
		case MW_LEVEL_006:
			break;
*/
		case MW_LEVEL_007:

//			mwSpace->gravity = cpv(0,150); 
			if( i51AdeOsGetTick()-OldTime >= 150 && WaterSprayFlag == iTRUE )
			{
				OldTime = i51AdeOsGetTick();

				iVectInit2(&pos,25*COORDINATE_RATE,257*COORDINATE_RATE);
				body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
				iVectInit2(&(body->V),16*COORDINATE_RATE,-2*COORDINATE_RATE);
				i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);
			}

			
			break;			

		default:
			break;
	}
#endif

	return iTRUE;	

}

iBOOL mwLevelWaitInit( mwLevelNum level )
{
	int i = 0;
	iS16 TotalFrame = 5;
	iU32 ImageId[5] = 
	{
		LeadWait00,
		LeadWait00,
		LeadWait01,
		LeadWait00,
		LeadWait00,
	};


#if defined(PKIG_SCREEN_240X320)
	iS16 WAIT_X[MW_LEVEL_MAX] = {103, 39, 176, 34, 171, 71,102, 106,};
	iS16 WAIT_Y[MW_LEVEL_MAX] = {242, 250,250, 248,252,251,253,247,};
	iS16 WAIT_W = 43;
	iS16 WAIT_H = 54;

#elif defined(PKIG_SCREEN_240X400)	

	iS16 WAIT_X[MW_LEVEL_MAX] = {97, 29, 167, 27, 164, 63, 95, 99,};
	iS16 WAIT_Y[MW_LEVEL_MAX] = {299, 309, 309, 305, 311, 309, 312, 305,};
	iS16 WAIT_W = 57;
	iS16 WAIT_H = 71;

#elif defined(PKIG_SCREEN_320X480)
		iS16 WAIT_X[MW_LEVEL_MAX] = {130,44,228,39,223,88,131,136};
		iS16 WAIT_Y[MW_LEVEL_MAX] = {352,363,363,359,369,365,369,360};
		iS16 WAIT_W = 72;
		iS16 WAIT_H = 90;
#endif



	CrocodileWait.Container = mwContainer;
	CrocodileWait.CurrPlayFrame = 0;
	CrocodileWait.DesRect.X = WAIT_X[level-1];
	CrocodileWait.DesRect.Y = WAIT_Y[level-1];
	CrocodileWait.DesRect.Width = WAIT_W;
	CrocodileWait.DesRect.Height = WAIT_H;
	CrocodileWait.FrameTimeMul = 500;
	CrocodileWait.ImageHeight = WAIT_H;
	for( i=0; i<TotalFrame; i++ )
	{
		CrocodileWait.ImageId[i] = ImageId[i];
	}
	CrocodileWait.ImageTotalFrame = TotalFrame;
	CrocodileWait.ImageWidth = WAIT_W;
	CrocodileWait.OldPlayTime = i51AdeOsGetTick();

	return iTRUE;	

}

iBOOL mwLevelBatheInit( mwLevelNum level )
{
	int i = 0;
	iU32 ImageId[3] = 
	{
		LeadBathe00,
		LeadBathe01,
		LeadBathe02
	};
	iS16 TotalFrame = 3; 

#if defined(PKIG_SCREEN_240X320)

	//iS16 BANTH_X[MW_LEVEL_MAX] = {103, 39, 176, 34, 37, 171, 152, 172, 71, 172, 102, 37,  106, 172, 120};
	//iS16 BANTH_Y[MW_LEVEL_MAX] = {242, 250,250, 248,176,252, 224, 251, 251,114, 253, 250, 247, 45,  204 };
	iS16 BANTH_W = 43;
	iS16 BANTH_H = 54;

	iS16 BANTH_X[MW_LEVEL_MAX] = {103, 39, 176, 34, 171, 71,102, 106,};
	iS16 BANTH_Y[MW_LEVEL_MAX] = {242, 250,250, 248,252,251,253,247,};


#elif defined(PKIG_SCREEN_240X400)	

	iS16 BANTH_X[MW_LEVEL_MAX] = {97, 29, 167, 27, 164, 63, 95, 99,};
	iS16 BANTH_Y[MW_LEVEL_MAX] = {299, 309, 309, 305, 311, 309, 312, 305,};

	//iS16 BANTH_X[MW_LEVEL_MAX] = {97,	29,	167/*,	27,	28,	164,	143,	164,	63,	162,	95,	31,	99,	164,	113*/};
	//iS16 BANTH_Y[MW_LEVEL_MAX] = {299,	309,	309/*,	305,	215,	311,	275,	309,	309,	138,	312,	309,	305,	51,	251*/ };
	iS16 BANTH_W = 72;
	iS16 BANTH_H = 90;

#elif defined(PKIG_SCREEN_320X480)

		iS16 BANTH_X[MW_LEVEL_MAX] = {130,44,228,39,223,88,131,136};
		iS16 BANTH_Y[MW_LEVEL_MAX] = {352,363,363,359,369,365,369,360};
		iS16 BANTH_W = 72;
		iS16 BANTH_H = 90;
		
#endif
	CrocodileBathe.Container = mwContainer;
	CrocodileBathe.CurrPlayFrame = 0;
	CrocodileBathe.DesRect.X = BANTH_X[level-1];
	CrocodileBathe.DesRect.Y = BANTH_Y[level-1];
	CrocodileBathe.DesRect.Width = BANTH_W;
	CrocodileBathe.DesRect.Height = BANTH_H;
	CrocodileBathe.FrameTimeMul = 250;
	CrocodileBathe.ImageHeight = BANTH_H;
	for( i=0; i<TotalFrame; i++ )
	{
		CrocodileBathe.ImageId[i] = ImageId[i];
	}
	CrocodileBathe.ImageTotalFrame = TotalFrame;
	CrocodileBathe.ImageWidth = BANTH_W;
	CrocodileBathe.OldPlayTime = i51AdeOsGetTick();

	return iTRUE;	

}

iBOOL mwLevelRedrawBG( void )
{
// 实时绘制背景
	iS32 CopyLength = 0;
	iS32 TempX = 0;
	iS32 TempY = 0;
	int X=0,Y = 0;
	for( Y = 0; Y<SCREEN_HEIGHT_HALF; Y++ )
	{
		if ( CopyLength>0 )
		{
#ifdef __MODIS_DEBUG__
			i51AdeStdMemcpy16((void *)(mwScreenBuffer+2*TempY*320+2*TempX),
				(void *)(TempScreenBuf + TempY*2*SCREEN_WIDTH + TempX*2), 
				CopyLength);
			i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*320+2*TempX),
				(void *)(TempScreenBuf+(TempY*2+1) * SCREEN_WIDTH + TempX*2), 
				CopyLength);

#else
			i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*SCREEN_WIDTH+2*TempX),
				(void *)(TempScreenBuf+(TempY*2)*SCREEN_WIDTH +TempX*2), 
				CopyLength);
			i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*SCREEN_WIDTH+2*TempX),
				(void *)(TempScreenBuf+(TempY*2+1) *SCREEN_WIDTH +TempX*2), 
				CopyLength);
#endif
			CopyLength = 0; 		

		}

		for( X = 0; X<SCREEN_WIDTH_HALF; X++ )
		{
			if(mwWorldAttribute[Y][X]==0)
			{
				if( CopyLength==0 )
				{
					TempY = Y;
					TempX = X;
				}
				CopyLength += 4;
			}
			else if ( CopyLength>0 )
			{
#ifdef __MODIS_DEBUG__
				i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*320+2*TempX),
					(void *)(TempScreenBuf+(TempY*2)*SCREEN_WIDTH + TempX*2), 
					CopyLength);
				i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*320+2*TempX),
					(void *)(TempScreenBuf+(TempY*2+1)*SCREEN_WIDTH+TempX*2), 
					CopyLength);

#else
				i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*SCREEN_WIDTH+2*TempX),
					(void *)(TempScreenBuf+(TempY*2)*SCREEN_WIDTH+TempX*2), 
					CopyLength);
				i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*SCREEN_WIDTH+2*TempX),
					(void *)(TempScreenBuf+(TempY*2+1)*SCREEN_WIDTH+TempX*2), 
					CopyLength);

#endif
				CopyLength = 0;			
			}
				
		}
	}
	if( CopyLength>0 )
	{
#ifdef __MODIS_DEBUG__
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*320+2*TempX),
			(void *)(TempScreenBuf+(TempY*2)*SCREEN_WIDTH+TempX*2), 
			CopyLength);
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*320+2*TempX),
			(void *)(TempScreenBuf+(TempY*2+1)*SCREEN_WIDTH+TempX*2), 
			CopyLength);
#else
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*SCREEN_WIDTH+2*TempX),
			(void *)(TempScreenBuf+(TempY*2)*SCREEN_WIDTH+TempX*2), 
			CopyLength);
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*SCREEN_WIDTH+2*TempX),
			(void *)(TempScreenBuf+(TempY*2+1)*SCREEN_WIDTH+TempX*2), 
			CopyLength);

#endif
		CopyLength = 0;
	}
	return iTRUE;
}

iBOOL mwLevelRedrawFG( void )
{
//	return iTRUE;
// 实时绘制前景
	iS32 CopyLength = 0;
	iS32 TempX = 0;
	iS32 TempY = 0;
	int X=0, Y=0;
	for( Y = 0; Y<SCREEN_HEIGHT_HALF; Y++ )
	{
		if( CopyLength>0 )
		{
#ifdef __MODIS_DEBUG__
			i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*320+2*TempX),
				(void *)(TempScreenBuf+(TempY*2)*SCREEN_WIDTH+TempX*2), 
				CopyLength);
			i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*320+2*TempX),
				(void *)(TempScreenBuf+(TempY*2+1)*SCREEN_WIDTH+TempX*2), 
				CopyLength);
#else
			i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*SCREEN_WIDTH+2*TempX),
				(void *)(TempScreenBuf+(TempY*2)*SCREEN_WIDTH + TempX*2), 
				CopyLength);
			i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*SCREEN_WIDTH+2*TempX),
				(void *)(TempScreenBuf+(TempY*2+1)*SCREEN_WIDTH+TempX*2), 
				CopyLength);

#endif
			CopyLength = 0;
		}

		for( X = 0; X<SCREEN_WIDTH_HALF; X++ )
		{
			if(mwWorldAttribute[Y][X])
			{
				if( CopyLength==0 )
				{
					TempY = Y;
					TempX = X;
				}
				CopyLength += 4;
			}
			else if ( CopyLength>0 )
			{
#ifdef __MODIS_DEBUG__
				i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*320+2*TempX),
					(void *)(TempScreenBuf+(TempY*2)*SCREEN_WIDTH + TempX*2), 
					CopyLength);
				i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*320+2*TempX),
					(void *)(TempScreenBuf+(TempY*2+1)*SCREEN_WIDTH +TempX*2), 
					CopyLength);
#else
				i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*SCREEN_WIDTH+2*TempX),
					(void *)(TempScreenBuf+(TempY*2)*SCREEN_WIDTH+TempX*2), 
					CopyLength);
				i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*SCREEN_WIDTH+2*TempX),
					(void *)(TempScreenBuf+(TempY*2+1)*SCREEN_WIDTH + TempX*2), 
					CopyLength);
#endif
				CopyLength = 0;			
			}
				
		}
	}
	if( CopyLength>0 )
	{
#ifdef __MODIS_DEBUG__
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*320+2*TempX),
			(void *)(TempScreenBuf+(TempY*2)*SCREEN_WIDTH +TempX*2), 
			CopyLength);
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*320+2*TempX),
			(void *)(TempScreenBuf+(TempY*2+1)*SCREEN_WIDTH +TempX*2), 
			CopyLength);
#else

		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*SCREEN_WIDTH+2*TempX),
			(void *)(TempScreenBuf+(TempY*2)*SCREEN_WIDTH +TempX*2), 
			CopyLength);
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*SCREEN_WIDTH+2*TempX),
			(void *)(TempScreenBuf+(TempY*2+1)*SCREEN_WIDTH +TempX*2), 
			CopyLength);
#endif
		CopyLength = 0;
	}
	return iTRUE;
}

// 手指点屏幕新刮开泥土的坐标增量
#define Diameter 29
iS16 PixelAdds[Diameter][3] = 
{
//	-3, 3,	-15,
	-6, 6,	-14,
	-8, 8,	-13,
	-9, 9,	-12,
	-10,	10, -11,
	-11,	11, -10,
	-12,	12, -9,
	-13,	13, -8,
	-13,	13, -7,
	-14,	14, -6,
	-14,	14, -5,
	-14,	14, -4,
	-15,	15, -3,
	-15,	15, -2,
	-15,	15, -1,
	-15,	15, 0,
	-15,	15, 1,
	-15,	15, 2,
	-15,	15, 3,
	-14,	14, 4,
	-14,	14, 5,
	-14,	14, 6,
	-13,	13, 7,
	-13,	13, 8,
	-12,	12, 9,
	-11,	11, 10,
	-10,	10, 11,
	-9, 9,	12,
	-8, 8,	13,
	-6, 6,	14,
//	-3, 3,	15

};

iBOOL mwTempScreenBufUpdate( mwLevelNum level )
{
	iRECT DesRect;
	int i = 0;
	iS16 BathX[MAX_SCREEN_TYPE][MW_LEVEL_MAX] = {98,35,219};
	iS16 BathY[MAX_SCREEN_TYPE][MW_LEVEL_MAX] = {345,375,375};
	iS16 BathW[MAX_SCREEN_TYPE] = {126};
	iS16 BathH[MAX_SCREEN_TYPE] = {115};
	iU32 RockId[MW_LEVEL_MAX] = {
			RockLevel001,
			RockLevel002,
			RockLevel003
			,RockLevel004,
			//RockLevel005,
			RockLevel006,
			//RockLevel007,
			//RockLevel008,
			RockLevel009,
			//RockLevel010,
			RockLevel011,
			//RockLevel012,
			RockLevel013,
			//RockLevel014,
			//RockLevel015
		};
	
	iS16 MaxX = 0;
	iS16 MaxY = 0;
	int X=0, Y=0;
	iRECT SrcRect;
	
//	iU32 MudId[MW_LEVEL_MAX] = {MudLevel001,MudLevel002,MudLevel003};
//iU32 OldTime = i51AdeOsGetTick();
	mwLevelDrawBKwall();
//iLog("BKwall:OldTime = %d, CurrTime=%d,TimeMul = %d", OldTime, i51AdeOsGetTick(), i51AdeOsGetTick()-OldTime);
//OldTime = i51AdeOsGetTick();

	mwLevelRedrawFG();
//iLog("mwLevelRedrawFG:OldTime = %d, CurrTime=%d,TimeMul = %d", OldTime, i51AdeOsGetTick(), i51AdeOsGetTick()-OldTime);
	
	SWMW_i51KitG2DrawImage(&iMageInfo[RockId[level-1]], iNULL, iNULL);
//iLog("Rock:OldTime = %d, CurrTime=%d,TimeMul = %d", OldTime, i51AdeOsGetTick(), i51AdeOsGetTick()-OldTime);



	mwLevelDrawPause();
	mwLevelDrawRestart();

	if(AlgaeNeed)
	{
		MaxX = SCREEN_WIDTH/2;
		MaxY = SCREEN_HEIGHT/2;
		for( Y = 0; Y< MaxY; Y++ )
		{
			for( X = 0; X< MaxX; X++ )
			{
				if( mwWorldAttribute[Y][X]> WORD_ALGAE )
				{
					DesRect.X = X*2-4;
					DesRect.Y = Y*2-4;
					DesRect.Width = 15;
					DesRect.Height = 15;

					SrcRect.X = 0;
					SrcRect.Y = ((mwWorldAttribute[Y][X]>>4)-1)*DesRect.Height;
					SrcRect.Width = DesRect.Width;
					SrcRect.Height = DesRect.Height;

					SWMW_i51KitG2DrawImage(&iMageInfo[Algae], &DesRect, &SrcRect);

					mwWorldAttribute[Y][X] = WORD_ALGAE;
					
				}
			}
		}
	}

#ifdef __MODIS_DEBUG__
	for( i=0;i<SCREEN_HEIGHT; i++)
	{
		i51AdeStdMemcpy16( (void *)(TempScreenBuf+ i*SCREEN_WIDTH),
			(void *)(mwScreenBuffer+i*320),			 
			SCREEN_WIDTH*2);
	}
#else
	i51AdeStdMemcpy16( (void *)TempScreenBuf, (void *)mwScreenBuffer, SCREEN_HEIGHT*SCREEN_WIDTH*2);
#endif


 	mwPublicAnimation(&CrocodileWait);
	mwLevelDrawDuck();

	return iTRUE;
	
}
	 iS16 OldGridding_X = 0;
	 iS16 OldGridding_Y = 0;
	 iS16 CurrGridding_X = 0;
	 iS16 CurrGridding_Y = 0;

iBOOL mwEarthCutting( mwLevelNum level)
{
/*
	static iS16 OldGridding_X = 0;
	static iS16 OldGridding_Y = 0;
	static iS16 CurrGridding_X = 0;
	static iS16 CurrGridding_Y = 0;
*/
	// 获得的按键数据
	LLPInputStruct TempKeyData;
	
	iU8 RedrawFlag = iFALSE;
	iU8 FirstKeyFlag = iFALSE;
	iU16 PenDownX = 0;
	iU16 PenDownY = 0;
	int i = 0, j =0;
	iS32 StartX = 0;
	iS32 EndX = 0;
	iS32 CurrY = 0; 
	iS16 Mul_X[8] = {-SideLength,0,SideLength,-SideLength,SideLength,-SideLength,0,SideLength};
	iS16 Mul_Y[8] = {-SideLength, -SideLength, -SideLength, 0 ,0, SideLength,SideLength,SideLength};				
	iS32 CurrMulX = 0;
	iS32 CurrMulY = 0;
	iS32 BestMoveMul = 0;
	iS32 BestMoveNum = 0;				
	iS32 CurrMoveMul = 0;
	iS16 Word_W = 0;
	iS16 Word_H = 0;
	iS16 X = 0, Y = 0;
	
	if( level == MW_LEVEL_CHOOSE )
	{
		return iTRUE;
	}

	TempKeyData.type = mwKeyData.type;
	
	while(TempKeyData.type==LLP_PEN_INPUT)
	{
		if( iNULL == FirstKeyFlag )
		{
			FirstKeyFlag = iTRUE;
			i51AdeStdMemcpy16(&TempKeyData, &mwKeyData, sizeof(LLPInputStruct));
		}
		
		if( TempKeyData.u.pen.type==i51_MSG_PADDOWN )
		{
			PenDownX = TempKeyData.u.pen.x;
			PenDownY = TempKeyData.u.pen.y;
		
			PenDownX = (PenDownX)/10*10 + 5;
			PenDownY = (PenDownY)/10*10 + 5;
			OldGridding_X = PenDownX;
			OldGridding_Y = PenDownY;
			CurrGridding_X = OldGridding_X;
			CurrGridding_Y = OldGridding_Y;
		
		}
		else if( TempKeyData.u.pen.type==i51_MSG_PADMOVE )
		{
			PenDownX = TempKeyData.u.pen.x;
			PenDownY = TempKeyData.u.pen.y;
		
			PenDownX = (PenDownX)/10*10 + 5;
			PenDownY = (PenDownY)/10*10 + 5;
			
			CurrGridding_X = PenDownX;
			CurrGridding_Y = PenDownY;

			if( (OldGridding_X%10)!=5 || (OldGridding_Y%10)!=5 )
			{
				return iFALSE;
			}
		}

		if( (OldGridding_X%10)!=5 || (OldGridding_Y%10)!=5 )
		{
//			iLog("OldGridding_X:%d,OldGridding_Y:%d",OldGridding_X,OldGridding_Y);
			return iFALSE;
		}

		while(1)
		{
			for( i = 0; i < Diameter; i++ )
			{
				StartX = OldGridding_X + PixelAdds[i][0];
				EndX = OldGridding_X + PixelAdds[i][1];
				CurrY = OldGridding_Y +PixelAdds[i][2]; 
				if( CurrY<0 || CurrY>= SCREEN_HEIGHT )
				{
					continue;
				}
				if( StartX<0 ) StartX = 0;
				if( EndX>=SCREEN_WIDTH )EndX = SCREEN_WIDTH;
				for( j = StartX; j<EndX; j++ )
				{
					if( (mwWorldAttribute[CurrY/2][j/2]&WORD_MUD) == WORD_MUD )
					{
						mwWorldAttribute[CurrY/2][j/2] = WORD_SPACE;
						GriddingMud[CurrY/SideLength][j/SideLength] |= Gridding_MUD_Update;
						RedrawFlag = iTRUE;
					}
				}
			}
			
			mwLevelGriddingUpdate(OldGridding_X/SideLength, OldGridding_Y/SideLength);
//			iLog("OldGridding_X:%d,OldGridding_Y:%d",OldGridding_X,OldGridding_Y);

			if( OldGridding_X==CurrGridding_X && OldGridding_Y==CurrGridding_Y )
			{
				break;
			}
			else 
			{
				CurrMulX = CurrGridding_X - OldGridding_X;
				CurrMulY = CurrGridding_Y - OldGridding_Y;
				BestMoveMul = AbsInt( CurrMulX-Mul_X[0]) + AbsInt(CurrMulY - Mul_Y[0]);
				BestMoveNum = 0;
				
				for(i = 1; i< 8; i++)
				{
					CurrMoveMul = AbsInt( CurrMulX-Mul_X[i]) + AbsInt(CurrMulY - Mul_Y[i]);
					if( CurrMoveMul<BestMoveMul)
					{
						BestMoveMul = CurrMoveMul;
						BestMoveNum = i;
					}
				}

				OldGridding_X += Mul_X[BestMoveNum];
				OldGridding_Y += Mul_Y[BestMoveNum];
				
			}
		}

		llp_get_input(&TempKeyData);

	}
	
	if(RedrawFlag)
	{
//iU32 count = 0;
		mwTempScreenBufUpdate(level);
		
		// 新划开的区域去除原有约束
		mwLevelWordUpdate();

		Word_W = SCREEN_WIDTH/SideLength;
		Word_H = SCREEN_HEIGHT/SideLength;
		for( Y = 0; Y < Word_H; Y++ )
		{
			for( X = 0; X < Word_W; X++ )
			{
				if( GriddingUpdataFlag[Y][X] )
				{
					mwLevelShapeAdd(X, Y, GriddingType[Y][X]);
//					iLog("X:%d,Y:%d",X,Y);
//					count++;
				}
			}
		}
//iLog("count:%d",count);
		
//		memset(GriddingUpdataFlag,0,SCREEN_HEIGHT*SCREEN_WIDTH/SideLength/SideLength);

	}

	memset(GriddingUpdataFlag,0,SCREEN_HEIGHT*SCREEN_WIDTH/SideLength/SideLength);

	return iTRUE;

}


iBOOL mwLevelDrawDuck( void  )
{
#define DUCK_ANI_NUM	3
#ifdef PKIG_SCREEN_240X320
#define WATER_DUCK_WIDTH  36
#define WATER_DUCK_HEIGHT 36

#elif  defined PKIG_SCREEN_240X400
#define WATER_DUCK_WIDTH  21
#define WATER_DUCK_HEIGHT 21


#elif defined PKIG_SCREEN_320X480
#define WATER_DUCK_WIDTH  53
#define WATER_DUCK_HEIGHT 53

#endif

	iRECT DesRect;
	iRECT SrcRect;

	//iU32 DuckId[DUCK_ANI_NUM] = {WaterDuck00,WaterDuck02,
	//	WaterDuck04,WaterDuck05,WaterDuck06,
	//	WaterDuck07,WaterDuck08,WaterDuck09,WaterDuck10};
	int i = 0;

#ifdef PKIG_SCREEN_240X320
	DesRect.X = 1;
	DesRect.Y = 4;

#elif defined PKIG_SCREEN_240X400
	DesRect.X = 15;
	DesRect.Y = 7;

#elif defined PKIG_SCREEN_320X480
	DesRect.X = 15;
	DesRect.Y = 7;

#endif
	DesRect.Width = 26;
	DesRect.Height= 23;
	
	for( i = 0; i<DUCK_NUM; i++ )
	{
		if( DuckMess[i].WaterNum<DUCK_ANI_NUM)
		{
			SrcRect.X = 0;
			SrcRect.Y = DuckMess[i].WaterNum * WATER_DUCK_HEIGHT;
			SrcRect.Width = WATER_DUCK_WIDTH;
			SrcRect.Height = WATER_DUCK_HEIGHT;
			SWMW_i51KitG2DrawImage(&iMageInfo[WaterDuck], &(DuckMess[i].Rect), &SrcRect);

			
			if(DuckMess[i].WaterNum>=DUCK_MAX_RUSH_NUM && (i51AdeOsGetTick()-DuckMess[i].OldRushTime>=300) )
			{
				DuckMess[i].OldRushTime = i51AdeOsGetTick();
				DuckMess[i].WaterNum++;
			}
		}
	}

	if( Duck_Manage.ResidueDuckNum < DUCK_NUM )
	{
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = DUCK_WIDTH;
		SrcRect.Height = DUCK_HEIGHT;
		//SWMW_i51KitG2DrawImage(mwContainer, DuckGet, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[DuckGet], &DesRect, &SrcRect);

	}
	else
	{
		SrcRect.X = 0;
		SrcRect.Y = DUCK_HEIGHT;
		SrcRect.Width = DUCK_WIDTH;
		SrcRect.Height = DUCK_HEIGHT;

		//SWMW_i51KitG2DrawImage(mwContainer, DuckGet, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[DuckGet], &DesRect, &SrcRect);

	}
	DesRect.X += DUCK_WIDTH;

	if( Duck_Manage.ResidueDuckNum < DUCK_NUM-1 )
	{
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = DUCK_WIDTH;
		SrcRect.Height = DUCK_HEIGHT;

		//SWMW_i51KitG2DrawImage(mwContainer, DuckGet, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[DuckGet], &DesRect, &SrcRect);

	}
	else
	{
		SrcRect.X = 0;
		SrcRect.Y = DUCK_HEIGHT;
		SrcRect.Width = DUCK_WIDTH;
		SrcRect.Height = DUCK_HEIGHT;

		//SWMW_i51KitG2DrawImage(mwContainer, DuckGet, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[DuckGet], &DesRect, &SrcRect);

	}
	DesRect.X += DUCK_WIDTH;

	if( Duck_Manage.ResidueDuckNum < DUCK_NUM-2 )
	{
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = DUCK_WIDTH;
		SrcRect.Height = DUCK_HEIGHT;

		//SWMW_i51KitG2DrawImage(mwContainer, DuckGet, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[DuckGet], &DesRect, &SrcRect);

	}
	else
	{
		SrcRect.X = 0;
		SrcRect.Y = DUCK_HEIGHT;
		SrcRect.Width = DUCK_WIDTH;
		SrcRect.Height = DUCK_HEIGHT;

		//SWMW_i51KitG2DrawImage(mwContainer, DuckGet, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[DuckGet], &DesRect, &SrcRect);

	}
	DesRect.X += DUCK_WIDTH;


		
	return iTRUE;

}

iBOOL mwLevelDrawOrgan( void  )
{
	int i = 0;
	
	iRECT DesRect;
	iRECT SrcRect;


	iS32 StartX = 0;
	iS32 StartY = 0;
	
//	SpeedKeep_Manage.ActiveSpeedKeepNum = 0;
//	SpeedKeepMess[0].KeepSpeedFlag = iFALSE;

	for( i = 0; i<Organ_Manage.TotalOrganNum; i++ )
	{
		if( OrganMess[i].OrgneState == ORGAN_STATE_MOVE )
		{ 
			// xie
			iS32 TempOrganX = 0, TempOrganY = 0;
			iS32 StartX = 0, EndX = 0;
			iS32 StartY = 0, EndY = 0;
			StartX = OrganMess[i].OrganStartRect.X/COORDINATE_RATE;
			StartY = OrganMess[i].OrganStartRect.Y/COORDINATE_RATE;
			EndX = (OrganMess[i].OrganStartRect.X+ OrganMess[i].OrganStartRect.Width)/COORDINATE_RATE;
			EndY = (OrganMess[i].OrganStartRect.Y+ OrganMess[i].OrganStartRect.Height)/COORDINATE_RATE;
			if( StartX<0 ) StartX = 0;
			if( StartY<0 ) StartY = 0;
			if( EndX>SCREEN_WIDTH ) EndX = SCREEN_WIDTH;
			if( EndY>SCREEN_HEIGHT ) EndY = SCREEN_HEIGHT;
			
			for(TempOrganY = StartY; TempOrganY<EndY; TempOrganY+=2 )
			{
				for(TempOrganX = StartX; TempOrganX<EndX; TempOrganX+=2 )
				{
					if( mwWorldAttribute[TempOrganY/2][TempOrganX/2] == WORD_ORGAN )
					{
						mwWorldAttribute[TempOrganY/2][TempOrganX/2] = WORD_SPACE;
					}
				}
			}
		//end
			OrganMess[i].OrganCurrRect.X += OrganMess[i].OrganMulX;
			OrganMess[i].OrganCurrRect.Y += OrganMess[i].OrganMulY;
			if
			( 
				(OrganMess[i].OrganMulX>0 && OrganMess[i].OrganCurrRect.X >= OrganMess[i].OrganEndRect.X)
			||	(OrganMess[i].OrganMulX<0 && OrganMess[i].OrganCurrRect.X <= OrganMess[i].OrganEndRect.X)
			)
			{
				OrganMess[i].OrganCurrRect.X = OrganMess[i].OrganEndRect.X;
				OrganMess[i].OrgneState = ORGAN_STATE_DES;
			}

			if
			( 
				(OrganMess[i].OrganMulY>0 && OrganMess[i].OrganCurrRect.Y >= OrganMess[i].OrganEndRect.Y)
			||	(OrganMess[i].OrganMulY<0 && OrganMess[i].OrganCurrRect.Y <= OrganMess[i].OrganEndRect.Y)
			)
			{
				OrganMess[i].OrganCurrRect.Y = OrganMess[i].OrganEndRect.Y;
				OrganMess[i].OrgneState = ORGAN_STATE_DES;
			}

			if( SpeedKeep_Manage.TotalSpeedKeepNum>0 )
			{
				SpeedKeep_Manage.ActiveSpeedKeepNum = 1;
				SpeedKeepMess[0].KeepSpeedFlag = iTRUE;
				SpeedKeepMess[0].Rect.X = OrganMess[i].OrganCurrRect.X/COORDINATE_RATE;
				if( CurrLevelNum==MW_LEVEL_012)
				{
					SpeedKeepMess[0].Rect.Y = OrganMess[i].OrganCurrRect.Y/COORDINATE_RATE-100;
					SpeedKeepMess[0].Rect.Height= OrganMess[i].OrganStartRect.Height/COORDINATE_RATE+100;
				}
				else
				{
					SpeedKeepMess[0].Rect.Y = OrganMess[i].OrganCurrRect.Y/COORDINATE_RATE;
					SpeedKeepMess[0].Rect.Height= OrganMess[i].OrganStartRect.Height/COORDINATE_RATE;
				}
				SpeedKeepMess[0].Rect.Width= OrganMess[i].OrganStartRect.Width/COORDINATE_RATE+100;
			}

		}
		else
		{
			SpeedKeep_Manage.ActiveSpeedKeepNum = 0;
			SpeedKeepMess[0].KeepSpeedFlag = iFALSE;
		}
	}

	//iLog("111 mwLevelDrawOrgan");

	if( Organ_Manage.TotalOrganNum>0 )
	{
		StartX = OrganMess[0].OrganCurrRect.X;
		StartY = OrganMess[0].OrganCurrRect.Y;

		DesRect.X = StartX/COORDINATE_RATE;
		DesRect.Y = StartY/COORDINATE_RATE;
		DesRect.Width = Organ_Key_W;
		DesRect.Height= Organ_Key_H;

		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = Organ_Key_W;
		SrcRect.Height = Organ_Key_H;

		SWMW_i51KitG2DrawImage(&iMageInfo[Organ], &DesRect, &SrcRect);


		if( OrganMess[0].Direction==ORGAN_DIR_VERTICAL )DesRect.Y += Organ_Key_H;
		else DesRect.X += Organ_Key_W;
		SrcRect.X = 0;
		SrcRect.Y = Organ_Key_H;
		SrcRect.Width = Organ_Key_W;
		SrcRect.Height = Organ_Key_H;
		//SWMW_i51KitG2DrawImage(mwContainer, Organ, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[Organ], &DesRect, &SrcRect);

		if( OrganMess[0].Direction==ORGAN_DIR_VERTICAL )DesRect.Y += Organ_Key_H;
		else DesRect.X += Organ_Key_W;
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = Organ_Key_W;
		SrcRect.Height = Organ_Key_H;
		//SWMW_i51KitG2DrawImage(mwContainer, Organ, &DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[Organ], &DesRect, &SrcRect);

		if( Organ_Manage.TotalOrganNum>1 )
		{
			StartX = OrganMess[1].OrganCurrRect.X/COORDINATE_RATE;
			StartY = OrganMess[1].OrganCurrRect.Y/COORDINATE_RATE;

			DesRect.X = StartX;
			DesRect.Y = StartY;
			DesRect.Width = Organ_Key_W;
			DesRect.Height= Organ_Key_H;

			SrcRect.X = 0;
			SrcRect.Y = Organ_Key_H * 2;
			SrcRect.Width = Organ_Key_W;
			SrcRect.Height = Organ_Key_H;
			//SWMW_i51KitG2DrawImage(mwContainer, Organ, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[Organ], &DesRect, &SrcRect);

			if( OrganMess[1].Direction==ORGAN_DIR_VERTICAL )DesRect.Y += Organ_Key_H;
			else DesRect.X += Organ_Key_W;
			SrcRect.X = 0;
			SrcRect.Y = Organ_Key_H * 3;
			SrcRect.Width = Organ_Key_W;
			SrcRect.Height = Organ_Key_H;
			//SWMW_i51KitG2DrawImage(mwContainer, Organ, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[Organ], &DesRect, &SrcRect);

			if( OrganMess[1].Direction==ORGAN_DIR_VERTICAL )DesRect.Y += Organ_Key_H;
			else DesRect.X += Organ_Key_W;
			SrcRect.X = 0;
			SrcRect.Y = Organ_Key_H * 2;
			SrcRect.Width = Organ_Key_W;
			SrcRect.Height = Organ_Key_H;

			//SWMW_i51KitG2DrawImage(mwContainer, Organ, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[Organ], &DesRect, &SrcRect);
		}	

		if( Organ_Manage.TotalOrganNum>2 )
		{
			StartX = OrganMess[2].OrganCurrRect.X/COORDINATE_RATE;
			StartY = OrganMess[2].OrganCurrRect.Y/COORDINATE_RATE;
		
			DesRect.X = StartX;
			DesRect.Y = StartY;
			DesRect.Width = Organ_Key_W;
			DesRect.Height= Organ_Key_H;

			SrcRect.X = 0;
			SrcRect.Y = Organ_Key_H * 2;
			SrcRect.Width = Organ_Key_W;
			SrcRect.Height = Organ_Key_H;

			//SWMW_i51KitG2DrawImage(mwContainer, Organ, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[Organ], &DesRect, &SrcRect);
		
			if( OrganMess[2].Direction==ORGAN_DIR_VERTICAL )DesRect.Y += Organ_Key_H;
			else DesRect.X += Organ_Key_W;
			SrcRect.X = 0;
			SrcRect.Y = Organ_Key_H * 5;
			SrcRect.Width = Organ_Key_W;
			SrcRect.Height = Organ_Key_H;

			//SWMW_i51KitG2DrawImage(mwContainer, Organ, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[Organ], &DesRect, &SrcRect);
		
			if( OrganMess[2].Direction==ORGAN_DIR_VERTICAL )DesRect.Y += Organ_Key_H;
			else DesRect.X += Organ_Key_W;
			SrcRect.X = 0;
			SrcRect.Y = Organ_Key_H * 4;
			SrcRect.Width = Organ_Key_W;
			SrcRect.Height = Organ_Key_H;

			//SWMW_i51KitG2DrawImage(mwContainer, Organ, &DesRect, &SrcRect);		
			SWMW_i51KitG2DrawImage(&iMageInfo[Organ], &DesRect, &SrcRect);
		}	

	}	
	
	//iLog("222 mwLevelDrawOrgan");

	return iTRUE;

}

iBOOL mwLevelSetImageMem( mwLevelNum level )
{

#define PUBLIC_IMAGE_MEM	19

//i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL);
	//iU32 ImageId[PUBLIC_IMAGE_MEM] = {
	//		DuckMud,DuckGet,LeadWait00,LeadWait01,
	//		LeadWait02,LeadWait03,LeadWait04,LeadWait05,
	//		LeadWait06,LeadWait07,LeadWait08,LeadWait09,
	//		LeadWait10,BKwall,WaterDuck00,WaterDuck02,
	//		LevelNum,WordLevel,BlackBK
	//		
	//	};
	iU32 ImageId[PUBLIC_IMAGE_MEM] = {
		LeadWait00,LeadWait01,/*LeadWait02,LeadWait03,
		LeadWait04,LeadWait05,LeadWait06,LeadWait07,
		LeadWait08,LeadWait09,LeadWait10,*/BKwall,
		DuckGet,WaterDuck,LevelNum,WordLevel,
		BlackBK,Pause,Restart
	};

/*
	iU32 WordLevelNum[15] = {
			WordLevel001,WordLevel002,WordLevel003,WordLevel004,WordLevel005,
			WordLevel006,WordLevel007,WordLevel008,WordLevel009,WordLevel010,
			WordLevel011,WordLevel012,WordLevel013,WordLevel014,WordLevel015,
		};
*/
	
	iU32 RockLevelNum[TOTAL_LEVEL] = {
			RockLevel001,RockLevel002,RockLevel003/*,RockLevel004,RockLevel005,
			RockLevel006,RockLevel007,RockLevel008,RockLevel009,RockLevel010,
			RockLevel011,RockLevel012,RockLevel013,RockLevel014,RockLevel015*/
		};

	iU32 MudLevelNum[TOTAL_LEVEL] = {
			MudLevel001,MudLevel002,MudLevel003/*,MudLevel004,MudLevel005,
			MudLevel006,MudLevel007,MudLevel008,MudLevel009,MudLevel010,
			MudLevel011,MudLevel012,MudLevel013,MudLevel014,MudLevel015*/
		};

	iS16 MemImage = 0;

	wmw_game_page();

	
	//i51KitG2SetImageMemoryCopy(mwContainer, PUBLIC_IMAGE_MEM, ImageId);

	ImageId[MemImage] = RockLevelNum[level-1];
	MemImage++;
	ImageId[MemImage] = MudLevelNum[level-1];
	MemImage++;

	switch( level )
	{
	case MW_LEVEL_004:
	case MW_LEVEL_005:
	case MW_LEVEL_013:
		ImageId[MemImage] = Algae;
		MemImage ++;
		break;
	case MW_LEVEL_007:
	case MW_LEVEL_009:
	case MW_LEVEL_015:
		ImageId[MemImage] = Organ;
		MemImage++;
		break;
	case MW_LEVEL_008:
	case MW_LEVEL_010:
		ImageId[MemImage] = Organ;
		MemImage++;
		ImageId[MemImage] = Algae;
		MemImage++;
		break;
	}


	if( MemImage>TOTAL_LEVEL )
	{
		iLog("MemImage:%d",MemImage);
		return iFALSE;
	}

	//i51KitG2SetImageMemoryCopy(mwContainer, MemImage, ImageId);

	return iTRUE;
	
}

iBOOL mwLevelUnsetImageMem( mwLevelNum level )
{
	iU32 ImageId[TOTAL_LEVEL];


	//iU32 WordLevelNum[15] = {
	//		WordLevel001,WordLevel002,WordLevel003,WordLevel004,WordLevel005,
	//		WordLevel006,WordLevel007,WordLevel008,WordLevel009,WordLevel010,
	//		WordLevel011,WordLevel012,WordLevel013,WordLevel014,WordLevel015,
	//	};

	iU32 RockLevelNum[TOTAL_LEVEL] = {
			RockLevel001,RockLevel002,RockLevel003/*,RockLevel004,RockLevel005,
			RockLevel006,RockLevel007,RockLevel008,RockLevel009,RockLevel010,
			RockLevel011,RockLevel012,RockLevel013,RockLevel014,RockLevel015*/
		};

	iU32 MudLevelNum[TOTAL_LEVEL] = {
			MudLevel001,MudLevel002,MudLevel003/*,MudLevel004,MudLevel005,
			MudLevel006,MudLevel007,MudLevel008,MudLevel009,MudLevel010,
			MudLevel011,MudLevel012,MudLevel013,MudLevel014,MudLevel015*/
		};
	
	switch( level ) //xie
	{
	case MW_LEVEL_001:
		//wmw_image_destroy();
		public_image_destroy(&iMageInfo[RockLevel001]);
		public_image_destroy(&iMageInfo[MudLevel001]);

		break;
	case MW_LEVEL_002:
		public_image_destroy(&iMageInfo[RockLevel002]);
		public_image_destroy(&iMageInfo[MudLevel002]);

		break;
	case MW_LEVEL_003:
		public_image_destroy(&iMageInfo[RockLevel003]);
		public_image_destroy(&iMageInfo[MudLevel003]);

		break;

	}

//	ImageId[0] = WordLevelNum[level-1];
	//iS16 MemImage = 0;
	//ImageId[MemImage] = RockLevelNum[level-1];
	//MemImage++;
	//ImageId[MemImage] = MudLevelNum[level-1];
	//MemImage++;

	//switch( level )
	//{
	//case MW_LEVEL_004:
	//case MW_LEVEL_005:
	//case MW_LEVEL_013:
	//	ImageId[MemImage] = Algae;
	//	MemImage ++;
	//	break;
	//case MW_LEVEL_007:
	//case MW_LEVEL_009:
	//case MW_LEVEL_015:
	//	ImageId[MemImage] = Organ;
	//	MemImage++;
	//	break;
	//case MW_LEVEL_008:
	//case MW_LEVEL_010:
	//	ImageId[MemImage] = Organ;
	//	MemImage++;
	//	ImageId[MemImage] = Algae;
	//	MemImage++;
	//	break;
	//}



	//if( MemImage>TOTAL_LEVEL )
	//{
	//	iLog("MemImage:%d",MemImage);
	//	return iFALSE;
	//}
	
	//i51KitG2UnsetImageMemoryCopy(mwContainer, MemImage, ImageId);

	return iTRUE;
	
}

