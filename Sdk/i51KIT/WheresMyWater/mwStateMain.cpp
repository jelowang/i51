
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#define STATE_MAIN_PLAYGAME_X	95
#define STATE_MAIN_PLAYGAME_Y	122
#define STATE_MAIN_PLAYGAME_W	128
#define STATE_MAIN_PLAYGAME_H	44
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

	if( iFALSE == Flag )
	{
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
	
		Flag = iTRUE;
		iU32 ImageId[12] = {
			MainLeadWait00,MainLeadWait01,MainLeadWait02,
			MainLeadWait03,MainLeadWait04,MainLeadWait05,
			MainLeadWait06,MainLeadWait07,MainLeadWait08,
			MainLeadWait09,MainLeadWait10,MainLeadWait11
		};
		AniMainLeadWait.Container = mwContainer;
		AniMainLeadWait.CurrPlayFrame = 0;
		AniMainLeadWait.DesRect.X = MainLeadWait_X;
		AniMainLeadWait.DesRect.Y = MainLeadWait_Y;
		AniMainLeadWait.DesRect.Width = MainLeadWait_W;
		AniMainLeadWait.DesRect.Height = MainLeadWait_H;
		AniMainLeadWait.FrameTimeMul = 250;
		for( int i=0; i<12; i++ )
		{
			AniMainLeadWait.ImageId[i] = ImageId[i];
		}
		AniMainLeadWait.ImageTotalFrame = 12;
		AniMainLeadWait.ImageWidth = MainLeadWait_W;
		AniMainLeadWait.OldPlayTime = i51AdeOsGetTick();
	}
	
	mwWordPause = iTRUE;
	i51KitG2DrawImage(mwContainer, Main, iNULL, iNULL);

	iRECT DesRect;
	DesRect.X = STATE_MAIN_PLAYGAME_X;
	DesRect.Y = STATE_MAIN_PLAYGAME_Y;
	DesRect.Width = STATE_MAIN_PLAYGAME_W;
	DesRect.Height= STATE_MAIN_PLAYGAME_H;
	i51KitG2DrawImage(mwContainer, PlayShow, &DesRect, iNULL);

	DesRect.X = STATE_MAIN_SET_X;
	DesRect.Y = STATE_MAIN_SET_Y;
	DesRect.Width = STATE_MAIN_SET_W;
	DesRect.Height= STATE_MAIN_SET_H;
	i51KitG2DrawImage(mwContainer, SetShow,&DesRect, iNULL);

	DesRect.X = STATE_MAIN_EXIT_X;
	DesRect.Y = STATE_MAIN_EXIT_Y;
	DesRect.Width = STATE_MAIN_EXIT_W;
	DesRect.Height= STATE_MAIN_EXIT_H;
	i51KitG2DrawImage(mwContainer, ExitShow,&DesRect, iNULL);

	mwPublicAnimation(&AniMainLeadWait);
	
	if(mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
	{
		iU16 PenDownX = mwKeyData.u.pen.x;
		iU16 PenDownY = mwKeyData.u.pen.y;

		if( (PenDownX>=STATE_MAIN_PLAYGAME_X) && (PenDownX<(STATE_MAIN_PLAYGAME_X+STATE_MAIN_PLAYGAME_W))
			&&(PenDownX>=STATE_MAIN_PLAYGAME_Y) && (PenDownY<(STATE_MAIN_PLAYGAME_Y+STATE_MAIN_PLAYGAME_H))
		)
		{
			iRECT DesRect;
			DesRect.X = STATE_MAIN_PLAYGAME_X;
			DesRect.Y = STATE_MAIN_PLAYGAME_Y;
			DesRect.Width = STATE_MAIN_PLAYGAME_W;
			DesRect.Height= STATE_MAIN_PLAYGAME_H;
			i51KitG2DrawImage(mwContainer, PlayPress, &DesRect, iNULL);
			CurrGameState = GAME_STATE_MODE;
			CurrLevelState = MW_LEVEL_INIT;
		}
		else if( (PenDownX>=STATE_MAIN_SET_X) && (PenDownX<(STATE_MAIN_SET_X+STATE_MAIN_SET_W))
				&&(PenDownY>=STATE_MAIN_SET_Y) && (PenDownY<(STATE_MAIN_SET_Y+STATE_MAIN_SET_H))
			)
			{
				iRECT DesRect;
				DesRect.X = STATE_MAIN_SET_X;
				DesRect.Y = STATE_MAIN_SET_Y;
				DesRect.Width = STATE_MAIN_SET_W;
				DesRect.Height= STATE_MAIN_SET_H;
				i51KitG2DrawImage(mwContainer, SetPress,&DesRect, iNULL);
				CurrGameState = GAME_STATE_SET;
			}
		else if( (PenDownX>=STATE_MAIN_EXIT_X) && (PenDownX<(STATE_MAIN_EXIT_X+STATE_MAIN_EXIT_W))
				&&(PenDownY>=STATE_MAIN_EXIT_Y) && (PenDownY<(STATE_MAIN_EXIT_Y+STATE_MAIN_EXIT_H))
			)
			{
				iRECT DesRect;
				DesRect.X = STATE_MAIN_EXIT_X;
				DesRect.Y = STATE_MAIN_EXIT_Y;
				DesRect.Width = STATE_MAIN_EXIT_W;
				DesRect.Height= STATE_MAIN_EXIT_H;
				i51KitG2DrawImage(mwContainer, ExitPress,&DesRect, iNULL);
				CurrGameState = GAME_STATE_EXIT;
			}
		
	}
	return iTRUE;
}


