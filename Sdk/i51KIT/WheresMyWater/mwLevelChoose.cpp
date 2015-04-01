
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#define LEVEL_START_RECORD_X	208
#define LEVEL_START_RECORD_Y	13
#define LEVEL_START_LEVEL1_X	23
#define LEVEL_START_LEVEL1_Y	83
#define LEVEL_START_LEVEL_ADDX	60
#define LEVEL_START_LEVEL_ADDY	60
#define LEVEL_START_DUCK_NUM_X	186
#define LEVEL_START_DUCK_NUM_Y	13
#define ICON_WIDTH	43
#define ICON_HEIGHT	43

#define ModeLeadBathe_X		35
#define ModeLeadBathe_Y		248
#define ModeLeadBathe_W		240
#define ModeLeadBathe_H		227

#define MODE_WATER_X1	0
#define MODE_WATER_Y1	385
#define MODE_WATER_W1	512
#define MODE_WATER_H1	96

#define MODE_WATER_X2	0
#define MODE_WATER_Y2	424
#define MODE_WATER_W2	512
#define MODE_WATER_H2	96

#endif

#define MAXDUCK_ONELEVEL	3
#define ICON_NUM_ONELINE	5
#define ICON_NUM_MAXLINE	3

iU8 LevelDuckNum[TOTAL_LEVEL] = {3,3,3,3,1,0,2,1};
iU32 UnlockMaxLevel = 15;
iU32 CurrTotalDuck = 15;
iU32 MoveOldTime = 0;
#define MOVE_MUL_X 5
#define MOVE_MUL_Y 5
iRECT MoveRect;
iRECT Water1;
iRECT Water2;

iS16 MulWater1 = 8;
iS16 MulWater2 = -8;

static iS8 ChooseErrFlag = iFALSE;
static iU32 ChooseErrTime = 0;

iBOOL mwLevelChooseAnimation( void )
{
	return iTRUE;
#if 0
	iRECT DesRect;
	static iU8 Flag = iFALSE;

	if( MoveRect.Y<= -SCREEN_HEIGHT && MoveRect.X<= -SCREEN_WIDTH )
	{	
		if( i51AdeOsGetTick()-MoveOldTime<3000 )
		{
			return iFALSE;
		}
		Flag = iFALSE;
		return iTRUE;
	}

	if( i51AdeOsGetTick()-MoveOldTime<=50 )
	{
		return iFALSE;
	}
	MoveOldTime = i51AdeOsGetTick();
	
	if( iFALSE == Flag )
	{
		Flag = iTRUE;
		MoveRect.X = 0;
		MoveRect.Y = 0;
		MoveRect.Width= SCREEN_WIDTH;
		MoveRect.Height= SCREEN_HEIGHT;
	}

	if( MoveRect.Y>-SCREEN_HEIGHT )
	{
		i51KitG2DrawImage(mwContainer, StartMove0, &MoveRect, iNULL);
		DesRect.X = 0;
		DesRect.Y = MoveRect.Y + SCREEN_HEIGHT;
		DesRect.Width = SCREEN_WIDTH;
		DesRect.Height = SCREEN_HEIGHT;
		i51KitG2DrawImage(mwContainer, StartMove1, &DesRect, iNULL);
		MoveRect.Y -= MOVE_MUL_Y;
	}
	else if( MoveRect.X > -SCREEN_WIDTH )
	{
		MoveRect.Y = -SCREEN_HEIGHT;
		DesRect.X = MoveRect.X;
		DesRect.Y = 0;
		DesRect.Width = SCREEN_WIDTH;
		DesRect.Height = SCREEN_HEIGHT;
		i51KitG2DrawImage(mwContainer, StartMove1, &DesRect, iNULL);
		DesRect.X = MoveRect.X+SCREEN_WIDTH;
		DesRect.Y = 0;
		DesRect.Width = SCREEN_WIDTH;
		DesRect.Height = SCREEN_HEIGHT;
		i51KitG2DrawImage(mwContainer, StartMove2, &DesRect, iNULL);
		MoveRect.X -= MOVE_MUL_X;
	}

	return iFALSE;
#endif
}

