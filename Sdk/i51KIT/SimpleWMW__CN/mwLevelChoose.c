
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#define LEVEL_START_RECORD_X	208
#define LEVEL_START_RECORD_Y	13
#define LEVEL_START_LEVEL1_X	19
#define LEVEL_START_LEVEL1_Y	49
#define LEVEL_START_LEVEL_ADDX	43
#define LEVEL_START_LEVEL_ADDY	44
#define LEVEL_START_DUCK_NUM_X	146
#define LEVEL_START_DUCK_NUM_Y	12
#define ICON_WIDTH	31
#define ICON_HEIGHT	31

#define ModeLeadBathe_X		32
#define ModeLeadBathe_Y		168
#define ModeLeadBathe_W		164
#define ModeLeadBathe_H		155

#define MODE_WATER_X1	0
#define MODE_WATER_Y1	253
#define MODE_WATER_W1	319
#define MODE_WATER_H1	60

#define MODE_WATER_X2	0
#define MODE_WATER_Y2	280
#define MODE_WATER_W2	319
#define MODE_WATER_H2	60

#elif defined(PKIG_SCREEN_240X400)	
#define LEVEL_START_RECORD_X	154
#define LEVEL_START_RECORD_Y	12
#define LEVEL_START_LEVEL1_X	14
#define LEVEL_START_LEVEL1_Y	69
#define LEVEL_START_LEVEL_ADDX	45
#define LEVEL_START_LEVEL_ADDY	45
#define LEVEL_START_DUCK_NUM_X	139
#define LEVEL_START_DUCK_NUM_Y	6
#define ICON_WIDTH	33
#define ICON_HEIGHT	34

#define ModeLeadBathe_X		19
#define ModeLeadBathe_Y		194
#define ModeLeadBathe_W	210
#define ModeLeadBathe_H		198

#define MODE_WATER_X1	0
#define MODE_WATER_Y1	320
#define MODE_WATER_W1	386
#define MODE_WATER_H1	80

#define MODE_WATER_X2	0
#define MODE_WATER_Y2	354
#define MODE_WATER_W2	386
#define MODE_WATER_H2	80

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

iU8 LevelDuckNum[TOTAL_LEVEL] ;
//iU32 UnlockMaxLevel = 15;
iU32 UnlockMaxLevel = 3;
iU32 CurrTotalDuck = 15;
iU32 MoveOldTime = 0;
#define MOVE_MUL_X 5
#define MOVE_MUL_Y 5
iRECT MoveRect;
iRECT Water1;
iRECT Water2;

iS16 MulWater1 = 8;
iS16 MulWater2 = -8;

//static iS8 ChooseErrFlag = iFALSE;
//static iU32 ChooseErrTime = 0;
 iS8 ChooseErrFlag = iFALSE;
 iU32 ChooseErrTime = 0;

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
	iU32 PicHeight = 0;
	
	if( Number>99 || iNULL==DesRect) return iFALSE;

	if (ImageID == NumBig)
	{
		i51KitG2GetImageAttrEx((iU32)NumBig_Hex,  &ImageWidth, &ImageHeight);

	}
	else if (ImageID == NumSmall)
	{
		i51KitG2GetImageAttrEx((iU32)NumSmall_Hex,  &ImageWidth, &ImageHeight);

	}
	//i51KitG2GetImageAttr(mwContainer, ImageID, &ImageWidth, &ImageHeight, iNULL);

	PicHeight = ImageHeight/12;
	
	if( Number<10 )
	{
		SrcRect.X = 0;
		SrcRect.Y = Number*PicHeight;
		SrcRect.Width = ImageWidth;
		SrcRect.Height = PicHeight;
		//i51KitG2DrawImage(mwContainer, ImageID, DesRect, &SrcRect);

		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], DesRect, &SrcRect);

	}
	else
	{
		SrcRect.X = 0;
		SrcRect.Y = (Number/10)*PicHeight;
		SrcRect.Width = ImageWidth;
		SrcRect.Height = PicHeight;
		//i51KitG2DrawImage(mwContainer, ImageID, DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], DesRect, &SrcRect);

		DesRect->X += ImageWidth;
		SrcRect.X = 0;
		SrcRect.Y = (Number%10)*PicHeight;
		SrcRect.Width = ImageWidth;
		SrcRect.Height = PicHeight;
		//i51KitG2DrawImage(mwContainer, ImageID, DesRect, &SrcRect);
		SWMW_i51KitG2DrawImage(&iMageInfo[ImageID], DesRect, &SrcRect);

	}

	return iTRUE;
	
}

