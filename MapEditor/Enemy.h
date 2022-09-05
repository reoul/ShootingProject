#pragma once

#include "GameObject.h"
#include "Sprite.h"

class Enemy : public GameObject
{
public:
	int m_nHp;
	int m_nMoveInterval; // todo : public 변수 private으로 이동
public:
	Enemy();
	~Enemy();
	void Initialize(int x, int y, int currentFrame, int frameInterval, int moveInterval);
	void SetSprite(Sprite* _leftidle, Sprite* _rightidle, Sprite* _leftup, Sprite* _rightup, Sprite* _lefthide,
	               Sprite* _righthide, Sprite* _leftattack, Sprite* _rightattack);
	void Draw(LPDIRECTDRAWSURFACE7 surface);
private:
	int m_nLastMoveTime;
	float m_nMoveSpeedFold; //움직임 속도의 배율(1이 기본, 2는 2배)
	int draw_x; //캐릭터를 화면에 그려줄때 그려주는 x좌표
	int draw_y; //캐릭터를 화면에 그려줄때 그려주는 y좌표

	Sprite* m_pLeftIdleSprite;
	Sprite* m_pRightIdleSprite;
	Sprite* m_pLeftUpSprite;
	Sprite* m_pRightUpSprite;
	Sprite* m_pLeftHideSprite;
	Sprite* m_pRightHideSprite;
	Sprite* m_pLeftAttackSprite;
	Sprite* m_pRightAttackSprite;
};
