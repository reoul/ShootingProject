#pragma once
#include "GameObject.h"
#include "Vector2.h"

class Sprite;

class SnowBall : public GameObject
{
public:
	SnowBall();
	~SnowBall();
	void Initialize(int x, int y, Timer* timer, int moveFrame, Sprite* sprite);
	void SetDirection(Vector2 _direction);
	void CheckMove();
	void Draw(LPDIRECTDRAWSURFACE7 surface);
	void IsHit();
	void SetUse(bool use);
	bool IsUse();
private:
	int draw_x;
	int draw_y;
	int m_nLastMoveTime;
	int m_nMoveInterval;
	bool m_bIsUse;
	RECT snowHitRect;
};
