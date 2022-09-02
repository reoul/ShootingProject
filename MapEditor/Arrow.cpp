#include "Arrow.h"

#include <Windows.h>
#include <vector>

#include "Camera.h"
#include "Player.h"
#include "Block.h"
#include "Boss.h"
#include "GameEnum.h"
#include "Map.h"
#include "Vector2.h"

using namespace std;

extern DIRECTION curPlayerDirection;

extern Arrow arrow[TOTAL_ARROW];
extern Camera camera;
extern Player player;
extern Block wall[139];
extern Boss boss;
extern Timer g_Timer;
extern Map map;
extern vector<Vector2> wallBlock;

Arrow::Arrow()
{
	m_nSpeedX = 0;
	m_nSpeedY = 0;
	m_nPower = 0;
	m_nCount = 0;
	draw_x = 0;
	draw_y = 0;
	m_pArrowSprite = NULL;
	m_nLastMoveTime = 0;
	m_nMoveInterval = 0;
	m_nTurnRadian = 1;
	m_bIsCharging = false;
	m_bIsUse = false;
	m_bIsHit = false;
	m_bIsHoming = false;
	m_pTarget = &boss;
	moveSpeed = 50;
}

Arrow::~Arrow()
{
}

void Arrow::Initialize(int x, int y, Timer* timer, int moveFrame, CSprite8* arrowSprite)
{
	m_nSpeedX = 0;
	m_nSpeedY = 0;
	m_nPower = 0;
	m_nCount = 0;
	draw_x = 0;
	draw_y = 0;
	m_nShootTime = 10000000;
	m_pArrowSprite = arrowSprite;
	m_nLastMoveTime = 0;
	m_nMoveInterval = moveFrame;
	m_bIsCharging = false;
	m_bIsUse = false;
	m_bIsHit = false;
	int cnt = 0;
	SetRect(&arrowHitRect[cnt++], 10, 4, -4, 4);
	SetRect(&arrowHitRect[cnt++], -4, 4, 10, 4);
	SetRect(&arrowHitRect[cnt++], 4, 10, 4, -4);
	SetRect(&arrowHitRect[cnt++], 4, -4, 4, 10);
	SetRect(&arrowHitRect[cnt++], 8, 8, 0, 0);
	SetRect(&arrowHitRect[cnt++], 0, 8, 8, 0);
	SetRect(&arrowHitRect[cnt++], 8, 0, 0, 8);
	SetRect(&arrowHitRect[cnt], 0, 0, 8, 8);
	GameObject::Initialize(arrowSprite->GetSprite(DIRECTION::UP), x, y, timer, 0, 0, 0);
}

void Arrow::SetSpeedXY(float power, Vector2 direction)
{
	m_nCount = 0;
	m_nPower = power;
	m_nSpeedX = direction.x * power;
	m_nSpeedY = direction.y * power;
	m_direction = direction;
	m_nTurnRadian = 1;
	m_bIsLive = true;
	m_nShootTime = g_Timer.time();
}

void Arrow::CheckSprite()
{
	SetSprite(m_pArrowSprite->GetSprite(DIRECTION::RIGHT));
	SetHitRect(arrowHitRect[(int)curPlayerDirection - 1]);
}

void Arrow::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	if (!m_bIsLive)
		return;
	draw_x = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f));
	draw_y = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f));
	GameObject::Draw(false, draw_x, draw_y, surface);
}

