#include "Enemy.h"
#include "Camera.h"
#include "SettingData.h"

extern Camera camera;

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(int x, int y, int currentFrame, int frameInterval, int moveInterval)
{
	m_nMoveInterval = moveInterval;
	GameObject::Initialize(m_pLeftIdleSprite, x, y, currentFrame, frameInterval, 0);
}

void Enemy::SetSprite(Sprite* _leftidle, Sprite* _rightidle, Sprite* _leftup, Sprite* _rightup,
                      Sprite* _lefthide, Sprite* _righthide, Sprite* _leftattack, Sprite* _rightattack)
{
	m_pLeftIdleSprite = _leftidle;
	m_pRightIdleSprite = _rightidle;
	m_pLeftUpSprite = _leftup;
	m_pRightUpSprite = _rightup;
	m_pLeftHideSprite = _lefthide;
	m_pRightHideSprite = _righthide;
	m_pLeftAttackSprite = _leftattack;
	m_pRightAttackSprite = _rightattack;
	GameObject::SetSprite(m_pLeftIdleSprite);
}

void Enemy::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	draw_x = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f));
	draw_y = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f));
	GameObject::Draw(true, draw_x, draw_y, surface);
}
