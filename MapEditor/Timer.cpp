#include "Timer.h"
#pragma comment(lib,"winmm.lib")
#include <Windows.h>

Timer::Timer()
{
	m_nStartTime = 0;
	deltaTime = 0;
}

Timer::~Timer()
{
}

void Timer::start()
{
	m_nStartTime = timeGetTime();
}

int Timer::time()
{
	return (timeGetTime() - m_nStartTime);
}

int Timer::GetTime()
{
	return timeGetTime();
}

bool Timer::elapsed(int& stime, int interval)
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
