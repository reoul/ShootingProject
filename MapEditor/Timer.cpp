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

/**
 * 게임이 켜져있는 시간
 * \return 게임 시간
 */
system_clock::time_point Timer::Now()
{
	return system_clock::time_point(system_clock::now() - g_timer.mStartTime);
}

/**
 * 현재 시간을 기준으로 해당 시간이 지났는지 확인
 * \param time 비교 시간
 * \param interval 시간 간격
 * \return 시간이 지났으면 true, 아니면 false
 */
bool Timer::Elapsed(system_clock::time_point& time, int interval)
{
	const system_clock::time_point curTime = Now();
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
