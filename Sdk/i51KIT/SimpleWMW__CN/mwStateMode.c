
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#endif

#define PIC_MOVE_MUL	10

iBOOL mwStateMode( void )
{
#if 0
#if defined(PKIG_SCREEN_240X320)
	iS16 SlideLine_X = 43;
	iS16 SlideLine_Y = 191;
	iS16 SlideLine_W = 157;
	iS16 SlideLine_H = 18;
	iS16 SlidePiece_X = 55;
	iS16 SlidePiece_Y = 193;
	iS16 SlidePiece_W = 11;
	iS16 SlidePiece_H = 12;
	iS16 SlidePiece_OFFSET_X = 18;
	iS16 SlidePoint_X = 58;
	iS16 SlidePoint_Y = 197;
	iS16 SlidePoint_W = 6;
	iS16 SlidePoint_H = 5;
	iS16 SlidePoint_OFFSET_X = 18;
	iS16 CurrWord_X = 58;
	iS16 CurrWord_Y = 44;
	iS16 CurrWord_W = 130;
	iS16 CurrWord_H = 20;
	iS16 CurrPic_X = 67;
	iS16 CurrPic_Y = 70;
	iS16 CurrPic_W = 112;
	iS16 CurrPic_H = 113;
	iS16 ShouCang_X = 76;
	iS16 ShouCang_Y = 252;
	iS16 ShouCang_W = 97;
	iS16 ShouCang_H = 27;
	iS16 ChengJiu_X = 76;
	iS16 ChengJiu_Y = 284;
	iS16 ChengJiu_W = 97;
	iS16 ChengJiu_H = 27;

	iS16 ChsLvMode_X = 0;
	iS16 ChsLvMode_Y = 219;
	iS16 ChsLvMode_W = SCREEN_WIDTH;
	iS16 ChsLvMode_H = 101;
	iS16 NumFrame_W = 92;
	iS16 NumFrame_H = 32;

	iS16 Block_W = 61;
	iS16 Block_H = 61;
	iS16 Block_StartX = 0;
	iS16 Block_StartY = 118;
	iS16 NumFrameMulX = 80;
	iS16 NumFrameMulY = -58;

#elif defined(PKIG_SCREEN_240X400)	
	iS16 SlideLine_X = 33;
	iS16 SlideLine_Y = 223;
	iS16 SlideLine_W = 171;
	iS16 SlideLine_H = 22;
	iS16 SlidePiece_X = 51;
	iS16 SlidePiece_Y = 226;
	iS16 SlidePiece_W = 13;
	iS16 SlidePiece_H = 15;
	iS16 SlidePoint_X = 54;
	iS16 SlidePoint_Y = 231;
	iS16 SlidePoint_W = 5;
	iS16 SlidePoint_H = 6;
	iS16 CurrWord_X = 55;
	iS16 CurrWord_Y = 48;
	iS16 CurrWord_W = 130;
	iS16 CurrWord_H = 20;
	iS16 CurrPic_X = 58;
	iS16 CurrPic_Y = 76;
	iS16 CurrPic_W = 123;
	iS16 CurrPic_H = 137;
	iS16 ShouCang_X = 70;
	iS16 ShouCang_Y = 301;
	iS16 ShouCang_W = 101;
	iS16 ShouCang_H = 34;
	iS16 ChengJiu_X = 70;
	iS16 ChengJiu_Y = 345;
	iS16 ChengJiu_W = 101;
	iS16 ChengJiu_H = 34;

	iS16 ChsLvMode_X = 0;
	iS16 ChsLvMode_Y = 253;
	iS16 ChsLvMode_W = SCREEN_WIDTH;
	iS16 ChsLvMode_H = 147;
	iS16 NumFrame_W = 92;
	iS16 NumFrame_H = 32;

	iS16 Block_W = 61;
	iS16 Block_H = 61;
	iS16 Block_StartX = 0;
	iS16 Block_StartY = 120;
	iS16 NumFrameMulX = 81;
	iS16 NumFrameMulY = -70;
	iS16 SlidePiece_OFFSET_X = 19;
	iS16 SlidePoint_OFFSET_X = 19;

#elif defined(PKIG_SCREEN_320X480)
	iS16 SlideLine_X = 46;
	iS16 SlideLine_Y = 269;
	iS16 SlideLine_W = 226;
	iS16 SlideLine_H = 25;
	iS16 SlidePiece_X = 68;
	iS16 SlidePiece_Y = 273;
	iS16 SlidePiece_W = 26;
	iS16 SlidePiece_H = 17;
	iS16 SlidePoint_X = 74;
	iS16 SlidePoint_Y = 278;
	iS16 SlidePoint_W = 26;
	iS16 SlidePoint_H = 8;
	iS16 CurrPic_X = 80;
	iS16 CurrPic_Y = 96;
	iS16 CurrPic_W = 162;
	iS16 CurrPic_H = 161;
	iS16 ShouCang_X = 95;
	iS16 ShouCang_Y = 361;
	iS16 ShouCang_W = 128;
	iS16 ShouCang_H = 41;
	iS16 ChengJiu_X = 96;
	iS16 ChengJiu_Y = 414;
	iS16 ChengJiu_W = 128;
	iS16 ChengJiu_H = 41;

	iS16 ChsLvMode_X = 0;
	iS16 ChsLvMode_Y = 305;
	iS16 ChsLvMode_W = SCREEN_WIDTH;
	iS16 ChsLvMode_H = 175;
	iS16 NumFrame_W = 121;
	iS16 NumFrame_H = 37;

	iS16 Block_W = 81;
	iS16 Block_H = 81;
	iS16 Block_StartX = 0;
	iS16 Block_StartY = 160;
	iS16 NumFrameMulX = 106;
	iS16 NumFrameMulY = -83;

	iS16 SlidePiece_OFFSET_X = 18;
	iS16 SlidePoint_OFFSET_X = 18;

#ifdef LANGUAGE_ENGLISH
	iS16 CurrWord_X = 70;
	iS16 CurrWord_Y = 65;
	iS16 CurrWord_W = 187;
	iS16 CurrWord_H = 28;

#else
	iS16 CurrWord_X = 75;
	iS16 CurrWord_Y = 65;
	iS16 CurrWord_W = 168;
	iS16 CurrWord_H = 26;

#endif

#endif
	static iS8 Flag = iFALSE;
	static iS8 PromptType = 0;
	static iS8 CurrChoose = 1;
	static iS8 OldChoose = 1;
	static iU32 OldTime = 0;
	static iRECT OldRect;
	static iRECT NewRect;
	static iS16 RectMul = 0;

#define MODE_MEM_PIC	11
//	iU32 ImageId[8] = {ModeChsBK,ModeChsWord,PromptFrame,PromptWord,
//		SlideLine,SlidePiece,SlidePoint,SlideRock};
	iU32 ImageId[MODE_MEM_PIC] = {
			ModeChsWord,PromptFrame,PromptWord,
			SlideLine,SlidePiece,SlidePoint,SlideRock,XiaoWanPiPic,
			SlideRock,ChooseBKHead,ChooseBKBlock
		};
	iRECT DesRect;
	iRECT SrcRect;
	iRECT TempDes;
	static iU16 PenDownX;
	static iU16 PenDownY;
	static iU8 PenDownFlag = iFALSE;
	
	int X,Y,i;
	static iU8 PromptFlag = iFALSE;
	iS16 PenMoveX = 0;

//iLog("start mwStateMode,Flag=%d",Flag);
#if SOUND_NEED

	
	if(iFALSE==MusicCloseFlag && SoundChoosePlay == iFALSE)
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

		//pfunc->public_audio_play(0, SD_CHOOSE, AUDIO_PLAYER_MP3, iTRUE);

	}
