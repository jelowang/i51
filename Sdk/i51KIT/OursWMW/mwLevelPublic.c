

#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
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
iU8 mwWorldAttribute[SCREEN_HEIGHT_HALF][SCREEN_WIDTH_HALF] = {0};
// 关卡暂停的标记
iU8 mwWordPause = iFALSE;
// 音乐开关的标记
iU8 MusicCloseFlag = iFALSE;
// 音效开关的标记
iU8 SoundCloseFlag = iFALSE;
// 关卡开始的时间
iU32 TimeLevelStart = 0;
// 关卡当前的时间
iU32 TimeLevelCurr = 0;
// 屏幕buff备份
iU16 TempScreenBuf[SCREEN_HEIGHT][SCREEN_WIDTH] = {0}; 

#define DRAW_LINE	0

#define Organ_Key_W 32
#define Organ_Key_H 32 

// 屏幕网格管理器
static iU8  GriddingMud[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength] = {0};
// 屏幕网格状态
static iU8  GriddingType[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength] = {0};
// 屏幕网格是否需要更新的标记
static iU8  GriddingUpdataFlag[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength] = {0};
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

	i51KitG2DrawLine( Point, 0XFF00);
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
			DrawWaterDrop(body, (iShapeCircle *)shape, 0XFF0);
			break;
		case I51_SEGMENT_SHAPE:
			drawSegmentShape(body, (iShapeSeg*)shape, space);
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
			drawObject(Shape, World);
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
	CurrTotalWaterNum = 0;
//iU32 OldTime = i51AdeOsGetTick();
//iLog("Start cpWordDraw");
	drawSpace(mwSpace);
//iU32 CurrTime = i51AdeOsGetTick();
//iLog("drawSpace time mul:%d",CurrTime-OldTime);
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

//	pfunc->llp_get_input(&mwKeyData);

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
			mwInitLevel005();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_006:
			mwInitLevel006();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_007:
			mwInitLevel007();
			CurrLevelState = MW_LEVEL_RUN;
			break;
		case MW_LEVEL_008:
			mwInitLevel008();
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

iBOOL mwLevelRun( mwLevelNum level)
{
	if(!mwWordPause)	
	{
		mwEarthCutting(CurrLevelNum);
	}
	switch(level)
	{
		case MW_LEVEL_CHOOSE:
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
			mwRunLevel005();
			break;
		case MW_LEVEL_006:
			mwRunLevel006();
			break;
		case MW_LEVEL_007:
			mwRunLevel007();
			break;
		case MW_LEVEL_008:
			mwRunLevel008();
			break;
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
			iLog("mwLevelRun:%d",level);
			break;
	}

if(!mwWordPause)	
{
	cpWorldUpdata();
}

	return iTRUE;
}

iBOOL mwLevelPause( mwLevelNum level)
{
	iRECT DesRect;
	static iU8 InitFlag = iFALSE;
	iU16 PenDownX = 0;
	iU16 PenDownY = 0;

	if( iFALSE == InitFlag )
	{
		InitFlag = iTRUE;
		mwLevelDrawPause();
		
		i51KitG2SetAlpha(160);
		i51KitG2CleanScreen(0X0, iNULL);
		i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);

		DesRect.X = PAUSE_CONTINUE_X;
		DesRect.Y = PAUSE_CONTINUE_Y;
		DesRect.Width= PAUSE_CONTINUE_W;
		DesRect.Height= PAUSE_CONTINUE_H;
		i51KitG2DrawImage(mwContainer, PsContinueShow, &DesRect, iNULL);
		DesRect.X = PAUSE_CHOOSE_X;
		DesRect.Y = PAUSE_CHOOSE_Y;
		DesRect.Width= PAUSE_CHOOSE_W;
		DesRect.Height= PAUSE_CHOOSE_H;
		i51KitG2DrawImage(mwContainer, PsChsShow, &DesRect, iNULL);
		DesRect.X = PAUSE_MAIN_X;
		DesRect.Y = PAUSE_MAIN_Y;
		DesRect.Width= PAUSE_MAIN_W;
		DesRect.Height= PAUSE_MAIN_H;
		i51KitG2DrawImage(mwContainer, PsMainShow, &DesRect, iNULL);
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
			i51KitG2DrawImage(mwContainer, PsContinuePrs, &DesRect, iNULL);
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
			i51KitG2DrawImage(mwContainer, PsChsPrs, &DesRect, iNULL);
			mwWordPause = iTRUE;
			InitFlag = iFALSE;
			mwLevelUnsetImageMem(level);
			CurrLevelState = MW_LEVEL_INIT;
			CurrLevelNum = MW_LEVEL_CHOOSE;
		}
		else if( PenDownX>=PAUSE_MAIN_X&&PenDownX<(PAUSE_MAIN_X+PAUSE_MAIN_W)
		&&PenDownY>=PAUSE_MAIN_Y&&PenDownY<(PAUSE_MAIN_Y+PAUSE_MAIN_H))
		{
			DesRect.X = PAUSE_MAIN_X;
			DesRect.Y = PAUSE_MAIN_Y;
			DesRect.Width= PAUSE_MAIN_W;
			DesRect.Height= PAUSE_MAIN_H;
			i51KitG2DrawImage(mwContainer, PsMainPrs, &DesRect, iNULL);
			mwWordPause = iTRUE;
			CurrGameState = GAME_STATE_MAIN;
			i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL );
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
	
	i51KitG2GetImageAttr(Handle, ImageID, &ImageWidth, &ImageHeight, iNULL );
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
		i51KitG2DrawImage(Handle, ImageID, &DesRect, &SrcRect);
		return iTRUE;
	}
	
	while(Num>0)
	{
		SrcRect.Y = (Num%10)*DesRect.Height;
		i51KitG2DrawImage(Handle, ImageID, &DesRect, &SrcRect);
		DesRect.X -= DesRect.Width;
		Num /= 10;
	}

	return iTRUE;
	
}
	
