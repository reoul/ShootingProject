#include "arrow.h"

#include "camera.h"
#include "myplayer.h"
#include "block.h"
#include "boss.h"
#include <Windows.h>
#include "gameEnum.h"
#include "map.h"
#include "Vector2.h"
#include <vector>

using namespace std;

extern DISTANCE_STATE curPlayerDistanceState;

extern CArrow arrow[TOTAL_ARROW];
extern CCamera camera;
extern CPlayer player;
extern CBLOCK wall[139];
extern CBoss boss;
extern CTimer g_Timer;
extern Map map;
extern vector<Vector2> wallBlock;

CArrow::CArrow()
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

CArrow::~CArrow()
{

}

void CArrow::Initialize(int x, int y, CTimer* timer, int moveFrame, CSprite8* arrowSprite)
{
	m_nSpeedX = 0;
	m_nSpeedY = 0;
	m_nPower = 0;
	m_nCount = 0;
	draw_x = 0;
	draw_y = 0;
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
	CGObject::Initialize(arrowSprite->GetUp(), x, y, timer, 0, 0, 0);
}

void CArrow::SetSpeedXY(float power, Vector2 direction)
{
	m_nCount = 0;
	m_nPower = power;
	m_nSpeedX = direction.x * power;
	m_nSpeedY = direction.y * power;
	m_direction = direction;
	m_nTurnRadian = 1;
	m_bIsLive = true;
	//switch ((int)curPlayerDistanceState)
	//{
	//case 1:
	//	m_nSpeedX = -1 * power;
	//	m_nSpeedY = 0 * power;
	//	//SetSprite(m_pArrowSprite->GetLeft());
	//	break;
	//case 2:
	//	m_nSpeedX = 1 * power;
	//	m_nSpeedY = 0 * power;
	//	//SetSprite(m_pArrowSprite->GetRight());
	//	break;
	//case 3:
	//	m_nSpeedX = 0 * power;
	//	m_nSpeedY = -1 * power;
	//	//SetSprite(m_pArrowSprite->GetUp());
	//	break;
	//case 4:
	//	m_nSpeedX = 0 * power;
	//	m_nSpeedY = 1 * power;
	//	//SetSprite(m_pArrowSprite->GetDown());
	//	break;
	//case 5:
	//	m_nSpeedX = -0.7 * power;
	//	m_nSpeedY = -0.7 * power;
	//	//SetSprite(m_pArrowSprite->GetLeftUp());
	//	break;
	//case 6:
	//	m_nSpeedX = 0.7 * power;
	//	m_nSpeedY = -0.7 * power;
	//	//SetSprite(m_pArrowSprite->GetRightUp());
	//	break;
	//case 7:
	//	m_nSpeedX = -0.7 * power;
	//	m_nSpeedY = 0.7 * power;
	//	//SetSprite(m_pArrowSprite->GetLeftDown());
	//	break;
	//case 8:
	//	m_nSpeedX = 0.7 * power;
	//	m_nSpeedY = 0.7 * power;
	//	//SetSprite(m_pArrowSprite->GetRightDown());
	//	break;
	//}
}

void CArrow::CheckSprite()
{
	switch (curPlayerDistanceState)
	{
	case DISTANCE_STATE::LEFT:
		SetSprite(m_pArrowSprite->GetLeft());
		break;
	case DISTANCE_STATE::RIGHT:
		SetSprite(m_pArrowSprite->GetRight());
		break;
	case DISTANCE_STATE::UP:
		SetSprite(m_pArrowSprite->GetUp());
		break;
	case DISTANCE_STATE::DOWN:
		SetSprite(m_pArrowSprite->GetDown());
		break;
	case DISTANCE_STATE::LEFTUP:
		SetSprite(m_pArrowSprite->GetLeftUp());
		break;
	case DISTANCE_STATE::RIGHTUP:
		SetSprite(m_pArrowSprite->GetRightUp());
		break;
	case DISTANCE_STATE::LEFTDOWN:
		SetSprite(m_pArrowSprite->GetLeftDown());
		break;
	case DISTANCE_STATE::RIGHTDOWN:
		SetSprite(m_pArrowSprite->GetRightDown());
		break;
	}
	SetSprite(m_pArrowSprite->GetRight());
	SetHitRect(arrowHitRect[(int)curPlayerDistanceState - 1]);
}

void CArrow::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	if (!m_bIsLive)
		return;
	draw_x = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH*0.5f));
	draw_y = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT*0.5f));
	CGObject::Draw(false, draw_x, draw_y, surface);
}

