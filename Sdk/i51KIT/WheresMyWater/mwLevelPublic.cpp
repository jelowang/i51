

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

static void update(int ticks)
{
	int steps = 1;
	cpFloat dt = 1.0f/60.0f;
	
	for(int i=0; i<steps; i++){
		cpSpaceStep(mwSpace, dt);
	}
}

typedef struct drawSpaceOptions {
	int drawHash;
	int drawBBs;
	int drawShapes;
	float collisionPointSize;
	float bodyPointSize;
	float lineThickness;
} drawSpaceOptions;

drawSpaceOptions options = {
	0,
	0,
	1,
	4.0f,
	0.0f,
	1.5f,
};

void drawSegmentShape(cpBody *body, cpSegmentShape *seg, cpSpace *space)
{
#if DRAW_LINE
	cpVect a = seg->ta;
	cpVect b = seg->tb;

	iS32 Point[2][2] = {0};// = {(iS32)a.x,(iS32)a.y,(iS32)b.x,(iS32)b.y};
	Point[0][0]= (iS32)(a.x);
	Point[0][1] = (iS32)(a.y);
	Point[1][0]= (iS32)(b.x);
	Point[1][1]= (iS32)(b.y);
	i51KitG2DrawLine( Point, 0XFF00);
#endif	
}

//iS32 CurrX = 0;
//iS32 CurrY = 0;

static void
drawPolyShape(cpBody *body, cpPolyShape *poly, cpSpace *space)
{
	int count = poly->numVerts;
	int i = 0;
	iS32 PolygonPoint[10][2]= {0};
	cpVect * verts = poly->verts;

	for( int i=0; i<Organ_Manage.TotalOrganNum; i++ )
	{
		cpShape * TempShape = (cpShape*)poly;
		if(	OrganMess[i].OrgneState != ORGAN_STATE_SRC
		&&	TempShape->block.ShapeType == OrganMess[i].OrganType )
		{
			iS16 StartX = OrganMess[i].OrganCurrRect.X;
			iS16 StartY = OrganMess[i].OrganCurrRect.Y;
			iS16 EndX = StartX + OrganMess[i].OrganStartRect.Width;
			iS16 EndY = StartY + OrganMess[i].OrganStartRect.Height;
			
			cpVect tris[4];
			tris[0] = cpv(StartX,StartY);
			tris[1] = cpv(StartX,EndY);
			tris[2] = cpv(EndX,EndY);
			tris[3] = cpv(EndX,StartY);

			cpSpaceRemoveShape(mwSpace, (cpShape*)(poly));
			cpShapeFree((cpShape*)(poly));
			
			cpBody * staticBody = (cpBody*)&(mwSpace->staticBody);
			cpShape * shape = iNULL;
			shape = cpSpaceAddShape(mwSpace, cpPolyShapeNew(staticBody, 4, tris, cpvzero));
			shape->e = SHAPE_E; 
			shape->u = SHAPE_U; 
			shape->block.ShapeType = OrganMess[i].OrganType;
			OrganMess[i].OrganType = SHAPE_TYPE_UPDATA;

			verts = ((cpPolyShape *)shape)->verts;
#if 0			
			cpBody * body = cpSpaceAddBody(mwSpace, cpBodyNew(1.0f, cpMomentForPoly(1.0f, 4, tris, cpvzero)));
			cpShape * shape = cpSpaceAddShape(mwSpace, cpPolyShapeNew(body, 4, tris, cpvzero));
			shape->e = SHAPE_E; 
			shape->u = SHAPE_U; 
			shape->block.ShapeType = OrganMess[i].OrganType;

			cpBody * staticBody = (cpBody*)(&mwSpace->staticBody);

			cpVect ta = cpv(OrganMess[i].OrganCurrRect.X, OrganMess[i].OrganCurrRect.Y);
			cpVect tb = cpv(OrganMess[i].OrganCurrRect.X + OrganMess[i].OrganStartRect.Width,
				OrganMess[i].OrganCurrRect.Y + OrganMess[i].OrganStartRect.Height);
			
			cpSpaceRemoveShape(mwSpace, (cpShape*)(seg));
			cpShapeFree((cpShape*)(seg));
			TempShape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, ta,tb,RECT_SEG_R));
			TempShape->e = SHAPE_E; 
			TempShape->u = SHAPE_U; 
			TempShape->block.ShapeType = OrganMess[i].OrganType;
			OrganMess[i].OrganType = SHAPE_TYPE_UPDATA;
#endif			
		}
	}	
#if DRAW_LINE	
	for( i = 0; i< count; i++ )
	{
		 PolygonPoint[i][0]= (iS32)(verts[i].x);
		 PolygonPoint[i][1] = (iS32)(verts[i].y);
	}

	i51KitG2DrawPolygon(count, PolygonPoint, 0XF800);
#endif
}

static void
drawObject(cpShape *shape, cpSpace *space)
{
	cpBody *body = shape->body;
	
	switch(shape->klass->type){
		case CP_CIRCLE_SHAPE:
			DrawWaterDrop(body, (cpCircleShape *)shape, 0XFF0);
			break;
		case CP_SEGMENT_SHAPE:
			drawSegmentShape(body, (cpSegmentShape *)shape, space);
			break;
		case CP_POLY_SHAPE:
			drawPolyShape(body, (cpPolyShape *)shape, space);
			break;
		default:
			break;
	}
}

