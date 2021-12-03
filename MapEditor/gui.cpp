#include "gui.h"
#include "myplayer.h"

extern CPlayer player;

Gui::Gui()
{
	m_x = 0;
	m_y = 0;
	m_nCurrentFrame = 0;
	m_nLastFrameTime = 0;
	m_nFrameInterval = 0;
}

Gui::~Gui()
{
}

void Gui::Initialize(int x, int y, CSprite* sprite, CTimer* timer, int frameInterval)
{
	m_x = x;
	m_y = y;
	m_pSprite = sprite;
	m_pTimer = timer;
	m_nFrameInterval = frameInterval;
}

void Gui::Drawing(LPDIRECTDRAWSURFACE7 lpsurface)
{
	if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))
		m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();
	m_pSprite->Drawing(m_nCurrentFrame, m_x, m_y, lpsurface, true);
}

void Gui::DrawingBossHp(LPDIRECTDRAWSURFACE7 lpsurface)
{
	if(m_pTimer->elapsed(m_nLastFrameTime,m_nFrameInterval))
		m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();
	m_pSprite->DrawingBossHp(m_nCurrentFrame, m_x, m_y, lpsurface, true);
}

void Gui::DrawingPlayerHp(LPDIRECTDRAWSURFACE7 lpsurface)
{
	m_pSprite->Drawing(m_nCurrentFrame, m_x, m_y, lpsurface, true);
}

void Gui::SetFrame(int frame)
{
	m_nCurrentFrame = frame;
}

int Gui::GetFrame()
{
	return m_nCurrentFrame;
}
