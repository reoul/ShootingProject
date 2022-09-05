#pragma once
#include <chrono>

using namespace std;
using namespace chrono;

class Timer
{
public:
	static float deltaTime;
	Timer();
	~Timer() = default;
	static void Start();
	static system_clock::time_point Now();
	static bool Elapsed(system_clock::time_point& time, int interval);
	static void UpdateDeltaTime();
private:
	system_clock::time_point mStartTime;
	system_clock::time_point mPrevFrameTime;
};