iBOOL mwLevelWin( mwLevelNum level)
{
	iS32 TimeMul = 20000 - TimeLevelCurr + TimeLevelStart;
	static iU8 Flag = iFALSE;

#if SOUND_NEED
	if(iFALSE==SoundCloseFlag && SoundWinPlay== iFALSE)
	{
		SoundWinPlay = iTRUE;
		pfunc->public_audio_play(
			0, 
			MW_SOUND_NAME_WIN, 
			i51AdeStdWStrlen((iU16*)MW_SOUND_NAME_WIN), 
			i51_MMI_DEC_FILE_BASED,
			AUDIO_PLAYER_WAV_0, 
			iFALSE
			);
	}
#endif

	if( !Flag)
	{
		Flag = iTRUE;
		mwLevelBatheInit( level );
		if( TimeMul>0 ) CurrTimeWin = TimeMul/1000*10;
		else CurrTimeWin = 0;

		CurrDuckWin = (Duck_Manage.TotalDuckNum - Duck_Manage.ResidueDuckNum) * 100;

		TimeLevelStart = i51AdeOsGetTick();
	}
	else if( i51AdeOsGetTick() - TimeLevelStart > 3000 )
	{
	 	Flag = iFALSE;
		if( WaterInBathtub>WIN_WATER ) CurrWaterWin = (WaterInBathtub-WIN_WATER)*40;
		if( CurrWaterWin>100 ) CurrWaterWin = 100;

		CurrTotalWin = CurrTimeWin + CurrDuckWin + CurrWaterWin;

		WaterInBathtub = 0;
		
		CurrLevelState = MW_LEVEL_RESULT;
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
	i51AdeStdMemcpy16( (void *)(mwScreenBuffer),
			(void *)(&TempScreenBuf[0][0]), 
			SCREEN_WIDTH*SCREEN_HEIGHT*2);
	mwPublicAnimation(&CrocodileBathe);
	mwLevelDrawDuck();
	cpWordDraw();

	mwLevelDrawOrgan();

	return iTRUE;
}


iBOOL mwLevelResult( mwLevelNum level)
{
	static iU8 Flag = iFALSE;
	static iU32 LevelErrStartTime = 0;
	static iU8 LevelErrFlag = iFALSE;
	
	iRECT DesRect;
	iU32 ImageWidth = 0;
	iU32 ImageHeight = 0;
	
	static iU8 DrawBlankAlpha = iFALSE;
	iU16 PenDownX = 0;
	iU16 PenDownY = 0;
	iU32 ImageId[2] = {WinBk,WinCurtain};

	iFILE File = iNULL;

	iU32 Read;

	if( !Flag)
	{
		Flag = iTRUE;

		File = i51AdeOsFileOpen((iU16 *)MW_DATA_PATH, i51_OS_FL_WRITE|i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ );
		i51AdeOsFileSetSeek(File, 10, i51_OS_FL_SEEK_HEAD);
		i51AdeOsFileWriteInt32(File, &CurrTotalDuck );
		i51AdeOsFileWriteInt32(File, &UnlockMaxLevel  );
		i51AdeOsFileWrite(File, LevelDuckNum, TOTAL_LEVEL, &Read);
		i51AdeOsFileClose(File);
		

		i51KitG2SetImageMemoryCopy( mwContainer, 2, ImageId);

		i51KitG2GetImageAttr(mwContainer, WinBk, &ImageWidth, &ImageHeight, iNULL);
		DesRect.X = 0;
		DesRect.Y = 0;
		DesRect.Width = ImageWidth;
		DesRect.Height = ImageHeight;
		while(DesRect.X<SCREEN_WIDTH)
		{
			DesRect.Y = 0;
			i51KitG2DrawImage(mwContainer, WinBk, &DesRect, iNULL);
			DesRect.Y += ImageHeight;
			i51KitG2DrawImage(mwContainer, WinBk, &DesRect, iNULL);
			DesRect.Y += ImageHeight;
			i51KitG2DrawImage(mwContainer, WinBk, &DesRect, iNULL);
			DesRect.X += ImageWidth;
			i51KitG2DrawImage(mwContainer, WinBk, &DesRect, iNULL);
		}
		
		i51KitG2GetImageAttr(mwContainer, WinCurtain, &ImageWidth, &ImageHeight, iNULL);
		DesRect.X = 0;
		DesRect.Y = 0;
		DesRect.Width = ImageWidth;
		DesRect.Height = ImageHeight;
		while(DesRect.X<SCREEN_WIDTH)
		{
			i51KitG2DrawImage(mwContainer, WinCurtain, &DesRect, iNULL);
			DesRect.X += ImageWidth;
		}
		i51KitG2UnsetImageMemoryCopy( mwContainer, 2, ImageId);

		switch(Duck_Manage.ResidueDuckNum)
		{
			case 0:
				i51KitG2GetImageAttr(mwContainer, WinVery, &ImageWidth, &ImageHeight, iNULL);
				DesRect.X = 37;
				DesRect.Y = 38;
				DesRect.Width = ImageWidth;
				DesRect.Height = ImageHeight;
				i51KitG2DrawImage(mwContainer, WinVery, &DesRect, iNULL);

				i51KitG2GetImageAttr(mwContainer, WinDuckOK, &ImageWidth, &ImageHeight, iNULL);
				DesRect.X = 13;
				DesRect.Y = 78;
				DesRect.Width = ImageWidth;
				DesRect.Height = ImageHeight;
				i51KitG2DrawImage(mwContainer, WinDuckOK, &DesRect, iNULL);
				DesRect.X += 41;
				i51KitG2DrawImage(mwContainer, WinDuckOK, &DesRect, iNULL);
				DesRect.X += 41;
				i51KitG2DrawImage(mwContainer, WinDuckOK, &DesRect, iNULL);
				break;
			case 1:
				i51KitG2GetImageAttr(mwContainer, WinGood, &ImageWidth, &ImageHeight, iNULL);
				DesRect.X = 37;
				DesRect.Y = 38;
				DesRect.Width = ImageWidth;
				DesRect.Height = ImageHeight;
				i51KitG2DrawImage(mwContainer, WinGood, &DesRect, iNULL);

				i51KitG2GetImageAttr(mwContainer, WinDuckOK, &ImageWidth, &ImageHeight, iNULL);
				DesRect.X = 13;
				DesRect.Y = 78;
				DesRect.Width = ImageWidth;
				DesRect.Height = ImageHeight;
				i51KitG2DrawImage(mwContainer, WinDuckOK, &DesRect, iNULL);
				DesRect.X += 41;
				i51KitG2DrawImage(mwContainer, WinDuckOK, &DesRect, iNULL);
				DesRect.X += 41;
				i51KitG2DrawImage(mwContainer, WinDuckErr, &DesRect, iNULL);				
				break;
			case 2:
				i51KitG2GetImageAttr(mwContainer, WinOK, &ImageWidth, &ImageHeight, iNULL);
				DesRect.X = 37;
				DesRect.Y = 38;
				DesRect.Width = ImageWidth;
				DesRect.Height = ImageHeight;
				i51KitG2DrawImage(mwContainer, WinOK, &DesRect, iNULL);

				i51KitG2GetImageAttr(mwContainer, WinDuckOK, &ImageWidth, &ImageHeight, iNULL);
				DesRect.X = 13;
				DesRect.Y = 78;
				DesRect.Width = ImageWidth;
				DesRect.Height = ImageHeight;
				i51KitG2DrawImage(mwContainer, WinDuckOK, &DesRect, iNULL);
				DesRect.X += 41;
				i51KitG2DrawImage(mwContainer, WinDuckErr, &DesRect, iNULL);				
				DesRect.X += 41;
				i51KitG2DrawImage(mwContainer, WinDuckErr, &DesRect, iNULL);				
				break;
			default:
				i51KitG2GetImageAttr(mwContainer, WinDuckErr, &ImageWidth, &ImageHeight, iNULL);
				DesRect.X = 13;
				DesRect.Y = 78;
				DesRect.Width = ImageWidth;
				DesRect.Height = ImageHeight;
				i51KitG2DrawImage(mwContainer, WinDuckErr, &DesRect, iNULL);
				DesRect.X += 41;
				i51KitG2DrawImage(mwContainer, WinDuckErr, &DesRect, iNULL);				
				DesRect.X += 41;
				i51KitG2DrawImage(mwContainer, WinDuckErr, &DesRect, iNULL);				
				break;				
		}

		i51KitG2GetImageAttr(mwContainer, WinTime, &ImageWidth, &ImageHeight, iNULL);
		DesRect.X = 162;
		DesRect.Y = 37;
		DesRect.Width = ImageWidth;
		DesRect.Height = ImageHeight;
		i51KitG2DrawImage(mwContainer, WinTime, &DesRect, iNULL);
		mwLevelShowNum(mwContainer,NumSmall,CurrTimeWin,283,40);

		i51KitG2GetImageAttr(mwContainer, WinDuck, &ImageWidth, &ImageHeight, iNULL);
		DesRect.X = 162;
		DesRect.Y = 55;
		DesRect.Width = ImageWidth;
		DesRect.Height = ImageHeight;
		i51KitG2DrawImage(mwContainer, WinDuck, &DesRect, iNULL);
		mwLevelShowNum(mwContainer,NumSmall,CurrDuckWin,283,58);
	
		i51KitG2GetImageAttr(mwContainer, WinWater, &ImageWidth, &ImageHeight, iNULL);
		DesRect.X = 162;
		DesRect.Y = 73;
		DesRect.Width = ImageWidth;
		DesRect.Height = ImageHeight;
		i51KitG2DrawImage(mwContainer, WinWater, &DesRect, iNULL);
		mwLevelShowNum(mwContainer,NumSmall,CurrWaterWin,283,76);
	
		i51KitG2GetImageAttr(mwContainer, WinLine, &ImageWidth, &ImageHeight, iNULL);
		DesRect.X = 160;
		DesRect.Y = 91;
		DesRect.Width = ImageWidth;
		DesRect.Height = ImageHeight;
		i51KitG2DrawImage(mwContainer, WinLine, &DesRect, iNULL);

		i51KitG2GetImageAttr(mwContainer, WinNun, &ImageWidth, &ImageHeight, iNULL);
		DesRect.X = 167;
		DesRect.Y = 97;
		DesRect.Width = ImageWidth;
		DesRect.Height = ImageHeight;
		i51KitG2DrawImage(mwContainer, WinNun, &DesRect, iNULL);
		mwLevelShowNum(mwContainer,NumBig,CurrTotalWin,279,99);
	
		i51KitG2GetImageAttr(mwContainer, WinCrocodile, &ImageWidth, &ImageHeight, iNULL);
		DesRect.X = 0;
		DesRect.Y = 255;
		DesRect.Width = ImageWidth;
		DesRect.Height = ImageHeight;
		i51KitG2DrawImage(mwContainer, WinCrocodile, &DesRect, iNULL);

	}

	if( LevelErrFlag )
	{
		if( iFALSE == DrawBlankAlpha)
		{
			DrawBlankAlpha = iTRUE;
			i51KitG2SetAlpha(128);
			i51KitG2CleanScreen(0X0, iNULL);
			i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
		}
		if( mwPublicDrawPrompt(LevelErrStartTime, Prompt_Time, 0) )
		{
			LevelErrStartTime = 0;
			LevelErrFlag = iFALSE;
			Flag = iFALSE;
			DrawBlankAlpha = iFALSE;
		}
		return iTRUE;
	}

	if(mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
	{
		PenDownX = mwKeyData.u.pen.x;
		PenDownY = mwKeyData.u.pen.y;

		i51KitG2GetImageAttr(mwContainer, WinAgain, &ImageWidth, &ImageHeight, iNULL);
		if( PenDownX>=RESULT_AGAIN_X&&PenDownX<(RESULT_AGAIN_X+ImageWidth)
		&&PenDownY>=RESULT_AGAIN_Y&&PenDownY<(RESULT_AGAIN_Y+ImageHeight))
		{
			DesRect.X = RESULT_AGAIN_X;
			DesRect.Y = RESULT_AGAIN_Y;
			DesRect.Width= ImageWidth;
			DesRect.Height= ImageHeight;
			i51KitG2DrawImage(mwContainer, WinAgain, &DesRect, iNULL);
			mwWordPause = iFALSE;
			CurrLevelState = MW_LEVEL_INIT;
			Flag = iFALSE;
			pfunc->public_audio_stop(AUDIO_PLAYER_WAV_0);
			SoundWinPlay = iFALSE;
		}

		i51KitG2GetImageAttr(mwContainer, WinNext, &ImageWidth, &ImageHeight, iNULL);
		if( PenDownX>=RESULT_NEXT_X&&PenDownX<(RESULT_NEXT_X+ImageWidth)
		&&PenDownY>=RESULT_NEXT_Y&&PenDownY<(RESULT_NEXT_Y+ImageHeight))
		{
			DesRect.X = RESULT_NEXT_X;
			DesRect.Y = RESULT_NEXT_Y;
			DesRect.Width= ImageWidth;
			DesRect.Height= ImageHeight;
			i51KitG2DrawImage(mwContainer, WinNext, &DesRect, iNULL);
			Flag = iFALSE;
			if( CurrLevelNum>=MW_LEVEL_MAX)
//			if( CurrLevelNum>=UnlockMaxLevel)
			{
				LevelErrStartTime = i51AdeOsGetTick();
				LevelErrFlag = iTRUE;
			}
			else
			{
				mwLevelUnsetImageMem(level);
				mwWordPause = iFALSE;
				CurrLevelState = MW_LEVEL_INIT;
				CurrLevelNum = (mwLevelNum)((int)CurrLevelNum + 1);
				pfunc->public_audio_stop(AUDIO_PLAYER_WAV_0);
				SoundWinPlay = iFALSE;
			}
		}

		i51KitG2GetImageAttr(mwContainer, WinChoose, &ImageWidth, &ImageHeight, iNULL);
		if( PenDownX>=RESULT_CHOOSE_X&&PenDownX<(RESULT_CHOOSE_X+ImageWidth)
		&&PenDownY>=RESULT_CHOOSE_Y&&PenDownY<(RESULT_CHOOSE_Y+ImageHeight))
		{
			DesRect.X = RESULT_CHOOSE_X;
			DesRect.Y = RESULT_CHOOSE_Y;
			DesRect.Width= ImageWidth;
			DesRect.Height= ImageHeight;
			i51KitG2DrawImage(mwContainer, WinChoose, &DesRect, iNULL);
			mwWordPause = iTRUE;
			CurrLevelState = MW_LEVEL_INIT;
			CurrLevelNum = MW_LEVEL_CHOOSE;
			mwLevelExit(level);
			i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL);
			Flag = iFALSE;
			pfunc->public_audio_stop(AUDIO_PLAYER_WAV_0);
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

//iLog("Number1:%d,Number2:%d,ImageID:%d,Des:%X",Number1,Number2,ImageID,DesRect);
//iLog("DesRect:X=%d,Y=%d,W=%d,H=%d",DesRect->X,DesRect->Y,DesRect->Width,DesRect->Height);

	if( Number1>999 || Number2>999 || iNULL==DesRect) return iFALSE;

	i51KitG2DrawImage(Container, NumFrame, DesRect, iNULL);
//iLog("NumFrame");
	
	i51KitG2GetImageAttr(Container, ImageID, &ImageWidth, &ImageHeight, iNULL);
//iLog("1ImageWidth=%d,ImageH=%d",ImageWidth,ImageHeight);
//iLog("2ImageWidth=%d,ImageH=%d",ImageWidth,ImageHeight);
	OneNumHeight = ImageHeight/12;
//OneNumHeight = 228/12;
//iLog("3ImageWidth=%d,ImageH=%d",ImageWidth,ImageHeight);

	TempSrcRect.X = 0;
	TempSrcRect.Y = 0;
	TempSrcRect.Width = ImageWidth;
	TempSrcRect.Height= OneNumHeight;

//iLog("DesRect:0X%X",DesRect);
	TempDesRect.X = DesRect->X+8;
	TempDesRect.Y = DesRect->Y+8;
	TempDesRect.Width= DesRect->Width;
	TempDesRect.Height= DesRect->Height;
//iLog("DesRect:X=%d,Y=%d,W=%d,H=%d",DesRect->X,DesRect->Y,DesRect->Width,DesRect->Height);
//iLog("Number1:%d",Number1);
	if( Number1<10 )
	{
//iLog("Number1:%d",Number1);
//		TempDesRect.X += 8;
		TempSrcRect.Y = OneNumHeight*Number1;
		i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		TempDesRect.X += ImageWidth;
	}
	else if(Number1<100 )
	{
//iLog("Number1:%d",Number1);
//iLog("TempSrcRect:X=%d,Y=%d,W=%d,H=%d",TempSrcRect.X,TempSrcRect.Y,TempSrcRect.Width,TempSrcRect.Height);
		TempSrcRect.Y = OneNumHeight*(Number1/10);
//iLog("TempSrcRect:X=%d,Y=%d,W=%d,H=%d",TempSrcRect.X,TempSrcRect.Y,TempSrcRect.Width,TempSrcRect.Height);
		i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		TempDesRect.X += ImageWidth;
		TempSrcRect.Y = OneNumHeight*(Number1%10);
//iLog("TempSrcRect:X=%d,Y=%d,W=%d,H=%d",TempSrcRect.X,TempSrcRect.Y,TempSrcRect.Width,TempSrcRect.Height);
		i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		TempDesRect.X += ImageWidth;
	}
	else
	{
//iLog("Number1:%d",Number1);
		TempSrcRect.Y = OneNumHeight*(Number1/100);
		i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		TempDesRect.X += ImageWidth;
		TempSrcRect.Y = OneNumHeight*((Number1%100)/10);
		i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		TempDesRect.X += ImageWidth;
		TempSrcRect.Y = OneNumHeight*((Number1%10));
		i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		TempDesRect.X += ImageWidth;
	}
//iLog("Draw Num1");

	//	画反斜杠
	TempSrcRect.Y = OneNumHeight*10;
	i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
	TempDesRect.X += ImageWidth;
//iLog("Draw //");

	if( Number2<10 )
	{
		TempSrcRect.Y = OneNumHeight*Number2;
		i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
	}
	else if(Number2<100 )
	{
		TempSrcRect.Y = OneNumHeight*(Number2/10);
		i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		TempDesRect.X += ImageWidth;
		TempSrcRect.Y = OneNumHeight*(Number2%10);
		i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
	}
	else
	{
		TempSrcRect.Y = OneNumHeight*(Number2/100);
		i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		TempDesRect.X += ImageWidth;
		TempSrcRect.Y = OneNumHeight*((Number2%100)/10);
		i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
		TempDesRect.X += ImageWidth;
		TempSrcRect.Y = OneNumHeight*((Number2%10));
		i51KitG2DrawImage(Container, ImageID, &TempDesRect, &TempSrcRect);
	}
//iLog("Draw Num2");

	return iTRUE;
}

iBOOL mwLevelDrawRestart( void )
{
	iRECT DesRect;
	DesRect.X = GAME_RESTART_X;
	DesRect.Y = GAME_RESTART_Y;
	DesRect.Width= GAME_RESTART_W;
	DesRect.Height= GAME_RESTART_H;
	i51KitG2DrawImage(mwContainer, RestartShow, &DesRect, iNULL);
	return iTRUE;
}

iBOOL mwLevelPressRestart( iU16 X, iU16 Y)
{
	iRECT DesRect;
	if( X>=GAME_RESTART_X&&X<(GAME_RESTART_X+GAME_RESTART_W)
	&&Y>=GAME_RESTART_Y&&Y<(GAME_RESTART_Y+GAME_RESTART_H))
	{
		DesRect.X = GAME_RESTART_X;
		DesRect.Y = GAME_RESTART_Y;
		DesRect.Width= GAME_RESTART_W;
		DesRect.Height= GAME_RESTART_H;
		i51KitG2DrawImage(mwContainer, RestartPress, &DesRect, iNULL);
		WaterInBathtub = 0;
		return iTRUE;
	}
	return iFALSE;
}

iBOOL mwLevelDrawPause( void )
{
	iRECT DesRect;
	DesRect.X = GAME_PAUSE_X;
	DesRect.Y = GAME_PAUSE_Y;
	DesRect.Width= GAME_PAUSE_W;
	DesRect.Height= GAME_PAUSE_H;
	i51KitG2DrawImage(mwContainer, PauseShow, &DesRect, iNULL);
	return iTRUE;
}

iBOOL mwLevelPressPause( iU16 X, iU16 Y)
{
	iRECT DesRect;
	if( X>=GAME_PAUSE_X&&X<(GAME_PAUSE_X+GAME_PAUSE_W)
	&&Y>=GAME_PAUSE_Y&&Y<(GAME_PAUSE_Y+GAME_PAUSE_H))
	{
		DesRect.X = GAME_PAUSE_X;
		DesRect.Y = GAME_PAUSE_Y;
		DesRect.Width= GAME_PAUSE_W;
		DesRect.Height= GAME_PAUSE_H;
		i51KitG2DrawImage(mwContainer, PausePress, &DesRect, iNULL);
		return iTRUE;
	}
	return iFALSE;
}

iBOOL mwLevelDrawBKwall( void )
{
	iS16 BKwall_H = 60;
	iRECT DesRect;
	DesRect.X = 0;
	DesRect.Y =0;
	DesRect.Width = SCREEN_WIDTH;
	DesRect.Height = BKwall_H;

	while(DesRect.Y<SCREEN_HEIGHT)
	{
		i51KitG2DrawImage(mwContainer, BKwall, &DesRect, iNULL);
		DesRect.Y += BKwall_H;
	}

	return iTRUE;
}
iBOOL mwLevelDrawReturn( void )
{
	iRECT DesRect;
	DesRect.X = GAME_RETURN_X;
	DesRect.Y = GAME_RETURN_Y;
	DesRect.Width= GAME_RETURN_W;
	DesRect.Height= GAME_RETURN_H;
	i51KitG2DrawImage(mwContainer, ReturnShow, &DesRect, iNULL);
	return iTRUE;
}

iBOOL mwLevelPressReturn( iU16 X, iU16 Y)
{
	iRECT DesRect;
	if( X>=GAME_RETURN_X&&X<(GAME_RETURN_X+GAME_RETURN_W)
	&&Y>=GAME_RETURN_Y&&Y<(GAME_RETURN_Y+GAME_RETURN_H))
	{
		DesRect.X = GAME_RETURN_X;
		DesRect.Y = GAME_RETURN_Y;
		DesRect.Width= GAME_RETURN_W;
		DesRect.Height= GAME_RETURN_H;
		i51KitG2DrawImage(mwContainer, ReturnPress, &DesRect, iNULL);
		return iTRUE;
	}
	return iFALSE;
}

#define WORD_LEVEL_WIDTH	164
#define WORD_LEVEL_HEIGHT	22
#define STED_NUM_WIDTH	16
#define STED_NUM_HEIGHT	16

iBOOL mwLevelDrawPrompt( mwLevelNum level )
{
	iRECT RectDesLevel;
	iRECT RectDesPrompt;
	iRECT RectDesStep;
	iRECT RectDesBK;
	iRECT RectSrc;
	iU32 WordLevelNum[15] = {
			WordLevel001,WordLevel002,WordLevel003,WordLevel004,WordLevel005,
			WordLevel006,WordLevel007,WordLevel008,WordLevel009,WordLevel010,
			WordLevel011,WordLevel012,WordLevel013,WordLevel014,WordLevel015,
		};

	RectDesLevel.X = 0;
	RectDesLevel.Y = 449;
	RectDesLevel.Width = WORD_LEVEL_WIDTH;
	RectDesLevel.Height = WORD_LEVEL_HEIGHT;

	RectDesStep.X = 70;
	RectDesStep.Y = 450;
	RectDesStep.Width = 16;
	RectDesStep.Height = 19;

	RectDesPrompt.X = 78;
	RectDesPrompt.Y = 180;
	RectDesPrompt.Width = WORD_LEVEL_WIDTH;
	RectDesPrompt.Height = WORD_LEVEL_HEIGHT;

	RectDesBK.X = 24;
	RectDesBK.Y = 168;
	RectDesBK.Width = 272;
	RectDesBK.Height = 48;
	i51KitG2SetAlpha(128);
	i51KitG2DrawImage(mwContainer, BlackBK, &RectDesBK, iNULL);
	i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
	
	RectSrc.X = 0;
	RectSrc.Y = 0;
	RectSrc.Width = WORD_LEVEL_WIDTH;
	RectSrc.Height = WORD_LEVEL_HEIGHT;
	i51KitG2DrawImage(mwContainer, WordLevel, &RectDesLevel, iNULL);

	i51KitG2DrawImage(mwContainer, WordLevelNum[level-1], &RectDesPrompt, iNULL);

	RectSrc.Width = STED_NUM_WIDTH;
	RectSrc.Height = STED_NUM_HEIGHT;
	RectSrc.Y = 0;
	RectSrc.X = STED_NUM_WIDTH;
	i51KitG2DrawImage(mwContainer, LevelNum, &RectDesStep, &RectSrc);
	RectDesStep.X += STED_NUM_WIDTH;
	
	RectSrc.X = STED_NUM_WIDTH*10;
	i51KitG2DrawImage(mwContainer, LevelNum, &RectDesStep, &RectSrc);
	RectDesStep.X += STED_NUM_WIDTH;

	if(level>9)
	{
		RectSrc.X = STED_NUM_WIDTH;
		i51KitG2DrawImage(mwContainer, LevelNum, &RectDesStep, &RectSrc);
		RectDesStep.X += STED_NUM_WIDTH;
		RectSrc.X = STED_NUM_WIDTH*(level-10);
		i51KitG2DrawImage(mwContainer, LevelNum, &RectDesStep, &RectSrc);
	}
	else
	{
		RectSrc.X = STED_NUM_WIDTH*level;
		i51KitG2DrawImage(mwContainer, LevelNum, &RectDesStep, &RectSrc);
	}
	return iTRUE;
}

iBOOL mwLevelDuckInit(mwLevelNum level )
{
#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)
	iS16 Duck_X[MW_LEVEL_MAX][DUCK_NUM] = {
			{137,139,137},	{147,151,154},	{33,122,165},
			{166,227,264},	{251,259,259},	{136,75,204},
			{30,10,163},	{6,32,177}, 	{46,199,234},
			{246,94,29},	{26,244,32},	{92,196,136},
			{22,134,260},	{95,207,109},	{199,240,30}
		};
	iS16 Duck_Y[MW_LEVEL_MAX][DUCK_NUM] = { 
			{100,149,198},	{139,212,284},	{264,264,329},
			{221,210,297},	{81,163,273},	{188,294,293},
			{198,368,227},	{190,345,231},	{229,310,148},
			{47,290,378},	{273,270,101},	{42,42,293},
			{370,226,166},	{107,219,307},	{68,146,258}
		};

	iS16 Duck_W = 53;
	iS16 Duck_H = 53;
	int  i = 0;
#endif

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
#elif defined(PKIG_SCREEN_240X400)	
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
		PipeMess[0].SpeedX = 2;
		PipeMess[0].SpeedY = 2;
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
		PipeMess[0].SpeedX = 2;
		PipeMess[0].SpeedY = 2;
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
		PipeMess[0].SpeedX = 2;
		PipeMess[0].SpeedY = 2;
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
		PipeMess[0].SpeedX = 2;
		PipeMess[0].SpeedY = 2;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_005 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 98;
		PipeMess[0].RectWaterIn.Y = 390;
		PipeMess[0].RectWaterIn.Width = 28;
		PipeMess[0].RectWaterIn.Height = 20;
		PipeMess[0].WaterOutX = 53;
		PipeMess[0].WaterOutY = 267;
		PipeMess[0].SpeedX = 2;
		PipeMess[0].SpeedY = 2;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 93;
		PipeMess[1].RectWaterIn.Y = 40;
		PipeMess[1].RectWaterIn.Width = 30;
		PipeMess[1].RectWaterIn.Height = 50;
		PipeMess[1].WaterOutX = PipeOutX_HOLD;
		PipeMess[1].WaterOutY = PipeOutY_HOLD;
		PipeMess[1].SpeedX = PipeSpeedX_HOLD;
		PipeMess[1].SpeedY = PipeSpeedY_HOLD;
		Pipe_Manage.PipeNum = 2;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_006 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 150;
		PipeMess[0].RectWaterIn.Y = 440;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 233;
		PipeMess[0].WaterOutY = 386;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}		
	else if( level==MW_LEVEL_007 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 255;
		PipeMess[0].RectWaterIn.Y = 265;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 205;
		PipeMess[0].WaterOutY = 339;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
		Pipe_Manage.PipeNum = 1;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_008 )
	{
		PipeMess[0].ValidityFlag = iTRUE;
		PipeMess[0].RectWaterIn.X = 80;
		PipeMess[0].RectWaterIn.Y = 430;
		PipeMess[0].RectWaterIn.Width = 30;
		PipeMess[0].RectWaterIn.Height = 30;
		PipeMess[0].WaterOutX = 233;
		PipeMess[0].WaterOutY = 379;
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
		Pipe_Manage.PipeNum = 1;
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
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
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
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;
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
		PipeMess[1].RectWaterIn.X = 76;
		PipeMess[1].RectWaterIn.Y = 252;
		PipeMess[1].RectWaterIn.Width = 20;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 130;
		PipeMess[1].WaterOutY = 217;
		PipeMess[1].SpeedX = 16*COORDINATE_RATE;
		PipeMess[1].SpeedY = -2*COORDINATE_RATE;

		PipeMess[2].ValidityFlag = iTRUE;
		PipeMess[2].RectWaterIn.X = 182;
		PipeMess[2].RectWaterIn.Y = 210;
		PipeMess[2].RectWaterIn.Width = 20;
		PipeMess[2].RectWaterIn.Height = 20;
		PipeMess[2].WaterOutX = 238;
		PipeMess[2].WaterOutY = 262;
		PipeMess[2].SpeedX = 16*COORDINATE_RATE;
		PipeMess[2].SpeedY = -2*COORDINATE_RATE;

		PipeMess[3].ValidityFlag = iTRUE;
		PipeMess[3].RectWaterIn.X = 290;
		PipeMess[3].RectWaterIn.Y = 256;
		PipeMess[3].RectWaterIn.Width = 20;
		PipeMess[3].RectWaterIn.Height = 20;
		PipeMess[3].WaterOutX = 500;
		PipeMess[3].WaterOutY = 500;
		PipeMess[3].SpeedX = 16*COORDINATE_RATE;
		PipeMess[3].SpeedY = -2*COORDINATE_RATE;

		PipeMess[4].ValidityFlag = iTRUE;
		PipeMess[4].RectWaterIn.X = 180;
		PipeMess[4].RectWaterIn.Y = 184;
		PipeMess[4].RectWaterIn.Width = 20;
		PipeMess[4].RectWaterIn.Height = 20;
		PipeMess[4].WaterOutX = 95;
		PipeMess[4].WaterOutY = 150;
		PipeMess[4].SpeedX = -8*COORDINATE_RATE;
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
		PipeMess[0].SpeedY = 1*COORDINATE_RATE;

		PipeMess[1].ValidityFlag = iTRUE;
		PipeMess[1].RectWaterIn.X = 275;
		PipeMess[1].RectWaterIn.Y = 150;
		PipeMess[1].RectWaterIn.Width = 30;
		PipeMess[1].RectWaterIn.Height = 20;
		PipeMess[1].WaterOutX = 284;
		PipeMess[1].WaterOutY = 47;
		PipeMess[1].SpeedX = -10*COORDINATE_RATE;
		PipeMess[0].SpeedY = -2*COORDINATE_RATE;

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
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;

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
		PipeMess[0].SpeedY = 1*COORDINATE_RATE;

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
		PipeMess[0].SpeedX = 20;
		PipeMess[0].SpeedY = 5;

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
#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)

	SpeedKeep_Manage.TotalSpeedKeepNum = 0;
	SpeedKeep_Manage.ActiveSpeedKeepNum = 0;


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
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)

	switch(level)
	{
		case MW_LEVEL_001:
		case MW_LEVEL_002:
		case MW_LEVEL_003:
		case MW_LEVEL_004:
		case MW_LEVEL_005:
		case MW_LEVEL_006:
			break;
		case MW_LEVEL_007:
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
			break;
		case MW_LEVEL_008:
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
		shape = (iShape*)i51KitP2CreateShapeSeg((iHANDLE)mwSpace, staticBody, ROCK_SEG_R, tris[1],tris[2]);
		shape->ShapeGroup = OrganMess[i].OrganType;
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

iS16 BathX[MW_LEVEL_MAX][4] = {
	{120,132,190,207},	{28,43,101,120},	{213,228,287,301},
	{23,39,99,115}, 	{35,50,102,115},	{215,226,281,293},
	{186,197,252,268},	{214,226,283,294},	{76,90,144,159},
	{215,227,281,294},	{121,134,189,203},	{32,46,99,114},
	{127,140,193,206},	{214,219,279,295},	{146,157,213,225}
};
iS16 BathY[MW_LEVEL_MAX][4] = {
	{410,438,438,410},	{419,450,450,419},	{420,451,451,420}, 
	{415,449,449,415},	{310,343,343,310},	{424,453,453,424},
	{382,409,409,382},	{422,449,449,422},	{423,451,451,422},
	{217,246,246,217},	{424,454,454,424},	{421,451,451,422},
	{418,447,447,419},	{114,143,143,114},	{351,380,380,351}
};

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
	File = iNULL;
	File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\wmwdata.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
	if( iNULL==File )
	{	
		iLog("open wmwdata.re err");
		return iFALSE;
	}
	
	Flag = i51AdeOsFileSetSeek(File, ((iU16)level+1)*4, i51_OS_FL_SEEK_HEAD);
	if( iNULL==Flag )
	{
		iLog("Init set seek err!");
		return iFALSE;
	}
	
    Flag = i51AdeOsFileReadInt32( File, &MapDataOffset);   
	if( iNULL==Flag )
	{
		iLog("read map data offset err!");
		return iFALSE;
	}
	
	Flag = i51AdeOsFileSetSeek(File, MapDataOffset, i51_OS_FL_SEEK_HEAD);
	if( iNULL==Flag )
	{
		iLog("Init set seek err!");
		return iFALSE;
	}
	
    Flag = i51AdeOsFileReadInt32( File, &MapDataLength);   
	if( iNULL==Flag )
	{
		iLog("read length err!");
		return iFALSE;
	}
	
    Flag = i51AdeOsFileReadInt32( File, &ShapeNum);   
	if( iNULL==Flag )
	{
		iLog("read shape num err!");
		return iFALSE;
	}
	
    Flag = i51AdeOsFileReadInt32( File, &LoopNum);   
	if( iNULL==Flag )
	{
		iLog("read loop num err!");
		return iFALSE;
	}

	if(ShapeNum) ShapeX = i51KitP2Malloc( ShapeNum*3);
	if(LoopNum) LoopX = i51KitP2Malloc( LoopNum*2);

	if( (iNULL==ShapeX&&ShapeNum) || (iNULL==LoopX&&LoopNum) )
	{
		iLog("ShapeNum:%d,LoopNum:%d",ShapeNum, LoopNum );
		iLog("Malloc err:ShapeX:%X,LoopX:%X,%d,%d",ShapeNum*3,LoopNum*2);
		return iFALSE;
	}

	Flag = i51AdeOsFileRead(File, mwWorldAttribute, MapDataLength, &ReadDataLength);
	if( iNULL==Flag )
	{
		iLog("read map data err!");
		return iFALSE;
	}

	if(ShapeNum)
	{
		Flag = i51AdeOsFileRead(File, ShapeX, ShapeNum*3, &ReadDataLength);
		if( iNULL==Flag )
		{
			iLog("read shape err!");
			return iFALSE;
		}
	}

	if(LoopNum)
	{
		Flag = i51AdeOsFileRead(File, LoopX, LoopNum*2, &ReadDataLength);
		if( iNULL==Flag )
		{
			iLog("read loop err!");
			return iFALSE;
		}
	}

	ShapeY = ShapeX + ShapeNum;
	ShapeT = ShapeY + ShapeNum;

	LoopY = LoopX + LoopNum;
	
	i51AdeOsFileClose(File);

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

	mwLevelSetImageMem(level);
	
	mwLevelSpeedKeepInit(CurrLevelNum);

	// 清空按键队列
	while(1)
	{
		pfunc->llp_get_input(&mwKeyData);
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
#elif defined(PKIG_SCREEN_240X400)	
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
			AlgaeNeed = iTRUE;
			for( Y = 210; Y<216; Y++ )
			{
				for( X = 104; X<126; X++)
				{
					mwWorldAttribute[Y][X] = WORD_ALGAE;
				}
			}
			break;
		case MW_LEVEL_008:
			AlgaeNeed = iTRUE;
			for( Y = 152; Y<215; Y++ )
			{
				for( X = 128-(Y-152); X<128-(Y-152)+10; X++)
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
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)


	switch(level)
	{
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
		case MW_LEVEL_011:
//			mwSpace->gravity = cpv(0,150); 
			if( i51AdeOsGetTick()-OldTime >= 150 )
			{
				OldTime = i51AdeOsGetTick();

				iVectInit2(&pos,25*COORDINATE_RATE,257*COORDINATE_RATE);
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

				iVectInit2(&pos,167*COORDINATE_RATE,400*COORDINATE_RATE);
				body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
				iVectInit2(&(body->V),-18*COORDINATE_RATE,0);
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
#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)
		iS16 WAIT_X[MW_LEVEL_MAX] = {130,44,228 ,39, 44, 223,196,224,88,224,131,40,136,224,155};
		iS16 WAIT_Y[MW_LEVEL_MAX] = {352,363,363,359,251,369, 324,363,365,159,369,365,360,55,295 };
		iS16 WAIT_W = 72;
		iS16 WAIT_H = 90;
		iS16 TotalFrame = 11; 
		iU32 ImageId[11] = 
			{
				LeadWait00,
				LeadWait01,
				LeadWait02,
				LeadWait03,
				LeadWait04,
				LeadWait05,
				LeadWait06,
				LeadWait07,
				LeadWait08,
				LeadWait09,
				LeadWait10
			};
#endif
	CrocodileWait.Container = mwContainer;
	CrocodileWait.CurrPlayFrame = 0;
	CrocodileWait.DesRect.X = WAIT_X[level-1];
	CrocodileWait.DesRect.Y = WAIT_Y[level-1];
	CrocodileWait.DesRect.Width = WAIT_W;
	CrocodileWait.DesRect.Height = WAIT_H;
	CrocodileWait.FrameTimeMul = 250;
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
#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)
		iS16 BANTH_X[MW_LEVEL_MAX] = {130,44,228 ,39, 44, 223,196,224,88,224,131,40,136,224,155};
		iS16 BANTH_Y[MW_LEVEL_MAX] = {352,363,363,359,251,369, 324,363,365,159,369,365,360,55,295 };
		iS16 BANTH_W = 72;
		iS16 BANTH_H = 90;
		iS16 TotalFrame = 3; 
		iU32 ImageId[3] = 
			{
				LeadBathe00,
				LeadBathe01,
				LeadBathe02
			};
		
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
			i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*SCREEN_WIDTH+2*TempX),
				(void *)(&TempScreenBuf[TempY*2][TempX*2]), 
				CopyLength);
			i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*SCREEN_WIDTH+2*TempX),
				(void *)(&TempScreenBuf[TempY*2+1][TempX*2]), 
				CopyLength);
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
				i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*SCREEN_WIDTH+2*TempX),
					(void *)(&TempScreenBuf[TempY*2][TempX*2]), 
					CopyLength);
				i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*SCREEN_WIDTH+2*TempX),
					(void *)(&TempScreenBuf[TempY*2+1][TempX*2]), 
					CopyLength);
				CopyLength = 0;			
			}
				
		}
	}
	if( CopyLength>0 )
	{
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*SCREEN_WIDTH+2*TempX),
			(void *)(&TempScreenBuf[TempY*2][TempX*2]), 
			CopyLength);
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*SCREEN_WIDTH+2*TempX),
			(void *)(&TempScreenBuf[TempY*2+1][TempX*2]), 
			CopyLength);
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
			i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*SCREEN_WIDTH+2*TempX),
				(void *)(&TempScreenBuf[TempY*2][TempX*2]), 
				CopyLength);
			i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*SCREEN_WIDTH+2*TempX),
				(void *)(&TempScreenBuf[TempY*2+1][TempX*2]), 
				CopyLength);
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
				i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*SCREEN_WIDTH+2*TempX),
					(void *)(&TempScreenBuf[TempY*2][TempX*2]), 
					CopyLength);
				i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*SCREEN_WIDTH+2*TempX),
					(void *)(&TempScreenBuf[TempY*2+1][TempX*2]), 
					CopyLength);
				CopyLength = 0;			
			}
				
		}
	}
	if( CopyLength>0 )
	{
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+2*TempY*SCREEN_WIDTH+2*TempX),
			(void *)(&TempScreenBuf[TempY*2][TempX*2]), 
			CopyLength);
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+(2*TempY+1)*SCREEN_WIDTH+2*TempX),
			(void *)(&TempScreenBuf[TempY*2+1][TempX*2]), 
			CopyLength);
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
	iS16 BathX[MAX_SCREEN_TYPE][MW_LEVEL_MAX] = {98,35,219};
	iS16 BathY[MAX_SCREEN_TYPE][MW_LEVEL_MAX] = {345,375,375};
	iS16 BathW[MAX_SCREEN_TYPE] = {126};
	iS16 BathH[MAX_SCREEN_TYPE] = {115};
	iU32 RockId[MW_LEVEL_MAX] = {
			RockLevel001,RockLevel002,RockLevel003,RockLevel004,RockLevel005,
			RockLevel006,RockLevel007,RockLevel008,RockLevel009,RockLevel010,
			RockLevel011,RockLevel012,RockLevel013,RockLevel014,RockLevel015
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
	
//OldTime = i51AdeOsGetTick();
	i51KitG2DrawImage(mwContainer, RockId[level-1], iNULL, iNULL);
//iLog("Rock:OldTime = %d, CurrTime=%d,TimeMul = %d", OldTime, i51AdeOsGetTick(), i51AdeOsGetTick()-OldTime);
	//i51KitG2DrawImage(mwContainer, MudId[level-1], iNULL, iNULL);
	
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

					i51KitG2DrawImage(mwContainer, Algae, &DesRect, &SrcRect);

					mwWorldAttribute[Y][X] = WORD_ALGAE;
					
				}
			}
		}
	}
	
	i51AdeStdMemcpy16( (void *)TempScreenBuf, (void *)mwScreenBuffer, SCREEN_HEIGHT*SCREEN_WIDTH*2);

 	mwPublicAnimation(&CrocodileWait);
	mwLevelDrawDuck();

	return iTRUE;
	
}