void
drawSpace(cpSpace *space, drawSpaceOptions *options)
{
	if(options->drawShapes){
		cpSpaceHashEach(space->activeShapes, (cpSpaceHashIterator)drawObject, space);
		cpSpaceHashEach(space->staticShapes, (cpSpaceHashIterator)drawObject, space);
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
	drawSpace(mwSpace, &options);
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
	if(!mwSpace)
	{
		cpInitChipmunk();
		cpResetShapeIdCounter();
	}
	else
	{
		cpSpaceFreeChildren(mwSpace);
		cpSpaceFree(mwSpace);
		//mwSpace = iNULL;
	}
	
	mwSpace= cpSpaceNew();	
	mwSpace->iterations = 1;
	mwSpace->elasticIterations = 10;	
	mwSpace->gravity = cpv(0, 500);	
//	mwSpace->sleepTimeThreshold = 0.2f;

	
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
};

iBOOL mwLevelExit( mwLevelNum level)
{
	if( mwSpace )
	{
		cpSpaceFreeChildren(mwSpace);
		cpSpaceFree(mwSpace);
		mwSpace = iNULL;
	}
	return iTRUE;
}

iBOOL mwLevelInit( mwLevelNum level)
{
	// 清除关卡暂停标记
	mwWordPause = iFALSE;
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
		iU16 PenDownX = mwKeyData.u.pen.x;
		iU16 PenDownY = mwKeyData.u.pen.y;

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
	
	if( !Flag)
	{
		Flag = iTRUE;
		iU32 ImageId[2] = {WinBk,WinCurtain};

		iFILE File = i51AdeOsFileOpen((iU16 *)MW_DATA_PATH, i51_OS_FL_WRITE|i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ );
		i51AdeOsFileSetSeek(File, 10, i51_OS_FL_SEEK_HEAD);
		i51AdeOsFileWriteInt32(File, &CurrTotalDuck );
		i51AdeOsFileWriteInt32(File, &UnlockMaxLevel  );
		iU32 Read;
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
		static iU8 DrawBlankAlpha = iFALSE;

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
		iU16 PenDownX = mwKeyData.u.pen.x;
		iU16 PenDownY = mwKeyData.u.pen.y;

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
	if( X>=GAME_RESTART_X&&X<(GAME_RESTART_X+GAME_RESTART_W)
	&&Y>=GAME_RESTART_Y&&Y<(GAME_RESTART_Y+GAME_RESTART_H))
	{
		iRECT DesRect;
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
	if( X>=GAME_PAUSE_X&&X<(GAME_PAUSE_X+GAME_PAUSE_W)
	&&Y>=GAME_PAUSE_Y&&Y<(GAME_PAUSE_Y+GAME_PAUSE_H))
	{
		iRECT DesRect;
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
	if( X>=GAME_RETURN_X&&X<(GAME_RETURN_X+GAME_RETURN_W)
	&&Y>=GAME_RETURN_Y&&Y<(GAME_RETURN_Y+GAME_RETURN_H))
	{
		iRECT DesRect;
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
			{30,10,163},	{6,32,177},		{46,199,234},
			{246,94,29},	{26,244,32},	{92,196,136},
			{22,134,260},	{95,207,109},	{199,240,30}
		};
	iS16 Duck_Y[MW_LEVEL_MAX][DUCK_NUM] = { 
			{100,149,198},	{139,212,284},	{264,264,329},
			{221,226,297},	{81,163,273},	{188,294,293},
			{198,368,227},	{190,339,231},	{229,310,148},
			{47,290,378},	{273,270,101},	{42,42,293},
			{370,226,166},	{107,219,307},	{68,146,258}
		};
	iS16 Duck_W = 53;
	iS16 Duck_H = 53;
#endif

	for( int i=0; i<DUCK_NUM; i++ )
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
	if( level==MW_LEVEL_001 )
	{
		#define PipeNum_001 1
		iS16 WaterIn_X[PipeNum_001] = {150};
		iS16 WaterIn_Y[PipeNum_001] = {280};
		iS16 WaterIn_W[PipeNum_001] = {32};
		iS16 WaterIn_H[PipeNum_001] = {20};
		iS16 WaterOut_X[PipeNum_001] = {145};
		iS16 WaterOut_Y[PipeNum_001] = {375};
		iS16 OutSpeed_X[PipeNum_001] = {20};
		iS16 OutSpeed_Y[PipeNum_001] = {5};

		for( int i=0; i<PipeNum_001; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_001;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_002 )
	{
	#define PipeNum_002 1
		iS16 WaterIn_X[PipeNum_002] = {150};
		iS16 WaterIn_Y[PipeNum_002] = {360};
		iS16 WaterIn_W[PipeNum_002] = {40};
		iS16 WaterIn_H[PipeNum_002] = {25};
		iS16 WaterOut_X[PipeNum_002] = {54};
		iS16 WaterOut_Y[PipeNum_002] = {382};
		iS16 OutSpeed_X[PipeNum_002] = {20};
		iS16 OutSpeed_Y[PipeNum_002] = {5};

		for( int i=0; i<PipeNum_002; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_002;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_003 )
	{
	#define PipeNum_003 1
		iS16 WaterIn_X[PipeNum_003] = {40};
		iS16 WaterIn_Y[PipeNum_003] = {410};
		iS16 WaterIn_W[PipeNum_003] = {30};
		iS16 WaterIn_H[PipeNum_003] = {20};
		iS16 WaterOut_X[PipeNum_003] = {238};
		iS16 WaterOut_Y[PipeNum_003] = {382};
		iS16 OutSpeed_X[PipeNum_003] = {20};
		iS16 OutSpeed_Y[PipeNum_003] = {5};

		for( int i=0; i<PipeNum_003; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_003;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_004 )
	{
		#define PipeNum_004 1
		iS16 WaterIn_X[PipeNum_004] = {275};//,8,40,126};
		iS16 WaterIn_Y[PipeNum_004] = {385};//,260,284,282};
		iS16 WaterIn_W[PipeNum_004] = {30};//,20,10,10};
		iS16 WaterIn_H[PipeNum_004] = {30};//,10,20,20};
		iS16 WaterOut_X[PipeNum_004] = {50};//,23,54,138};
		iS16 WaterOut_Y[PipeNum_004] = {376};//,273,286,288};
		iS16 OutSpeed_X[PipeNum_004] = {20};//,PipeSpeedXFromY,PipeSpeedXFromY,PipeSpeedX_HOLD};
		iS16 OutSpeed_Y[PipeNum_004] = {5};//,PipeSpeedY_HOLD,PipeSpeedYZeor,PipeSpeedY_INYX};

		for( int i=0; i<PipeNum_004; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_004;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_005 )
	{
		#define PipeNum_005 2
		iS16 WaterIn_X[PipeNum_005] = {98,93};
		iS16 WaterIn_Y[PipeNum_005] = {390,40};
		iS16 WaterIn_W[PipeNum_005] = {28,30};
		iS16 WaterIn_H[PipeNum_005] = {20,50};
		iS16 WaterOut_X[PipeNum_005] = {53,PipeOutX_HOLD};
		iS16 WaterOut_Y[PipeNum_005] = {267,PipeOutY_HOLD};
		iS16 OutSpeed_X[PipeNum_005] = {20,PipeSpeedX_HOLD};
		iS16 OutSpeed_Y[PipeNum_005] = {5,PipeSpeedY_HOLD};

		for( int i=0; i<PipeNum_005; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_005;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_006 )
	{
		#define PipeNum_006 1
		iS16 WaterIn_X[PipeNum_006] = {150};
		iS16 WaterIn_Y[PipeNum_006] = {440};
		iS16 WaterIn_W[PipeNum_006] = {30};
		iS16 WaterIn_H[PipeNum_006] = {30};
		iS16 WaterOut_X[PipeNum_006] = {233};
		iS16 WaterOut_Y[PipeNum_006] = {386};
		iS16 OutSpeed_X[PipeNum_006] = {20};
		iS16 OutSpeed_Y[PipeNum_006] = {5};

		for( int i=0; i<PipeNum_006; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_006;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}		
	else if( level==MW_LEVEL_007 )
	{
		#define PipeNum_007 1
		iS16 WaterIn_X[PipeNum_007] = {255};
		iS16 WaterIn_Y[PipeNum_007] = {265};
		iS16 WaterIn_W[PipeNum_007] = {30};
		iS16 WaterIn_H[PipeNum_007] = {30};
		iS16 WaterOut_X[PipeNum_007] = {205};
		iS16 WaterOut_Y[PipeNum_007] = {339};
		iS16 OutSpeed_X[PipeNum_007] = {20};
		iS16 OutSpeed_Y[PipeNum_007] = {5};

		for( int i=0; i<PipeNum_007; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_007;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_008 )
	{
	#define PipeNum_008 1
		iS16 WaterIn_X[PipeNum_008] = {80};
		iS16 WaterIn_Y[PipeNum_008] = {430};
		iS16 WaterIn_W[PipeNum_008] = {30};
		iS16 WaterIn_H[PipeNum_008] = {30};
		iS16 WaterOut_X[PipeNum_008] = {233};
		iS16 WaterOut_Y[PipeNum_008] = {379};
		iS16 OutSpeed_X[PipeNum_008] = {20};
		iS16 OutSpeed_Y[PipeNum_008] = {5};

		for( int i=0; i<PipeNum_008; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_008;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_009 )
	{
		#define PipeNum_009 1
		iS16 WaterIn_X[PipeNum_009] = {209};
		iS16 WaterIn_Y[PipeNum_009] = {411};
		iS16 WaterIn_W[PipeNum_009] = {30};
		iS16 WaterIn_H[PipeNum_009] = {30};
		iS16 WaterOut_X[PipeNum_009] = {97};
		iS16 WaterOut_Y[PipeNum_009] = {379};
		iS16 OutSpeed_X[PipeNum_009] = {20};
		iS16 OutSpeed_Y[PipeNum_009] = {5};

		for( int i=0; i<PipeNum_009; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_009;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_010 )
	{
		#define PipeNum_010 1
		iS16 WaterIn_X[PipeNum_010] = {270};
		iS16 WaterIn_Y[PipeNum_010] = {395};
		iS16 WaterIn_W[PipeNum_010] = {30};
		iS16 WaterIn_H[PipeNum_010] = {30};
		iS16 WaterOut_X[PipeNum_010] = {233};
		iS16 WaterOut_Y[PipeNum_010] = {175};
		iS16 OutSpeed_X[PipeNum_010] = {20};
		iS16 OutSpeed_Y[PipeNum_010] = {5};

		for( int i=0; i<PipeNum_010; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_010;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_011 )
	{
		#define PipeNum_011 7
		iS16 WaterIn_X[PipeNum_011] = {144,76,182,290,180,37,257};
		iS16 WaterIn_Y[PipeNum_011] = {316,252,210,256,184,374,374};
		iS16 WaterIn_W[PipeNum_011] = {30,20,20,20,20,20,20};
		iS16 WaterIn_H[PipeNum_011] = {30,20,20,20,20,20,20};
		iS16 WaterOut_X[PipeNum_011] = {141,130,238,500,95,131,194};
		iS16 WaterOut_Y[PipeNum_011] = {387,217,262,500,150,298,298};
		iS16 OutSpeed_X[PipeNum_011] = {20,300,300,300,-120,150,-150};
		iS16 OutSpeed_Y[PipeNum_011] = {5,-5,-5,-5,-120,-350,-350};

		for( int i=0; i<PipeNum_011; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_011;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_012 )
	{
		#define PipeNum_012 2
		iS16 WaterIn_X[PipeNum_012] = {9,275};
		iS16 WaterIn_Y[PipeNum_012] = {307,150};
		iS16 WaterIn_W[PipeNum_012] = {30,30};
		iS16 WaterIn_H[PipeNum_012] = {30,20};
		iS16 WaterOut_X[PipeNum_012] = {53,284};
		iS16 WaterOut_Y[PipeNum_012] = {379,47};
		iS16 OutSpeed_X[PipeNum_012] = {20,-200};
		iS16 OutSpeed_Y[PipeNum_012] = {5,-30};

		for( int i=0; i<PipeNum_012; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_012;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);	
	}
	else if( level==MW_LEVEL_013 )
	{
		#define PipeNum_013 3
		iS16 WaterIn_X[PipeNum_013] = {267,240,100};
		iS16 WaterIn_Y[PipeNum_013] = {387,208,222};
		iS16 WaterIn_W[PipeNum_013] = {30,30,50};
		iS16 WaterIn_H[PipeNum_013] = {30,20,80};
		iS16 WaterOut_X[PipeNum_013] = {148,158,PipeOutX_HOLD};
		iS16 WaterOut_Y[PipeNum_013] = {378,232,PipeOutY_HOLD};
		iS16 OutSpeed_X[PipeNum_013] = {20,0,200};
		iS16 OutSpeed_Y[PipeNum_013] = {5,20,PipeSpeedY_HOLD};

		for( int i=0; i<PipeNum_013; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_013;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);	
	}
	else if( level==MW_LEVEL_014 )
	{
		#define PipeNum_014 4
		iS16 WaterIn_X[PipeNum_014] = {244,33,42,270};
		iS16 WaterIn_Y[PipeNum_014] = {176,435,318,245};
		iS16 WaterIn_W[PipeNum_014] = {30,30,30,30};
		iS16 WaterIn_H[PipeNum_014] = {40,30,30,30};
		iS16 WaterOut_X[PipeNum_014] = {235,170,167,40};
		iS16 WaterOut_Y[PipeNum_014] = {75,329,227,105};
		iS16 OutSpeed_X[PipeNum_014] = {20,-200,200,200};
		iS16 OutSpeed_Y[PipeNum_014] = {5,-5,-5,-5};

		for( int i=0; i<PipeNum_014; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_014;
		Pipe_Manage.Data = (PipeData*)(&PipeMess);
	}
	else if( level==MW_LEVEL_015 )
	{
		#define PipeNum_015 3
		iS16 WaterIn_X[PipeNum_015] = {237,31,253};
		iS16 WaterIn_Y[PipeNum_015] = {208,306,279};
		iS16 WaterIn_W[PipeNum_015] = {30,30,40};
		iS16 WaterIn_H[PipeNum_015] = {30,30,30};
		iS16 WaterOut_X[PipeNum_015] = {166,45,195};
		iS16 WaterOut_Y[PipeNum_015] = {316,412,145};
		iS16 OutSpeed_X[PipeNum_015] = {20,0,30};
		iS16 OutSpeed_Y[PipeNum_015] = {5,10,-250};

		for( int i=0; i<PipeNum_015; i++)
		{
			PipeMess[i].ValidityFlag = iTRUE;
			PipeMess[i].RectWaterIn.X = WaterIn_X[i];
			PipeMess[i].RectWaterIn.Y = WaterIn_Y[i];
			PipeMess[i].RectWaterIn.Width = WaterIn_W[i];
			PipeMess[i].RectWaterIn.Height = WaterIn_H[i];
			PipeMess[i].WaterOutX = WaterOut_X[i];
			PipeMess[i].WaterOutY = WaterOut_Y[i];
			PipeMess[i].SpeedX = OutSpeed_X[i];
			PipeMess[i].SpeedY = OutSpeed_Y[i];
		}
		Pipe_Manage.PipeNum = PipeNum_015;
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
		SpeedKeepMess[0].SpeedX = 220; 
		SpeedKeepMess[0].SpeedY = 220;
		SpeedKeepMess[0].Rect.X = 8;
		SpeedKeepMess[0].Rect.Y= 265;
		SpeedKeepMess[0].Rect.Width = 44;
		SpeedKeepMess[0].Rect.Height = 44;

		SpeedKeepMess[1].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[1].MoveX = 0;
		SpeedKeepMess[1].MoveY = 0;
		SpeedKeepMess[1].SpeedX = 300; 
		SpeedKeepMess[1].SpeedY = 20;
		SpeedKeepMess[1].Rect.X = 52;
		SpeedKeepMess[1].Rect.Y= 288;
		SpeedKeepMess[1].Rect.Width = 67;
		SpeedKeepMess[1].Rect.Height = 20;

		SpeedKeepMess[2].KeepSpeedFlag = iTRUE;
		SpeedKeepMess[2].MoveX = 0;
		SpeedKeepMess[2].MoveY = 0;
		SpeedKeepMess[2].SpeedX = 220; 
		SpeedKeepMess[2].SpeedY = -150;
		SpeedKeepMess[2].Rect.X = 128;
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
		SpeedKeepMess[0].MoveX = 5;
		SpeedKeepMess[0].MoveY = 0;
		SpeedKeepMess[0].SpeedX = 300; 
		SpeedKeepMess[0].SpeedY = -5;
	}	
#endif

	return iTRUE;

}

iBOOL mwLevelOrganInit( mwLevelNum level  )
{
#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)
	Organ_Manage.ResidueOrganNum = 0;
	Organ_Manage.TotalOrganNum = 0;
	Organ_Manage.OrganMess = iNULL;

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
			OrganMess[1].KeyRect.Width = 40;
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
	for( int i=0; i<Organ_Manage.TotalOrganNum; i++ )
	{
		iS16 StartX = OrganMess[i].OrganStartRect.X;
		iS16 StartY = OrganMess[i].OrganStartRect.Y;
		iS16 EndX = StartX + OrganMess[i].OrganStartRect.Width;
		iS16 EndY = StartY + OrganMess[i].OrganStartRect.Height;

		OrganMess[i].OrganCurrRect.X = OrganMess[i].OrganStartRect.X;
		OrganMess[i].OrganCurrRect.Y = OrganMess[i].OrganStartRect.Y;
		OrganMess[i].OrganCurrRect.Width = OrganMess[i].OrganStartRect.Width;
		OrganMess[i].OrganCurrRect.Height= OrganMess[i].OrganStartRect.Height;
		OrganMess[i].OrganEndRect.Width = OrganMess[0].OrganStartRect.Width;
		OrganMess[i].OrganEndRect.Height= OrganMess[0].OrganStartRect.Height;

		cpVect tris[4];
		tris[0] = cpv(StartX,StartY);
		tris[1] = cpv(StartX,EndY);
		tris[2] = cpv(EndX,EndY);
		tris[3] = cpv(EndX,StartY);

		cpBody * staticBody = (cpBody*)&(mwSpace->staticBody);
		cpShape * shape = iNULL;
		shape = cpSpaceAddShape(mwSpace, cpPolyShapeNew(staticBody, 4, tris, cpvzero));
	 	shape->e = SHAPE_E; 
		shape->u = SHAPE_U; 
		shape->block.ShapeType = OrganMess[i].OrganType;
	}
#endif	
	return iTRUE;
}

static iBOOL mwLevelShapeDelet (cpShape * shape)
{
	if( iNULL != shape )
	{
		cpSpaceRemoveShape(mwSpace,shape);
		cpShapeFree(shape);
		shape = iNULL;
	}
	return iTRUE;
}

static iBOOL mwLevelWordUpdate( void )
{
	cpBody *staticBody = &(mwSpace->staticBody);
	cpHashSet * StaticShape = mwSpace->staticShapes->handleSet;
	cpHashSet * AcitveShape = mwSpace->activeShapes->handleSet;

	iU16 CurrShapeX = 0;
	iU16 CurrShapeY = 0;
	iU16 OffSetY = SCREEN_WIDTH/SideLength;
	
	for(int i=0; i<StaticShape->size; i++)
	{
		cpHashSetBin *bin = StaticShape->table[i];
		while(bin)
//		if(bin)
		{
			cpHashSetBin *next = bin->next;	
			cpHandle *hand = (cpHandle *)(bin->elt);
			cpShape *shape = (cpShape *)hand->obj;
			bin = next;		
			CurrShapeX = shape->block.BlockX;
			CurrShapeY = shape->block.BlockY;
			if( shape->block.ShapeType == SHAPE_TYPE_DELETABLE		
				&&(shape->block.BlockType  != GriddingType[CurrShapeY][CurrShapeX]
				|| ( GriddingMud[CurrShapeY][CurrShapeX]==Gridding_MUD_No 
				|| (GriddingMud[CurrShapeY][CurrShapeX]&Gridding_MUD_Update)==Gridding_MUD_Update))
			)
			{
	//			GriddingMud[CurrShapeY][CurrShapeX]  = Gridding_MUD_No;
				shape->block.BlockType == Gridding_TYPE_SPACE;
//				WordFlag[CurrShapeY*OffSetY+CurrShapeX] = iFALSE;
				mwLevelShapeDelet(shape);
//				StaticShape = mwSpace->staticShapes->handleSet;
//				i = 0;
			}
		}
	}

	for(int i=0; i<AcitveShape->size; i++)
	{
		cpHashSetBin *bin = AcitveShape->table[i];
		while(bin)
//		if(bin)
		{
			cpHashSetBin *next = bin->next;	
			cpHandle *hand = (cpHandle *)bin->elt;
			cpShape *shape = (cpShape *)hand->obj;
//			func(bin->elt, data);		
			bin = next;		
			CurrShapeX = shape->block.BlockX;
			CurrShapeY = shape->block.BlockY;
			if( shape->block.ShapeType == SHAPE_TYPE_DELETABLE		
				&&(shape->block.BlockType  != GriddingType[CurrShapeY][CurrShapeX]
				|| ( GriddingMud[CurrShapeY][CurrShapeX]==Gridding_MUD_No 
				|| (GriddingMud[CurrShapeY][CurrShapeX]&Gridding_MUD_Update)==Gridding_MUD_Update))
			)
			{
//				WordFlag[CurrShapeY*OffSetY+CurrShapeX] = iFALSE;
				shape->block.BlockType == Gridding_TYPE_SPACE;
				mwLevelShapeDelet(shape);
	//			AcitveShape = mwSpace->activeShapes->handleSet;
	//			i = 0;
			}
		}
	}	


	return iTRUE;
	
}

static iBOOL mwLevelShapeAdd (iS16 ShapeX, iS16 ShapeY, iU8 ShapeType )
{
	iS32 StartX = (ShapeX)*SideLength;
	iS32 EndX = (ShapeX+1)*SideLength;
	iS32 StartY = (ShapeY)*SideLength;
	iS32 EndY = (ShapeY+1)*SideLength;
	iS32 EndX2 = StartX + SideLength/2;
	iS32 EndY2 = StartY + SideLength/2;
	
	cpBody *staticBody = &(mwSpace->staticBody);
	cpShape *shape = iNULL;

	if( ShapeX<0 || ShapeX>=SCREEN_WIDTH/SideLength 
	||	ShapeY<0 || ShapeY>=SCREEN_HEIGHT/SideLength 
	||	ShapeType>Gridding_TYPE_RIGHT_3 || ShapeType==Gridding_TYPE_SPACE )
	{
		iLog("mwLevelShapeAdd err:CurrLevelNum:%d,ShapeX:%d,ShapeY:%d,ShapeType:%d",CurrLevelNum,ShapeX,ShapeY,ShapeType);
		return iFALSE;
	}
	
	switch( ShapeType )
	{
		case Gridding_TYPE_TOP:
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(StartX,StartY), cpv(EndX,StartY), RECT_SEG_R));
			break;		
		case Gridding_TYPE_BUTTOM:
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(StartX,EndY), cpv(EndX,EndY), RECT_SEG_R));
			break;		
		case Gridding_TYPE_LEFT:
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(StartX,StartY), cpv(StartX,EndY), RECT_SEG_R));
			break;		
		case Gridding_TYPE_RIGHT:
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(EndX,StartY), cpv(EndX,EndY), RECT_SEG_R));
			break;		
		case Gridding_TYPE_LT:
		case Gridding_TYPE_RB:
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(StartX,EndY), cpv(EndX,StartY), RECT_SEG_R));
			break;		
		case Gridding_TYPE_LB:
		case Gridding_TYPE_RT:
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(StartX,StartY), cpv(EndX,EndY), RECT_SEG_R));
			break;		
		case Gridding_TYPE_TOP_3:
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(StartX,StartY), cpv(EndX2,EndY2), RECT_SEG_R));
			shape->e = SHAPE_E; 
			shape->u = SHAPE_U; 
			shape->block.BlockType = GriddingType[ShapeY][ShapeX];
			shape->block.BlockX = ShapeX;
			shape->block.BlockY = ShapeY;
			shape->block.ShapeType = SHAPE_TYPE_DELETABLE;
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(EndX,StartY), cpv(EndX2,EndY2), RECT_SEG_R));
//			GriddingAdd[ShapeY][ShapeX] = 1;
			break;		
		case Gridding_TYPE_BUTTOM_3:
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(StartX,EndY), cpv(EndX2,EndY2), RECT_SEG_R));
			shape->e = SHAPE_E; 
			shape->u = SHAPE_U; 
			shape->block.BlockType = GriddingType[ShapeY][ShapeX];
			shape->block.BlockX = ShapeX;
			shape->block.BlockY = ShapeY;
			shape->block.ShapeType = SHAPE_TYPE_DELETABLE;
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(EndX,EndY), cpv(EndX2,EndY2), RECT_SEG_R));
//			GriddingAdd[ShapeY][ShapeX] = 1;
			break;		
		case Gridding_TYPE_LEFT_3:
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(StartX,StartY), cpv(EndX2,EndY2), RECT_SEG_R));
			shape->e = SHAPE_E; 
			shape->u = SHAPE_U; 
			shape->block.BlockType = GriddingType[ShapeY][ShapeX];
			shape->block.BlockX = ShapeX;
			shape->block.BlockY = ShapeY;
			shape->block.ShapeType = SHAPE_TYPE_DELETABLE;
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(StartX,EndY), cpv(EndX2,EndY2),RECT_SEG_R));
//			GriddingAdd[ShapeY][ShapeX] = 1;
			break;		
		case Gridding_TYPE_RIGHT_3:
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(EndX,StartY), cpv(EndX2,EndY2), RECT_SEG_R));
			shape->e = SHAPE_E; 
			shape->u = SHAPE_U; 
			shape->block.BlockType = GriddingType[ShapeY][ShapeX];
			shape->block.BlockX = ShapeX;
			shape->block.BlockY = ShapeY;
			shape->block.ShapeType = SHAPE_TYPE_DELETABLE;
			shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(EndX,EndY), cpv(EndX2,EndY2), RECT_SEG_R));
