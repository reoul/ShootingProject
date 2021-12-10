#ifndef __enemy_h__
#define __enemy_h__

#include "gobject.h"
#include "sprite.h"
#include "camera.h"

class Enemy : public CGObject
{
public:
	int m_nHp;
	int m_nMoveInterval;
private:
	int m_nLastMoveTime;
	float m_nMoveSpeedFold;		//움직임 속도의 배율(1이 기본, 2는 2배)
	int draw_x;		//캐릭터를 화면에 그려줄때 그려주는 x좌표
	int draw_y;		//캐릭터를 화면에 그려줄때 그려주는 y좌표

	CSprite* m_pLeftIdleSprite;
	CSprite* m_pRightIdleSprite;
	CSprite* m_pLeftUpSprite;
	CSprite* m_pRightUpSprite;
	CSprite* m_pLeftHideSprite;
	CSprite* m_pRightHideSprite;
	CSprite* m_pLeftAttackSprite;
	CSprite* m_pRightAttackSprite;


public:
	Enemy();
	~Enemy();
	void Initialize(int x, int y, CTimer* timer, int currentFrame, int frameInterval, int moveInterval);
	void SetSprite(CSprite* _leftidle, CSprite* _rightidle, CSprite* _leftup, CSprite* _rightup, CSprite* _lefthide, CSprite* _righthide, CSprite* _leftattack, CSprite* _rightattack);
	void Draw(LPDIRECTDRAWSURFACE7 surface);
};

#endif