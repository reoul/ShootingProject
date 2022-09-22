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
	Boss();
	~Boss() = default;
	bool GetIsRoll() const;
	void Initialize(int x, int y, int currentFrame, int frameInterval, int moveInterval);
	void SetSprite(Sprite* _sleep, Sprite8* _idle, Sprite8* _roll, Sprite8* _attack, Sprite8* _dead);
	void Draw(LPDIRECTDRAWSURFACE7 surface);
	bool CanMove();
	void MoveANDCheckState();
	void MoveInit();
	void NextPattern();
	float GetSpeedX() const;
	float GetSpeedY() const;
	void Attack();
	void Hit(int damage);
	void CheckPlayerDirection();
	void CheckDirectionState();
	void CheckSpeedXY();
	void CheckSprite();
	int GetCntAttack() const;
	int GetHp() const;
	Vector2 GetPlayerDirection() const;
private:
	bool mIsRoll;
	bool mIsHitWall;
	bool mIsRoar;
	system_clock::time_point mLastMoveTime;
	int mMoveInterval;
	float mSpeedX;
	float mSpeedY;
	float mMoveSpeedFold; //움직임 속도의 배율(1이 기본, 2는 2배)
	int mDrawX; //캐릭터를 화면에 그려줄때 그려주는 x좌표
	int mDrawY; //캐릭터를 화면에 그려줄때 그려주는 y좌표
	int mHp;
	EAction mPattern[9];
	int mCurPatternIndex;
	int mAttackCnt;
	int mHitWallCnt;
	system_clock::time_point mLastIdleTime;
	int mIdleFrameInterval;
	Vector2 mPlayerDirection;

	RECT mIdleHitRect[8];
	RECT mRollHitRect;
	RECT mAttackHitRect[8];
	RECT mSleepHitRect;

	Sprite* mCurSpritePtr;
	Sprite* mOldSpritePtr;
	Sprite* mSleepSpritePtr;

	Sprite8* mIdleSpritePtr;
	Sprite8* mRollSpritePtr;
	Sprite8* mAttackSpritePtr;
	Sprite8* mDeadSpritePtr;
};

inline int Boss::GetHp() const
{
	return mHp;
}

inline bool Boss::GetIsRoll() const
{
	return mIsRoll;
}

inline float Boss::GetSpeedX() const
{
	return mSpeedX;
}

inline float Boss::GetSpeedY() const
{
	return mSpeedY;
}

inline int Boss::GetCntAttack() const
{
	return mAttackCnt;
}

inline Vector2 Boss::GetPlayerDirection() const
{
	return mPlayerDirection;
}
