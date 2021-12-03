#pragma comment(lib,"winmm.lib")
#include <Windows.h>

#include "timer.h"

CTimer::CTimer()
{
	m_nStartTime = 0;
	deltaTime = 0;
}

CTimer::~CTimer()
{
}

void CTimer::start()
{
	m_nStartTime = timeGetTime();
}

int CTimer::time()
{
	return (timeGetTime() - m_nStartTime);
}

int CTimer::GetTime()
{
	return timeGetTime();
}

bool CTimer::elapsed(int &stime, int interval)
{
	int current_time = time();
	if (current_time >= stime + interval)
	{
		stime = current_time;
		return true;
	}
	else
		return false;
}