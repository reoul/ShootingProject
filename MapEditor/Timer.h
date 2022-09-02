#pragma once

class Timer
{
public:
	float deltaTime;
	Timer();
	~Timer();
	void start();
	int time();
	bool elapsed(int& time, int interval);
private:
	int GetTime();
	int m_nStartTime;
};
