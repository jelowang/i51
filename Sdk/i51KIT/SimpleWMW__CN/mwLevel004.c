
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#endif

iBOOL mwInitLevel004( void )
{

	iBody*body;//, *staticBody = &(mwSpace->staticBody);
	iShape *shape;
	iS32 radius = WATER_RADIUS;
	iS32 i=0,j=0;
	iVECT pos;
	mwInitSpace(CurrLevelNum);	
	iVectInit2(&mwSpace->WorldGravity, 0, 150);

	for( i=0; i<3; i++){
		for( j=0; j<10; j++){
#if defined(PKIG_SCREEN_240X320)
			iVectInit2(&pos,(j)*4*COORDINATE_RATE+15*COORDINATE_RATE,(i)*4*COORDINATE_RATE+50*COORDINATE_RATE);

#elif defined(PKIG_SCREEN_240X400)	
			iVectInit2(&pos,(j)*4*COORDINATE_RATE+15*COORDINATE_RATE,(i)*4*COORDINATE_RATE+50*COORDINATE_RATE);

#elif defined(PKIG_SCREEN_320X480)	
			iVectInit2(&pos,(j)*4*COORDINATE_RATE+15*COORDINATE_RATE,(i)*4*COORDINATE_RATE+90*COORDINATE_RATE);

#endif

			body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
			iVectInit2(&(body->MAX_V),WATER_MAX_SPEED,WATER_MAX_SPEED);
			iVectInit2(&(body->V),0,0);
			shape = i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);
		}
	}

	mwLevelWordInit(CurrLevelNum);
	mwLevelDuckInit(CurrLevelNum);
	mwLevelPipeInit(CurrLevelNum);
	mwLevelWaitInit(CurrLevelNum);
	mwLevelAlgaeInit(CurrLevelNum);

	mwLevelDrawBKwall();
	SWMW_i51KitG2DrawImage(&iMageInfo[MudLevel004], iNULL, iNULL);
	SWMW_i51KitG2DrawImage(&iMageInfo[RockLevel004], iNULL, iNULL);

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
	TimeLevelStart = i51AdeOsGetTick();
	mwLevelOrganInit(CurrLevelNum);

	return iTRUE;

}

iBOOL mwRunLevel004( void )
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
			wmw_pause_page();
			CurrLevelState = MW_LEVEL_PAUSE;
			mwWordPause = iTRUE;
		}
	}

	return iTRUE;
	
}