iBOOL mwLevelChooseInitDraw( void )
{
	iRECT DesRect;
	iRECT SrcRect;
	iU8 CurrDuckNum = 0;
	//iU32 ICON_NUM_ID[4] = {ChsLv0,ChsLv1,ChsLv2,ChsLv3};

#ifdef PKIG_SCREEN_240X320
	iS16 Bath_X = 0;
	iS16 Bath_Y = 231;
	iS16 Bath_W = SCREEN_WIDTH;
	iS16 Bath_H = 90;

	iS16 Word_X = 56;
	iS16 Word_Y = 192;
	iS16 Word_W = 171;
	iS16 Word_H = 24;

	iS16 Block_W = 61;
	iS16 Block_H = 61;
	iS16 Block_StartX = 0;
	iS16 Block_StartY = 118;
	iS16 NumFrame_W = 92;
	iS16 NumFrame_H = 32;

#elif defined PKIG_SCREEN_240X400

	iS16 Bath_X = 0;
	iS16 Bath_Y = 360;
	iS16 Bath_W = SCREEN_WIDTH;
	iS16 Bath_H = 120;

	iS16 Word_X = 75;
	iS16 Word_Y = 268;
	iS16 Word_W = 171;
	iS16 Word_H = 24;

	iS16 Block_W = 61;
	iS16 Block_H = 61;
	iS16 Block_StartX = 0;
	iS16 Block_StartY = 118;
	iS16 NumFrame_W = 92;
	iS16 NumFrame_H = 32;


#elif defined PKIG_SCREEN_320X480

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
	iS16 NumFrame_W = 121;
	iS16 NumFrame_H = 37;

#endif

	int X,Y,i;
	
#if SOUND_NEED
	if(iFALSE==MusicCloseFlag && SoundChoosePlay == iFALSE)  //xie
	//if(iFALSE==MusicCloseFlag && SoundWinPlay == iFALSE)
	{
		//SoundWinPlay = iTRUE;

		SoundChoosePlay = iTRUE;

		//pfunc->public_audio_play(
		//	0, 
		//	MW_SOUND_NAME_CHOOSE, 
		//	i51AdeStdWStrlen((iU16*)MW_SOUND_NAME_CHOOSE), 
		//	i51_MMI_DEC_FILE_BASED,
		//	AUDIO_PLAYER_WAV_0, 
		//	iTRUE
		//	);
		
		public_audio_play_x(0, wmw_SoundChoose_mp3, wmw_SoundChoose_mp3_len,i51_MMI_DEC_MEM_BASED, AUDIO_PLAYER_MP3, iTRUE);
	}
#endif

//iLog("start choose!");
	if(UnlockMaxLevel>TOTAL_LEVEL)
	{
		iLog("MaxLevel:%d>TOTAL_LEVEL:%d",UnlockMaxLevel,TOTAL_LEVEL);
		return iFALSE;
	}

	SWMW_i51KitG2DrawImage(&iMageInfo[ChooseBKHead], iNULL, iNULL);

	for( Y = Block_StartY; Y<=SCREEN_HEIGHT; Y+=Block_H )
	{
		for( X = Block_StartX; X<=SCREEN_WIDTH; X+=Block_W )
		{
			DesRect.X = X;
			DesRect.Y = Y;
			DesRect.Width= Block_W;
			DesRect.Height= Block_H;
			SWMW_i51KitG2DrawImage(&iMageInfo[ChooseBKBlock], &DesRect, iNULL);
		}
	}


	DesRect.X = Bath_X;
	DesRect.Y = Bath_Y;
	DesRect.Width= Bath_W;
	DesRect.Height= Bath_H;
	//SWMW_i51KitG2DrawImage(mwContainer, ChsLvBath, &DesRect, iNULL);
	SWMW_i51KitG2DrawImage(&iMageInfo[ChsLvBath], &DesRect, iNULL);

	
	DesRect.X = Word_X;
	DesRect.Y = Word_Y;
	DesRect.Width= Word_W;
	DesRect.Height= Word_H;

	mwLevelDrawReturn();
	
	DesRect.Width = ICON_WIDTH;
	DesRect.Height= ICON_HEIGHT;
	for( i = 0; i< TOTAL_LEVEL; i++ )
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
			SrcRect.X = 0;
			SrcRect.Y = CurrDuckNum * 2 * ICON_HEIGHT ;
			SrcRect.Width = ICON_WIDTH;
			SrcRect.Height = ICON_HEIGHT;
			//i51KitG2DrawImage(mwContainer, ChsLv, &DesRect, &SrcRect);

			SWMW_i51KitG2DrawImage(&iMageInfo[ChsLv], &DesRect, &SrcRect);


			if( i<9 )
			{
				DesRect.X += 14;
			}
			else
			{
				DesRect.X += 7;
			}
			DesRect.Y += 7;
#if defined(PKIG_SCREEN_240X320)
			mwLevelDrawNum(i+1,NumSmall,&DesRect);
#elif defined(PKIG_SCREEN_240X400)	
			mwLevelDrawNum(i+1,NumSmall,&DesRect);
#elif defined(PKIG_SCREEN_320X480)	
			mwLevelDrawNum(i+1,NumBig,&DesRect);
#endif
		}
		else
		{
			SrcRect.X = 0;
			SrcRect.Y = ICON_HEIGHT * 8;
			SrcRect.Width = ICON_WIDTH;
			SrcRect.Height = ICON_HEIGHT;			
			//i51KitG2DrawImage(mwContainer, ChsLv, &DesRect, &SrcRect);
			SWMW_i51KitG2DrawImage(&iMageInfo[ChsLv], &DesRect, &SrcRect);


		}
	}

	DesRect.X = LEVEL_START_DUCK_NUM_X;
	DesRect.Y = LEVEL_START_DUCK_NUM_Y;
	DesRect.Width = NumFrame_W;
	DesRect.Height = NumFrame_H;
