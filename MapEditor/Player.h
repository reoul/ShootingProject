#pragma once
#include <ddraw.h>
#include "GameObject.h"

class Arrow;
class Sprite8;
class CSptire;

class Player : public GameObject
{
public:
	Player();
	~Player() = default;
	bool GetIsRoll() const;
	void SetArrow(Arrow* arrow);
	Arrow* GetCurArrow() const;
	void Initialize(int x, int y, int currentFrame, int frameInterval, int moveInterval, int skillFrameInterval);
	void SetWalkSprite(Sprite8* spritePtr);
	void SetRollSprite(Sprite8* spritePtr);
	void SetDeadSprite(Sprite8* spritePtr);
	void SetSpriteAndHitRect();
	void SetBowSprite(Sprite8* _walk, Sprite8* _roll, Sprite8* _attack);
	void SetSkillSprite(Sprite* spritePtr);
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
	bool IsArrowReady() const;
	float GetSpeedX() const;
	float GetSpeedY() const;
	float GetSkillCoolTimePercent() const;
	float GetRollCoolTimePercent() const;
	bool IsArrowNull() const;
	bool IsUsingSkill() const;
	void CheckWallCollision(); //충돌검사
	int GetHp() const;
	void CreateSkillArrow();
	void CheckKeyBoard();
private:
	enum { SkillCoolTime = 10000, RollCoolTime = 1000 };
	bool mIsSkill; //스킬 사용 체크
	bool mIsMove; //이동중 체크
	bool mIsUseBow;
	bool mIsRoll;
	system_clock::time_point mLastMoveTime;
	int mMoveInterval;
	system_clock::time_point mLastSkillTime;
	system_clock::time_point mLastRollTime;
	float mSpeedX;
	float mSpeedY;
	float mOldX;
	float mOldY;
	float mMoveSpeedFold; //움직임 속도의 배율(1이 기본, 2는 2배)
	int mDrawX; //캐릭터를 화면에 그려줄때 그려주는 x좌표
	int mDrawY; //캐릭터를 화면에 그려줄때 그려주는 y좌표
	int mHp;
	RECT mWalkHitRect[8];
	RECT mRollHitRect[8][6];

	Arrow* mCurArrowPtr;
	Sprite* mCurSpritePtr;
	Sprite* mOldSpritePtr;
	Sprite* mCurBowSpritePtr;
	Sprite* mOldBowSpritePtr;
	Sprite* mSkillSpritePtr;

	Sprite8* mWalkSpritePtr;
	Sprite8* mRollSpritePtr;
	Sprite8* mDeadSpritePtr;
	Sprite8* mBowWalkSpritePtr;
	Sprite8* mBowRollSpritePtr;
	Sprite8* mBowAttackSpritePtr;
};

inline void Player::SetWalkSprite(Sprite8* spritePtr)
{
	mWalkSpritePtr = spritePtr;
}

inline void Player::SetRollSprite(Sprite8* spritePtr)
{
	mRollSpritePtr = spritePtr;
}

inline void Player::SetDeadSprite(Sprite8* spritePtr)
{
	mDeadSpritePtr = spritePtr;
}

inline void Player::SetSkillSprite(Sprite* spritePtr)
{
	mSkillSpritePtr = spritePtr;
}

inline Arrow* Player::GetCurArrow() const
{
	return mCurArrowPtr;
}

inline bool Player::IsUsingSkill() const
{
	return mIsSkill;
}

inline int Player::GetHp() const
{
	return mHp;
}

inline bool Player::GetIsRoll() const
{
	return mIsRoll;
}

inline void Player::Left()
{
	mSpeedX -= mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime();
}

inline void Player::Right()
{
	mSpeedX += mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime();
}

inline void Player::Up()
{
	mSpeedY -= mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime();
}

inline void Player::Down()
{
	mSpeedY += mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime();
}

inline float Player::GetSpeedX() const
{
	return mSpeedX;
}

inline float Player::GetSpeedY() const
{
	return mSpeedY;
}
