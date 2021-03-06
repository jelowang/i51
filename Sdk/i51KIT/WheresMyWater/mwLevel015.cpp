
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#endif

iBOOL mwInitLevel015( void )
{
	mwInitSpace(CurrLevelNum);	
	cpBody *body;
	cpBody *staticBody = &(mwSpace->staticBody);
	cpShape *shape;
	cpFloat radius = WATER_RADIUS;
	
for(int i=0; i<2; i++){
	for(int j=0; j<5; j++){
		cpVect pos = cpv((j)*8+31,(i)*8+83);
		body = cpSpaceAddBody(mwSpace, cpBodyNew(40.0f, cpMomentForCircle(40.0f, 0.0f, radius, cpvzero)));
		cpBodySetPos(body, pos);
		cpBodySetID(body,i*50+j);
		body->v = cpv(body->v.x,-(cpfabs(body->v.y)));

		shape = cpSpaceAddShape(mwSpace, cpCircleShapeNew(body, radius, cpvzero));
		shape->e = WATER_E; 
		shape->u = WATER_U; 
	}
}

for(int i=0; i<2; i++){
	for(int j=0; j<10; j++){
		cpVect pos = cpv((j)*8+20,(i)*8+211);
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

	mwLevelDrawBKwall();
	i51KitG2DrawImage(mwContainer, MudLevel015, iNULL, iNULL);
	i51KitG2DrawImage(mwContainer, RockLevel015, iNULL, iNULL);

	mwLevelDrawPause();
	mwLevelDrawRestart();

	mwLevelOrganInit(CurrLevelNum);

    i51AdeStdMemcpy16( (void *)TempScreenBuf, (void *)mwScreenBuffer, SCREEN_HEIGHT*SCREEN_WIDTH*2);
	mwTempScreenBufUpdate(CurrLevelNum);

	mwLevelSpeedKeepInit(CurrLevelNum);

	mwLevelDrawDuck();
	mwLevelDrawOrgan();
	TimeLevelStart = i51AdeOsGetTick();
	return iTRUE;
}

iBOOL mwRunLevel015( void )
{

// 	mwEarthCutting(CurrLevelNum);

i51AdeStdMemcpy16( (void *)(mwScreenBuffer),
		(void *)(&TempScreenBuf[0][0]), 
		SCREEN_WIDTH*SCREEN_HEIGHT*2);

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
	mwLevelDrawOrgan();
	
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





