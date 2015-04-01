#ifndef __MARMOTA_H__
#define __MARMOTA_H__

#include "marmota_backend_defines.h"
#include "marmota_types.h"
#include "marmota_application.h"
#include "marmota_graphics.h"
#include "marmota_audio.h"
#include "marmota_record.h"
#include "marmota_font.h"

typedef struct{
	mmDisplayDevice *display;
	mmGraphics		*g;
	mmAudio			*audio;
	mmRecord		*record;
}mmMarmota;
extern mmMarmota *marmota;

typedef struct mmApplication{
	mmCallBackFunc onStart;
	mmCallBackFunc onDispose;
	mmCallBackFunc onSuspend;
	mmCallBackFunc onResume;
	mmU32 timerPeriod;
	mmU32 deltaTime;

}mmApplication;
extern mmApplication mmApp;

extern mmU32 mmSystemState;
enum mmSystemStateEnum{
	mmSystemState_HasEntryed,
	mmSystemState_ExitCallBySelf,
	mmSystemState_AudPlayerIsActive,
	mmSystemState_FmRadioIsActive,
	mmSystemState_HasInterrupted,
};
extern mmBool mmGetSystemState(/*const*/ mmU32 indicator);
extern void mmSetSystemState(/*const*/ mmBool isSet, /*const*/ mmU32 indicator);

typedef struct {
	mmCallBackFunc initial;
	mmCallBackFunc destory;
	mmCallBackFunc frameProc;
	mmHardwareEvenCallBack hardwareEvent;
}mmState;

typedef struct mmStateManger{
	mmState *states;
	mmState *currentState;
	mmState *nextState;
	mmBool state_is_changed;
	mmU32 stateid;
}mmStateManger;
extern mmStateManger *mmStateMan;
extern mmBool mmStatesInit(mmU8 statesNumber, /*const*/ void* states_procs[]);
//extern void mmEntryState(/*const*/ mmU8 stateid);

#endif
