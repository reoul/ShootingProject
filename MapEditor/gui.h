#ifndef __gui_h__
#define __gui_h__

#include "sprite.h"
#include "timer.h"

class Gui
{
private:
	float m_x;
	float m_y;
	int m_nCurrentFrame;
	int m_nLastFrameTime;
	int m_nFrameInterval;
	CSprite* m_pSprite;
	CTimer* m_pTimer;
public:
	Gui();
	~Gui();
	void Initialize(int x, int y, CSprite* sprite,CTimer* timer, int frameInterval);
	void Drawing(LPDIRECTDRAWSURFACE7 lpsurface);
	void DrawingBossHp(LPDIRECTDRAWSURFACE7 lpsurface);
	void DrawingPlayerHp(LPDIRECTDRAWSURFACE7 lpsurface);
	void SetFrame(int frame);
	int GetFrame();
};

#endif
