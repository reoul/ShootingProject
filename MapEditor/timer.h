#ifndef __timer_h__
#define __timer_h__

class CTimer
{
private:
	int m_nStartTime;

public:
	float deltaTime;
	CTimer();
	~CTimer();
	void start();
	int time();
	int GetTime();
	bool elapsed(int &time, int interval);
};

#endif