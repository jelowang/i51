
#ifndef THE_PCheck_H
#define THE_PCheck_H

#include "i51.h"
#include "i51KitAde.h"
#include "i51KitStd.h"
#include "i51KitG2.h"

#define     BENCHMARK_IDLE    -1
#define     BENCHMARK_CPU    1
#define     BENCHMARK_IO        2
#define     BENCHMARK_G2        4
#define   BENCHMARK_MEDIA   3
#define    BENCHMARK_RESULT    5

#define     BENCHMARK_INT        0
#define     BENCHMARK_FLOAT    1
#define     BENCHMARK_FILE    2
#define     BENCHMARK_SEEK    3
#define     BENCHMARK_WRITE    4
#define     BENCHMARK_READ    5
#define     BENCHMARK_RAM    6
#define     BENCHMARK_GRAPHA    7
#define     BENCHMARK_IMAGE    8
#define     BENCHMARK_FONT    9

#define MAX_TEST_NUM_CPU    50
#define MAX_TEST_NUM_IO    10
#define MAX_TEST_NUM_G2    50
#define MAX_TEST_NUM_FONT    20

typedef enum
{
    adapter = 0, // 0
    kernel,
    benchmark,
    dotpay, // 3
    mmi,
    system,
    mmi_input0, // 6
    mmi_input1,
    mmi_input2,
    mmi_sms, // 9
    mmi_socket,
    mmi_sound,
    mmi_shock,
    mmi_hook,
    mmi_settime, // 14
    mmi_sound_mid, //15
    mmi_sound_wav,
    mmi_sound_amr,
    mmi_sound_mp3,
    mmi_sound_Recordpcm,
    mmi_sound_Recordamr, //20
    status_max // 21
}STATUS_E;

void DrowMenu(void);
void DrowSMSTest(void);
void DrowInputTest(void);
void DrowSocketTest(void);
void DrowMMITest(void);
void DrowMakeCallTest(void);
void DrowShockTest(void);
void i51InterTTouchDo ( iU16 event , iU32 value );
void i51InterShockTouchDo ( iU16 event , iU32 value );
void i51InterSetTimeTouchDo ( iU16 event , iU32 value );
void i51InterHookTouchDo ( iU16 event , iU32 value );
void i51InterCallTouchDo ( iU16 event , iU32 value );
void i51InterMMITouchDo ( iU16 event , iU32 value );
void i51InterMMISubPlayerTouchDo ( iU16 event , iU32 value );
void i51InterMMISubRecorderTouchDo ( iU16 event , iU32 value );
void i51InterSocketTouchDo ( iU16 event , iU32 value );
void i51InterSMSTouchDo ( iU16 event , iU32 value );
void i51InterInputTouchDo ( iU16 event , iU32 value );
void i51AdeTesterRun (void);
void i51KernelTestRun(void);
void i51SetAllStatics(void);
void i51WriteError(void *str);
void DrowTestLog(iCHAR * text,iU8 DrowFlag);
iBOOL BenchmarkMain( void );
void i51PhoneDrawTitle(void *tile,iU16 num);
void DrowG2G2(void);
void DrowG2IO(void);
void DrowG2CPU(void);
void DrowG2TestLog(iCHAR * text);
void DrowCheckResult(void);
void DrawG2Result(void);
void InterSetFous(iU8 fous);
void DrawText(iU16 width , iU16 height , const iU16* text , iS32 desx , iS32 desy , iCOLOR color);
void TestMediaSpeed(void);
void i51PhoneDrawRect(void);
void InterKeyBack(void);
void InterKeyDo(iU8 num);
void i51InputCheckDo(iU8 para);
void i51SMSDO(iU8 para);
void SMSDoOk(void);
iBOOL DoSocket(void);
void i51MMIDO(iU8 para);
void i51CallDo(iU8 para);
void i51ShockDo(iU8 para);
void SetHook(void);
void i51SetTimeDo(void);
void SetTime(void);
void ResetInterStatus(void);
void SetMoveFous(iU8 fous);
void DrawSetTimeTest(void);
void DrawHookTest(void);
void DrowG2Sound(void);
void ResetInter();
void DrawTowButton();
void DrawCloseButton();
void i51GetAllStatus();
void UpdataInterStatus();
#endif