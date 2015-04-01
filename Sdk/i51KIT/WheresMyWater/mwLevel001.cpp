
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#endif



iBOOL mwInitLevel001( void )
{

	mwInitSpace(CurrLevelNum);	

	cpBody *body;//, *staticBody = &(mwSpace->staticBody);
	cpShape *shape;
	cpFloat radius = WATER_RADIUS;
	for(int i=0; i<5; i++){
		for(int j=0; j<4; j++){
			cpVect pos = cpv((j)*4+150,(i)*4+45);
			body = cpSpaceAddBody(mwSpace, cpBodyNew(40.0f, cpMomentForCircle(40.0f, 0.0f, radius, cpvzero)));
			cpBodySetPos(body, pos);
//			cpBodySetID(body,i*50+j);
			body->v = cpv(0,0);
			shape = cpSpaceAddShape(mwSpace, cpCircleShapeNew(body, radius, cpvzero));
			shape->e = WATER_E; 
			shape->u = WATER_U; 
		}
	}
	
	mwLevelWordInit(CurrLevelNum);
	mwLevelDuckInit(CurrLevelNum);
	mwLevelPipeInit(CurrLevelNum);
	mwLevelWaitInit(CurrLevelNum);
	mwLevelOrganInit(CurrLevelNum);

	mwLevelDrawBKwall();
	i51KitG2DrawImage(mwContainer, MudLevel001, iNULL, iNULL);
	i51KitG2DrawImage(mwContainer, RockLevel001, iNULL, iNULL);

	mwLevelDrawPause();
	mwLevelDrawRestart();

    i51AdeStdMemcpy16( (void *)TempScreenBuf, (void *)mwScreenBuffer, SCREEN_HEIGHT*SCREEN_WIDTH*2);

	mwTempScreenBufUpdate(CurrLevelNum);

	mwLevelDrawDuck();
	TimeLevelStart = i51AdeOsGetTick();

	return iTRUE;

}

iBOOL mwRunLevel001( void )
{
//iLog("mwRunLevel001");
	i51AdeStdMemcpy16( (void *)(mwScreenBuffer),
		(void *)(&TempScreenBuf[0][0]), 
		SCREEN_WIDTH*SCREEN_HEIGHT*2);

//iLog("CrocodileWait");
	mwPublicAnimation(&CrocodileWait);
//iLog("mwLevelDrawDuck");
	mwLevelDrawDuck();

//iLog("cpWordDraw");
	cpWordDraw();
	if( TempScreenUpdataFlag )
	{
//iLog("TempScreenUpdataFlag");
		TempScreenUpdataFlag = iFALSE;
//iLog("mwTempScreenBufUpdate");
		mwTempScreenBufUpdate(CurrLevelNum);
//iLog("cpWordDraw1");
		cpWordDraw();
	}


	TimeLevelCurr= i51AdeOsGetTick();
	if( TimeLevelCurr- TimeLevelStart < 3000 )
	{
//iLog("mwLevelDrawPrompt");
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
//iLog("RunLevel1 OK");
	return iTRUE;
	
}


