
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)

#define STATE_MAIN_PLAYGAME_X	71
#define STATE_MAIN_PLAYGAME_Y	81
#define STATE_MAIN_PLAYGAME_W	97
#define STATE_MAIN_PLAYGAME_H	27
#define STATE_MAIN_SET_X	82
#define STATE_MAIN_SET_Y	113
#define STATE_MAIN_SET_W	74
#define STATE_MAIN_SET_H	27
#define STATE_MAIN_EXIT_X	82
#define STATE_MAIN_EXIT_Y	149
#define STATE_MAIN_EXIT_W	74
#define STATE_MAIN_EXIT_H	27
#define MainLeadWait_X		102
#define MainLeadWait_Y		206
#define MainLeadWait_W		96
#define MainLeadWait_H		91

#elif defined(PKIG_SCREEN_240X400)	

#define STATE_MAIN_PLAYGAME_X	      71
#define STATE_MAIN_PLAYGAME_Y	      100
#define STATE_MAIN_PLAYGAME_W      96
#define STATE_MAIN_PLAYGAME_H	       34
#define STATE_MAIN_SET_X	83
#define STATE_MAIN_SET_Y	143
#define STATE_MAIN_SET_W	73
#define STATE_MAIN_SET_H	35
#define STATE_MAIN_EXIT_X	83
#define STATE_MAIN_EXIT_Y	186
#define STATE_MAIN_EXIT_W	73
#define STATE_MAIN_EXIT_H	35
#define MainLeadWait_X		95
#define MainLeadWait_Y		245
#define MainLeadWait_W		126
#define MainLeadWait_H		135

#elif defined(PKIG_SCREEN_320X480)	

#define STATE_MAIN_PLAYGAME_X	95
#define STATE_MAIN_PLAYGAME_Y	122
#define STATE_MAIN_PLAYGAME_W	128
#define STATE_MAIN_PLAYGAME_H	41
#define STATE_MAIN_SET_X	110
#define STATE_MAIN_SET_Y	173
#define STATE_MAIN_SET_W	97
#define STATE_MAIN_SET_H	41
#define STATE_MAIN_EXIT_X	110
#define STATE_MAIN_EXIT_Y	224
#define STATE_MAIN_EXIT_W	97
#define STATE_MAIN_EXIT_H	41
#define MainLeadWait_X		138
#define MainLeadWait_Y		310
#define MainLeadWait_W		126
#define MainLeadWait_H		135

#endif
// 鳄鱼等待水来动画结构
AnimationStr AniMainLeadWait;

