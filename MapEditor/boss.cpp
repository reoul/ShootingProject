#include "boss.h"
#include <math.h>
#include "timer.h"
#include "snowball.h"
#include "block.h"
#include <dsound.h>
#include "DSUTIL.h"
#include "gameEnum.h"

extern CTimer g_Timer;
extern CSnowBall snowball[TOTAL_SNOWBALL];
extern CBLOCK wall[139];
extern int cntVibes;
extern bool boss_HitWall;

extern HSNDOBJ Sound[10];

CBoss::CBoss()
{

}
CBoss::~CBoss()
{

}
extern CCamera camera;
extern CPlayer player;

extern DISTANCE_TYPE curPlayerDistanceState;
extern DISTANCE_TYPE curBossDistanceState;
extern ACTION_TYPE curPlayerActionState;
extern ACTION_TYPE curBossActionState;

extern CArrow arrow[TOTAL_ARROW];

void CBoss::Initialize(int x, int y, CTimer* timer, int currentFrame, int frameInterval, int moveInterval)
{
	draw_x = x;
	draw_y = y;
	pattern[0] = 5;
	pattern[1] = 0;
	pattern[2] = 4;
	pattern[3] = 0;
	pattern[4] = 3;
	pattern[5] = 0;
	pattern[6] = 3;
	pattern[7] = 0;
	pattern[8] = 3;
	curPattern = 0;
	curBossDistanceState = DISTANCE_TYPE::DOWN;
	curBossActionState = (ACTION_TYPE)pattern[curPattern];
	m_nMoveSpeed = 1;
	m_nMoveSpeedFold = 1;
	m_bIsRoll = false;
	m_bIsHitWall = false;
	m_bIsRoar = false;
	m_nSpeedX = 0;
	m_nSpeedY = 0;
	cntAttack = 0;
	m_nHp = 1000;
	cntHitWall = 0;
	m_nIdleFrameInterval = 300;
	moveSpeed = 1600;
	m_nLastIdleTime = g_Timer.time();


	SetRect(&sleepHitRect, 67, 32, 64, 74);
	SetRect(&rollHitRect, 38, 13, 39, 74);
	int cnt = 0;
	SetRect(&idleHitRect[cnt++], 55, 33, 30, 64);
	SetRect(&idleHitRect[cnt++], 37, 33, 52, 53);
	SetRect(&idleHitRect[cnt++], 68, 28, 68, 64);
	SetRect(&idleHitRect[cnt++], 65, 32, 66, 69);
	SetRect(&idleHitRect[cnt++], 62, 30, 40, 62);
	SetRect(&idleHitRect[cnt++], 40, 30, 64, 62);
	SetRect(&idleHitRect[cnt++], 51, 28, 48, 55);
	SetRect(&idleHitRect[cnt], 50, 28, 58, 53);
	cnt = 0;
	SetRect(&attackHitRect[cnt++], 34, 56, 27, 43);
	SetRect(&attackHitRect[cnt++], 36, 60, 34, 42);
	SetRect(&attackHitRect[cnt++], 51, 54, 52, 51);
	SetRect(&attackHitRect[cnt++], 34, 37, 38, 52);
	SetRect(&attackHitRect[cnt++], 37, 49, 37, 39);
	SetRect(&attackHitRect[cnt++], 44, 52, 35, 40);
	SetRect(&attackHitRect[cnt++], 48, 55, 34, 45);
	SetRect(&attackHitRect[cnt], 41, 50, 46, 46);

	CGObject::Initialize(m_pCurSprite, x, y, timer, currentFrame, frameInterval, 0);

	m_nMoveInterval = moveInterval;
}

void CBoss::SetSprite(CSprite* _sleep, CSprite8* _idle, CSprite8* _roll, CSprite8* _attack, CSprite8* _dead)
{
	m_pBoss_SleepSprite = _sleep;
	m_pBoss_IdleSprite = _idle;
	m_pBoss_RollSprite = _roll;
	m_pBoss_AttackSprite = _attack;
	m_pBoss_DeadSprite = _dead;
	m_pCurSprite = _sleep;
	m_pOldSprite = _idle->GetLeft();
}