iBOOL mwEarthCutting( mwLevelNum level)
{
	static iS16 OldGridding_X = 0;
	static iS16 OldGridding_Y = 0;
	static iS16 CurrGridding_X = 0;
	static iS16 CurrGridding_Y = 0;

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

		pfunc->llp_get_input(&TempKeyData);

	}
	
	if(RedrawFlag)
	{
iU32 count = 0;
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
					count++;
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
#define DUCK_ANI_NUM	9

	iRECT DesRect;
	iU32 DuckId[DUCK_ANI_NUM] = {WaterDuck00,WaterDuck02,
		WaterDuck04,WaterDuck05,WaterDuck06,
		WaterDuck07,WaterDuck08,WaterDuck09,WaterDuck10};
	int i = 0;

	DesRect.X = 15;
	DesRect.Y = 7;
	DesRect.Width = 53;
	DesRect.Height= 53;
	
	for( i = 0; i<DUCK_NUM; i++ )
	{
		if( DuckMess[i].WaterNum<DUCK_ANI_NUM)
		{
//			i51KitG2DrawImage(mwContainer, DuckMud, &(DuckMess[i].Rect), iNULL);
			i51KitG2DrawImage(mwContainer, DuckId[DuckMess[i].WaterNum], &(DuckMess[i].Rect), iNULL);
//			i51KitG2DrawImage(mwContainer, DuckMud, &DesRect, iNULL);
			
			if(DuckMess[i].WaterNum>=DUCK_MAX_RUSH_NUM && (i51AdeOsGetTick()-DuckMess[i].OldRushTime>=100) )
			{
				DuckMess[i].OldRushTime = i51AdeOsGetTick();
				DuckMess[i].WaterNum++;
			}
		}
	}

	if( Duck_Manage.ResidueDuckNum < DUCK_NUM )
	{
		i51KitG2DrawImage(mwContainer, DuckGet, &DesRect, iNULL);
	}
	else
	{
		i51KitG2DrawImage(mwContainer, DuckMud, &DesRect, iNULL);
	}
	DesRect.X += DUCK_WIDTH;

	if( Duck_Manage.ResidueDuckNum < DUCK_NUM-1 )
	{
		i51KitG2DrawImage(mwContainer, DuckGet, &DesRect, iNULL);
	}
	else
	{
		i51KitG2DrawImage(mwContainer, DuckMud, &DesRect, iNULL);
	}
	DesRect.X += DUCK_WIDTH;

	if( Duck_Manage.ResidueDuckNum < DUCK_NUM-2 )
	{
		i51KitG2DrawImage(mwContainer, DuckGet, &DesRect, iNULL);
	}
	else
	{
		i51KitG2DrawImage(mwContainer, DuckMud, &DesRect, iNULL);
	}
	DesRect.X += DUCK_WIDTH;
		
	return iTRUE;

}

