#pragma once
#include <ddraw.h>
#include "GameObject.h"
#include "Sprite.h"
#include "Camera.h"
#include "Sprite8.h"
#include "Vector2.h"

class Boss : public GameObject
{
public:
	int m_nHp; // todo : public 변수 private으로 옮기기
public:
	Boss();
	~Boss();
	bool GetIsRoll();
	void Initialize(int x, int y, int currentFrame, int frameInterval, int moveInterval);
	void SetSprite(Sprite* _sleep, CSprite8* _idle, CSprite8* _roll, CSprite8* _attack, CSprite8* _dead);
	void Draw(LPDIRECTDRAWSURFACE7 surface);
	bool CanMove();
	void MoveANDCheckState();
	void MoveInit();
	void NextPattern();
	float GetSpeedX();
	float GetSpeedY();
	void Attack();
	void Hit(int damege);
	void CheckPlayerDirection();
	void CheckDirectionState();
	void CheckSpeedXY();
	void CheckSprite();
	int GetCntAttack();
	int GetHp();
	Vector2 GetPlayerDirection();
private:
	bool m_bIsRoll;
	bool m_bIsHitWall;
	bool m_bIsRoar;
	system_clock::time_point m_nLastMoveTime;
	int m_nMoveInterval;
	int m_nMoveSpeed;
	float m_nSpeedX;
	float m_nSpeedY;
	float m_nMoveSpeedFold; //움직임 속도의 배율(1이 기본, 2는 2배)
	int draw_x; //캐릭터를 화면에 그려줄때 그려주는 x좌표
	int draw_y; //캐릭터를 화면에 그려줄때 그려주는 y좌표
	int pattern[9];
	int curPattern;
	int cntAttack;
	int cntHitWall;
	system_clock::time_point m_nLastIdleTime;
	int m_nIdleFrameInterval;
	Vector2 playerDirection;

	RECT idleHitRect[8];
	RECT rollHitRect;
	RECT attackHitRect[8];
	RECT sleepHitRect;

	Sprite* m_pCurSprite;
	Sprite* m_pOldSprite;
	Sprite* m_pBoss_SleepSprite;

	CSprite8* m_pBoss_IdleSprite;
	CSprite8* m_pBoss_RollSprite;
	CSprite8* m_pBoss_AttackSprite;
	CSprite8* m_pBoss_DeadSprite;
};
