#include "snowball.h"
#include "camera.h"
#include "block.h"

extern CCamera camera;
extern CBLOCK wall[139];

CSnowBall::CSnowBall()
{

}
CSnowBall::~CSnowBall()
{

}

void CSnowBall::Initialize(int x, int y, CTimer* timer, int moveFrame, CSprite* sprite)
{
	draw_x = 0;
	draw_y = 0;
	m_nLastMoveTime = 0;
	m_nMoveInterval = moveFrame;
	SetRect(&snowHitRect, 24, 24, 24, 24);
	CGObject::Initialize(sprite, x, y, timer, 0, 0, 0);
}

void CSnowBall::SetDirection(Vector2 _direction)
{
	CGObject::SetDirection(_direction);
	m_bIsUse = true;
}

void CSnowBall::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	draw_x = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH*0.5f));
	draw_y = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT*0.5f));
	CGObject::Draw(false, draw_x, draw_y, surface);
}

void CSnowBall::CheckMove()
{
	if (!m_bIsLive)
		return;

	if (m_pTimer->elapsed(m_nLastFrameTime, m_nMoveInterval))
	{
		SetX(GetPos().x + m_direction.x * 20);
		SetY(GetPos().y + m_direction.y * 20);
	}

	RECT rect;
	if (!((GetPos().x > 1200) && (GetPos().x < 2000) && (GetPos().y > 1300) && (GetPos().y < 2000)))
	{
		for (int i = 0; i < 139; i++)
		{
			SetRect(&rect, wall[i].GetPos().x * 32, wall[i].GetPos().y * 32, wall[i].GetPos().x * 32 + 32, wall[i].GetPos().y * 32 + 32);
			if (CheckHit(rect))
			{
				/*SetX(GetX() - direction.x * 20);
				SetY(GetY() - direction.y * 20);*/
				m_bIsUse = false;
			}
		}
	}
	
	SetHitRect(snowHitRect);

	/*if (GetX() < 1100 || GetX() > 2100 || GetY() < 1100 || GetY() > 2100)
	{
		m_bIsUse = false;
	}*/
}

void CSnowBall::IsHit()
{

}

void CSnowBall::SetUse(bool use)
{
	m_bIsUse = use;
}
bool CSnowBall::IsUse()
{
	return m_bIsUse;
}