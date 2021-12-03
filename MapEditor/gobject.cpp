#include "gobject.h"
#include "myplayer.h"
#include "boss.h"
#include "camera.h"
#include "gameEnum.h"

extern enum ACTION_STATE;	//�÷��̾ ���� �ൿ�� ���ϴ���
extern ACTION_STATE curPlayerActionState;
extern ACTION_STATE curBossActionState;

extern CPlayer player;
extern CBoss boss;
extern CCamera camera;

CGObject::CGObject()
{
	m_nCurrentFrame = 0;
	m_bIsLive = false;
	Degree = 0;
	m_direction = Vector2();
	moveSpeed = 0;
}

CGObject::~CGObject()
{

}

void CGObject::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int SkillFrameInterval)
{
	m_pSprite = pSprite;
	m_nCurrentFrame = CurrentFrame;
	m_x = x;
	m_y = y;
	m_nFrameInterval = FrameInterval;
	m_nSkillFrameInterval = SkillFrameInterval;
	m_bIsLive = true;
	m_pTimer = timer;
	curState = OBJECT_STATE::PLYAER;
	m_nLastFrameTime = m_pTimer->time();
}

CSprite* CGObject::GetSprite()
{
	return m_pSprite;
}

bool CGObject::IsLive()
{
	return m_bIsLive;
}

void CGObject::Kill()
{
	m_bIsLive = false;
}

void CGObject::SetState(OBJECT_STATE state)
{
	curState = state;
}

void CGObject::SetSprite(CSprite* sprite)
{
	m_nCurrentFrame = 0;
	m_pSprite = sprite;
}

void CGObject::SetSprite(CSprite* sprite, CSprite* bowSprite)
{
	if(!player.IsUsingSkill())
		m_nCurrentFrame = 0;
	m_pSprite = sprite;
	m_pBowSprite = bowSprite;
}

void CGObject::Draw(bool isMove, int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_bIsLive)
		return;
	switch (curState)
	{
	case OBJECT_STATE::PLYAER:		//�÷��̾�
		if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))
		{
			if ((curPlayerActionState == ACTION_STATE::ROLL) && ((m_nCurrentFrame + 1) == m_pSprite->GetNumberOfFrame()))
			{
				curPlayerActionState = ACTION_STATE::MOVE;
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
		if (curPlayerActionState != 6)
			m_pBowSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_STATE::ARROW:		//ȭ��
		m_pSprite->Rotate(m_direction.VectorToAngle(), m_nCurrentFrame);
		m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_STATE::BOSS:		//����
		if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))
		{
			if (!(curBossActionState == ACTION_STATE::DEAD))
			{
				if (curBossActionState == ACTION_STATE::ATTACK)
				{
					if (m_nCurrentFrame == 2)
						boss.Attack();
				}
				m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();
				if (curBossActionState == ACTION_STATE::ATTACK)
					if (m_nCurrentFrame == 0)
						boss.CheckDirectionState();
			}
			else
			{
				m_nCurrentFrame = ++m_nCurrentFrame;
				if (m_nCurrentFrame == m_pSprite->GetNumberOfFrame())
					m_nCurrentFrame = m_pSprite->GetNumberOfFrame()-1;
			}
		}
		m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_STATE::SNOWBALL:		//������
		m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_STATE::BLOCK:		//��
		if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))
		{
			m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();
		}
		m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_STATE::EDITOR_BLOCK:	//������ ���
		m_pSprite->BlockDrawing(x, y, lpSurface);
		break;
	case OBJECT_STATE::ENEMY:		//��
		if (m_pTimer->elapsed(m_nLastFrameTime, m_nFrameInterval))
		{
			m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();
		}
		m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
		break;
	}
}

void CGObject::SkillDraw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!m_bIsLive)
		return;
	if (m_pTimer->elapsed(m_nLastSkillFrameTime, m_nSkillFrameInterval))
			m_nCurrentFrame = ++m_nCurrentFrame % m_pSprite->GetNumberOfFrame();
	m_pSprite->Drawing(m_nCurrentFrame, x, y, lpSurface);
}

Vector2 CGObject::GetPos()
{
	return Vector2(m_x, m_y);
}

void CGObject::SetX(float x)
{
	m_x = x;
}

void CGObject::SetY(float y)
{
	m_y = y;
}

void CGObject::SetXY(float x, float y)
{
	m_x = x;
	m_y = y;
}

void CGObject::SetFrameInterval(int frame)
{
	m_nFrameInterval = frame;
}

void CGObject::SetHitRect(int left,int top,int right,int bottom)
{
	SetRect(&hitRect, left, top, right, bottom);
}

void CGObject::SetHitRect(RECT rect)
{
	SetRect(&hitRect, rect.left, rect.top, rect.right, rect.bottom);
}

void CGObject::SetHitWallRect()
{
	SetRect(&hitWallRect, m_pSprite->GetWidth() >> 1, m_pSprite->GetHeight() >> 1, m_pSprite->GetWidth() >> 1, m_pSprite->GetHeight() >> 1);
}

RECT CGObject::GetHitRect()
{
	RECT rect;
	rect.left = GetPos().x - hitRect.left;
	rect.top = GetPos().y - hitRect.top;
	rect.right = GetPos().x + hitRect.right;
	rect.bottom = GetPos().y + hitRect.bottom;
	return rect;
}

RECT CGObject::GetHitRectImageRect()
{
	RECT rect;
	rect.left = GetPos().x - (m_pSprite->GetWidth() >> 1);
	rect.top = GetPos().y - (m_pSprite->GetHeight() >> 1);
	rect.right = GetPos().x + (m_pSprite->GetWidth() >> 1);
	rect.bottom = GetPos().y + (m_pSprite->GetHeight() >> 1);
	return rect;
}

RECT CGObject::GetHitRectWallRect()
{
	RECT rect;
	rect.left = GetPos().x - hitWallRect.left;
	rect.top = GetPos().y - hitWallRect.top;
	rect.right = GetPos().x + hitWallRect.right;
	rect.bottom = GetPos().y + hitWallRect.bottom;
	return rect;
}

bool CGObject::CheckHit(RECT rect)
{
	RECT myHitRect = GetHitRect();
	RECT _hitrect;
	if (IntersectRect(&_hitrect, &myHitRect, &rect))
	{
		return true;
	}
	return false;
}

bool CGObject::CheckHitImageRect(RECT rect)
{
	RECT myHitRect = GetHitRectImageRect();
	RECT _hitrect;
	if (IntersectRect(&_hitrect, &myHitRect, &rect))
	{
		return true;
	}
	return false;
}

bool CGObject::CheckHitWallRect(RECT rect)
{
	RECT myHitRect = GetHitRectWallRect();
	RECT _hitrect;
	if (IntersectRect(&_hitrect, &myHitRect, &rect))
	{
		return true;
	}
	return false;
}

RECT CGObject::GetHitRect2(RECT rect)
{
	RECT myHitRect = GetHitRectWallRect();
	RECT _hitrect;
	IntersectRect(&_hitrect, &myHitRect, &rect);
	return _hitrect;
}

void CGObject::SetDirection(Vector2 _direction)
{
	m_direction = _direction;
}

Vector2 CGObject::GetDirection()
{
	return m_direction;
}
