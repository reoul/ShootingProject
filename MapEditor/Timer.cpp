#include "Timer.h"
#pragma comment(lib,"winmm.lib")
#include <Windows.h>

static Timer g_timer;

Timer::Timer()
	: mStartTime(system_clock::now())
	, mPrevFrameTime(system_clock::now())
	, mDeltaTime(0.001f)
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

float Timer::GetDeltaTime()
{
	return g_timer.mDeltaTime;
}

void Timer::UpdateDeltaTime()
{
	const system_clock::time_point curTime = system_clock::now();
	const duration<float> frameTime = curTime - g_timer.mPrevFrameTime;
	g_timer.mDeltaTime = frameTime.count();
	g_timer.mPrevFrameTime = curTime;
}