//			GriddingAdd[ShapeY][ShapeX] = 1;
			break;		
		default:
			iLog("shape type:Level:%d,X:%d,Y:%d,T:%d",CurrLevelNum,ShapeX,ShapeX,ShapeType);
			return iFALSE;
			break;
	}

 	shape->e = SHAPE_E; 
	shape->u = SHAPE_U; 
	shape->block.BlockType = GriddingType[ShapeY][ShapeX];
	shape->block.BlockX = ShapeX;
	shape->block.BlockY = ShapeY;
	shape->block.ShapeType = SHAPE_TYPE_DELETABLE;

	return iTRUE;
	
}

static iBOOL mwLevelShapeLoop( iS16 LoopStartX, iS16 LoopStartY )
{
	iS16 ShapeX[8] = {
		LoopStartX,LoopStartX+1,LoopStartX+1,LoopStartX+1,
		LoopStartX,LoopStartX-1,LoopStartX-1,LoopStartX-1
		};
	iS16 ShapeY[8] = {
		LoopStartY,LoopStartY,LoopStartY+1,LoopStartY+2,
		LoopStartY+2,LoopStartY+2,LoopStartY+1,LoopStartY
		};
	iS16 ShapeType[8] = {1,2,3,4,5,6,7,8};

	for( int i=0; i<8; i++ )
	{
		GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
		mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
	}

	return iTRUE;
}

