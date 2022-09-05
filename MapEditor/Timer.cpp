#include "Timer.h"
#pragma comment(lib,"winmm.lib")
#include <Windows.h>

float Timer::deltaTime = 0.001f;
static Timer g_timer;

Timer::Timer()
	: mStartTime(system_clock::now())
	, mPrevFrameTime(system_clock::now())
{
}

void Timer::Start()
{
	g_timer.mStartTime = system_clock::now();
}

system_clock::time_point Timer::Now()
{
	return system_clock::time_point(system_clock::now() - g_timer.mStartTime);
}

bool Timer::Elapsed(system_clock::time_point& time, int interval)
{
	const system_clock::time_point curTime = system_clock::now();
	const milliseconds intervalTime(interval);
	if (curTime >= time + intervalTime)
	{
		time = curTime;
		return true;
	}

	return false;
}

void Timer::UpdateDeltaTime()
{
	const system_clock::time_point curTime = system_clock::now();
	const duration<float> frameTime = curTime - g_timer.mPrevFrameTime;
	Timer::deltaTime = frameTime.count();
	g_timer.mPrevFrameTime = curTime;
}