void CBoss::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	CGObject::Draw(true, draw_x, draw_y, surface);
}
bool CBoss::GetIsRoll()
{
	return m_bIsRoll;
}

bool CBoss::CanMove()
{
	if (!m_bIsLive)
		return false;
	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
		return true;
	return false;
}

float CBoss::GetSpeedX()
{
	return m_nSpeedX;
}
float CBoss::GetSpeedY()
{
	return m_nSpeedY;
}

void CBoss::Attack()
{
	SndObjPlay(Sound[5], NULL);
	CheckPlayerDirection();
	snowball[cntAttack].SetDirection(playerDirection);
	snowball[cntAttack].SetX(GetPos().x);
	snowball[cntAttack].SetY(GetPos().y);
	if (cntAttack >= 3)
	{
		NextPattern();
		CheckSprite();
	}
	cntAttack++;
}

void CBoss::Hit(int damege)
{
	m_nHp -= damege;
	if ((m_nHp < 300) && !m_bIsRoar)
	{
		SndObjPlay(Sound[7], NULL);
		m_bIsRoar = true;
	}
	if (m_nHp < 0)
	{
		SndObjPlay(Sound[7], NULL);
		curBossActionState = ACTION_TYPE::DEAD;
	}
}

int CBoss::GetCntAttack()
{
	return cntAttack;
}

int CBoss::GetHp()
{
	return m_nHp;
}

void CBoss::CheckPlayerDirection()
{
	float x = player.GetPos().x - GetPos().x;
	float y = player.GetPos().y - GetPos().y;
	int distance = (int)sqrtf(pow(x, 2) + pow(y,2));

	playerDirection.SetRect(x / distance, y / distance);
}

void CBoss::CheckDirectionState()
{
	CheckPlayerDirection();
	if (playerDirection.y < 0)
	{
		if ((playerDirection.x > -0.3f) && (playerDirection.x < 0.3f))
			curBossDistanceState = DISTANCE_TYPE::UP;
		else if (playerDirection.x < -0.3f)
			curBossDistanceState = DISTANCE_TYPE::LEFTUP;
		else if (playerDirection.x> 0.3f)
			curBossDistanceState = DISTANCE_TYPE::RIGHTUP;
	}
	else
	{
		if ((playerDirection.x > -0.3f) && (playerDirection.x < 0.3f))
			curBossDistanceState = DISTANCE_TYPE::DOWN;
		else if (playerDirection.x < -0.3f)
			curBossDistanceState = DISTANCE_TYPE::LEFTDOWN;
		else if (playerDirection.x > 0.3f)
			curBossDistanceState = DISTANCE_TYPE::RIGHTDOWN;
	}
	if (playerDirection.x < 0)
	{
		if ((playerDirection.y > -0.3f) && (playerDirection.y < 0.3f))
			curBossDistanceState = DISTANCE_TYPE::LEFT;
	}
	else
	{
		if ((playerDirection.y > -0.3f) && (playerDirection.y < 0.3f))
			curBossDistanceState = DISTANCE_TYPE::RIGHT;
	}
}

void CBoss::CheckSpeedXY()
{
	m_nSpeedX = playerDirection.x * moveSpeed * g_Timer.deltaTime;
	m_nSpeedY = playerDirection.y * moveSpeed * g_Timer.deltaTime;
}

