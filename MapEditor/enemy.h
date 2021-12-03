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
	float m_nMoveSpeedFold;		//������ �ӵ��� ����(1�� �⺻, 2�� 2��)
	int draw_x;		//ĳ���͸� ȭ�鿡 �׷��ٶ� �׷��ִ� x��ǥ
	int draw_y;		//ĳ���͸� ȭ�鿡 �׷��ٶ� �׷��ִ� y��ǥ

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