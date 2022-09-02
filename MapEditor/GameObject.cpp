#include "GameObject.h"
#include "Player.h"
#include "Boss.h"
#include "Camera.h"
#include "GameEnum.h"

extern enum ACTION; //플레이어가 무슨 행동을 취하는지
extern ACTION curPlayerAction;
extern ACTION curBossAction;

extern Player player;
extern Boss boss;
extern Camera camera;

GameObject::GameObject()
{
	m_nCurrentFrame = 0;
	m_bIsLive = false;
	Degree = 0;
	m_direction = Vector2();
	moveSpeed = 0;
}

GameObject::~GameObject()
{
}

void GameObject::Initialize(Sprite* pSprite, int x, int y, Timer* timer, int CurrentFrame, int FrameInterval,
                            int SkillFrameInterval)
{
	m_pSprite = pSprite;
	m_nCurrentFrame = CurrentFrame;
	m_x = x;
	m_y = y;
	m_nFrameInterval = FrameInterval;
	m_nSkillFrameInterval = SkillFrameInterval;
	m_bIsLive = true;
	m_pTimer = timer;
	curState = OBJECT_TYPE::PLYAER;
	m_nLastFrameTime = m_pTimer->time();
}

Sprite* GameObject::GetSprite()
{
	return m_pSprite;
}

bool GameObject::IsLive()
{
	return m_bIsLive;
}

void GameObject::Kill()
{
	m_bIsLive = false;
}

void GameObject::SetState(OBJECT_TYPE state)
{
	curState = state;
}

void GameObject::SetSprite(Sprite* sprite)
{
	m_nCurrentFrame = 0;
	m_pSprite = sprite;
}

void GameObject::SetSprite(Sprite* sprite, Sprite* bowSprite)
{
	if (!player.IsUsingSkill())
		m_nCurrentFrame = 0;
	m_pSprite = sprite;
	m_pBowSprite = bowSprite;
}

void GameObject::Draw(bool isMove, int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_bIsLive)
		return;
	switch (curState)
	{
	case OBJECT_TYPE::PLYAER: //플레이어
		if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))
		{
			if ((curPlayerAction == ACTION::ROLL) && ((m_nCurrentFrame + 1) == m_pSprite->GetNumberOfFrame()))
			{
				curPlayerAction = ACTION::MOVE;
				isMove = true;
				player.MoveANDCheckState();
				m_nCurrentFrame = m_pSprite->GetNumberOfFrame() - 1;
			}
			if (isMove)
				m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();
			else
				m_nCurrentFrame = 0;
		}
		m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		if (curPlayerAction != 6)
			m_pBowSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_TYPE::ARROW: //화살
		m_pSprite->Rotate(m_direction.VectorToAngle(), m_nCurrentFrame);
		m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_TYPE::BOSS: //보스
		if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))
		{
			if (!(curBossAction == ACTION::DEAD))
			{
				if (curBossAction == ACTION::ATTACK)
				{
					if (m_nCurrentFrame == 2)
						boss.Attack();
				}
				m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();
				if (curBossAction == ACTION::ATTACK)
					if (m_nCurrentFrame == 0)
						boss.CheckDirectionState();
			}
			else
			{
				m_nCurrentFrame = ++m_nCurrentFrame;
				if (m_nCurrentFrame == m_pSprite->GetNumberOfFrame())
					m_nCurrentFrame = m_pSprite->GetNumberOfFrame() - 1;
			}
		}
		m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_TYPE::SNOWBALL: //눈덩이
		m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_TYPE::BLOCK: //블럭
		if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))
		{
			m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();
		}
		m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_TYPE::EDITOR_BLOCK: //에디터 블록
		m_pSprite->BlockDrawing(x, y, lpSurface);
		break;
	case OBJECT_TYPE::ENEMY: //적
		if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))
		{
			m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();
		}
		m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	default:
		m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	}
}

void GameObject::SkillDraw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_bIsLive)
		return;
	if (m_pTimer->elapsed(m_nLastSkillFrameTime, m_nSkillFrameInterval))
		m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();
	m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
}

Vector2 GameObject::GetPos()
{
	return Vector2(m_x, m_y);
}

void GameObject::SetX(float x)
{
	m_x = x;
}

void GameObject::SetY(float y)
{
	m_y = y;
}

void GameObject::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void GameObject::SetFrameInterval(int frame)
{
	m_nFrameInterval = frame;
}

void GameObject::SetHitRect(int left, int top, int right, int bottom)
{
	SetRect(&hitRect, left, top, right, bottom);
}

void GameObject::SetHitRect(RECT rect)
{
	SetRect(&hitRect, rect.left, rect.top, rect.right, rect.bottom);
}

void GameObject::SetHitWallRect()
{
	SetRect(&hitWallRect, m_pSprite->GetWidth() >> 1, m_pSprite->GetHeight() >> 1, m_pSprite->GetWidth() >> 1,
	        m_pSprite->GetHeight() >> 1);
}

RECT GameObject::GetHitRect()
{
	RECT rect;
	rect.left = GetPos().x - hitRect.left;
	rect.top = GetPos().y - hitRect.top;
	rect.right = GetPos().x + hitRect.right;
	rect.bottom = GetPos().y + hitRect.bottom;
	return rect;
}

RECT GameObject::GetHitRectImageRect()
{
	RECT rect;
	rect.left = GetPos().x - (m_pSprite->GetWidth() >> 1);
	rect.top = GetPos().y - (m_pSprite->GetHeight() >> 1);
	rect.right = GetPos().x + (m_pSprite->GetWidth() >> 1);
	rect.bottom = GetPos().y + (m_pSprite->GetHeight() >> 1);
	return rect;
}

RECT GameObject::GetHitRectWallRect()
{
	RECT rect;
	rect.left = GetPos().x - hitWallRect.left;
	rect.top = GetPos().y - hitWallRect.top;
	rect.right = GetPos().x + hitWallRect.right;
	rect.bottom = GetPos().y + hitWallRect.bottom;
	return rect;
}

bool GameObject::CheckHit(RECT rect)
{
	RECT myHitRect = GetHitRect();
	RECT _hitrect;
	if (IntersectRect(&_hitrect, &myHitRect, &rect))
	{
		return true;
	}
	return false;
}

bool GameObject::CheckHitImageRect(RECT rect)
{
	RECT myHitRect = GetHitRectImageRect();
	RECT _hitrect;
	if (IntersectRect(&_hitrect, &myHitRect, &rect))
	{
		return true;
	}
	return false;
}

bool GameObject::CheckHitWallRect(RECT rect)
{
	RECT myHitRect = GetHitRectWallRect();
	RECT _hitrect;
	if (IntersectRect(&_hitrect, &myHitRect, &rect))
	{
		return true;
	}
	return false;
}

RECT GameObject::GetHitRect2(RECT rect)
{
	RECT myHitRect = GetHitRectWallRect();
	RECT _hitrect;
	IntersectRect(&_hitrect, &myHitRect, &rect);
	return _hitrect;
}

void GameObject::SetDirection(Vector2 _direction)
{
	m_direction = _direction;
}

Vector2 GameObject::GetDirection()
{
	return m_direction;
}