iBOOL mwLevelDrawOrgan( void  )
{
	int i = 0;
	
	iRECT DesRect;

	iS32 StartX = 0;
	iS32 StartY = 0;
	
//	SpeedKeep_Manage.ActiveSpeedKeepNum = 0;
//	SpeedKeepMess[0].KeepSpeedFlag = iFALSE;

	for( i = 0; i<Organ_Manage.TotalOrganNum; i++ )
	{
		if( OrganMess[i].OrgneState == ORGAN_STATE_MOVE )
		{
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

	if( Organ_Manage.TotalOrganNum>0 )
	{
		StartX = OrganMess[0].OrganCurrRect.X;
		StartY = OrganMess[0].OrganCurrRect.Y;

		DesRect.X = StartX/COORDINATE_RATE;
		DesRect.Y = StartY/COORDINATE_RATE;
		DesRect.Width = Organ_Key_W;
		DesRect.Height= Organ_Key_H;
		i51KitG2DrawImage(mwContainer, OrganRed3, &DesRect, iNULL);

		if( OrganMess[0].Direction==ORGAN_DIR_VERTICAL )DesRect.Y += Organ_Key_H;
		else DesRect.X += Organ_Key_W;
		i51KitG2DrawImage(mwContainer, OrganRed1, &DesRect, iNULL);

		if( OrganMess[0].Direction==ORGAN_DIR_VERTICAL )DesRect.Y += Organ_Key_H;
		else DesRect.X += Organ_Key_W;
		i51KitG2DrawImage(mwContainer, OrganRed3, &DesRect, iNULL);

		if( Organ_Manage.TotalOrganNum>1 )
		{
			StartX = OrganMess[1].OrganCurrRect.X/COORDINATE_RATE;
			StartY = OrganMess[1].OrganCurrRect.Y/COORDINATE_RATE;

			DesRect.X = StartX;
			DesRect.Y = StartY;
			DesRect.Width = Organ_Key_W;
			DesRect.Height= Organ_Key_H;
			i51KitG2DrawImage(mwContainer, OrganGreen3, &DesRect, iNULL);

			if( OrganMess[1].Direction==ORGAN_DIR_VERTICAL )DesRect.Y += Organ_Key_H;
			else DesRect.X += Organ_Key_W;
			i51KitG2DrawImage(mwContainer, OrganGreen1, &DesRect, iNULL);

			if( OrganMess[1].Direction==ORGAN_DIR_VERTICAL )DesRect.Y += Organ_Key_H;
			else DesRect.X += Organ_Key_W;
			i51KitG2DrawImage(mwContainer, OrganGreen3, &DesRect, iNULL);
		}	

		if( Organ_Manage.TotalOrganNum>2 )
		{
			StartX = OrganMess[2].OrganCurrRect.X/COORDINATE_RATE;
			StartY = OrganMess[2].OrganCurrRect.Y/COORDINATE_RATE;
		
			DesRect.X = StartX;
			DesRect.Y = StartY;
			DesRect.Width = Organ_Key_W;
			DesRect.Height= Organ_Key_H;
			i51KitG2DrawImage(mwContainer, OrganBlue3, &DesRect, iNULL);
		
			if( OrganMess[2].Direction==ORGAN_DIR_VERTICAL )DesRect.Y += Organ_Key_H;
			else DesRect.X += Organ_Key_W;
			i51KitG2DrawImage(mwContainer, OrganBlue1, &DesRect, iNULL);
		
			if( OrganMess[2].Direction==ORGAN_DIR_VERTICAL )DesRect.Y += Organ_Key_H;
			else DesRect.X += Organ_Key_W;
			i51KitG2DrawImage(mwContainer, OrganBlue3, &DesRect, iNULL);		
		}	

	}	
	
	return iTRUE;

}

iBOOL mwLevelSetImageMem( mwLevelNum level )
{

#define PUBLIC_IMAGE_MEM	19

//i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL);
	iU32 ImageId[PUBLIC_IMAGE_MEM] = {
			DuckMud,DuckGet,LeadWait00,LeadWait01,
			LeadWait02,LeadWait03,LeadWait04,LeadWait05,
			LeadWait06,LeadWait07,LeadWait08,LeadWait09,
			LeadWait10,BKwall,WaterDuck00,WaterDuck02,
			LevelNum,WordLevel,BlackBK
			
		};

/*
	iU32 WordLevelNum[15] = {
			WordLevel001,WordLevel002,WordLevel003,WordLevel004,WordLevel005,
			WordLevel006,WordLevel007,WordLevel008,WordLevel009,WordLevel010,
			WordLevel011,WordLevel012,WordLevel013,WordLevel014,WordLevel015,
		};
*/
	
	iU32 RockLevelNum[TOTAL_LEVEL] = {
			RockLevel001,RockLevel002,RockLevel003,RockLevel004,RockLevel005,
			RockLevel006,RockLevel007,RockLevel008,RockLevel009,RockLevel010,
			RockLevel011,RockLevel012,RockLevel013,RockLevel014,RockLevel015
		};

	iU32 MudLevelNum[TOTAL_LEVEL] = {
			MudLevel001,MudLevel002,MudLevel003,MudLevel004,MudLevel005,
			MudLevel006,MudLevel007,MudLevel008,MudLevel009,MudLevel010,
			MudLevel011,MudLevel012,MudLevel013,MudLevel014,MudLevel015
		};

	iS16 MemImage = 0;

	
	i51KitG2SetImageMemoryCopy(mwContainer, PUBLIC_IMAGE_MEM, ImageId);

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
		case MW_LEVEL_015:
			ImageId[MemImage] = OrganRed1;
			MemImage++;
			ImageId[MemImage] = OrganRed3;
			MemImage++;
			break;
		case MW_LEVEL_008:
			ImageId[MemImage] = OrganRed1;
			MemImage++;
			ImageId[MemImage] = OrganRed3;
			MemImage++;
			ImageId[MemImage] = OrganGreen1;
			MemImage++;
			ImageId[MemImage] = OrganGreen3;
			MemImage++;
			ImageId[MemImage] = Algae;
			MemImage++;
			break;
		case MW_LEVEL_009:
			ImageId[MemImage] = OrganRed1;
			MemImage++;
			ImageId[MemImage] = OrganRed3;
			MemImage++;
			ImageId[MemImage] = OrganGreen1;
			MemImage++;
			ImageId[MemImage] = OrganGreen3;
			MemImage++;
			break;
		case MW_LEVEL_010:
			ImageId[MemImage] = OrganRed1;
			MemImage++;
			ImageId[MemImage] = OrganRed3;
			MemImage++;
			ImageId[MemImage] = OrganGreen1;
			MemImage++;
			ImageId[MemImage] = OrganGreen3;
			MemImage++;
			ImageId[MemImage] = OrganBlue1;
			MemImage++;
			ImageId[MemImage] = OrganBlue3;
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

	i51KitG2SetImageMemoryCopy(mwContainer, MemImage, ImageId);

	return iTRUE;
	
}

iBOOL mwLevelUnsetImageMem( mwLevelNum level )
{
	iU32 ImageId[TOTAL_LEVEL];


	iU32 WordLevelNum[15] = {
			WordLevel001,WordLevel002,WordLevel003,WordLevel004,WordLevel005,
			WordLevel006,WordLevel007,WordLevel008,WordLevel009,WordLevel010,
			WordLevel011,WordLevel012,WordLevel013,WordLevel014,WordLevel015,
		};

	iU32 RockLevelNum[TOTAL_LEVEL] = {
			RockLevel001,RockLevel002,RockLevel003,RockLevel004,RockLevel005,
			RockLevel006,RockLevel007,RockLevel008,RockLevel009,RockLevel010,
			RockLevel011,RockLevel012,RockLevel013,RockLevel014,RockLevel015
		};

	iU32 MudLevelNum[TOTAL_LEVEL] = {
			MudLevel001,MudLevel002,MudLevel003,MudLevel004,MudLevel005,
			MudLevel006,MudLevel007,MudLevel008,MudLevel009,MudLevel010,
			MudLevel011,MudLevel012,MudLevel013,MudLevel014,MudLevel015
		};
	

//	ImageId[0] = WordLevelNum[level-1];
	iS16 MemImage = 0;
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
		case MW_LEVEL_015:
			ImageId[MemImage] = OrganRed1;
			MemImage++;
			ImageId[MemImage] = OrganRed3;
			MemImage++;
			break;
		case MW_LEVEL_008:
			ImageId[MemImage] = OrganRed1;
			MemImage++;
			ImageId[MemImage] = OrganRed3;
			MemImage++;
			ImageId[MemImage] = OrganGreen1;
			MemImage++;
			ImageId[MemImage] = OrganGreen3;
			MemImage++;
			ImageId[MemImage] = Algae;
			MemImage++;
			break;
		case MW_LEVEL_009:
			ImageId[MemImage] = OrganRed1;
			MemImage++;
			ImageId[MemImage] = OrganRed3;
			MemImage++;
			ImageId[MemImage] = OrganGreen1;
			MemImage++;
			ImageId[MemImage] = OrganGreen3;
			MemImage++;
			break;
		case MW_LEVEL_010:
			ImageId[MemImage] = OrganRed1;
			MemImage++;
			ImageId[MemImage] = OrganRed3;
			MemImage++;
			ImageId[MemImage] = OrganGreen1;
			MemImage++;
			ImageId[MemImage] = OrganGreen3;
			MemImage++;
			ImageId[MemImage] = OrganBlue1;
			MemImage++;
			ImageId[MemImage] = OrganBlue3;
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
	
	i51KitG2UnsetImageMemoryCopy(mwContainer, MemImage, ImageId);

	return iTRUE;
	
}