#if defined(PKIG_SCREEN_240X320)
	mwDrawNumStep(mwContainer, NumSmall, &DesRect, CurrTotalDuck, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_240X400)	
	mwDrawNumStep(mwContainer, NumSmall, &DesRect, CurrTotalDuck, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_320X480)	
	mwDrawNumStep(mwContainer, NumBig, &DesRect, CurrTotalDuck, TOTAL_DUCK_NUM);
#endif

	//DesRect.X = LEVEL_START_DUCK_NUM_X;
	//DesRect.Y = LEVEL_START_DUCK_NUM_Y;
	//DesRect.Width = 121;
	//DesRect.Height = 37;
	//mwDrawNumStep(mwContainer, NumBig, &DesRect, CurrTotalDuck, TOTAL_DUCK_NUM);
//	iU32 ImageId[2] = {ChsLvBK};
//	i51KitG2SetImageMemoryCopy(mwContainer, 1, ImageId);

//iLog("end choose!");

	return iTRUE;
}

iBOOL mwInitLevelChoose( void )
{
	mwWordPause = iTRUE;
	//if( mwLevelChooseAnimation() )
	//{
	//	if( mwLevelChooseInitDraw() ) return iTRUE;
	//	else return iFALSE;
	//
	//}

	return iTRUE;
}