void CArrow::CheckMove()
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
				if (map.GetStage() == 0)
				{
					SetRect(&rect, wallBlock[i].x * 32, wallBlock[i].y * 32, wallBlock[i].x * 32 + 32, wallBlock[i].y * 32 + 32);
				}
				else
				{
					SetRect(&rect, wall[i].GetPos().x * 32, wall[i].GetPos().y * 32, wall[i].GetPos().x * 32 + 32, wall[i].GetPos().y * 32 + 32);
				}
				if (CheckHit(rect))
				{
					//SetX(GetPos().x - m_nSpeedX);
					//SetY(GetPos().y - m_nSpeedY);
					//IsHit();
					m_bIsUse = false;
					m_bIsLive = false;
					m_bIsHoming = false;
				}
			}
		}

		/*if (GetX() < 1110)
		{
			SetX(1111);
			m_nSpeedX = 0;
		}
		if (GetX() > 2090)
		{
			SetX(2089);
			m_nSpeedX = 0;
		}
		if (GetY() < 1116)
		{
			SetY(1117);
			m_nSpeedY = 0;
		}
		if (GetY() > 2084)
		{
			SetY(2083);
			m_nSpeedY = 0;
		}*/
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
		m_nCount++;

	}
}

void CArrow::IsCharging()
{
	m_bIsCharging = true;
}

CArrow* CArrow::FindNotUseArrow()
{
	for (int i = 0; i < TOTAL_ARROW; i++)
	{
		if (!arrow[i].GetIsUse())
			return &arrow[i];
	}
	return NULL;
}

void CArrow::SetIsUse(bool use)
{
	m_bIsUse = use;
}

void CArrow::SetCharging(bool _charging)
{
	m_bIsCharging = _charging;
}

bool CArrow::GetIsUse()
{
	return m_bIsUse;
}

bool CArrow::GetIsCharging()
{
	return m_bIsCharging;
}

bool CArrow::GetChargingBow()
{
	for (int i = 0; i < TOTAL_ARROW; i++)
	{
		if (arrow[i].GetIsCharging())
			return true;
	}
	return false;
}

bool CArrow::IsZeroArrow()
{
	for (int i = 0; i < TOTAL_ARROW; i++)
	{
		if (!arrow[i].GetIsUse())
			return false;
	}
	return true;
}

bool CArrow::IsSpeedZero()
{
	if ((abs(m_nSpeedX) == 0) && (abs(m_nSpeedY) == 0))
		return true;
	return false;
}

void CArrow::IsHit()
{
	m_bIsHit = true;
	m_bIsUse = false;
	m_bIsLive = false;
	m_bIsHoming = false;
	m_nSpeedX = 0;
	m_nSpeedY = 0;
	//m_nSpeedX = -m_nSpeedX*0.2f;
	//m_nSpeedY = -m_nSpeedY*0.2f;
}

int CArrow::GetPower()
{
	return (int)m_nPower;
}

bool CArrow::GetIsHit()
{
	return m_bIsHit;
}

void CArrow::SetIsHit(bool isHit)
{
	m_bIsHit = isHit;
}

void CArrow::SetHoming(bool _homing)
{
	m_bIsHoming = _homing;
}

void CArrow::HomingArrow()
{
	if (!m_bIsLive && !m_bIsHoming)
		return;
	Vector2 curPos;
	curPos.SetRect(GetPos().x, GetPos().y);
	Vector2 toDir = m_pTarget->GetPos() - curPos;
	toDir = toDir.normalize();
	float cross = (toDir.x * m_direction.y) - (toDir.y * m_direction.x);		//어느방향으로 돌지 계산

	float tmp = (toDir.x * m_direction.x) + (toDir.y * m_direction.y);
	if (tmp < -1)
		tmp = -1;
	if (tmp > 1)
		tmp = 1;
	float angle = acosf(tmp);		//두 벡터의 내각(0~180 양수)
	//if (isnan(angle))
	//	angle = 0;
	if (cross > 0)		//외적 값이 음수면 시계방향으로 회전
		angle *= -1;

	m_direction.Rotate(angle * m_nTurnRadian);   // 회전

	m_nTurnRadian += 1;

	m_nSpeedX = m_direction.x * 5 * moveSpeed * g_Timer.deltaTime;
	m_nSpeedY = m_direction.y * 5 * moveSpeed * g_Timer.deltaTime;

    //turnRadian += 5.0f * dt;    // 회전 속도 증가

//	   if(turnRadian >= maxRadian)
//        direction = toDir;
//출처: https://eastroot1590.tistory.com/entry/유도탄-만들기Guided-Missile [글그리 블로그]

}

Vector2 CArrow::GetSpeedXY()
{
	return Vector2(m_nSpeedX,m_nSpeedY);
}

bool CArrow::IsHoming()
{
	return m_bIsHoming;
}
