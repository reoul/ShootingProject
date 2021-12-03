#ifndef __snowball_h__
#define __snowball_h__

#include "gobject.h"
#include "sprite.h"
#include "define.h"
#include "Vector2.h"

class CSnowBall : public CGObject
{
private:
	int draw_x;
	int draw_y;
	int m_nLastMoveTime;
	int m_nMoveInterval;
	bool m_bIsUse;
	RECT snowHitRect;
public:
	CSnowBall();
	~CSnowBall();
	void Initialize(int x, int y, CTimer* timer, int moveFrame, CSprite* sprite);
	void SetDirection(Vector2 _direction);
	void CheckMove();
	void Draw(LPDIRECTDRAWSURFACE7 surface);
	void IsHit();
	void SetUse(bool use);
	bool IsUse();
};

#endif
