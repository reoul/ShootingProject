#ifndef __timer_h__
#define __timer_h__

class CTimer
{
private:
	int m_nStartTime;
	int GetTime();

public:
	float deltaTime;
	CTimer();
	~CTimer();
	void start();
	int time();
	bool elapsed(int &time, int interval);
};

#endif