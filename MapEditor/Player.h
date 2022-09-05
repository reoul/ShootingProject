#pragma once
#include <ddraw.h>
#include "GameObject.h"

class Arrow;
class CSprite8;
class CSptire;

class Player : public GameObject
{
public:
	Player();
	~Player();
	bool GetIsRoll();
	void SetArrow(Arrow* arrow);
	Arrow* GetCurArrow();
	void Initialize(int x, int y, int currentFrame, int frameInterval, int moveInterval,
	                int skillFrameInterval);
	void SetWalkSprite(CSprite8* sprite);
	void SetRollSprite(CSprite8* sprite);
	void SetDeadSprite(CSprite8* sprite);
	void SetSpriteAndHitRect();
	void SetBowSprite(CSprite8* _walk, CSprite8* _roll, CSprite8* _attack);
	void SetSkillSprite(Sprite* _skill);
	void Draw(LPDIRECTDRAWSURFACE7 surface);
	void CheckUseSkill();
	void Skill();
	void Roll();
	void Left();
	void Right();
	void Up();
	void Down();
	void Hit(); //플레이어 피격
	bool CanMove(); //이동 가능 여부 확인
	void MoveANDCheckState();
	void MoveInit();
	bool IsArrowReady();
	float GetSpeedX();
	float GetSpeedY();
	float GetSkillCoolTimePercent();
	float GetRollCoolTimePercent();
	bool IsArrowNull();
	bool IsUsingSkill();
	void CheckWallCollision(); //충돌검사
	int GetHp();
	void CreateSkillArrow();
	void CheckKeyBoard();
private:
	enum { SkillCoolTime = 10000, RollCoolTime = 1000 };
	bool m_bIsSkill; //스킬 사용 체크
	bool m_bIsMove; //이동중 체크
	bool m_bIsUseBow;
	bool m_bIsRoll;
	system_clock::time_point m_nLastMoveTime;
	int m_nMoveInterval;
	system_clock::time_point m_nLastSkillTime;
	system_clock::time_point m_nLastRollTime;
	float m_nSpeedX;
	float m_nSpeedY;
	float m_nOldX;
	float m_nOldY;
	float m_nMoveSpeedFold; //움직임 속도의 배율(1이 기본, 2는 2배)
	int draw_x; //캐릭터를 화면에 그려줄때 그려주는 x좌표
	int draw_y; //캐릭터를 화면에 그려줄때 그려주는 y좌표
	int m_nHp;
	RECT walkHitRect[8];
	RECT rollHitRect[8][6];

	Arrow* curArrow;
	Sprite* m_pCurSprite;
	Sprite* m_pOldSprite;
	Sprite* m_pCurBowSprite;
	Sprite* m_pOldBowSprite;
	Sprite* m_pSkillSprite;

	CSprite8* m_pWalkSprite;
	CSprite8* m_pRollSprite;
	CSprite8* m_pDeadSprite;
	CSprite8* m_pBowWalkSprite;
	CSprite8* m_pBow_RollSprite;
	CSprite8* m_pBow_AttackSprite;
};