void CBoss::CheckSprite()
{
	if (g_Timer.elapsed(m_nLastIdleTime, m_nIdleFrameInterval) && ((int)curBossActionState == 0)&& !((int)curBossActionState == 6))
		NextPattern();
	if ((int)curBossActionState == 6)
	{
		switch (curBossDistanceState)
		{
		case DISTANCE_TYPE::LEFT:
			m_pCurSprite = m_pBoss_DeadSprite->GetLeft();
			break;
		case DISTANCE_TYPE::RIGHT:
			m_pCurSprite = m_pBoss_DeadSprite->GetRight();
			break;
		case DISTANCE_TYPE::UP:
			m_pCurSprite = m_pBoss_DeadSprite->GetUp();
			break;
		case DISTANCE_TYPE::DOWN:
			m_pCurSprite = m_pBoss_DeadSprite->GetDown();
			break;
		case DISTANCE_TYPE::LEFTUP:
			m_pCurSprite = m_pBoss_DeadSprite->GetLeftUp();
			break;
		case DISTANCE_TYPE::RIGHTUP:
			m_pCurSprite = m_pBoss_DeadSprite->GetRightUp();
			break;
		case DISTANCE_TYPE::LEFTDOWN:
			m_pCurSprite = m_pBoss_DeadSprite->GetLeftDown();
			break;
		case DISTANCE_TYPE::RIGHTDOWN:
			m_pCurSprite = m_pBoss_DeadSprite->GetRightDown();
			break;
		}
	}
	else if (!m_bIsRoll)
	{
		if ((int)curBossActionState == 5)
			m_pCurSprite = m_pBoss_SleepSprite;
		else
		{
			switch (curBossDistanceState)
			{
			case DISTANCE_TYPE::LEFT:
				m_pCurSprite = !(curBossActionState == ACTION_TYPE::ROLL) ? m_pBoss_IdleSprite->GetLeft() : m_pBoss_RollSprite->GetLeft();
				if ((int)curBossActionState == 4)
					m_pCurSprite = m_pBoss_AttackSprite->GetLeft();
				break;
			case DISTANCE_TYPE::RIGHT:
				m_pCurSprite = !(curBossActionState == ACTION_TYPE::ROLL) ? m_pBoss_IdleSprite->GetRight() : m_pBoss_RollSprite->GetRight();
				if ((int)curBossActionState == 4)
					m_pCurSprite = m_pBoss_AttackSprite->GetRight();
				break;
			case DISTANCE_TYPE::UP:
				m_pCurSprite = !(curBossActionState == ACTION_TYPE::ROLL) ? m_pBoss_IdleSprite->GetUp() : m_pBoss_RollSprite->GetUp();
				if ((int)curBossActionState == 4)
					m_pCurSprite = m_pBoss_AttackSprite->GetUp();
				break;
			case DISTANCE_TYPE::DOWN:
				m_pCurSprite = !(curBossActionState == ACTION_TYPE::ROLL) ? m_pBoss_IdleSprite->GetDown() : m_pBoss_RollSprite->GetDown();
				if ((int)curBossActionState == 4)
					m_pCurSprite = m_pBoss_AttackSprite->GetDown();
				break;
			case DISTANCE_TYPE::LEFTUP:
				m_pCurSprite = !(curBossActionState == ACTION_TYPE::ROLL) ? m_pBoss_IdleSprite->GetLeftUp() : m_pBoss_RollSprite->GetLeftUp();
				if ((int)curBossActionState == 4)
					m_pCurSprite = m_pBoss_AttackSprite->GetLeftUp();
				break;
			case DISTANCE_TYPE::RIGHTUP:
				m_pCurSprite = !(curBossActionState == ACTION_TYPE::ROLL) ? m_pBoss_IdleSprite->GetRightUp() : m_pBoss_RollSprite->GetRightUp();
				if ((int)curBossActionState == 4)
					m_pCurSprite = m_pBoss_AttackSprite->GetRightUp();
				break;
			case DISTANCE_TYPE::LEFTDOWN:
				m_pCurSprite = !(curBossActionState == ACTION_TYPE::ROLL) ? m_pBoss_IdleSprite->GetLeftDown() : m_pBoss_RollSprite->GetLeftDown();
				if ((int)curBossActionState == 4)
					m_pCurSprite = m_pBoss_AttackSprite->GetLeftDown();
				break;
			case DISTANCE_TYPE::RIGHTDOWN:
				m_pCurSprite = !(curBossActionState == ACTION_TYPE::ROLL) ? m_pBoss_IdleSprite->GetRightDown() : m_pBoss_RollSprite->GetRightDown();
				if ((int)curBossActionState == 4)
					m_pCurSprite = m_pBoss_AttackSprite->GetRightDown();
				break;
			}
		}
	}
	if (m_pOldSprite != m_pCurSprite && (!m_bIsRoll || (curBossActionState == ACTION_TYPE::DEAD)))
	{
		m_pOldSprite = m_pCurSprite;
		CGObject::SetSprite(m_pCurSprite);
		switch (curBossActionState)
		{
		case ACTION_TYPE::IDLE:
			SetFrameInterval(300);
			break;
		case ACTION_TYPE::ROLL:
			SetFrameInterval(50);
			break;
		case ACTION_TYPE::ATTACK:
			SetFrameInterval(100);
			break;
		case ACTION_TYPE::DEAD:
			SetFrameInterval(300);
		}

		if (curBossActionState == ACTION_TYPE::ROLL)
			m_bIsRoll = true;
	}

	switch (curBossActionState)
	{
	case ACTION_TYPE::IDLE:
		SetHitRect(idleHitRect[m_nCurrentFrame]);
		break;
	case ACTION_TYPE::ROLL:
		SetHitRect(rollHitRect);
		break;
	case ACTION_TYPE::ATTACK:
		SetHitRect(attackHitRect[m_nCurrentFrame]);
		break;
	case ACTION_TYPE::SLEEP:
		SetHitRect(sleepHitRect);
		break;
	}
}

