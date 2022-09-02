#pragma once
#include "Sprite.h"
#include "Timer.h"

class Gui
{
public:
	Gui();
	~Gui();
	void Initialize(int x, int y, Sprite* sprite, Timer* timer, int frameInterval);
	virtual void Drawing(LPDIRECTDRAWSURFACE7 lpsurface);
	void DrawingBossHp(LPDIRECTDRAWSURFACE7 lpsurface);
	void DrawingPlayerHp(LPDIRECTDRAWSURFACE7 lpsurface);
	void SetFrame(int frame);
	int GetFrame();
protected:
	float m_x;
	float m_y;
	Sprite* m_pSprite;
private:
	int m_nCurrentFrame;
	int m_nLastFrameTime;
	int m_nFrameInterval;
	Timer* m_pTimer;
};