void Arrow::CheckMove()
{
	if (!m_bIsLive)
		return;
	if (m_pTimer->elapsed(m_nLastFrameTime, m_nMoveInterval))
	{
		if (m_bIsHoming)
			HomingArrow();
		SetX(GetPos().x + (m_nSpeedX * g_Timer.deltaTime * moveSpeed * (m_bIsHoming ? 4 : 1)));
		SetY(GetPos().y + (m_nSpeedY * g_Timer.deltaTime * moveSpeed * (m_bIsHoming ? 4 : 1)));
		RECT rect;
		if (!((GetPos().x > 1100) && (GetPos().x < 2100) && (GetPos().y > 1400) && (GetPos().y < 2100)))
		{
			for (int i = 0; i < 139; i++)
			{
				if (map.GetStageNum() == 0)
				{
					SetRect(&rect, wallBlock[i].x * 32, wallBlock[i].y * 32, wallBlock[i].x * 32 + 32,
					        wallBlock[i].y * 32 + 32);
				}
				else
				{
					SetRect(&rect, wall[i].GetPos().x * 32, wall[i].GetPos().y * 32, wall[i].GetPos().x * 32 + 32,
					        wall[i].GetPos().y * 32 + 32);
				}
				if (CheckHit(rect))
				{
					m_bIsUse = false;
					m_bIsLive = false;
					m_bIsHoming = false;
				}
			}
		}

		if (!m_bIsHoming)
		{
			m_nSpeedX -= m_nSpeedX * 0.02f * m_nCount * moveSpeed * g_Timer.deltaTime;
			m_nSpeedY -= m_nSpeedY * 0.02f * m_nCount * moveSpeed * g_Timer.deltaTime;
		}

		if ((abs(m_nSpeedX) < 0.1f) && (abs(m_nSpeedY) < 0.1f))
		{
			m_bIsUse = false;
			m_bIsLive = false;
			m_bIsHoming = false;
			m_nSpeedX = 0;
			m_nSpeedY = 0;
		}

		if (g_Timer.elapsed(m_nShootTime, 2000))
		{
			m_bIsUse = false;
			m_bIsLive = false;
			m_bIsHoming = false;
			m_nSpeedX = 0;
			m_nSpeedY = 0;
		}

		m_nCount++;
	}
}

void Arrow::IsCharging()
{
	m_bIsCharging = true;
}

Arrow* Arrow::FindNotUseArrow()
{
	for (int i = 0; i < TOTAL_ARROW; i++)
	{
		if (!arrow[i].GetIsUse())
			return &arrow[i];
	}
	return NULL;
}

void Arrow::SetIsUse(bool use)
{
	m_bIsUse = use;
}

void Arrow::SetCharging(bool _charging)
{
	m_bIsCharging = _charging;
}

bool Arrow::GetIsUse()
{
	return m_bIsUse;
}

bool Arrow::GetIsCharging()
{
	return m_bIsCharging;
}

bool Arrow::GetChargingBow()
{
	for (int i = 0; i < TOTAL_ARROW; i++)
	{
		if (arrow[i].GetIsCharging())
			return true;
	}
	return false;
}

bool Arrow::IsZeroArrow()
{
	for (int i = 0; i < TOTAL_ARROW; i++)
	{
		if (!arrow[i].GetIsUse())
			return false;
	}
	return true;
}

bool Arrow::IsSpeedZero()
{
	if ((abs(m_nSpeedX) == 0) && (abs(m_nSpeedY) == 0))
		return true;
	return false;
}

void Arrow::IsHit()
{
	m_bIsHit = true;
	m_bIsUse = false;
	m_bIsLive = false;
	m_bIsHoming = false;
	m_nSpeedX = 0;
	m_nSpeedY = 0;
}

int Arrow::GetPower()
{
	return (int)m_nPower;
}

bool Arrow::GetIsHit()
{
	return m_bIsHit;
}

void Arrow::SetIsHit(bool isHit)
{
	m_bIsHit = isHit;
}

void Arrow::SetHoming(bool _homing)
{
	m_bIsHoming = _homing;
}

void Arrow::HomingArrow()
{
	if (!m_bIsLive && !m_bIsHoming)
		return;
	Vector2 curPos;
	curPos.SetRect(GetPos().x, GetPos().y);
	Vector2 toDir = m_pTarget->GetPos() - curPos;
	toDir = toDir.normalize();
	float cross = (toDir.x * m_direction.y) - (toDir.y * m_direction.x); //어느방향으로 돌지 계산

	float tmp = (toDir.x * m_direction.x) + (toDir.y * m_direction.y);
	if (tmp < -1)
		tmp = -1;
	if (tmp > 1)
		tmp = 1;
	float angle = acosf(tmp); //두 벡터의 내각(0~180 양수)
	if (cross > 0) //외적 값이 음수면 시계방향으로 회전
		angle *= -1;

	m_direction.Rotate(angle * m_nTurnRadian); // 회전

	m_nTurnRadian += 1;

	m_nSpeedX = m_direction.x * 5 * moveSpeed * g_Timer.deltaTime;
	m_nSpeedY = m_direction.y * 5 * moveSpeed * g_Timer.deltaTime;
}

Vector2 Arrow::GetSpeedXY()
{
	return Vector2(m_nSpeedX, m_nSpeedY);
}

bool Arrow::IsHoming()
{
	return m_bIsHoming;
}
