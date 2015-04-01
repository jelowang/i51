//#include "../marmota/marmota.h"
#include "inc/mm_superangrybird_resource.h"
#include "mm_superangrybird.h"

//extern void mm_superangrybird_kernel_initial();
//extern void mm_superangrybird_kernel_destory();
//extern void mm_superangrybird_kernel_run();
//extern void mm_superangrybird_kernel_event(mmHardwareEventEnum eventType, mmU16 wParam, mmU16 mParam);
//
//
//extern void mm_superangrybird_mainmenu_initial();
//extern void mm_superangrybird_mainmenu_destory();
//extern void mm_superangrybird_mainmenu_run();
//extern void mm_superangrybird_mainmenu_event(mmHardwareEventEnum eventType, mmU16 wParam, mmU16 mParam);

extern mmU32 mmSBALastSuspendState;

extern mmPoint *mmSABFlyPathPoints;
extern mmPoint *mmSABBluePathPoint[2];

extern mmU8 mmSABDrawOnce;

extern mmBool isReadyToShoot;
extern mmBool isValidReady;

extern void mmSABListDestory();

void mmSABOnStart()
{
	/// register game application call back
	
	/// register game state call back
	/*const void* _state_procs_[MM_SUPERANGRYBIRD_STATE_MAX * 4] = 
	{
		(const void*)mm_superangrybird_mainmenu_initial,
		(const void*)mm_superangrybird_mainmenu_destory,
		(const void*)mm_superangrybird_mainmenu_run,
		(const void*)mm_superangrybird_mainmenu_event,
	
		(const void*)mm_superangrybird_kernel_initial,
		(const void*)mm_superangrybird_kernel_destory,
		(const void*)mm_superangrybird_kernel_run,
		(const void*)mm_superangrybird_kernel_event,
	};
	mmStatesInit((mmU8)MM_SUPERANGRYBIRD_STATE_MAX, _state_procs_);
//	mmEntryState(MM_SUPERANGRYBIRD_MAINMENU);*/
}

void mmSABOnDispose(){

}

void mmSABOnResume()
{
	/*if(mmGetStateID() == MM_SUPERANGRYBIRD_MAINMENU){
		if(1 == mmSABSound){
			if(3 == mmSABMainMenu)
				marmota->audio->playRawData((mmAudioData *)sab_audio_logo_resources,0);
			else
				marmota->audio->playRawData((mmAudioData *)sab_audio_resources, 1);
		}
	}
	else if(mmGetStateID() == MM_SUPERANGRYBIRD_KERNEL)
	{
		if(mmSABGameState == mmSABShowResult){
			mmSBALastSuspendState = mmSABGameState;
			mmSABGameState = mmSABUserOperation;
			mmSABIsBackFromSuspend = mmTrue;
		}
		else if(mmSABGameState == mmSABGameOption){
			mmSBALastSuspendState = mmSABGameState;
			mmSABGameState = mmSABUserOperation;
			mmSABIsBackFromSuspend = mmTrue;
		}
		else if(mmSABGameState == mmSABUserOperation){
			if(isReadyToShoot == mmTrue){
				birdPosition.x = BIRD_STARTPOSITION.x;
				birdPosition.y = BIRD_STARTPOSITION.y;
				isReadyToShoot = mmFalse;
				mmSABIsBackFromSuspend = mmTrue;
				isValidReady = mmFalse;
			}
		}
	}*/
}


void mmSABOnSuspend()
{
	//if(mmGetStateID() == MM_SUPERANGRYBIRD_MAINMENU /*&& mmSABMainMenu != -1*/)
	//{
	//	mmSABDrawOnce = 0;
	//	mmSABListDestory();
	//	marmota->audio->stopAllAudio();
	//	mm_superangrybird_mainmenu_destory();
	//}
	//else if(mmGetStateID() == MM_SUPERANGRYBIRD_KERNEL)
	//{
	//	/*mmSABBaseCanva = NULL;
	//	if(mmSABSceneCanva){
	//		marmota->g->destoryCanvas(mmSABSceneCanva);
	//		mmSABSceneCanva = null;
	//	}
	//	if(mmSABBgMoonCanva){
	//		marmota->g->destoryCanvas(mmSABBgMoonCanva);
	//		mmSABBgMoonCanva = NULL;
	//	}
	//	if(mmSABUIResCanva){
	//		marmota->g->destoryCanvas(mmSABUIResCanva);
	//		mmSABUIResCanva = NULL;
	//	}
	//	if(mmSABBgStoreyCanva){
	//		marmota->g->destoryCanvas(mmSABBgStoreyCanva);
	//		mmSABBgStoreyCanva = NULL;
	//	}*/
	//	
	//	if( mmSABFlyPathPoints )
	//	{	
	//		mmFree(mmSABFlyPathPoints);
	//		mmSABFlyPathPoints = null;
	//	}
	//
	//	if( mmSABBluePathPoint[0] )
	//	{
	//		mmFree(mmSABBluePathPoint[0]);
	//		mmSABBluePathPoint[0] = null;
	//	}
	//
	//	if( mmSABBluePathPoint[1] )
	//	{
	//		mmFree(mmSABBluePathPoint[1]);
	//		mmSABBluePathPoint[1] = null;
	//	}
	//	
	//	//marmota->audio->stopAllAudio();
	//}
}


void mm_superangrybird_entry()
{
	//mmApp.onStart		= mmSABOnStart;
	//mmApp.onDispose		= mmSABOnDispose;//游戏结束
	//mmApp.onSuspend		= mmSABOnSuspend;//中断，释放资源
	//mmApp.onResume		= mmSABOnResume;//中断返回
	//mmApp.timerPeriod	= 100;

	//mmMarmotaEntry();
	
}