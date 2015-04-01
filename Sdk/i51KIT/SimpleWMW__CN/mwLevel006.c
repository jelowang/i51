
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#endif

iBOOL mwInitLevel006( void )
{
	iBody*body;//, *staticBody = &(mwSpace->staticBody);
	iShape *shape;
	iS32 radius = WATER_RADIUS;
	iS32 i=0,j=0;
	iVECT pos;
	
	mwInitSpace(CurrLevelNum);	

	for( i=0; i<2; i++){
		for( j=0; j<3; j++){
#if defined(PKIG_SCREEN_240X320)
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+25*COORDINATE_RATE,(i)*4*COORDINATE_RATE+65*COORDINATE_RATE);

#elif defined(PKIG_SCREEN_240X400)	
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+27*COORDINATE_RATE,(i)*4*COORDINATE_RATE+82*COORDINATE_RATE);

#elif defined(PKIG_SCREEN_320X480)	
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+40*COORDINATE_RATE,(i)*4*COORDINATE_RATE+100*COORDINATE_RATE);

#endif

			body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
			iVectInit2(&(body->V),0,0);
			iVectInit2(&(body->MAX_V),WATER_MAX_SPEED,WATER_MAX_SPEED);
			shape = i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);
		}
	}

	for( i=0; i<2; i++){
		for( j=0; j<3; j++){
#if defined(PKIG_SCREEN_240X320)
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+200*COORDINATE_RATE,(i)*4*COORDINATE_RATE+60*COORDINATE_RATE);

#elif defined(PKIG_SCREEN_240X400)
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+205*COORDINATE_RATE,(i)*4*COORDINATE_RATE+80*COORDINATE_RATE);

#elif defined(PKIG_SCREEN_320X480)	
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+266*COORDINATE_RATE,(i)*4*COORDINATE_RATE+97*COORDINATE_RATE);

#endif

			body = i51KitP2CreateBody(mwSpace,&pos, MASS_WATER);
			iVectInit2(&(body->V),0,0);
			iVectInit2(&(body->MAX_V),WATER_MAX_SPEED,WATER_MAX_SPEED);
			shape = i51KitP2CreateShapeCircle(mwSpace, body, radius, &pos);
		}
	}

	for( i=0; i<2; i++){
		for( j=0; j<5; j++){
#if defined(PKIG_SCREEN_240X320)
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+105*COORDINATE_RATE,(i)*4*COORDINATE_RATE+180*COORDINATE_RATE);

#elif defined(PKIG_SCREEN_240X400)	
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+110*COORDINATE_RATE,(i)*4*COORDINATE_RATE+231*COORDINATE_RATE);

#elif defined(PKIG_SCREEN_320X480)	
			iVectInit2(&pos,(j)*6*COORDINATE_RATE+145*COORDINATE_RATE,(i)*4*COORDINATE_RATE+285*COORDINATE_RATE);

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

	mwLevelDrawBKwall();
	SWMW_i51KitG2DrawImage(&iMageInfo[MudLevel006], iNULL, iNULL);
	SWMW_i51KitG2DrawImage(&iMageInfo[RockLevel006], iNULL, iNULL);

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

iBOOL mwRunLevel006( void )
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





