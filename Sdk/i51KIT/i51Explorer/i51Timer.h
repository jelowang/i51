#if !defined(_i51Timer_h_)
#define  _i51Timer_h_
#include "i51.h"
#include "i51KitAde.h"
#include "i51Public.h"

#define  TIME_COUNT_LONG_PRESS  800
#define  TIME_COUNT_DOWN  100
typedef enum tagTIMER_ID
{  
	TIMER_ID_AUTO_SCROLL,
	TIMER_ID_LONG_PRESS,
	TIMER_ID_DOWN,
	TIMER_ID_RENDER_AUTO_SCROLL,
	TIMER_ID_MAX,
}TIMER_ID;


typedef struct tagTimer
{
	TIMER_ID id;
	int preTime;
	int curTime;
	iBOOL bStarted;
}Timer;


extern void i51TimerInit();
extern void i51TimerStop(TIMER_ID Id);
extern iBOOL i51TimerCheckActived(TIMER_ID Id);
extern void i51TimerRestart(TIMER_ID Id);
extern iBOOL i51TimerCheckFinished(TIMER_ID Id,int N);
extern int i51TimerGetTickCount(TIMER_ID Id);
#endif