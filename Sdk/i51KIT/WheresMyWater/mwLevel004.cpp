
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#endif

iBOOL mwInitLevel004( void )
{

	mwInitSpace(CurrLevelNum);	

	cpBody *body;//, *staticBody = &(mwSpace->staticBody);
	cpShape *shape;
	cpFloat radius = WATER_RADIUS;
	
for(int i=0; i<3; i++){
	for(int j=0; j<10; j++){
		cpVect pos = cpv((j)*6+15,(i)*4+90);
		body = cpSpaceAddBody(mwSpace, cpBodyNew(40.0f, cpMomentForCircle(40.0f, 0.0f, radius, cpvzero)));
		cpBodySetPos(body, pos);
		cpBodySetID(body,i*50+j);
		body->v = cpv(body->v.x,-(cpfabs(body->v.y)));

		shape = cpSpaceAddShape(mwSpace, cpCircleShapeNew(body, radius, cpvzero));
		shape->e = WATER_E; 
		shape->u = WATER_U; 
	}
}



	mwLevelWordInit(CurrLevelNum);
	mwLevelDuckInit(CurrLevelNum);
	mwLevelPipeInit(CurrLevelNum);
	mwLevelWaitInit(CurrLevelNum);
	mwLevelAlgaeInit(CurrLevelNum);

	mwLevelDrawBKwall();
	i51KitG2DrawImage(mwContainer, MudLevel004, iNULL, iNULL);
	i51KitG2DrawImage(mwContainer, RockLevel004, iNULL, iNULL);

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

iBOOL mwRunLevel004( void )
{

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
		iU16 PenDownX = mwKeyData.u.pen.x;
		iU16 PenDownY = mwKeyData.u.pen.y;

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



