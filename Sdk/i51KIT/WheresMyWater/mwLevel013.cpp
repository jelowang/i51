
#include "mwLevelPublic.h"

#if defined(PKIG_SCREEN_240X320)
#elif defined(PKIG_SCREEN_240X400)	
#elif defined(PKIG_SCREEN_320X480)	
#endif

iBOOL mwInitLevel013( void )
{
//iLog("mwInitLevel013");
	mwInitSpace(CurrLevelNum);	
//iLog("mwInitSpace OK");
	cpBody *body = iNULL;
	cpBody *staticBody = &(mwSpace->staticBody);
	cpShape *shape = iNULL;
	cpFloat radius = WATER_RADIUS;
	
for(int i=0; i<2; i++){
	for(int j=0; j<4; j++){
//iLog("i:%d,j:%d",i,j);
		cpVect pos = cpv((j)*6+198,(i)*6+30);
		body = cpSpaceAddBody(mwSpace, cpBodyNew(40.0f, cpMomentForCircle(40.0f, 0.0f, 2.0, cpvzero)));
//iLog("Add body:%X",body);
		cpBodySetPos(body, pos);
		cpBodySetID(body,i*50+j);
		body->v = cpv(body->v.x,-(cpfabs(body->v.y)));

//iLog("Start add shape:%X",shape);
		shape = cpSpaceAddShape(mwSpace, cpCircleShapeNew(body, radius, cpvzero));
		shape->e = WATER_E; 
		shape->u = WATER_U; 
//iLog("Add shape:%X",shape);
	}
}

//iLog("Add water OK ");

for(int i=0; i<2; i++){
	for(int j=0; j<10; j++){
		cpVect pos = cpv((j)*6+33,(i)*6+60);
		body = cpSpaceAddBody(mwSpace, cpBodyNew(40.0f, cpMomentForCircle(40.0f, 0.0f, radius, cpvzero)));
		cpBodySetPos(body, pos);
		cpBodySetID(body,i*50+j);
		body->v = cpv(body->v.x,-(cpfabs(body->v.y)));

		shape = cpSpaceAddShape(mwSpace, cpCircleShapeNew(body, radius, cpvzero));
		shape->e = WATER_E; 
		shape->u = WATER_U; 
	}
}
	
//iLog("Add water OK2 ");


	mwLevelWordInit(CurrLevelNum);
//iLog("mwLevelWordInit");
	mwLevelDuckInit(CurrLevelNum);
//iLog("mwLevelDuckInit");
	mwLevelPipeInit(CurrLevelNum);
//iLog("mwLevelPipeInit");
	mwLevelWaitInit(CurrLevelNum);
//iLog("mwLevelWaitInit");

	mwLevelDrawBKwall();
	i51KitG2DrawImage(mwContainer, MudLevel013, iNULL, iNULL);
	i51KitG2DrawImage(mwContainer, RockLevel013, iNULL, iNULL);

	mwLevelDrawPause();
	mwLevelDrawRestart();

	mwLevelOrganInit(CurrLevelNum);
	mwLevelAlgaeInit(CurrLevelNum);

    i51AdeStdMemcpy16( (void *)TempScreenBuf, (void *)mwScreenBuffer, SCREEN_HEIGHT*SCREEN_WIDTH*2);
	mwTempScreenBufUpdate(CurrLevelNum);

	mwLevelDrawDuck();
	mwLevelDrawOrgan();
	TimeLevelStart = i51AdeOsGetTick();
	return iTRUE;
}

iBOOL mwRunLevel013( void )
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