iBOOL mwLevelGriddingUpdate( iS16 GriddingX, iS16 GriddingY )
{

	if( GriddingX<0 || GriddingX>=SCREEN_WIDTH/SideLength || GriddingY<=0 || GriddingY>=SCREEN_HEIGHT/SideLength )
	{
		// 越界
		return iFALSE;
	}

	iS16 StartX = GriddingX*SideLength;
	iS16 StartY = GriddingY*SideLength;
	iS16 CurrGriddingX = 0;
	iS16 CurrGriddingY = 0;
	
	GriddingType[GriddingY][GriddingX] = Gridding_TYPE_SPACE;
	GriddingMud[GriddingY][GriddingX] = Gridding_MUD_No;
	GriddingUpdataFlag[GriddingY][GriddingX] = 0;

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

	for( int CurrGridding = 0; CurrGridding < 8 ; CurrGridding++ )
	{
		iU8 CurrGriddingNum = GriddingNum[CurrGridding];
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
		
		iU8 ContinueFlag = iFALSE;
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

		
		iU8 CurrType = GriddingType[CurrGriddingY][CurrGriddingX]-1;
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

	memset(GriddingMud,Gridding_MUD_FULL,SCREEN_HEIGHT*SCREEN_WIDTH/SideLength/SideLength);
	memset(GriddingType,Gridding_TYPE_SPACE,SCREEN_HEIGHT*SCREEN_WIDTH/SideLength/SideLength);
	memset(GriddingUpdataFlag,Gridding_TYPE_SPACE,SCREEN_HEIGHT*SCREEN_WIDTH/SideLength/SideLength);
//	memset(GriddingAdd,0X00,SCREEN_HEIGHT*SCREEN_WIDTH/SideLength/SideLength);
//	memset(AlgaeMess,0X00,SCREEN_HEIGHT*SCREEN_WIDTH/SideLength/SideLength);
	

	if( CurrLevelNum ==  MW_LEVEL_001 )
	{
		#define LEVEL001_NUM 22
		iS16 ShapeX[LEVEL001_NUM]= {
			9,10,10,11,12,13,14,15,16,17,18,19,20,21,22,23,
			17,18,19,15,15,16};
		iS16 ShapeY[LEVEL001_NUM]= {
			4,5,6,7,7,7,8,8,8,8,8,8,7,6,5,4,
			26,26,27,28,27,26};
		iU8 ShapeType[LEVEL001_NUM]= {
			6,7,6,6,5,5,6,5,5,5,5,4,4,4,4,4,
			1,2,2,7,8,8};

		for( int i=0; i<LEVEL001_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}
		mwLevelShapeLoop(16,11);
		mwLevelShapeLoop(16,16);
		mwLevelShapeLoop(16,21);

		
	}		
	else if( CurrLevelNum ==  MW_LEVEL_002 )
	{
		iS16 ShapeX[44]= {12,13,14,15,16,17,18,19,20,21,21,12,21,
			17,18,18,18,17,16,16,16,
			17,18,18,18,17,16,16,16,
			17,18,18,18,17,16,16,16,
			17,18,18,18,16,16,16};
		iS16 ShapeY[44]= {8,9,10,10,11,11,11,10,10,9,8,7,7,
			15,15,16,17,17,17,16,15,
			22,22,23,24,24,24,23,22,
			30,30,31,32,32,32,31,30,
			35,35,36,37,37,36,35};
		iU8 ShapeType[44]= {6,6,6,5,6,5,4,5,4,4,3,7,3,
			1,2,3,4,5,6,7,8,
			1,2,3,4,5,6,7,8,
			1,2,3,4,5,6,7,8,
			1,2,3,3,7,7,8};

		for( int i=0; i<44; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}
	}
	else if( CurrLevelNum ==  MW_LEVEL_003 )
	{
		iS16 ShapeX[14]= {10,11,11,12,13,14,15,16,17,18,18,19,10,19};
		iS16 ShapeY[14]= {6,7,8,8,9,9,9,9,8,8,7,6,5,5};
		iU8 ShapeType[14]= {6,7,6,5,6,5,5,4,5,4,3,4,7,3};

		for( int i=0; i<14; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}
	}
	else if( CurrLevelNum ==  MW_LEVEL_004 )
	{
		#define LEVEL004_NUM 18
		
		iS16 ShapeX[LEVEL004_NUM]= {
				1,2,3,4,5,6,7,
				14,13,
				22,23,24,25,26,27,28,29,30,
			};
		iS16 ShapeY[LEVEL004_NUM]= {
				10,10,10,10,10,10,10,
				21,22,
				31,30,29,28,28,28,28,28,28,
			};
		iU8 ShapeType[LEVEL004_NUM]= {
				5,5,5,5,5,5,5,
				4,4,
				4,4,4,5,5,5,5,5,5,
			};

		for( int i=0; i<LEVEL004_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}

		mwLevelShapeLoop(28,31);
	}
	else if( CurrLevelNum ==  MW_LEVEL_005 )
	{
		#define LEVEL005_NUM 84
		
		iS16 ShapeX[LEVEL005_NUM]= {
				6,6,6,6,
				8,8,8,8,
				27,28,28,28,27,26,26,26,
				28,29,29,29,28,27,27,27,
				28,29,29,29,28,27,27,27,
				10,11,11,9,12,
				12,12,12,12,13,14,15,16,17,18,19,20,20,21,22,23,23,23,23,
				24,24,24,24,24,
				20,21,21,21,21,21,21,21,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20
			};
		iS16 ShapeY[LEVEL005_NUM]= {
				5,6,7,8,
				5,6,7,8,
				9,9,10,11,11,11,10,9,
				18,18,19,20,20,20,19,18,
				28,28,29,30,30,30,29,28,
				39,39,40,39,39,
				0,1,2,3,4,5,6,7,7,8,9,10,11,12,13,13,14,15,16,
				33,34,35,36,37,
				17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39
			};
		iU8 ShapeType[LEVEL005_NUM]= {
				7,7,7,7,
				3,3,3,3,
				1,2,3,4,5,6,7,8,
				1,2,3,4,5,6,7,8,
				1,2,3,4,5,6,7,8,
				1,1,3,3,7,
				3,3,3,3,2,2,2,1,2,2,2,2,3,2,1,2,3,3,3,
				3,3,3,3,3,
				6,7,7,7,7,7,7,7,8,7,7,7,7,7,7,7,7,7,7,7,7,7,7
			};
	
		for( int i=0; i<LEVEL005_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}
	}
	else if( CurrLevelNum ==  MW_LEVEL_006 )
	{
		#define LEVEL006_NUM 48
		
		iS16 ShapeX[LEVEL006_NUM]= {
				14,14,14,
				17,17,17,
				2,2,3,4,5,5,6,7,8,9,10,11,12,13,14,15,
				29,29,28,27,26,26,25,24,23,22,21,20,19,18,17,
				13,14,15,16,17,18,
				14,15,16,17,18,
			};
		iS16 ShapeY[LEVEL006_NUM]= {
				35,36,37,
				37,36,35,
				10,11,12,12,12,11,10,9,10,10,10,11,11,12,13,14,
				10,11,11,11,11,10,9,9,9,10,10,11,12,13,14,
				22,22,23,23,22,22,
				29,29,29,29,29,
			};
		iU8 ShapeType[LEVEL006_NUM]= {
				8,7,6,
				4,3,2,
				7,6,6,5,4,3,4,5,6,5,5,6,5,6,6,7,
				3,4,5,5,6,7,5,5,5,4,5,4,4,4,3,
				5,5,6,4,5,5,
				5,5,5,5,5
			};
	
		for( int i=0; i<LEVEL006_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}

		mwLevelShapeLoop(9,30);
		mwLevelShapeLoop(22,30);

	}
	else if( CurrLevelNum ==  MW_LEVEL_007 )
	{
		#define LEVEL007_NUM 37
		
		iS16 ShapeX[LEVEL007_NUM]= {
				7,8,9,10,11,11,9,8,7,6,5,5,5,5,6,
				10,10,11,12,13,14,15,16,17,18,19,20,21,22,
				21,22,23,24,25,26,27,28
			};
		iS16 ShapeY[LEVEL007_NUM]= {
				28,28,28,29,30,31,33,33,33,32,32,31,30,29,28,
				7,8,9,10,10,10,10,10,10,10,10,9,8,7,
				21,21,21,21,21,21,21,21				
			};
		iU8 ShapeType[LEVEL007_NUM]= {
				1,1,2,2,2,3,5,5,6,5,6,7,7,8,8,
				7,6,6,6,5,5,5,5,5,5,4,4,4,4,
				5,5,5,5,5,5,5,5
			};
	
		for( int i=0; i<LEVEL007_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}

		mwLevelShapeLoop(3,38);
		mwLevelShapeLoop(5,21);
		mwLevelShapeLoop(18,23);
		mwLevelShapeLoop(27,26);

	}	
	else if( CurrLevelNum ==  MW_LEVEL_008 )
	{
	#define LEVEL008_NUM 31
		
		iS16 ShapeX[LEVEL008_NUM]= {
				19,20,21,22,23,24,
				20,21,21,21,21,19,19,19,19,
				6,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20
			};
		iS16 ShapeY[LEVEL008_NUM]= {
				30,30,30,30,30,30,
				24,24,25,26,27,27,26,25,24,
				5,6,7,7,8,8,8,8,8,8,8,8,8,8,7,7
			};
		iU8 ShapeType[LEVEL008_NUM]= {
				5,5,5,5,5,5,
				1,2,3,3,3,6,7,7,8,
				7,6,6,5,6,5,5,5,5,5,5,5,5,4,5,4
			};
	
		for( int i=0; i<LEVEL008_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}

		mwLevelShapeLoop(9,43);

	}
	else if( CurrLevelNum ==  MW_LEVEL_009 )
	{
		#define LEVEL009_NUM 64
		
		iS16 ShapeX[LEVEL009_NUM]= {
				19,19,19,18,19,20,21,22,23,24,25,26,27,27,27,26,25,
				21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,
				4,5,
				8,9,10,11,12,13,14,15,16,17,18,19,20,
				11,12,13,14,15,16,17,18,19,20,21,22,23
			};
		iS16 ShapeY[LEVEL009_NUM]= {
				5,6,7,8,9,10,11,12,12,13,14,14,15,16,17,18,19,
				16,17,18,18,19,19,20,20,20,21,21,21,22,22,23,23,23,24,25,
				26,27,
				28,29,29,29,29,30,30,30,31,31,32,32,33,
				27,27,27,28,28,29,29,29,30,30,31,31,32
			};
		iU8 ShapeType[LEVEL009_NUM]= {
				3,3,4,3,2,2,2,1,2,2,1,2,2,3,4,4,4,
				8,8,1,8,1,8,1,1,8,1,1,8,1,8,1,1,8,8,8,
				5,6,
				5,6,5,5,5,6,5,5,6,5,6,5,6,
				1,1,2,1,2,1,1,2,1,2,1,2,2
			};
	
		for( int i=0; i<LEVEL009_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}


	}	
	else if( CurrLevelNum ==  MW_LEVEL_010 )
	{
		#define LEVEL010_NUM 54
		
		iS16 ShapeX[LEVEL010_NUM]= {
				16,16,16,16,16,16,15,14,13,12,11,
				5,6,
				1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,
				12,13,
				4,
				17,16,15,14,13,12,11,10,9,8,7,7,7,7,7,7,7,7,7,8,9
			};
		iS16 ShapeY[LEVEL010_NUM]= {
				3,4,5,6,7,8,8,8,8,8,8,
				13,12,
				18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
				21,22,
				28,
				27,27,27,27,27,27,27,27,27,27,27,28,29,30,31,32,33,34,35,36,36
			}; 
		iU8 ShapeType[LEVEL010_NUM]= {
				3,3,3,3,3,4,5,5,5,5,5,
				4,4,
				5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
				6,6,
				4,
				1,1,1,1,1,1,1,1,1,1,8,7,7,7,7,7,7,7,6,6,5
			};
	
		for( int i=0; i<LEVEL010_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}

		mwLevelShapeLoop(4,38);
		mwLevelShapeLoop(26,5);

	}
	else if( CurrLevelNum ==  MW_LEVEL_011 )
	{
		#define LEVEL011_NUM 36
		
		iS16 ShapeX[LEVEL011_NUM]= {
				1,2,3,4,5,6,7,8,9,
				22,23,24,25,26,27,28,29,30,
				1,2,3,4,5,6,7,8,9,
				22,23,24,25,26,27,28,29,30
			};
		iS16 ShapeY[LEVEL011_NUM]= {
				17,17,18,18,18,18,18,18,18,
				17,18,18,18,18,18,18,18,17,
				31,31,31,32,32,32,31,31,31,
				31,32,32,32,32,32,32,31,31
			}; 
		iU8 ShapeType[LEVEL011_NUM]= {
				5,5,6,5,5,5,5,5,5,
				5,6,5,5,5,5,5,4,5,
				5,5,5,6,5,4,5,5,5,
				5,6,5,5,5,5,4,5,5,
			};
	
		for( int i=0; i<LEVEL011_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}

		mwLevelShapeLoop(4,36);
		mwLevelShapeLoop(26,36);

	}	
	else if( CurrLevelNum ==  MW_LEVEL_012 )
	{
		#define LEVEL012_NUM 28
		
		iS16 ShapeX[LEVEL012_NUM]= {
				7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
				25,24,24,24,24,25,26,27,28,29,
			};
		iS16 ShapeY[LEVEL012_NUM]= {
				15,15,15,15,16,17,18,19,19,19,19,19,18,17,16,15,15,15,
				0,1,2,3,4,5,5,5,5,5,
			}; 
		iU8 ShapeType[LEVEL012_NUM]= {
				5,5,5,5,6,6,6,6,5,5,5,4,4,4,4,5,5,5,
				8,8,7,7,6,6,5,5,5,5,
			};
	
		for( int i=0; i<LEVEL012_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}

		mwLevelShapeLoop(11,5);
		mwLevelShapeLoop(22,5);
		mwLevelShapeLoop(15,30);
		mwLevelShapeLoop(22,41);

	}
	else if( CurrLevelNum ==  MW_LEVEL_013 )
	{
		#define LEVEL013_NUM 86
		
		iS16 ShapeX[LEVEL013_NUM]= {
				2,3,3,4,5,6,7,8,9,9,10,
				19,20,21,22,23,24,25,26,26,25,24,23,22,21,20,20,20,21,22,23,24,25,26,27,28,29,28,
				29,28,29,29,29,28,27,26,25,25,26,27,28,29,30,30,30,30,30,30,
				13,12,12,12,12,12,12,12,12,13,
				18,19,20,21,22,23,24,25,25,26,27,28,29,29,30,30,30,31
			};
		iS16 ShapeY[LEVEL013_NUM]= {
				6,7,8,8,8,9,9,8,8,7,6,
				3,4,4,5,5,6,6,7,8,8,8,9,9,10,10,11,12,12,13,13,13,13,14,14,15,16,17,
				4,5,6,7,8,9,9,10,10,11,12,12,12,13,13,14,15,16,17,18,
				22,23,24,25,26,27,28,29,30,30,
				31,31,31,30,30,31,32,33,34,35,35,35,35,34,33,32,31,30
			}; 
		iU8 ShapeType[LEVEL013_NUM]= {
6,7,6,5,5,6,4,5,4,3,4,
6,6,5,6,5,6,5,6,1,1,8,1,8,1,8,7,6,5,6,5,5,5,6,5,6,7,8,
4,3,2,3,4,4,5,4,5,2,1,1,2,1,2,3,3,3,3,4,
7,8,7,7,7,7,7,7,6,5,
5,5,4,5,5,6,6,7,6,6,5,5,4,3,4,3,3,4,
			};
	
		for( int i=0; i<LEVEL013_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}

		mwLevelShapeLoop(4,38);
		mwLevelShapeLoop(28,38);

	}	else if( CurrLevelNum ==  MW_LEVEL_014 )
	{
		#define LEVEL014_NUM 93
		
		iS16 ShapeX[LEVEL014_NUM]= {
				12,13,13,13,13,14,15,15,16,16,16,16,16,16,16,16,15,15,15,15,15,15,
				15,15,14,13,12,12,12,
				10,10,10,10,10,10,10, 
				1,2,3,4,4,5,6,6,5,4,3,2,1,0,
				16,16,16,16,16,16,16,17,17,17,17,17,17,17,18,18,18,18,19,19,19,19,19,19,19,19,19,19,18,18,18,
				18,18,18,18,18,17,17,17,16,15,14,14
			};
		iS16 ShapeY[LEVEL014_NUM]= {
				0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
				29,30,31,32,32,33,34,
				39,40,41,42,43,44,45,
				6,7,8,8,9,10,11,12,13,14,14,14,14,15,
				0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
				36,37,38,39,40,41,42,43,44,45,46,47
			}; 
		iU8 ShapeType[LEVEL014_NUM]= {
				6,7,7,7,6,6,7,6,7,7,7,7,7,7,7,7,8,7,7,7,7,7,
				7,7,8,1,8,7,7,7,7,7,7,7,7,7,
				3,2,1,2,3,2,2,4,4,4,5,5,5,4,
				3,3,3,3,3,3,3,2,3,3,3,3,3,3,2,3,3,3,2,3,3,3,3,3,3,3,3,4,3,3,3,
				3,3,3,3,4,3,3,4,4,4,3,3,
			};
	
		for( int i=0; i<LEVEL014_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}

		mwLevelShapeLoop(4,44);
		mwLevelShapeLoop(6,32);
		mwLevelShapeLoop(27,25);
		mwLevelShapeLoop(22,23);
		mwLevelShapeLoop(24,18);
		mwLevelShapeLoop(11,12);
		mwLevelShapeLoop(6,32);

	}
	else if( CurrLevelNum ==  MW_LEVEL_015 )
	{
		#define LEVEL015_NUM 66
		
		iS16 ShapeX[LEVEL015_NUM]= {
				2,2,2,3,4,5,6,6,6,
				9,9,9,9,9,10,11,12,
				15,16,16,16,16,17,17,17,18,19,20,21,22,23,24,25,26,27,28, 
				5,6,7,8,9,9,9,9,9,9,8,7,6,5,4,3,2,1,1,1,1,1,2,3,4,
				3,4,5,
				27,28
			};
		iS16 ShapeY[LEVEL015_NUM]= {
				8,9,10,10,10,10,10,9,8,
				16,15,14,13,12,12,13,14,
				8,9,10,11,12,13,14,15,16,16,15,15,14,14,13,12,12,13,13,
				19,19,19,19,19,20,21,22,23,24,24,24,24,24,24,24,24,24,23,22,21,20,20,20,19,
				31,31,31,
				37,37
			}; 
		iU8 ShapeType[LEVEL015_NUM]= {
				7,7,6,5,5,5,4,3,3,
				6,7,7,7,8,2,2,2,
				6,7,7,7,6,7,7,6,6,4,5,4,5,4,4,5,5,6,5,
				1,1,1,1,2,3,3,3,3,4,5,5,5,5,5,5,5,6,7,7,7,8,1,1,8,
				8,1,2,
				1,2,
			};
	
		for( int i=0; i<LEVEL015_NUM; i++ )
		{
			GriddingType[ShapeY[i]][ShapeX[i]] = ShapeType[i];
			mwLevelShapeAdd(ShapeX[i], ShapeY[i], ShapeType[i]);
		}

		mwLevelShapeLoop(27,29);
		mwLevelShapeLoop(5,27);
		mwLevelShapeLoop(25,20);
		mwLevelShapeLoop(26,15);

	}

	for( int Y = 0; Y < GriddingNumH; Y++ )
	{
		for( int X = 0; X < GriddingNumW; X++)
		{
			int StartX = (X)*SideLength/2;
			int StartY = (Y)*SideLength/2;
			int EndX = (X+1)*SideLength/2;
			int EndY = (Y+1)*SideLength/2;
			iS16 TotalRockPoint = 0;
			iS16 TotalSpacePoint = 0;
			for(int TempY = StartY; TempY< EndY; TempY++ )
			{
				for(int TempX = StartX; TempX< EndX; TempX++ )
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

iBOOL mwLevelWordInit( mwLevelNum level)
{
//	iLog("Start mwLevelWordInit");
	iU32 ReadDataLength = 0;
	iFILE File = iNULL;
	switch(level)
	{
		case MW_LEVEL_001:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData001.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
			
		case MW_LEVEL_002:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData002.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_003:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData003.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_004:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData004.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_005:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData005.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_006:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData006.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_007:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData007.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_008:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData008.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_009:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData009.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_010:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData010.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_011:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData011.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_012:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData012.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_013:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData013.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_014:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData014.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		case MW_LEVEL_015:
			File = i51AdeOsFileOpen( (iU16*)(L"WheresMyWater\\WordData015.re"), i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
			break;
		default:
			break;	
	}
//	iLog("Start read word");
	i51AdeOsFileRead(File, mwWorldAttribute, SCREEN_HEIGHT*SCREEN_HEIGHT/4, &ReadDataLength);
//	iLog("close file:%d",ReadDataLength);
	i51AdeOsFileClose(File);
//	return iTRUE;

	// Create the static triangles.
	iS32 MulsX = SCREEN_WIDTH/SideLength;
	iS32 MulsY = SCREEN_HEIGHT/SideLength;
	cpShape *shape = iNULL;
	cpBody *staticBody = &(mwSpace->staticBody);

//	memset(GriddingMud,0,SCREEN_HEIGHT*SCREEN_WIDTH/SideLength/SideLength);
//	for(iS32 Y=0; Y<MulsY; Y++){
//		for(iS32 X=0; X<MulsX; X++)
//		{
//			GriddingMud[Y][X] = Gridding_MUD_FULL;
//			GriddingType[Y][X] = Gridding_TYPE_SPACE;
//			mwLevelShapeAdd(X, Y);
//		}
//	}

	mwLevelGriddingInit();

#if 1
	int Circle = 0;
	int Line = 0;
	for( int Y = 0 ; Y<SCREEN_HEIGHT_HALF; Y++ )
	{
		for( int X = 0; X <SCREEN_WIDTH_HALF; X ++)
		{
		//if( (mwWorldAttribute[Y][X]&0XF0) > 0) i51KitG2DrawPoint(X, Y, 0XF800);
		//continue;
			// 边缘点已经计算过着需要跳过此区域
			if( mwWorldAttribute[Y][X]&0X08 )
			{
				int CurrX = X;
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
				int TempX = X;
				int TempY = Y;
				X = 0;
				Circle++;
				Line = 0;
				// 环结束条件是与边缘点相邻的边缘点已经被标记
				while( !(mwWorldAttribute[TempY][TempX]&0X08) )
				{
					int Count = 0;
					int TenthX = TempX;
					int TenthY = TempY;
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
					
					iS32 Point[2][2];
					int CurrX = TempX;
					int CurrY = TempY;
					Point[0][0] = TempX;
					Point[0][1] = TempY;
		//			i51KitG2DrawPoint(TempX, TempY, 0X001F);
					mwWorldAttribute[CurrY][CurrX] |= 0X08;
					// 经过边缘点和边缘点相邻的第十个点的直线Ax+By+C = 0 的系数
					int A,B,C;
					int CalculateY = TempY;
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
						int Numerator = A*CurrX + B*CurrY + C;
						// 点到直线距离的分母
						int Denominator = A*A + B*B;
						// 点到直线的距离的平方
						int Space = Numerator*Numerator/Denominator;

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
				&&	( (cpfabs(Point[0][0]-Point[1][0])<3) || (cpfabs(Point[0][1]-Point[1][1])<3)) )
				{
					continue;
				}
//iLog("shape line i51AdeOsGetTotallMemoryLeft=%d",i51AdeOsGetTotallMemoryLeft());
				shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(Point[0][0]*2,Point[0][1]*2), cpv(Point[1][0]*2,Point[1][1]*2), ROCK_SEG_R));
				shape->e = 0.2f; 
				shape->u = 0.01f; 
//				shape->block.BlockX = X;
//				shape->block.BlockY = Y;
					Line ++;
				}
			}
		}
	}
//	iLog("创建石头边缘");
#endif
	
	for( int Y = 0 ; Y<SCREEN_HEIGHT_HALF; Y++ )
	{
		for( int X = 0; X <SCREEN_WIDTH_HALF; X ++)
		{
			mwWorldAttribute[Y][X] &= 0X07;
		}
	}

	iS16 BathX[MW_LEVEL_MAX][4] = {
			{120,132,190,207},	{28,43,101,120},	{213,228,287,301},
			{23,39,99,115},		{35,50,102,115},	{215,226,281,293},
			{186,197,252,268},	{214,226,283,294},	{76,90,144,159},
			{215,227,281,294},	{121,134,189,203},	{32,46,99,114},
			{127,140,193,206},	{214,219,279,295},	{146,157,213,225}
		};
	iS16 BathY[MW_LEVEL_MAX][4] = {
			{410,438,438,410},	{419,450,450,419},	{420,451,451,420}, 
			{415,449,449,415},	{310,337,337,310},	{424,450,450,424},
			{382,406,406,382},	{422,446,446,422},	{423,448,448,422},
			{217,243,243,217},	{424,451,451,424},	{421,448,448,422},
			{418,444,444,419},	{114,140,140,114},	{351,377,377,351}
		};
	
	shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(BathX[level-1][0],BathY[level-1][0]), cpv(BathX[level-1][1],BathY[level-1][1]), BATH_SEG_R));
	shape->e = SHAPE_E; 
	shape->u = SHAPE_U; 

	shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(BathX[level-1][1],BathY[level-1][1]), cpv(BathX[level-1][2],BathY[level-1][2]),BATH_SEG_R));
	shape->e = SHAPE_E; 
	shape->u = SHAPE_U; 
	
	shape = cpSpaceAddShape(mwSpace, cpSegmentShapeNew(staticBody, cpv(BathX[level-1][2],BathY[level-1][2]), cpv(BathX[level-1][3],BathY[level-1][3]), BATH_SEG_R));
	shape->e = SHAPE_E; 
	shape->u = SHAPE_U; 