static iBOOL mwLevelDrawNum(iU32 Number, iU32 ImageID, iRECT * DesRect)
{
	iU32 ImageWidth;
	iU32 ImageHeight;
	iRECT SrcRect;
	
	if( Number>99 || iNULL==DesRect) return iFALSE;
	i51KitG2GetImageAttr(mwContainer, ImageID, &ImageWidth, &ImageHeight, iNULL);

	iU32 PicHeight = ImageHeight/12;
	
	if( Number<10 )
	{
		SrcRect.X = 0;
		SrcRect.Y = Number*PicHeight;
		SrcRect.Width = ImageWidth;
		SrcRect.Height = PicHeight;
		i51KitG2DrawImage(mwContainer, ImageID, DesRect, &SrcRect);
	}
	else
	{
		SrcRect.X = 0;
		SrcRect.Y = (Number/10)*PicHeight;
		SrcRect.Width = ImageWidth;
		SrcRect.Height = PicHeight;
		i51KitG2DrawImage(mwContainer, ImageID, DesRect, &SrcRect);
		DesRect->X += ImageWidth;
		SrcRect.X = 0;
		SrcRect.Y = (Number%10)*PicHeight;
		SrcRect.Width = ImageWidth;
		SrcRect.Height = PicHeight;
		i51KitG2DrawImage(mwContainer, ImageID, DesRect, &SrcRect);
	}

	return iTRUE;
	
}

iBOOL mwLevelChooseInitDraw( void )
{
	iRECT DesRect;
	iU32 ICON_NUM_ID[4] = {ChsLv0,ChsLv1,ChsLv2,ChsLv3};
	iU8 CurrDuckNum = 0;
	iS16 Bath_X = 0;
	iS16 Bath_Y = 360;
	iS16 Bath_W = SCREEN_WIDTH;
	iS16 Bath_H = 120;

	iS16 Word_X = 75;
	iS16 Word_Y = 268;
	iS16 Word_W = 171;
	iS16 Word_H = 24;

	iS16 Block_W = 81;
	iS16 Block_H = 81;
	iS16 Block_StartX = 0;
	iS16 Block_StartY = 160;

#if SOUND_NEED
	if(iFALSE==SoundCloseFlag && SoundChoosePlay == iFALSE)
	{
		SoundChoosePlay = iTRUE;
		pfunc->public_audio_play(
			0, 
			MW_SOUND_NAME_CHOOSE, 
			i51AdeStdWStrlen((iU16*)MW_SOUND_NAME_CHOOSE), 
			i51_MMI_DEC_FILE_BASED,
			AUDIO_PLAYER_WAV_0, 
			iTRUE
			);
	}
#endif

//iLog("start choose!");
	if(UnlockMaxLevel>TOTAL_LEVEL)
	{
		iLog("MaxLevel:%d>TOTAL_LEVEL:%d",UnlockMaxLevel,TOTAL_LEVEL);
		return iFALSE;
	}

	i51KitG2DrawImage(mwContainer, ChooseBKHead, iNULL, iNULL);

	for( int Y = Block_StartY; Y<=SCREEN_HEIGHT; Y+=Block_H )
	{
		for( int X = Block_StartX; X<=SCREEN_WIDTH; X+=Block_W )
		{
			DesRect.X = X;
			DesRect.Y = Y;
			DesRect.Width= Block_W;
			DesRect.Height= Block_H;
			i51KitG2DrawImage(mwContainer, ChooseBKBlock, &DesRect, iNULL);
		}
	}


	DesRect.X = Bath_X;
	DesRect.Y = Bath_Y;
	DesRect.Width= Bath_W;
	DesRect.Height= Bath_H;
	i51KitG2DrawImage(mwContainer, ChsLvBath, &DesRect, iNULL);
	
	DesRect.X = Word_X;
	DesRect.Y = Word_Y;
	DesRect.Width= Word_W;
	DesRect.Height= Word_H;
	i51KitG2DrawImage(mwContainer, XiaoWanPiWord, &DesRect, iNULL);

	mwLevelDrawReturn();
	
	DesRect.Width = ICON_WIDTH;
	DesRect.Height= ICON_HEIGHT;
	for( int i = 0; i< TOTAL_LEVEL; i++ )
	{
		CurrDuckNum = LevelDuckNum[i];
		if(CurrDuckNum>MAXDUCK_ONELEVEL)
		{
//			iLog("Get level:%dduck numer error:%d",i,CurrDuckNum);
			continue;
		}
		DesRect.X = LEVEL_START_LEVEL1_X + (i%ICON_NUM_ONELINE)*LEVEL_START_LEVEL_ADDX;
		DesRect.Y = LEVEL_START_LEVEL1_Y + (i/ICON_NUM_ONELINE)*LEVEL_START_LEVEL_ADDY;
		if(i<UnlockMaxLevel)
		{
			i51KitG2DrawImage(mwContainer, ICON_NUM_ID[CurrDuckNum], &DesRect, iNULL);
			if( i<9 )
			{
				DesRect.X += 14;
			}
			else
			{
				DesRect.X += 7;
			}
			DesRect.Y += 7;
			mwLevelDrawNum(i+1,NumBig,&DesRect)			;
		}
		else
		{
			i51KitG2DrawImage(mwContainer, ChsLvLock, &DesRect, iNULL);
//			mwLevelDrawNum(i,NumBig,&DesRect)			;
		}
	}

	DesRect.X = LEVEL_START_DUCK_NUM_X;
	DesRect.Y = LEVEL_START_DUCK_NUM_Y;
	DesRect.Width = 121;
	DesRect.Height = 37;
	mwDrawNumStep(mwContainer, NumBig, &DesRect, CurrTotalDuck, TOTAL_DUCK_NUM);
//	iU32 ImageId[2] = {ChsLvBK};
//	i51KitG2SetImageMemoryCopy(mwContainer, 1, ImageId);

//iLog("end choose!");

	return iTRUE;
}

