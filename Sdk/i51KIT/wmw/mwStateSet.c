
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
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
#endif

static iU8 ShowProducerFlag = iFALSE;

iBOOL mwStateSet( void )
{
	iU16 PenDownX = 0;
	iU16 PenDownY = 0;
	iRECT DesRect;

#if SOUND_NEED
		if(iFALSE==MusicCloseFlag && SoundChoosePlay == iFALSE)
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
	if(ShowProducerFlag)
	{
		i51KitG2DrawImage(mwContainer, ProducerBK, iNULL, iNULL);
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
		i51KitG2DrawImage(mwContainer, SetBK, iNULL, iNULL);
		DesRect.X = 0;
		DesRect.Y = SCREEN_HEIGHT/2;
		DesRect.Width = SCREEN_WIDTH;
		DesRect.Height = SCREEN_HEIGHT/2;
		i51KitG2FlipImage(mwContainer, SetBK, &DesRect, iNULL);
		
		mwLevelDrawReturn();

		DesRect.X = MusicCloss_X;
		DesRect.Y = MusicCloss_Y;
		DesRect.Width = MusicCloss_W;
		DesRect.Height = MusicCloss_H;
		if(MusicCloseFlag  )
		{
			if( SoundChoosePlay )
			{
				pfunc->public_audio_stop(AUDIO_PLAYER_WAV_0);
				SoundChoosePlay = iFALSE;
			}
			i51KitG2DrawImage(mwContainer, MusicCloss, &DesRect, iNULL);
		}
		else
		{
#if SOUND_NEED
				if(iFALSE==MusicCloseFlag && SoundChoosePlay == iFALSE)
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
			i51KitG2DrawImage(mwContainer, MusicOpen, &DesRect, iNULL);
		}
		
		DesRect.X = SoundCloss_X;
		DesRect.Y = SoundCloss_Y;
		DesRect.Width = SoundCloss_W;
		DesRect.Height = SoundCloss_H;
		if(SoundCloseFlag)
		{
			i51KitG2DrawImage(mwContainer, SoundCloss, &DesRect, iNULL);
		}
		else
		{
			i51KitG2DrawImage(mwContainer, SoundOpen, &DesRect, iNULL);
		}

		DesRect.X = ProducerPress_X;
		DesRect.Y = ProducerPress_Y;
		DesRect.Width = ProducerPress_W;
		DesRect.Height = ProducerPress_H;
		i51KitG2DrawImage(mwContainer, ProducerShow, &DesRect, iNULL);
		
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
				MusicCloseFlag = !MusicCloseFlag;
			}

			if( PenDownX>=SoundCloss_X&&PenDownX<(SoundCloss_X+SoundCloss_W)
			&&PenDownY>=SoundCloss_Y&&PenDownY<(SoundCloss_Y+SoundCloss_H))
			{
				SoundCloseFlag = !SoundCloseFlag;
			}

			if( PenDownX>=ProducerPress_X&&PenDownX<(ProducerPress_X+ProducerPress_W)
			&&PenDownY>=ProducerPress_Y&&PenDownY<(ProducerPress_Y+ProducerPress_H))
			{
				DesRect.X = ProducerPress_X;
				DesRect.Y = ProducerPress_Y;
				DesRect.Width = ProducerPress_W;
				DesRect.Height = ProducerPress_H;
				i51KitG2DrawImage(mwContainer, ProducerPress, &DesRect, iNULL);
				ShowProducerFlag = iTRUE;
			}
		}

	}
	return iTRUE;
}




