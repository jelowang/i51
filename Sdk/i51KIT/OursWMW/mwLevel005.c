
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#endif

iBOOL mwInitLevel005( void )
{

	mwInitSpace(CurrLevelNum);	

	mwLevelWordInit(CurrLevelNum);
	mwLevelDuckInit(CurrLevelNum);
	mwLevelPipeInit(CurrLevelNum);
	mwLevelWaitInit(CurrLevelNum);
	mwLevelAlgaeInit(CurrLevelNum);

	mwLevelDrawBKwall();
	i51KitG2DrawImage(mwContainer, MudLevel005, iNULL, iNULL);
	i51KitG2DrawImage(mwContainer, RockLevel005, iNULL, iNULL);

	mwLevelDrawPause();
	mwLevelDrawRestart();
	i51AdeStdMemcpy16( (void *)TempScreenBuf, (void *)mwScreenBuffer, SCREEN_HEIGHT*SCREEN_WIDTH*2);
	mwLevelSpeedKeepInit(CurrLevelNum);
	mwTempScreenBufUpdate(CurrLevelNum);

	mwLevelDrawDuck();
	TimeLevelStart = i51AdeOsGetTick();
	mwLevelOrganInit(CurrLevelNum);

	return iTRUE;

}

iBOOL mwRunLevel005( void )
{
	iU16 PenDownX = 0;
	iU16 PenDownY = 0;

	mwLevelWaterSpray(CurrLevelNum);

//	mwEarthCutting(CurrLevelNum);

	i51AdeStdMemcpy16( (void *)(mwScreenBuffer),
		(void *)(&TempScreenBuf[0][0]), 
		SCREEN_WIDTH*SCREEN_HEIGHT*2);

	mwPublicAnimation(&CrocodileWait);
	mwLevelDrawDuck();

	cpWordDraw();
	if( TempScreenUpdataFlag )
	{
		TempScreenUpdataFlag = iFALSE;
		mwTempScreenBufUpdate(CurrLevelNum);
		cpWordDraw();
	}

	TimeLevelCurr= i51AdeOsGetTick();
	if( TimeLevelCurr- TimeLevelStart < PromptShowTime )
	{
		mwLevelDrawPrompt(CurrLevelNum);
	}

	if(mwKeyData.type==LLP_PEN_INPUT&& mwKeyData.u.pen.type==i51_MSG_PADDOWN )
	{
		PenDownX = mwKeyData.u.pen.x;
		PenDownY = mwKeyData.u.pen.y;

		if(mwLevelPressRestart(PenDownX, PenDownY))
		{
			CurrLevelState = MW_LEVEL_INIT;
		}

		else if( mwLevelPressPause(PenDownX, PenDownY))
		{
			CurrLevelState = MW_LEVEL_PAUSE;
			mwWordPause = iTRUE;
		}
	}

	return iTRUE;
	
}




