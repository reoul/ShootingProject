#ifndef __myplayer_h__
#define __myplayer_h__

#include <ddraw.h>
#include "gobject.h"
#include "sprite.h"
#include "camera.h"
#include "sprite8.h"
#include "arrow.h"

class CPlayer : public CGObject
{
private:
	bool m_bIsSkill;		//스킬 사용 체크
	bool m_bIsMove;			//이동중 체크
	bool m_bIsUseBow;
	bool m_bIsRoll;
	int m_nLastMoveTime;
	int m_nMoveInterval;
	int m_nLastSkillTime;
	int m_nSkillCoolTime;
	float m_nSpeedX;
	float m_nSpeedY;
	float m_nOldX;
	float m_nOldY;
	float m_nMoveSpeedFold;		//움직임 속도의 배율(1이 기본, 2는 2배)
	int draw_x;		//캐릭터를 화면에 그려줄때 그려주는 x좌표
	int draw_y;		//캐릭터를 화면에 그려줄때 그려주는 y좌표
	int m_nHp;
	RECT walkHitRect[8];
	RECT rollHitRect[8][6];

	CArrow* curArrow;
	CSprite* m_pCurSprite;
	CSprite* m_pOldSprite;
	CSprite* m_pCurBowSprite;
	CSprite* m_pOldBowSprite;
	CSprite* m_pSkillSprite;

	CSprite8* m_pWalkSprite;
	CSprite8* m_pRollSprite;
	CSprite8* m_pDeadSprite;
	CSprite8* m_pBowWalkSprite;
	CSprite8* m_pBow_RollSprite;
	CSprite8* m_pBow_AttackSprite;

public:
	CPlayer();
	~CPlayer();
	bool GetIsRoll();
	void SetArrow(CArrow* arrow);
	CArrow* GetCurArrow();
	void Initialize(int x, int y, CTimer* timer, int currentFrame, int frameInterval, int moveInterval, int skillFrameInterval);
	void SetWalkSprite(CSprite8* sprite);
	void SetRollSprite(CSprite8* sprite);
	void SetDeadSprite(CSprite8* sprite);
	void SetSpriteAndHitRect();
	void SetBowSprite(CSprite8* _walk, CSprite8* _roll, CSprite8* _attack);
	void SetSkillSprite(CSprite* _skill);
	void Draw(LPDIRECTDRAWSURFACE7 surface);
	void CheckUseSkill();
	void Skill();
	void Roll();
	void Left();
	void Right();
	void Up();
	void Down();
	void Hit();		//플레이어 피격
	bool CanMove();	//이동 가능 여부 확인
	void MoveANDCheckState();
	void MoveInit();
	bool IsArrowReady();
	float GetSpeedX();
	float GetSpeedY();
	bool IsArrowNull();
	bool IsUsingSkill();
	void CheckWallCollision();		//충돌검사
	int GetHp();
};

#endif