//	iLog("创建浴缸边缘");

	mwLevelWordUpdate();

	mwLevelSetImageMem(level);
	
	mwLevelSpeedKeepInit(CurrLevelNum);

	return iTRUE;
	
}

iBOOL mwLevelAlgaeInit( mwLevelNum level )
{
#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)
int Y,X;
int MaxX = SCREEN_WIDTH/2;
int MaxY = SCREEN_HEIGHT/2;
//	memset(AlgaeMess,0X00,MaxX*MaxY);

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
			for( Y = 211; Y<221; Y++ )
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
#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)

	static iU32 OldTime = 0;

	switch(level)
	{
		case MW_LEVEL_005:
			if( i51AdeOsGetTick()-OldTime >= 150 && WaterSprayFlag == iTRUE )
			{
				mwSpace->gravity = cpv(0,250); 
				OldTime = i51AdeOsGetTick();

				cpBody *body;//, *staticBody = &(mwSpace->staticBody);
				cpShape *shape;
				cpFloat radius = SPRAY_RADIUS;
				cpVect pos = cpv(32,61);
				body = cpSpaceAddBody(mwSpace, cpBodyNew(40.0f, cpMomentForCircle(40.0f, 0.0f, radius, cpvzero)));
				cpBodySetPos(body, pos);
				body->v = cpv(300,-10);
				
				shape = cpSpaceAddShape(mwSpace, cpCircleShapeNew(body, radius, cpvzero));
				shape->e = WATER_E; 
				shape->u = WATER_U; 
			}
			break;
		case MW_LEVEL_006:
			break;
		case MW_LEVEL_011:
//			mwSpace->gravity = cpv(0,150); 
			if( i51AdeOsGetTick()-OldTime >= 150 )
			{
				OldTime = i51AdeOsGetTick();

				cpBody *body;//, *staticBody = &(mwSpace->staticBody);
				cpShape *shape;
				cpFloat radius = WATER_RADIUS;
				cpVect pos = cpv(25,257);
				body = cpSpaceAddBody(mwSpace, cpBodyNew(10.0f, cpMomentForCircle(40.0f, 0.0f, radius, cpvzero)));
				cpBodySetPos(body, pos);
				body->v = cpv(300,-10);
				
				shape = cpSpaceAddShape(mwSpace, cpCircleShapeNew(body, radius, cpvzero));
				shape->e = WATER_E; 
				shape->u = WATER_U; 
			}
			break;			
		case MW_LEVEL_014:
		
//			mwSpace->gravity = cpv(0,50); 
		
			if( i51AdeOsGetTick()-OldTime >= 150 && WaterSprayFlag == iTRUE )
			{
				OldTime = i51AdeOsGetTick();
		
				cpBody *body;//, *staticBody = &(mwSpace->staticBody);
				cpShape *shape;
				cpFloat radius = WATER_RADIUS;
				cpVect pos = cpv(167,400);
				body = cpSpaceAddBody(mwSpace, cpBodyNew(40.0f, cpMomentForCircle(40.0f, 0.0f, radius, cpvzero)));
				cpBodySetPos(body, pos);
				body->v = cpv(-200,0);
				
				shape = cpSpaceAddShape(mwSpace, cpCircleShapeNew(body, radius, cpvzero));
				shape->e = WATER_E; 
				shape->u = WATER_U; 
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
#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)
		iS16 WAIT_X[MW_LEVEL_MAX] = {130,44,228 ,39, 44, 223,196,224,88,224,131,40,136,224,155};
		iS16 WAIT_Y[MW_LEVEL_MAX] = {352,363,363,359,251,371, 324,363,365,159,372,368,360,55,298 };
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
	for( int i=0; i<TotalFrame; i++ )
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
#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)
		iS16 BANTH_X[MW_LEVEL_MAX] = {130,44,228 ,39, 44, 223,196,224,88,224,131,40,136,224,155};
		iS16 BANTH_Y[MW_LEVEL_MAX] = {352,363,363,359,251,371, 324,363,365,159,372,368,360,55,298 };
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
	for( int i=0; i<TotalFrame; i++ )
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
	for(int Y = 0; Y<SCREEN_HEIGHT_HALF; Y++ )
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

		for( int X = 0; X<SCREEN_WIDTH_HALF; X++ )
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
	for(int Y = 0; Y<SCREEN_HEIGHT_HALF; Y++ )
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

		for( int X = 0; X<SCREEN_WIDTH_HALF; X++ )
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
		iS16 MaxX = SCREEN_WIDTH/2;
		iS16 MaxY = SCREEN_HEIGHT/2;
		for( int Y = 0; Y< MaxY; Y++ )
		{
			for( int X = 0; X< MaxX; X++ )
			{
				if( mwWorldAttribute[Y][X]> WORD_ALGAE )
				{
					iRECT DesRect;
					iRECT SrcRect;
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
			iU16 PenDownX = TempKeyData.u.pen.x;
			iU16 PenDownY = TempKeyData.u.pen.y;
		
			PenDownX = (PenDownX)/10*10 + 5;
			PenDownY = (PenDownY)/10*10 + 5;
			OldGridding_X = PenDownX;
			OldGridding_Y = PenDownY;
			CurrGridding_X = OldGridding_X;
			CurrGridding_Y = OldGridding_Y;
		
		}
		else if( TempKeyData.u.pen.type==i51_MSG_PADMOVE )
		{
			iU16 PenDownX = TempKeyData.u.pen.x;
			iU16 PenDownY = TempKeyData.u.pen.y;
		
			PenDownX = (PenDownX)/10*10 + 5;
			PenDownY = (PenDownY)/10*10 + 5;
			
			CurrGridding_X = PenDownX;
			CurrGridding_Y = PenDownY;
		}

		while(1)
		{
			for( int i = 0; i < Diameter; i++ )
			{
				iS32 StartX = OldGridding_X + PixelAdds[i][0];
				iS32 EndX = OldGridding_X + PixelAdds[i][1];
				iS32 CurrY = OldGridding_Y +PixelAdds[i][2]; 
				if( CurrY<0 || CurrY>= SCREEN_HEIGHT )
				{
					continue;
				}
				if( StartX<0 ) StartX = 0;
				if( EndX>=SCREEN_WIDTH )EndX = SCREEN_WIDTH;
				for( int j = StartX; j<EndX; j++ )
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

			if( OldGridding_X==CurrGridding_X && OldGridding_Y==CurrGridding_Y )
			{
				break;
			}
			else 
			{
				iS16 Mul_X[8] = {-SideLength,0,SideLength,-SideLength,SideLength,-SideLength,0,SideLength};
				iS16 Mul_Y[8] = {-SideLength, -SideLength, -SideLength, 0 ,0, SideLength,SideLength,SideLength};				
				iS32 CurrMulX = CurrGridding_X - OldGridding_X;
				iS32 CurrMulY = CurrGridding_Y - OldGridding_Y;
				iS32 BestMoveMul = cpfabs( CurrMulX-Mul_X[0]) + cpfabs(CurrMulY - Mul_Y[0]);
				iS32 BestMoveNum = 0;
				
				for(int i = 1; i< 8; i++)
				{
					iS32 CurrMoveMul = cpfabs( CurrMulX-Mul_X[i]) + cpfabs(CurrMulY - Mul_Y[i]);
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
		mwTempScreenBufUpdate(level);
		
		// 新划开的区域去除原有约束
		mwLevelWordUpdate();

		iS16 Word_W = SCREEN_WIDTH/SideLength;
		iS16 Word_H = SCREEN_HEIGHT/SideLength;

		for( iS16 Y = 0; Y < Word_H; Y++ )
		{
			for( iS16 X = 0; X < Word_W; X++ )
			{
				if( GriddingUpdataFlag[Y][X] )
				{
					mwLevelShapeAdd(X, Y, GriddingType[Y][X]);
				}
			}
		}
		
		memset(GriddingUpdataFlag,0,SCREEN_HEIGHT*SCREEN_WIDTH/SideLength/SideLength);

	}

	return iTRUE;
}


iBOOL mwLevelDrawDuck( void  )
{
#define DUCK_ANI_NUM	9

	iRECT DesRect;
	DesRect.X = 15;
	DesRect.Y = 7;
	DesRect.Width = 53;
	DesRect.Height= 53;
	iU32 DuckId[DUCK_ANI_NUM] = {WaterDuck00,WaterDuck02,
		WaterDuck04,WaterDuck05,WaterDuck06,
		WaterDuck07,WaterDuck08,WaterDuck09,WaterDuck10};
	
	for( int i = 0; i<DUCK_NUM; i++ )
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
	
	SpeedKeep_Manage.ActiveSpeedKeepNum = 0;
	SpeedKeepMess[0].KeepSpeedFlag = iFALSE;

	for( int i = 0; i<Organ_Manage.TotalOrganNum; i++ )
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
				SpeedKeepMess[0].Rect.X = OrganMess[i].OrganCurrRect.X;
				if( CurrLevelNum==MW_LEVEL_012)
				{
					SpeedKeepMess[0].Rect.Y = OrganMess[i].OrganCurrRect.Y-100;
					SpeedKeepMess[0].Rect.Height= OrganMess[i].OrganStartRect.Height+100;
				}
				else
				{
					SpeedKeepMess[0].Rect.Y = OrganMess[i].OrganCurrRect.Y;
					SpeedKeepMess[0].Rect.Height= OrganMess[i].OrganStartRect.Height;
				}
				SpeedKeepMess[0].Rect.Width= OrganMess[i].OrganStartRect.Width+100;
			}

		}
	}

	iRECT DesRect;

	if( Organ_Manage.TotalOrganNum>0 )
	{
		iS16 StartX = OrganMess[0].OrganCurrRect.X;
		iS16 StartY = OrganMess[0].OrganCurrRect.Y;

		DesRect.X = StartX;
		DesRect.Y = StartY;
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
			StartX = OrganMess[1].OrganCurrRect.X;
			StartY = OrganMess[1].OrganCurrRect.Y;

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
			StartX = OrganMess[2].OrganCurrRect.X;
			StartY = OrganMess[2].OrganCurrRect.Y;
		
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
	i51KitG2SetImageMemoryCopy(mwContainer, PUBLIC_IMAGE_MEM, ImageId);


	iU32 WordLevelNum[15] = {
			WordLevel001,WordLevel002,WordLevel003,WordLevel004,WordLevel005,
			WordLevel006,WordLevel007,WordLevel008,WordLevel009,WordLevel010,
			WordLevel011,WordLevel012,WordLevel013,WordLevel014,WordLevel015,
		};
//*/
	
	iU32 RockLevelNum[15] = {
			RockLevel001,RockLevel002,RockLevel003,RockLevel004,RockLevel005,
			RockLevel006,RockLevel007,RockLevel008,RockLevel009,RockLevel010,
			RockLevel011,RockLevel012,RockLevel013,RockLevel014,RockLevel015,
		};

	iU32 MudLevelNum[15] = {
			MudLevel001,MudLevel002,MudLevel003,MudLevel004,MudLevel005,
			MudLevel006,MudLevel007,MudLevel008,MudLevel009,MudLevel010,
			MudLevel011,MudLevel012,MudLevel013,MudLevel014,MudLevel015,
		};

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

	i51KitG2SetImageMemoryCopy(mwContainer, MemImage, ImageId);

	return iTRUE;
	
}

iBOOL mwLevelUnsetImageMem( mwLevelNum level )
{
	iU32 ImageId[15];

	iU32 WordLevelNum[15] = {
			WordLevel001,WordLevel002,WordLevel003,WordLevel004,WordLevel005,
			WordLevel006,WordLevel007,WordLevel008,WordLevel009,WordLevel010,
			WordLevel011,WordLevel012,WordLevel013,WordLevel014,WordLevel015,
		};
	
	iU32 RockLevelNum[15] = {
			RockLevel001,RockLevel002,RockLevel003,RockLevel004,RockLevel005,
			RockLevel006,RockLevel007,RockLevel008,RockLevel009,RockLevel010,
			RockLevel011,RockLevel012,RockLevel013,RockLevel014,RockLevel015,
		};

	iU32 MudLevelNum[15] = {
			MudLevel001,MudLevel002,MudLevel003,MudLevel004,MudLevel005,
			MudLevel006,MudLevel007,MudLevel008,MudLevel009,MudLevel010,
			MudLevel011,MudLevel012,MudLevel013,MudLevel014,MudLevel015,
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


	i51KitG2UnsetImageMemoryCopy(mwContainer, MemImage, ImageId);

	return iTRUE;
	
}

