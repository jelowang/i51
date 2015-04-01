
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#endif

iBOOL mwInitLevel014( void )
{
	int i = 0;

	mwInitSpace(CurrLevelNum);	

	mwLevelWordInit(CurrLevelNum);
	mwLevelDuckInit(CurrLevelNum);
	mwLevelPipeInit(CurrLevelNum);
	mwLevelWaitInit(CurrLevelNum);

	mwLevelDrawBKwall();
	//i51KitG2DrawImage(mwContainer, MudLevel014, iNULL, iNULL);
	//i51KitG2DrawImage(mwContainer, RockLevel014, iNULL, iNULL);

	mwLevelDrawPause();
	mwLevelDrawRestart();

#ifdef __MODIS_DEBUG__
	for( i=0;i<SCREEN_HEIGHT; i++)
	{
		i51AdeStdMemcpy16( (void *)(TempScreenBuf + i*SCREEN_WIDTH),
			(void *)(mwScreenBuffer+i*320),			 
			SCREEN_WIDTH*2);
	}
#else
	i51AdeStdMemcpy16( (void *)TempScreenBuf, (void *)mwScreenBuffer, SCREEN_HEIGHT*SCREEN_WIDTH*2);
#endif
	mwLevelSpeedKeepInit(CurrLevelNum);
	mwTempScreenBufUpdate(CurrLevelNum);

	mwLevelDrawDuck();
//	mwLevelDrawOrgan();
	TimeLevelStart = i51AdeOsGetTick();
	mwLevelOrganInit(CurrLevelNum);

	return iTRUE;

}

iBOOL mwRunLevel014( void )
{

	iU16 PenDownX = 0;
	iU16 PenDownY = 0;

#ifdef __MODIS_DEBUG__
	int i;	
	for( i=0;i<SCREEN_HEIGHT; i++)
	{
		i51AdeStdMemcpy16( (void *)(mwScreenBuffer+i*320),
			(void *)(TempScreenBuf + i*SCREEN_WIDTH), 
			SCREEN_WIDTH*2);
	}
#else
	i51AdeStdMemcpy16( (void *)(mwScreenBuffer),
		(void *)(TempScreenBuf), 
		SCREEN_WIDTH*SCREEN_HEIGHT*2);

#endif

	mwLevelWaterSpray(CurrLevelNum);

 	mwPublicAnimation(&CrocodileWait);
	mwLevelDrawDuck();
	
	if(	OrganMess[0].OrgneState != ORGAN_STATE_DES)
	{
		OrganMess[0].OrganType = SHAPE_TYPE_ORGAN_RED;
	}
	if(	OrganMess[1].OrgneState != ORGAN_STATE_DES)
	{
		OrganMess[1].OrganType = SHAPE_TYPE_ORGAN_GREEN;
	}
//	mwLevelDrawOrgan();
	
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





