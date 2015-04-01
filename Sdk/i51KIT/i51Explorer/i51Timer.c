#include "i51Timer.h"


static Timer m_Timer[TIMER_ID_MAX];
void i51TimerInit()
{  
	int i;
	for (i=0;i<TIMER_ID_MAX;i++)
	{
		m_Timer[i].preTime = i51AdeOsGetTick();
		m_Timer[i].curTime = m_Timer[i].preTime;
		m_Timer[i].bStarted = iFALSE;
	}
}
void i51TimerStop(TIMER_ID Id)
{
	m_Timer[Id].bStarted = iFALSE;
}
iBOOL i51TimerCheckActived(TIMER_ID Id)
{
	return m_Timer[Id].bStarted;
}
void i51TimerRestart(TIMER_ID Id)
{
	m_Timer[Id].preTime = i51AdeOsGetTick();
	//LOG("Pre = %d",m_Timer[Id].preTime);
	m_Timer[Id].curTime = i51AdeOsGetTick();
	m_Timer[Id].bStarted = iTRUE;
}

iBOOL i51TimerCheckFinished(TIMER_ID Id,int N)
{
	iBOOL ret;
	ret = iFALSE;
	if (m_Timer[Id].bStarted)
	{
		m_Timer[Id].curTime = i51AdeOsGetTick();
		LOG("Cur-Pre = %d",m_Timer[Id].curTime - m_Timer[Id].preTime);
		if (m_Timer[Id].curTime - m_Timer[Id].preTime >= N)
		{
			ret = iTRUE;
		}
	}
	else
	{
		ret = iFALSE;
	}

	return ret;
}
int i51TimerGetTickCount(TIMER_ID Id)
{
	int ret;
	ret = 0;
	if (m_Timer[Id].bStarted)
	{
		m_Timer[Id].curTime = i51AdeOsGetTick();
		ret = m_Timer[Id].curTime - m_Timer[Id].preTime;
	}
	else
	{
		ret = 0;
	}
	return ret;
}