iBOOL mwStateMain( void )
{
//	iRECT DesRect;
	static iU8 Flag = iFALSE;
	iU32 ImageId[5] = {
		MainLeadWait00,MainLeadWait00,MainLeadWait01,MainLeadWait00,MainLeadWait00,
		//MainLeadWait03,MainLeadWait04,MainLeadWait05,
		//MainLeadWait06,MainLeadWait07,MainLeadWait08,
		//MainLeadWait09,MainLeadWait10,MainLeadWait11
	};
	int i = 0;
	
	iRECT DesRect;
	iRECT SrcRect;
	iU16 PenDownX = 0;
	iU16 PenDownY = 0;
		
	if( iFALSE == Flag )
	{

		//MusicCloseFlag = SoundCloseFlag = iFALSE;

	#if 1 //xie
		if(iFALSE==MusicCloseFlag && SoundChoosePlay == iFALSE)
		{
			//SoundWinPlay = iTRUE;

			SoundChoosePlay = iTRUE;
			
			public_audio_play_x(0, wmw_SoundChoose_mp3, wmw_SoundChoose_mp3_len,i51_MMI_DEC_MEM_BASED, AUDIO_PLAYER_MP3, iTRUE);
		}
	#endif
	
		Flag = iTRUE;
		AniMainLeadWait.Container = mwContainer;
		AniMainLeadWait.CurrPlayFrame = 0;
		AniMainLeadWait.DesRect.X = MainLeadWait_X;
		AniMainLeadWait.DesRect.Y = MainLeadWait_Y;
		AniMainLeadWait.DesRect.Width = MainLeadWait_W;
		AniMainLeadWait.DesRect.Height = MainLeadWait_H;
		AniMainLeadWait.FrameTimeMul = 500;
		for( i=0; i<5; i++ )
		{
			AniMainLeadWait.ImageId[i] = ImageId[i];
		}
		AniMainLeadWait.ImageTotalFrame = 5;
		AniMainLeadWait.ImageWidth = MainLeadWait_W;
		AniMainLeadWait.OldPlayTime = i51AdeOsGetTick();
	}
	
	mwWordPause = iTRUE;
	SWMW_i51KitG2DrawImage(&iMageInfo[Main], iNULL, iNULL);

	DesRect.X = STATE_MAIN_PLAYGAME_X;
	DesRect.Y = STATE_MAIN_PLAYGAME_Y;
	DesRect.Width = STATE_MAIN_PLAYGAME_W;
	DesRect.Height= STATE_MAIN_PLAYGAME_H;
	SrcRect.X = 0;
	SrcRect.Y = 0;
	SrcRect.Width = STATE_MAIN_PLAYGAME_W;
	SrcRect.Height = STATE_MAIN_PLAYGAME_H;

	//i51KitG2DrawImage(mwContainer, Play, &DesRect, &SrcRect);

	DesRect.X = STATE_MAIN_SET_X;
	DesRect.Y = STATE_MAIN_SET_Y;
	DesRect.Width = STATE_MAIN_SET_W;
	DesRect.Height= STATE_MAIN_SET_H;
	SrcRect.X = 0;
	SrcRect.Y = 0;
	SrcRect.Width = STATE_MAIN_SET_W;
	SrcRect.Height = STATE_MAIN_SET_H;


	DesRect.X = STATE_MAIN_EXIT_X;
	DesRect.Y = STATE_MAIN_EXIT_Y;
	DesRect.Width = STATE_MAIN_EXIT_W;
	DesRect.Height= STATE_MAIN_EXIT_H;
	SrcRect.X = 0;
	SrcRect.Y = STATE_MAIN_SET_H * 2;
	SrcRect.Width = STATE_MAIN_SET_W;
	SrcRect.Height = STATE_MAIN_SET_H;


	mwPublicAnimation(&AniMainLeadWait);
	
	if(mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
	{
		PenDownX = mwKeyData.u.pen.x;
		PenDownY = mwKeyData.u.pen.y;

		if( (PenDownX>=STATE_MAIN_PLAYGAME_X) && (PenDownX<(STATE_MAIN_PLAYGAME_X+STATE_MAIN_PLAYGAME_W))
			&&(PenDownX>=STATE_MAIN_PLAYGAME_Y) && (PenDownY<(STATE_MAIN_PLAYGAME_Y+STATE_MAIN_PLAYGAME_H))
		)
		{
			//DesRect.X = STATE_MAIN_PLAYGAME_X;
			//DesRect.Y = STATE_MAIN_PLAYGAME_Y;
			//DesRect.Width = STATE_MAIN_PLAYGAME_W;
			//DesRect.Height= STATE_MAIN_PLAYGAME_H;
			//SrcRect.X = 0;
			//SrcRect.Y = STATE_MAIN_PLAYGAME_H;
			//SrcRect.Width = STATE_MAIN_PLAYGAME_W;
			//SrcRect.Height = STATE_MAIN_PLAYGAME_H;

			//i51KitG2DrawImage(mwContainer, Play, &DesRect, &SrcRect);

			//CurrGameState = GAME_STATE_MODE;
			//CurrLevelState = MW_LEVEL_INIT;

			wmw_image_destroy();
			wmw_choose_page();
			CurrGameState = GAME_STATE_PLAY;
			CurrLevelNum = MW_LEVEL_CHOOSE;
			CurrLevelState = MW_LEVEL_INIT;

			Flag = iFALSE;
		}
		else if( (PenDownX>=STATE_MAIN_SET_X) && (PenDownX<(STATE_MAIN_SET_X+STATE_MAIN_SET_W))
				&&(PenDownY>=STATE_MAIN_SET_Y) && (PenDownY<(STATE_MAIN_SET_Y+STATE_MAIN_SET_H))
			)
			{
				//DesRect.X = STATE_MAIN_SET_X;
				//DesRect.Y = STATE_MAIN_SET_Y;
				//DesRect.Width = STATE_MAIN_SET_W;
				//DesRect.Height= STATE_MAIN_SET_H;
				//SrcRect.X = 0;
				//SrcRect.Y = STATE_MAIN_SET_H;
				//SrcRect.Width = STATE_MAIN_SET_W;
				//SrcRect.Height = STATE_MAIN_SET_H;

				Flag = iFALSE;
				//CurrGameState = GAME_STATE_SET;

				//wmw_image_destroy();
				//wmw_partol_page();
				//iScreenFlag = iTRUE;
				//CurrGameState = GAME_STATE_PARTOL;
				wmw_portal_ui();
			}
		else if( (PenDownX>=STATE_MAIN_EXIT_X) && (PenDownX<(STATE_MAIN_EXIT_X+STATE_MAIN_EXIT_W))
				&&(PenDownY>=STATE_MAIN_EXIT_Y) && (PenDownY<(STATE_MAIN_EXIT_Y+STATE_MAIN_EXIT_H))
			)
			{
				//DesRect.X = STATE_MAIN_EXIT_X;
				//DesRect.Y = STATE_MAIN_EXIT_Y;
				//DesRect.Width = STATE_MAIN_EXIT_W;
				//DesRect.Height= STATE_MAIN_EXIT_H;
				//SrcRect.X =  0;
				//SrcRect.Y = STATE_MAIN_SET_H * 3;
				//SrcRect.Width = STATE_MAIN_SET_W;
				//SrcRect.Height = STATE_MAIN_SET_H;

				Flag = iFALSE;
				CurrGameState = GAME_STATE_EXIT;
			}
		
	}
	return iTRUE;
}


