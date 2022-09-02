#include "SnowBall.h"
#include "Camera.h"
#include "Block.h"
#include "SettingData.h"

extern Camera camera;
extern Block wall[139];

SnowBall::SnowBall()
{
}

SnowBall::~SnowBall()
{
}

void SnowBall::Initialize(int x, int y, Timer* timer, int moveFrame, Sprite* sprite)
{
	draw_x = 0;
	draw_y = 0;
	m_nLastMoveTime = 0;
	m_nMoveInterval = moveFrame;
	SetRect(&snowHitRect, 24, 24, 24, 24);
	GameObject::Initialize(sprite, x, y, timer, 0, 0, 0);
}

void SnowBall::SetDirection(Vector2 _direction)
{
	GameObject::SetDirection(_direction);
	m_bIsUse = true;
}

void SnowBall::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	draw_x = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f));
	draw_y = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f));
	GameObject::Draw(false, draw_x, draw_y, surface);
}

void SnowBall::CheckMove()
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
			SetRect(&rect, wall[i].GetPos().x * 32, wall[i].GetPos().y * 32, wall[i].GetPos().x * 32 + 32,
			        wall[i].GetPos().y * 32 + 32);
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

void SnowBall::IsHit()
{
}

void SnowBall::SetUse(bool use)
{
	m_bIsUse = use;
}

bool SnowBall::IsUse()
{
	return m_bIsUse;
}
