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
	bool m_bIsSkill;		//��ų ��� üũ
	bool m_bIsMove;			//�̵��� üũ
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
	float m_nMoveSpeedFold;		//������ �ӵ��� ����(1�� �⺻, 2�� 2��)
	int draw_x;		//ĳ���͸� ȭ�鿡 �׷��ٶ� �׷��ִ� x��ǥ
	int draw_y;		//ĳ���͸� ȭ�鿡 �׷��ٶ� �׷��ִ� y��ǥ
	int m_nHp;
	RECT walkHitRect[8];
	RECT rollHitRect[8][6];

	CArrow* curArrow;
	CSprite* m_pCurSprite;
	CSprite* m_pOldSprite;
	CSprite* m_pCurBowSprite;
	CSprite* m_pOldBowSprite;
	CSprite* m_pSkillSprite;
	CSprite* m_pWalk_LeftSprite;
	CSprite* m_pWalk_LeftUpSprite;
	CSprite* m_pWalk_LeftDownSprite;
	CSprite* m_pWalk_RightSprite;
	CSprite* m_pWalk_RightUpSprite;
	CSprite* m_pWalk_RightDownSprite;
	CSprite* m_pWalk_UpSprite;
	CSprite* m_pWalk_DownSprite;

	CSprite* m_pRoll_LeftSprite;
	CSprite* m_pRoll_LeftUpSprite;
	CSprite* m_pRoll_LeftDownSprite;
	CSprite* m_pRoll_RightSprite;
	CSprite* m_pRoll_RightUpSprite;
	CSprite* m_pRoll_RightDownSprite;
	CSprite* m_pRoll_UpSprite;
	CSprite* m_pRoll_DownSprite;

	CSprite8* m_pDeadSprite;
	CSprite8* m_pBow_WalkSprite;
	CSprite8* m_pBow_RollSprite;
	CSprite8* m_pBow_AttackSprite;

public:
	CPlayer();
	~CPlayer();
	bool GetIsRoll();
	void SetArrow(CArrow* arrow);
	CArrow* GetCurArrow();
	void Initialize(int x, int y, CTimer* timer, int currentFrame, int frameInterval, int moveInterval, int skillFrameInterval);
	void SetWalkSprite(CSprite* _walk_Left, CSprite* _walk_Leftup, CSprite* _walk_Leftdown, CSprite* _walk_Right, CSprite* _walk_RightUp, CSprite* _walk_RightDown,
		CSprite* _walk_Up, CSprite* _walk_Down,CSprite8* _dead);
	void SetRollSprite(CSprite* _Roll_Left, CSprite* _Roll_LeftUp, CSprite* _Roll_LeftDown, CSprite* _Roll_Right, CSprite* _Roll_RightUp, CSprite* _Roll_RightDown,
		CSprite* _Roll_Up, CSprite* _Roll_Down);
	void SetBowSprite(CSprite8* _walk, CSprite8* _roll, CSprite8* _attack);
	void SetSkillSprite(CSprite* _skill);
	void Draw(LPDIRECTDRAWSURFACE7 surface);
	void CheckUseSkill();
	void Skill();
	void Roll();
	void Left();	//�÷��̾� �����̵�
	void Right();	//�÷��̾� �������̵�
	void Up();		//�÷��̾� �����̵�
	void Down();	//�÷��̾� �Ʒ����̵�
	void Hit();		//�÷��̾� �ǰ�
	bool CanMove();	//�̵� ���� ���� Ȯ��
	void MoveANDCheckState();
	void MoveInit();
	bool IsArrowReady();
	float GetSpeedX();
	float GetSpeedY();
	bool IsArrowNull();
	bool IsUsingSkill();
	void CheckWallCollision();		//�浹�˻�
	int GetHp();
};

#endif