AnimationStr AniModeLeadBathe;
iBOOL mwRunLevelChoose( void )
{

	static iU8 Flag = iFALSE;
	int i;
	//iU32 ImageId[9] = {
	//	ModeLeadBathe00,ModeLeadBathe01,ModeLeadBathe02,
	//	ModeLeadBathe03,ModeLeadBathe04,ModeLeadBathe05,
	//	ModeLeadBathe06,ModeLeadBathe07,ModeLeadBathe08
	//};

	iRECT SrcRect;
	iRECT DesRect;

	int X,Y;

	iU16 PenDownX = 0;
	iU16 PenDownY = 0;


#ifdef PKIG_SCREEN_240X320

	iS16 Bath_X = 0;
	iS16 Bath_Y = 231;
	iS16 Bath_W = SCREEN_WIDTH;
	iS16 Bath_H = 90;

	iS16 Word_X = 56;
	iS16 Word_Y = 192;
	iS16 Word_W = 171;
	iS16 Word_H = 24;

	iS16 Block_W = 61;
	iS16 Block_H = 61;
	iS16 Block_StartX = 0;
	iS16 Block_StartY = 120;

#elif defined PKIG_SCREEN_240X400

	iS16 Bath_X = 0;
	iS16 Bath_Y = 300;
	iS16 Bath_W = SCREEN_WIDTH;
	iS16 Bath_H = 100;

	iS16 Word_X = 54;
	iS16 Word_Y = 239;
	iS16 Word_W = 171;
	iS16 Word_H = 24;

	iS16 Block_W = 61;
	iS16 Block_H = 61;
	iS16 Block_StartX = 0;
	iS16 Block_StartY = 120;

#elif defined PKIG_SCREEN_320X480

	iS16 Bath_X = 0;
	iS16 Bath_Y = 360;
	iS16 Bath_W = SCREEN_WIDTH;
	iS16 Bath_H = 120;

	//iS16 Word_X = 75;
	//iS16 Word_Y = 268;
	//iS16 Word_W = 171;
	//iS16 Word_H = 24;

	iS16 Block_W = 81;
	iS16 Block_H = 81;
	iS16 Block_StartX = 0;
	iS16 Block_StartY = 160;

	#ifdef  LANGUAGE_ENGLISH

		iS16 Word_X = 75;
		iS16 Word_Y = 268;
		iS16 Word_W = 187;
		iS16 Word_H = 28;
	
	#else
		iS16 Word_X = 75;
		iS16 Word_Y = 268;
		iS16 Word_W = 171;
		iS16 Word_H = 24;
	
	#endif


#endif


//	iS16 Bath_X = 0;
//	iS16 Bath_Y = 360;
//	iS16 Bath_W = SCREEN_WIDTH;
//	iS16 Bath_H = 120;
//
//#ifdef  LANGUAGE_ENGLISH
//	iS16 Word_X = 75;
//	iS16 Word_Y = 268;
//	iS16 Word_W = 187;
//	iS16 Word_H = 28;
//
//#else
//	iS16 Word_X = 75;
//	iS16 Word_Y = 268;
//	iS16 Word_W = 171;
//	iS16 Word_H = 24;
//
//#endif
//
//
//	iS16 Block_W = 81;
//	iS16 Block_H = 81;
//	iS16 Block_StartX = 0;
//	iS16 Block_StartY = 160;


	if( iFALSE == Flag )
	{
		Flag = iTRUE;
		AniModeLeadBathe.Container = mwContainer;
		AniModeLeadBathe.CurrPlayFrame = 0;
		AniModeLeadBathe.DesRect.X = ModeLeadBathe_X;
		AniModeLeadBathe.DesRect.Y = ModeLeadBathe_Y;
		AniModeLeadBathe.DesRect.Width = ModeLeadBathe_W;
		AniModeLeadBathe.DesRect.Height = ModeLeadBathe_H;
		AniModeLeadBathe.FrameTimeMul = 250;
		for( i=0; i<9; i++ )
		{
			//AniModeLeadBathe.ImageId[i] = ImageId[i];
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
		
		//ImageId[0] = ChooseBKBlock;
		//ImageId[2] = ChsLvBath;
		//ImageId[4] = ChooseWater;
		//ImageId[6] = XiaoWanPiWord;
		//i51KitG2SetImageMemoryCopy(mwContainer, 8, ImageId );
	}



	for( Y = Block_StartY; Y<=SCREEN_HEIGHT; Y+=Block_H )
	{
		for( X = Block_StartX; X<=SCREEN_WIDTH; X+=Block_W )
		{
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

			SWMW_i51KitG2DrawImage(&iMageInfo[ChooseBKBlock], &DesRect, &SrcRect);
		}
	}

	DesRect.X = Bath_X;
	DesRect.Y = Bath_Y;
	DesRect.Width= Bath_W;
	DesRect.Height= Bath_H;
	//i51KitG2DrawImage(mwContainer, ChsLvBath, &DesRect, iNULL);
	SWMW_i51KitG2DrawImage(&iMageInfo[ChsLvBath], &DesRect, iNULL);


	DesRect.X = Word_X;
	DesRect.Y = Word_Y;
	DesRect.Width= Word_W;
	DesRect.Height= Word_H;

	//i51KitG2SetAlpha(178);
	LocalG2AlphaSys = 178;
	SrcRect.X = 0;
	SrcRect.Y = 0;
	SrcRect.Width = MODE_WATER_W1;
	SrcRect.Height = MODE_WATER_H1;
	//i51KitG2DrawImage(mwContainer, ChooseWater, &Water1, &SrcRect);
	SWMW_i51KitG2DrawImage(&iMageInfo[ChooseWater], &Water1, &SrcRect);

	DesRect.X = Water1.X+MODE_WATER_W1;
	DesRect.Y = Water1.Y;
	DesRect.Width= Water1.Width;
	DesRect.Height= Water1.Height;
	//i51KitG2DrawImage(mwContainer, ChooseWater, &DesRect, &SrcRect);
	SWMW_i51KitG2DrawImage(&iMageInfo[ChooseWater],&DesRect, &SrcRect);

	Water1.X -= 8;
	if( Water1.X <= -MODE_WATER_W1 ) 
	{
		Water1.X += MODE_WATER_W1;
	}
	//i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);

	LocalG2AlphaSys = i51KITG2_ALPHA_NONEED;

	mwPublicAnimation(&AniModeLeadBathe);
	
	//i51KitG2SetAlpha(178);

	LocalG2AlphaSys = 178;

	SrcRect.X = 0;
	SrcRect.Y = MODE_WATER_H2;
	SrcRect.Width = MODE_WATER_W2;
	SrcRect.Height = MODE_WATER_H2;
	//i51KitG2DrawImage(mwContainer, ChooseWater, &Water2, &SrcRect);
	SWMW_i51KitG2DrawImage(&iMageInfo[ChooseWater],&Water2, &SrcRect);

	DesRect.X = Water2.X-MODE_WATER_W1;
	DesRect.Y = Water2.Y;
	DesRect.Width= Water2.Width;
	DesRect.Height= Water2.Height;
	//i51KitG2DrawImage(mwContainer, ChooseWater, &DesRect, &SrcRect);

	SWMW_i51KitG2DrawImage(&iMageInfo[ChooseWater],&DesRect, &SrcRect);


	Water2.X += 8;
	if( Water2.X >= SCREEN_WIDTH) 
	{
		Water2.X -= MODE_WATER_W1;
	}
	//i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);

	LocalG2AlphaSys = i51KITG2_ALPHA_NONEED;


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
		PenDownX = mwKeyData.u.pen.x;
		PenDownY = mwKeyData.u.pen.y;

		if(mwLevelPressReturn(PenDownX, PenDownY))
		{
			CurrGameState = GAME_STATE_MAIN;
			//i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL);
			wmw_image_destroy();
			wmw_main_page();
			Flag = iFALSE; 
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
				//iU32 ICON_NUM_ID[4] = {ChsLvPrs0,ChsLvPrs1,ChsLvPrs2,ChsLvPrs3};
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

				SrcRect.X = 0;
				SrcRect.Y = (LevelDuckNum[CurrChooseLevel] * 2 + 1) * ICON_HEIGHT;
				SrcRect.Width = ICON_WIDTH;
				SrcRect.Height = ICON_HEIGHT;
				//i51KitG2DrawImage(mwContainer, ChsLv, &DesRect, &SrcRect);

				CurrLevelNum = (mwLevelNum)(CurrChooseLevel + 1);
				CurrLevelState = MW_LEVEL_INIT;
				
				wmw_image_destroy();


				//i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL);
				Flag = iFALSE; 
				public_audio_stop(AUDIO_PLAYER_MP3);
				
				//SoundWinPlay = iFALSE;
				SoundChoosePlay = iFALSE; //xie


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