#endif

	if(!Flag)
	{
		Flag = iTRUE;
		PromptType = 0;
		CurrChoose = 1;
		OldChoose = 1;
 		//i51KitG2SetImageMemoryCopy(mwContainer, MODE_MEM_PIC, ImageId);

		OldRect.X = CurrPic_X;
		OldRect.Y = CurrPic_Y;
		OldRect.Width= CurrPic_W;
		OldRect.Height= CurrPic_H;

		NewRect.X = CurrPic_X;
		NewRect.Y = CurrPic_Y;
		NewRect.Width= CurrPic_W;
		NewRect.Height= CurrPic_H;

		TempDes.X = ChsLvMode_X;
		TempDes.Y = ChsLvMode_Y;
		TempDes.Width = ChsLvMode_W;
		TempDes.Height = ChsLvMode_H;
		i51KitG2DrawImage(mwContainer, ChsLvMode, &TempDes, iNULL );

 	}

	//Block_W = 81;
	//Block_H = 81;
	//Block_StartX = 0;
	//Block_StartY = 160;

	i51KitG2DrawImage(mwContainer, ChooseBKHead, iNULL, iNULL);

	for( Y = Block_StartY; Y<=ChsLvMode_Y; Y+=Block_H )
	{
		for( X = Block_StartX; X<=SCREEN_WIDTH; X+=Block_W )
		{
			DesRect.X = X;
			DesRect.Y = Y;
			DesRect.Width= Block_W;
			DesRect.Height= Block_H;
			if( Y+Block_H >= ChsLvMode_Y )
			{
				DesRect.Height = ChsLvMode_Y - Y;
			}
			i51KitG2DrawImage(mwContainer, ChooseBKBlock, &DesRect, iNULL);
		}
	}
	
	if( CurrChoose==OldChoose )
	{
		DesRect.X = CurrWord_X;
		DesRect.Y = CurrWord_Y;
		DesRect.Width= CurrWord_W;
		DesRect.Height= CurrWord_H;
		SrcRect.X = 0;
		SrcRect.Y = CurrWord_H*CurrChoose;
		SrcRect.Width = CurrWord_W;
		SrcRect.Height = CurrWord_H;
		i51KitG2DrawImage(mwContainer, ModeChsWord, &DesRect, &SrcRect);

		DesRect.X = CurrPic_X;
		DesRect.Y = CurrPic_Y;
		DesRect.Width= CurrPic_W;
		DesRect.Height= CurrPic_H;
		if( CurrChoose==1 )
		{
			i51KitG2DrawImage(mwContainer, XiaoWanPiPic, &DesRect, iNULL);
			DesRect.X = DesRect.X + NumFrameMulX;
			DesRect.Y = DesRect.Y + NumFrameMulY;
			DesRect.Width = NumFrame_W;
			DesRect.Height = NumFrame_H;
//iLog("Draw XiaoWanPi");
////iLog("DesRect:X=%d,Y=%d,W=%d,H=%d,Des=:0X%X",DesRect.X,DesRect.Y,DesRect.Width,DesRect.Height,&DesRect);
#if defined(PKIG_SCREEN_240X320)
			mwDrawNumStep(mwContainer, NumSmall, &DesRect, CurrTotalDuck, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_240X400)	
			mwDrawNumStep(mwContainer, NumSmall, &DesRect, CurrTotalDuck, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_320X480)	
			mwDrawNumStep(mwContainer, NumBig, &DesRect, CurrTotalDuck, TOTAL_DUCK_NUM);
#endif
//iLog("Draw mwDrawNumStep");
		}
		else
		{
			i51KitG2DrawImage(mwContainer, SlideRock, &DesRect, iNULL);
			DesRect.X = DesRect.X + NumFrameMulX;
			DesRect.Y = DesRect.Y + NumFrameMulY;
			DesRect.Width = NumFrame_W;
			DesRect.Height = NumFrame_H;
#if defined(PKIG_SCREEN_240X320)
			mwDrawNumStep(mwContainer, NumSmall, &DesRect, 0, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_240X400)	
			mwDrawNumStep(mwContainer, NumSmall, &DesRect, 0, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_320X480)	
			mwDrawNumStep(mwContainer, NumBig, &DesRect, 0, TOTAL_DUCK_NUM);
#endif
		}
	}
	else
	{

		DesRect.X = (CurrWord_X-CurrPic_X) + OldRect.X;
		DesRect.Y = (CurrWord_Y-CurrPic_Y) + OldRect.Y;
		DesRect.Width= CurrWord_W;
		DesRect.Height= CurrWord_H;
		SrcRect.X = 0;
		SrcRect.Y = CurrWord_H*OldChoose;
		SrcRect.Width = CurrWord_W;
		SrcRect.Height = CurrWord_H;
		i51KitG2DrawImage(mwContainer, ModeChsWord, &DesRect, &SrcRect);
		if(OldChoose==1)
		{
			i51KitG2DrawImage(mwContainer, XiaoWanPiPic, &OldRect, iNULL);
			DesRect.X = OldRect.X + NumFrameMulX;
			DesRect.Y = OldRect.Y + NumFrameMulY;
			DesRect.Width = NumFrame_W;
			DesRect.Height = NumFrame_H;
#if defined(PKIG_SCREEN_240X320)
			mwDrawNumStep(mwContainer, NumSmall, &DesRect, 0, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_240X400)	
			mwDrawNumStep(mwContainer, NumSmall, &DesRect, 0, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_320X480)	
			mwDrawNumStep(mwContainer, NumBig, &DesRect, 0, TOTAL_DUCK_NUM);
#endif
		}
		else
		{
			i51KitG2DrawImage(mwContainer, SlideRock, &OldRect, iNULL);
			DesRect.X = OldRect.X + NumFrameMulX;
			DesRect.Y = OldRect.Y + NumFrameMulY;
			DesRect.Width = NumFrame_W;
			DesRect.Height = NumFrame_H;
#if defined(PKIG_SCREEN_240X320)
			mwDrawNumStep(mwContainer, NumSmall, &DesRect, 0, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_240X400)	
			mwDrawNumStep(mwContainer, NumSmall, &DesRect, 0, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_320X480)	
			mwDrawNumStep(mwContainer, NumBig, &DesRect, 0, TOTAL_DUCK_NUM);
#endif
		}

		DesRect.X = (CurrWord_X-CurrPic_X) + NewRect.X;
		DesRect.Y = (CurrWord_Y-CurrPic_Y) + NewRect.Y;
		DesRect.Width= CurrWord_W;
		DesRect.Height= CurrWord_H;
		SrcRect.X = 0;
		SrcRect.Y = CurrWord_H*CurrChoose;
		SrcRect.Width = CurrWord_W;
		SrcRect.Height = CurrWord_H;
		i51KitG2DrawImage(mwContainer, ModeChsWord, &DesRect, &SrcRect);
		if( CurrChoose==1 )
		{
			i51KitG2DrawImage(mwContainer, XiaoWanPiPic, &NewRect, iNULL);
			DesRect.X = NewRect.X + NumFrameMulX;
			DesRect.Y = NewRect.Y + NumFrameMulY;
			DesRect.Width = NumFrame_W;
			DesRect.Height = NumFrame_H;
#if defined(PKIG_SCREEN_240X320)
			mwDrawNumStep(mwContainer, NumSmall, &DesRect, CurrTotalDuck, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_240X400)	
			mwDrawNumStep(mwContainer, NumSmall, &DesRect, CurrTotalDuck, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_320X480)	
			mwDrawNumStep(mwContainer, NumBig, &DesRect, CurrTotalDuck, TOTAL_DUCK_NUM);
#endif
		}
		else
		{
			i51KitG2DrawImage(mwContainer, SlideRock, &NewRect, iNULL);
			DesRect.X = NewRect.X + NumFrameMulX;
			DesRect.Y = NewRect.Y + NumFrameMulY;
			DesRect.Width = NumFrame_W;
			DesRect.Height = NumFrame_H;
			//mwDrawNumStep(mwContainer, NumBig, &DesRect, 0, TOTAL_DUCK_NUM);

#if defined(PKIG_SCREEN_240X320)
			mwDrawNumStep(mwContainer, NumSmall, &DesRect, 0, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_240X400)	
			mwDrawNumStep(mwContainer, NumSmall, &DesRect, 0, TOTAL_DUCK_NUM);
#elif defined(PKIG_SCREEN_320X480)	
			mwDrawNumStep(mwContainer, NumBig, &DesRect, 0, TOTAL_DUCK_NUM);
#endif
		}
		
		OldRect.X += RectMul;
		NewRect.X += RectMul;

		if( OldRect.X >= SCREEN_WIDTH || OldRect.X <= -CurrPic_W )
		{
			OldChoose = CurrChoose;
		}

		
	}

	mwLevelDrawReturn();

	DesRect.X = SlideLine_X;
	DesRect.Y = SlideLine_Y;
	DesRect.Width= SlideLine_W;
	DesRect.Height= SlideLine_H;
	i51KitG2DrawImage(mwContainer, SlideLine, &DesRect, iNULL);

	//for( i=0; i< 8; i++ )
	//{
	//	DesRect.X = SlidePoint_X+i*SlidePoint_W;
	//	DesRect.Y = SlidePoint_Y;
	//	DesRect.Width= SlidePoint_W;
	//	DesRect.Height= SlidePoint_H;
	//}

	for( i=0; i< 8; i++ )
	{
		DesRect.X = SlidePoint_X+i*SlidePoint_OFFSET_X;
		DesRect.Y = SlidePoint_Y;
		DesRect.Width= SlidePoint_W;
		DesRect.Height= SlidePoint_H;
		i51KitG2DrawImage(mwContainer, SlidePoint, &DesRect, iNULL);
	}

	DesRect.X = SlidePiece_X+CurrChoose*SlidePiece_OFFSET_X;
	DesRect.Y = SlidePiece_Y;
	DesRect.Width= SlidePiece_W;
	DesRect.Height= SlidePiece_H;
	i51KitG2DrawImage(mwContainer, SlidePiece, &DesRect, iNULL);

	//DesRect.X = SlidePiece_X+CurrChoose*SlidePiece_W;
	//DesRect.Y = SlidePiece_Y;
	//DesRect.Width= SlidePiece_W;
	//DesRect.Height= SlidePiece_H;