iBOOL mwInitLevelChoose( void )
{
	mwWordPause = iTRUE;
	if( mwLevelChooseAnimation() )
	{
		if( mwLevelChooseInitDraw() ) return iTRUE;
		else return iFALSE;
	
	}

	return iFALSE;
}

AnimationStr AniModeLeadBathe;
iBOOL mwRunLevelChoose( void )
{
#if 0
	MoveRect.X = 0 ;
	MoveRect.Y = 0 ;
	MoveRect.Width= SCREEN_WIDTH;
	MoveRect.X = SCREEN_HEIGHT;
#endif
	static iU8 Flag = iFALSE;

	if( iFALSE == Flag )
	{
		Flag = iTRUE;
		iU32 ImageId[9] = {
			ModeLeadBathe00,ModeLeadBathe01,ModeLeadBathe02,
			ModeLeadBathe03,ModeLeadBathe04,ModeLeadBathe05,
			ModeLeadBathe06,ModeLeadBathe07,ModeLeadBathe08
		};
		AniModeLeadBathe.Container = mwContainer;
		AniModeLeadBathe.CurrPlayFrame = 0;
		AniModeLeadBathe.DesRect.X = ModeLeadBathe_X;
		AniModeLeadBathe.DesRect.Y = ModeLeadBathe_Y;
		AniModeLeadBathe.DesRect.Width = ModeLeadBathe_W;
		AniModeLeadBathe.DesRect.Height = ModeLeadBathe_H;
		AniModeLeadBathe.FrameTimeMul = 250;
		for( int i=0; i<9; i++ )
		{
			AniModeLeadBathe.ImageId[i] = ImageId[i];
		}
		AniModeLeadBathe.ImageTotalFrame = 9;
		AniModeLeadBathe.ImageWidth = ModeLeadBathe_W;
		AniModeLeadBathe.OldPlayTime = i51AdeOsGetTick();

		Water1.X = MODE_WATER_X1;
		Water1.Y = MODE_WATER_Y1;
		Water1.Width= MODE_WATER_W1;
		Water1.Height= MODE_WATER_H1;
		
		Water2.X = (SCREEN_WIDTH-MODE_WATER_W2);
		Water2.Y = MODE_WATER_Y2;
		Water2.Width= MODE_WATER_W2;
		Water2.Height= MODE_WATER_H2;
		
		ImageId[0] = ChooseBKBlock;
		ImageId[1] = ChsLvBath;
		ImageId[2] = ChooseWater;
		ImageId[3] = XiaoWanPiWord;
		i51KitG2SetImageMemoryCopy(mwContainer, 4, ImageId );
	}

	iRECT SrcRect;
	iRECT DesRect;
	
//	mwLevelChooseInitDraw();

	iS16 Bath_X = 0;
	iS16 Bath_Y = 360;
	iS16 Bath_W = SCREEN_WIDTH;
	iS16 Bath_H = 120;

	iS16 Word_X = 75;
	iS16 Word_Y = 268;
	iS16 Word_W = 171;
	iS16 Word_H = 24;


	iS16 Block_W = 81;
	iS16 Block_H = 81;
	iS16 Block_StartX = 0;
	iS16 Block_StartY = 160;

	for( int Y = Block_StartY; Y<=SCREEN_HEIGHT; Y+=Block_H )
	{
		for( int X = Block_StartX; X<=SCREEN_WIDTH; X+=Block_W )
		{
			iRECT SrcRect;

			DesRect.X = X;
			DesRect.Y = Y;
			DesRect.Width= Block_W;
			DesRect.Height= Block_H;

			SrcRect.X = 0;
			SrcRect.Y = 0;
			SrcRect.Width = Block_W;
			SrcRect.Height = Block_H;
			
			if( DesRect.Y<ModeLeadBathe_Y )
			{
				DesRect.Height -= ModeLeadBathe_Y - DesRect.Y;
				SrcRect.Y += ModeLeadBathe_Y - DesRect.Y;
				DesRect.Y = ModeLeadBathe_Y;
			}

			i51KitG2DrawImage(mwContainer, ChooseBKBlock, &DesRect, &SrcRect);
		}
	}

	DesRect.X = Bath_X;
	DesRect.Y = Bath_Y;
	DesRect.Width= Bath_W;
	DesRect.Height= Bath_H;
	i51KitG2DrawImage(mwContainer, ChsLvBath, &DesRect, iNULL);
	
	DesRect.X = Word_X;
	DesRect.Y = Word_Y;
	DesRect.Width= Word_W;
	DesRect.Height= Word_H;
	i51KitG2DrawImage(mwContainer, XiaoWanPiWord, &DesRect, iNULL);

	i51KitG2SetAlpha(178);
	SrcRect.X = 0;
	SrcRect.Y = 0;
	SrcRect.Width = MODE_WATER_W1;
	SrcRect.Height = MODE_WATER_H1;
	i51KitG2DrawImage(mwContainer, ChooseWater, &Water1, &SrcRect);
	DesRect.X = Water1.X+MODE_WATER_W1;
	DesRect.Y = Water1.Y;
	DesRect.Width= Water1.Width;
	DesRect.Height= Water1.Height;
	i51KitG2DrawImage(mwContainer, ChooseWater, &DesRect, &SrcRect);
	Water1.X -= 8;
	if( Water1.X <= -MODE_WATER_W1 ) 
	{
		Water1.X += MODE_WATER_W1;
	}
	i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);

	mwPublicAnimation(&AniModeLeadBathe);
	
	i51KitG2SetAlpha(178);
	SrcRect.X = 0;
	SrcRect.Y = MODE_WATER_H2;
	SrcRect.Width = MODE_WATER_W2;
	SrcRect.Height = MODE_WATER_H2;
	i51KitG2DrawImage(mwContainer, ChooseWater, &Water2, &SrcRect);
	DesRect.X = Water2.X-MODE_WATER_W1;
	DesRect.Y = Water2.Y;
	DesRect.Width= Water2.Width;
	DesRect.Height= Water2.Height;
	i51KitG2DrawImage(mwContainer, ChooseWater, &DesRect, &SrcRect);
	Water2.X += 8;
	if( Water2.X >= SCREEN_WIDTH) 
	{
		Water2.X -= MODE_WATER_W1;
	}
	i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);

	if(ChooseErrFlag)
	{
		if( mwPublicDrawPrompt(Prompt_Time, ChooseErrTime, 0) )
		{
			mwLevelChooseInitDraw();
			ChooseErrFlag = iFALSE;
		}
	}

	if( ChooseErrFlag==iFALSE && mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
	{
		iU16 PenDownX = mwKeyData.u.pen.x;
		iU16 PenDownY = mwKeyData.u.pen.y;

		if(mwLevelPressReturn(PenDownX, PenDownY))
		{
			CurrGameState = GAME_STATE_MODE;
			i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL);
		}
		else if( ((PenDownX-LEVEL_START_LEVEL1_X)%LEVEL_START_LEVEL_ADDX < ICON_WIDTH)
		&&( (PenDownX-LEVEL_START_LEVEL1_X)/LEVEL_START_LEVEL_ADDX < ICON_NUM_ONELINE)
		&&( (PenDownY-LEVEL_START_LEVEL1_Y)%LEVEL_START_LEVEL_ADDY < ICON_HEIGHT)
		&&( (PenDownY-LEVEL_START_LEVEL1_Y)/LEVEL_START_LEVEL_ADDY < ICON_NUM_MAXLINE)
		&&( (PenDownX>=LEVEL_START_LEVEL1_X && PenDownX<= (LEVEL_START_LEVEL1_X +LEVEL_START_LEVEL_ADDX*4 + ICON_WIDTH )))
		&&( (PenDownY>=LEVEL_START_LEVEL1_Y && PenDownY<= (LEVEL_START_LEVEL1_Y +LEVEL_START_LEVEL_ADDY*2 + ICON_HEIGHT)))
		)
		{
//		iLog("Choose level");
			iU16 CurrChooseLevel = ((PenDownY-LEVEL_START_LEVEL1_Y)/LEVEL_START_LEVEL_ADDY)*ICON_NUM_ONELINE
				+ ((PenDownX-LEVEL_START_LEVEL1_X)/LEVEL_START_LEVEL_ADDX);
//	iLog("CurrChooseLevel:%d",CurrChooseLevel);
			if( CurrChooseLevel<UnlockMaxLevel )
			{
				iU32 ICON_NUM_ID[4] = {ChsLvPrs0,ChsLvPrs1,ChsLvPrs2,ChsLvPrs3};
				iRECT DesRect;
				DesRect.Width = ICON_WIDTH;
//iLog("DesRect.Width:%d",DesRect.Width);
				DesRect.Height= ICON_HEIGHT;
//iLog("DesRect.Height:%d",DesRect.Height);
				DesRect.X = LEVEL_START_LEVEL1_X + (((int)CurrChooseLevel)%((int)ICON_NUM_ONELINE))*LEVEL_START_LEVEL_ADDX;
//iLog("DesRect.X:%d",DesRect.X);
				DesRect.Y = LEVEL_START_LEVEL1_Y + (((int)CurrChooseLevel)/((int)ICON_NUM_ONELINE))*LEVEL_START_LEVEL_ADDY;
//iLog("DesRect.Y:%d",DesRect.Y);
//				mwLevelChooseInitDraw();
				i51KitG2DrawImage(mwContainer, ICON_NUM_ID[LevelDuckNum[CurrChooseLevel]], &DesRect, iNULL);
				CurrLevelNum = (mwLevelNum)(CurrChooseLevel + 1);
				CurrLevelState = MW_LEVEL_INIT;
				i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL);
				pfunc->public_audio_stop(AUDIO_PLAYER_WAV_0);
				SoundChoosePlay = iFALSE;
//iLog("CurrLevelNum:%d,CurrLevelState:%d",CurrLevelNum,CurrLevelState);
//				mwWordPause = iFALSE;
//				i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL);
//				iU32 ImageId[13] = {DuckMud,DuckGet,LeadWait00,LeadWait01,
//					LeadWait02,LeadWait03,LeadWait04,LeadWait05,
//					LeadWait06,LeadWait07,LeadWait08,LeadWait09,LeadWait10};
//				i51KitG2SetImageMemoryCopy(mwContainer, 13, ImageId);
			}
			else
			{
//				ChooseErrTime = i51AdeOsGetTick();
//				ChooseErrFlag = iTRUE;
			}
		}
	}
	return iTRUE;
}
