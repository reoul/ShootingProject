#include "enemy.h"
#include "camera.h"

extern CCamera camera;

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(int x, int y, CTimer* timer, int currentFrame, int frameInterval, int moveInterval)
{
	m_nMoveInterval = moveInterval;
	CGObject::Initialize(m_pLeftIdleSprite, x, y, timer, currentFrame, frameInterval, 0);
}

void Enemy::SetSprite(CSprite* _leftidle, CSprite* _rightidle, CSprite* _leftup, CSprite* _rightup, 
		CSprite* _lefthide, CSprite* _righthide, CSprite* _leftattack, CSprite* _rightattack)
{
	m_pLeftIdleSprite = _leftidle;
	m_pRightIdleSprite = _rightidle;
	m_pLeftUpSprite = _leftup;
	m_pRightUpSprite = _rightup;
	m_pLeftHideSprite = _lefthide;
	m_pRightHideSprite = _righthide;
	m_pLeftAttackSprite = _leftattack;
	m_pRightAttackSprite = _rightattack;
	CGObject::SetSprite(m_pLeftIdleSprite);
}

void Enemy::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	draw_x = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f));
	draw_y = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f));
	CGObject::Draw(true, draw_x, draw_y, surface);
}