//iLog("PromptType:%d",PromptType);
	if( PromptType>0 )
	{
		if( iFALSE==PromptFlag )
		{
			PromptFlag = iTRUE;
			TempDes.X = ChsLvMode_X;
			TempDes.Y = ChsLvMode_Y;
			TempDes.Width = ChsLvMode_W;
			TempDes.Height = ChsLvMode_H;
			i51KitG2DrawImage(mwContainer, ChsLvMode, &TempDes, iNULL );

			//i51KitG2SetAlpha(160);
			i51KitG2CleanScreen(0X0, iNULL);
			//i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);

		}

		DesRect.X = 0;
		DesRect.Y = 0;
		DesRect.Width= SCREEN_WIDTH;
		DesRect.Height= ChsLvMode_Y;
		//i51KitG2SetAlpha(160);
		i51KitG2CleanScreen(0X0, &DesRect);
		//i51KitG2SetAlpha(i51KITG2_ALPHA_NONEED);
		
		
		if (mwPublicDrawPrompt( Prompt_Time, OldTime, PromptType-1) )
		{
			TempDes.X = ChsLvMode_X;
			TempDes.Y = ChsLvMode_Y;
			TempDes.Width = ChsLvMode_W;
			TempDes.Height = ChsLvMode_H;
			i51KitG2DrawImage(mwContainer, ChsLvMode, &TempDes, iNULL );
			PromptType = 0;
			PromptFlag = iFALSE;
		}
	}
	
	if( PenDownFlag && PromptType==0 && mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADMOVE)
	{
		PenMoveX = mwKeyData.u.pen.x;
		if( PenMoveX>PenDownX && PenMoveX-PenDownX>20 )
		{
			PenDownFlag = iFALSE;
			CurrChoose --;
		}
		else if( PenMoveX<PenDownX && PenMoveX-PenDownX<-20 )
		{
			PenDownFlag = iFALSE;
			CurrChoose ++;
		}
		
		if( CurrChoose<0 ) CurrChoose = 0;
		else if( CurrChoose>7 ) CurrChoose = 7;
		if( CurrChoose!=OldChoose )
		{
			OldRect.X = CurrPic_X;
			OldRect.Y = CurrPic_Y;
			OldRect.Width= CurrPic_W;
			OldRect.Height= CurrPic_H;
			if( CurrChoose<OldChoose)
			{
				NewRect.X = -CurrPic_W;
				NewRect.Y = CurrPic_Y;
				NewRect.Width= CurrPic_W;
				NewRect.Height= CurrPic_H;
				RectMul=PIC_MOVE_MUL;
			}
			else
			{
				NewRect.X = SCREEN_WIDTH;
				NewRect.Y = CurrPic_Y;
				NewRect.Width= CurrPic_W;
				NewRect.Height= CurrPic_H;
				RectMul=-PIC_MOVE_MUL;
			}
		}
	}
	else if( PromptType==0 && mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
	{
		PenDownX = mwKeyData.u.pen.x;
		PenDownY = mwKeyData.u.pen.y;

		PenDownFlag = iTRUE;

		if( PenDownX>=ChengJiu_X&&PenDownX<(ChengJiu_X + ChengJiu_W)
		&&PenDownY>=ChengJiu_Y&&PenDownY<(ChengJiu_Y+ChengJiu_H))
		{
			DesRect.X = ChengJiu_X;
			DesRect.Y = ChengJiu_Y;
			DesRect.Width= ChengJiu_W;
			DesRect.Height= ChengJiu_H;

			SrcRect.X = 0;
			SrcRect.Y = ChengJiu_H;
			SrcRect.Width = ChengJiu_W;
			SrcRect.Height = ChengJiu_H;
			i51KitG2DrawImage(mwContainer, ShCangPress, &DesRect, &SrcRect);
			OldTime = i51AdeOsGetTick();
			PromptType = 2;
		}
		else if( PenDownX>=ShouCang_X&&PenDownX<(ShouCang_X + ShouCang_W)
		&&PenDownY>=ShouCang_Y&&PenDownY<(ShouCang_Y+ShouCang_H))
		{
			DesRect.X = ShouCang_X;
			DesRect.Y = ShouCang_Y;
			DesRect.Width= ShouCang_W;
			DesRect.Height= ShouCang_H;
			OldTime = i51AdeOsGetTick();
			PromptType = 2;
			i51KitG2DrawImage(mwContainer, ShCangPress, &DesRect, iNULL);
		}
		else if( PenDownX>=CurrPic_X&&PenDownX<(CurrPic_X + CurrPic_W)
		&&PenDownY>=CurrPic_Y&&PenDownY<(CurrPic_Y+CurrPic_H) )
		{
			if(CurrChoose!=1)
			{
				OldTime = i51AdeOsGetTick();
				PromptType = 1;
			}
			else
			{
				Flag = iFALSE;
				PromptType = 0;
				CurrChoose = 1;
				OldChoose = 1;
				//i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL);
				CurrGameState = GAME_STATE_PLAY;
				CurrLevelNum = MW_LEVEL_CHOOSE;
				CurrLevelState = MW_LEVEL_INIT;
			}
		}
		else if( PenDownX>=SlideLine_X&&PenDownX<(SlideLine_X + SlideLine_W)
		&&PenDownY>=SlideLine_Y&&PenDownY<(SlideLine_Y+SlideLine_H) )
		{
			CurrChoose = (PenDownX-SlideLine_X -SlidePoint_W/2 )/SlidePoint_W;
			if( CurrChoose<0 ) CurrChoose = 0;
			else if( CurrChoose>7 ) CurrChoose = 7;
			if( CurrChoose!=OldChoose )
			{
				OldRect.X = CurrPic_X;
				OldRect.Y = CurrPic_Y;
				OldRect.Width= CurrPic_W;
				OldRect.Height= CurrPic_H;
				if( CurrChoose<OldChoose)
				{
					NewRect.X = -CurrPic_W;
					NewRect.Y = CurrPic_Y;
					NewRect.Width= CurrPic_W;
					NewRect.Height= CurrPic_H;
					RectMul=PIC_MOVE_MUL;
				}
				else
				{
					NewRect.X = SCREEN_WIDTH;
					NewRect.Y = CurrPic_Y;
					NewRect.Width= CurrPic_W;
					NewRect.Height= CurrPic_H;
					RectMul=-PIC_MOVE_MUL;
				}
			}
		}else if( mwLevelPressReturn( PenDownX, PenDownY ))
		{
			Flag = iFALSE;
			CurrGameState = GAME_STATE_MAIN;
			//i51KitG2UnsetImageMemoryCopy(mwContainer, iNULL, iNULL);
		}
		
	}

//iLog("End mwStateMode");

#endif

	return iTRUE;
}