Vector2 CBoss::GetPlayerDirection()
{
	return playerDirection;
}

void CBoss::MoveANDCheckState()
{
	//if (!((int)curBossActionState == 3) && !((int)curBossActionState ==5))
	//{
	//	m_bIsRoll = false;
	//	if (camera.GetExpansion() == 1)			//화면이 확대가 안되어있을경우
	//		curBossActionState = (ACTION_STATE)0;		//캐릭터의 행동을 기본으로 초기화
	//	else
	//		curBossActionState = (ACTION_STATE)4;		//캐릭터의 행동을 공격중으로 초기화

	//	if (!(curBossActionState == (ACTION_STATE)4) && (m_nSpeedX != 0 || m_nSpeedY != 0))
	//		curBossActionState = (ACTION_STATE)3;
	//}

	//if (g_Timer.elapsed())

	if ((abs((long)m_nSpeedX) < 0.1f) && (abs((long)m_nSpeedY) < 0.1f) && m_bIsHitWall)
	{
 		m_nSpeedX = 0;
		m_nSpeedY = 0;
		NextPattern();
		m_bIsHitWall = false;
		cntHitWall = 0;
	}


	if (m_bIsHitWall)
	{
		m_nSpeedX -= m_nSpeedX * (0.03f * cntHitWall);
		m_nSpeedY -= m_nSpeedY * (0.03f * cntHitWall);
		cntHitWall++;
	}

	if (curBossActionState == ACTION_TYPE::ROLL)		//공격중이거나 기본상태가 아닐경우 캐릭터를 움직여준다
	{
		SetX(GetPos().x + m_nSpeedX);
		SetY(GetPos().y + m_nSpeedY);
	}

	RECT rect;
	if (!((GetPos().x > 1100) && (GetPos().x < 2100) && (GetPos().y > 1400) && (GetPos().y < 2100)))
	{
		for (int i = 0; i < 139; i++)
		{
			SetRect(&rect, wall[i].GetPos().x * 32, wall[i].GetPos().y * 32, wall[i].GetPos().x * 32 + 32, wall[i].GetPos().y * 32 + 32);
			if (CheckHit(rect))
			{
				SetX(GetPos().x - m_nSpeedX);
				SetY(GetPos().y - m_nSpeedY);
				m_nSpeedX = -m_nSpeedX;
				m_nSpeedY = -m_nSpeedY;
				m_bIsHitWall = true;
				cntVibes = 0;
				boss_HitWall = true;
				SndObjPlay(Sound[6], NULL);
			}
		}
	}

	//if (GetX() < 1150)
	//{
	//	m_nSpeedX = -m_nSpeedX;
	//	m_nSpeedY = m_nSpeedY;
	//	//NextPattern();
	//	SetX(1151);
	//	m_bIsHitWall = true;
	//}
	//if (GetX() > 2050)
	//{
	//	m_nSpeedX = -m_nSpeedX;
	//	m_nSpeedY = m_nSpeedY;
	//	//NextPattern();
	//	SetX(2049);
	//	m_bIsHitWall = true;
	//}
	//if (GetY() < 1150)
	//{
	//	m_nSpeedX = m_nSpeedX;
	//	m_nSpeedY = -m_nSpeedY;
	//	//NextPattern();
	//	SetY(1151);
	//	m_bIsHitWall = true;
	//}
	//if (GetY() > 2030)
	//{
	//	m_nSpeedX = m_nSpeedX;
	//	m_nSpeedY = -m_nSpeedY;
	//	//NextPattern();
	//	SetY(2029);
	//	m_bIsHitWall = true;
	//}

	draw_x = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH*0.5f));
	draw_y = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT*0.5f));

	//if (!m_bIsRoll)
	//{
	//	if (m_nSpeedX == 0)
	//	{
	//		if (m_nSpeedY < 0)		//up
	//			curBossDistanceState = (DISTANCE_STATE)3;
	//		else if (m_nSpeedY > 0)		//down
	//			curBossDistanceState = (DISTANCE_STATE)4;
	//	}
	//	else if (m_nSpeedX < 0)
	//	{
	//		if (m_nSpeedY == 0)				//left
	//			curBossDistanceState = (DISTANCE_STATE)1;
	//		else if (m_nSpeedY < 0)		//leftup
	//			curBossDistanceState = (DISTANCE_STATE)5;
	//		else if (m_nSpeedY > 0)		//leftdown
	//			curBossDistanceState = (DISTANCE_STATE)7;
	//	}
	//	else if (m_nSpeedX > 0)
	//	{
	//		if (m_nSpeedY == 0)				//right
	//			curBossDistanceState = (DISTANCE_STATE)2;
	//		else if (m_nSpeedY < 0)		//rightup
	//			curBossDistanceState = (DISTANCE_STATE)6;
	//		else if (m_nSpeedY > 0)		//rightdown
	//			curBossDistanceState = (DISTANCE_STATE)8;
	//	}
	//}
}

void CBoss::MoveInit()
{
	m_nSpeedX = 0;
	m_nSpeedY = 0;
}

void CBoss::NextPattern()
{
	if ((ACTION_TYPE)pattern[curPattern] == ACTION_TYPE::ROLL)
		m_nIdleFrameInterval = 1500;
	else
		m_nIdleFrameInterval = 300;
	curPattern++;
	if ((curPattern % 9) == 0)
	{
		curPattern = 1;
		cntAttack = 0;
	}
	switch ((ACTION_TYPE)pattern[curPattern])
	{
	case ACTION_TYPE::IDLE:
		m_nLastIdleTime = g_Timer.time();
		m_bIsRoll = false;
		MoveInit();
		break;
	case ACTION_TYPE::ROLL:
		CheckDirectionState();
		CheckSpeedXY();
		break;
	case ACTION_TYPE::ATTACK:
		CheckDirectionState();
		m_bIsRoll = false;
		break;
	}
	
	curBossActionState = (ACTION_TYPE)pattern[curPattern];
}