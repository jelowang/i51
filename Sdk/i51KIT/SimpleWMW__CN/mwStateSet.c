
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#define ProducerPress_X	72
#define ProducerPress_Y	178
#define ProducerPress_W	97
#define ProducerPress_H	29
#define MusicCloss_X	72
#define MusicCloss_Y	140
#define MusicCloss_W	97
#define MusicCloss_H	29
#define SoundCloss_X	72
#define SoundCloss_Y	103
#define SoundCloss_W	97
#define SoundCloss_H	29
#define Set_W			97
#define Set_H			29

#elif defined(PKIG_SCREEN_240X400)	
#define ProducerPress_X	72
#define ProducerPress_Y	218
#define ProducerPress_W	97
#define ProducerPress_H	37
#define MusicCloss_X	72
#define MusicCloss_Y	170
#define MusicCloss_W	97
#define MusicCloss_H	37
#define SoundCloss_X	72
#define SoundCloss_Y	123
#define SoundCloss_W	97
#define SoundCloss_H	37
#define Set_W			97
#define Set_H			37

#elif defined(PKIG_SCREEN_320X480)	
#define ProducerPress_X	97
#define ProducerPress_Y	262
#define ProducerPress_W	128
#define ProducerPress_H	41
#define MusicCloss_X	97
#define MusicCloss_Y	205
#define MusicCloss_W	128
#define MusicCloss_H	41
#define SoundCloss_X	97
#define SoundCloss_Y	149
#define SoundCloss_W	128
#define SoundCloss_H	41
#define Set_W			128
#define Set_H			41
#endif

static iU8 ShowProducerFlag = iFALSE;

iBOOL mwStateSet( void )
{
	iU16 PenDownX = 0;
	iU16 PenDownY = 0;
	iRECT DesRect;
	iRECT SrcRect;

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
			public_audio_play_x(0, wmw_SoundChoose_mp3, wmw_SoundChoose_mp3_len,i51_MMI_DEC_MEM_BASED, AUDIO_PLAYER_MP3, iTRUE);
			

		}
#endif
	if(ShowProducerFlag)
	{
		//i51KitG2DrawImage(mwContainer, ProducerBK, iNULL, iNULL);
		mwLevelDrawReturn();
		if(mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
		{
			PenDownX = mwKeyData.u.pen.x;
			PenDownY = mwKeyData.u.pen.y;
			if( mwLevelPressReturn(PenDownX, PenDownY))
			{
				ShowProducerFlag = iFALSE;
			}
		}
	}
	else
	{
		//i51KitG2DrawImage(mwContainer, SetBK, iNULL, iNULL);
		DesRect.X = 0;
		DesRect.Y = SCREEN_HEIGHT/2;
		DesRect.Width = SCREEN_WIDTH;
		DesRect.Height = SCREEN_HEIGHT/2;
		
		mwLevelDrawReturn();

		DesRect.X = MusicCloss_X;
		DesRect.Y = MusicCloss_Y;
		DesRect.Width = MusicCloss_W;
		DesRect.Height = MusicCloss_H;
		if(MusicCloseFlag  )
		{
			
#if 1
			if( SoundChoosePlay )
			{
				public_audio_stop(AUDIO_PLAYER_MP3);
				SoundChoosePlay = iFALSE;
			}
#else
				if( SoundWinPlay )
				{
					pfunc->public_audio_stop(AUDIO_PLAYER_MP3);
					SoundWinPlay = iFALSE;
				}

#endif
			SrcRect.X = 0;
			SrcRect.Y = Set_H * 3;
			SrcRect.Width = Set_W;
			SrcRect.Height = Set_H;


		}
		else
		{
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
					public_audio_play_x(0, wmw_SoundChoose_mp3, wmw_SoundChoose_mp3_len,i51_MMI_DEC_MEM_BASED, AUDIO_PLAYER_MP3, iTRUE);
					 

				}
#endif
			SrcRect.X = 0;
			SrcRect.Y = Set_H * 2;
			SrcRect.Width = Set_W;
			SrcRect.Height = Set_H;

		}
#if 0 //xie		
		DesRect.X = SoundCloss_X;
		DesRect.Y = SoundCloss_Y;
		DesRect.Width = SoundCloss_W;
		DesRect.Height = SoundCloss_H;


		if(SoundCloseFlag)
		{
			SrcRect.X = 0;
			SrcRect.Y = Set_H;
			SrcRect.Width = Set_W;
			SrcRect.Height = Set_H;

		}
		else
		{
			SrcRect.X = 0;
			SrcRect.Y = 0;
			SrcRect.Width = Set_W;
			SrcRect.Height = Set_H;

		}
#endif

		DesRect.X = ProducerPress_X;
		DesRect.Y = ProducerPress_Y;
		DesRect.Width = ProducerPress_W;
		DesRect.Height = ProducerPress_H;

		SrcRect.X = 0;
		SrcRect.Y = Set_H * 4;
		SrcRect.Width = Set_W;
		SrcRect.Height = Set_H;

		
		if(mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
		{
			PenDownX = mwKeyData.u.pen.x;
			PenDownY = mwKeyData.u.pen.y;
			
			if( mwLevelPressReturn(PenDownX, PenDownY))
			{
				CurrGameState = GAME_STATE_MAIN;
			}
			
			if( PenDownX>=MusicCloss_X&&PenDownX<(MusicCloss_X+MusicCloss_W)
			&&PenDownY>=MusicCloss_Y&&PenDownY<(MusicCloss_Y+MusicCloss_H))
			{
				SoundCloseFlag = MusicCloseFlag = !MusicCloseFlag;
				//pfunc->set_sound_state((iBOOL)!MusicCloseFlag);
			}
#if 0 //xie
			if( PenDownX>=SoundCloss_X&&PenDownX<(SoundCloss_X+SoundCloss_W)
			&&PenDownY>=SoundCloss_Y&&PenDownY<(SoundCloss_Y+SoundCloss_H))
			{
				SoundCloseFlag = !SoundCloseFlag;
			}
#endif 
			if( PenDownX>=ProducerPress_X&&PenDownX<(ProducerPress_X+ProducerPress_W)
			&&PenDownY>=ProducerPress_Y&&PenDownY<(ProducerPress_Y+ProducerPress_H))
			{
				DesRect.X = ProducerPress_X;
				DesRect.Y = ProducerPress_Y;
				DesRect.Width = ProducerPress_W;
				DesRect.Height = ProducerPress_H;

				SrcRect.X = 0;
				SrcRect.Y = Set_H * 5;
				SrcRect.Width = Set_W;
				SrcRect.Height = Set_H;

				ShowProducerFlag = iTRUE;
			}
		}

	}
	return iTRUE;
}




