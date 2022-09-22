#pragma once
#include "GameObject.h"
#include "Vector2.h"

class Sprite8;

class Arrow : public GameObject
{
public:
	Arrow();
	~Arrow() = default;
	void Initialize(int x, int y, int moveFrame, Sprite8* arrowSprite);
	void SetSpeedXY(float power, Vector2 direction);
	void CheckMove();
	void Draw(LPDIRECTDRAWSURFACE7 surface);
	void SetIsUse(bool use);
	void SetCharging(bool _charging);
	bool IsUse() const;
	bool IsCharging() const;
	bool IsSpeedZero() const;
	void CheckSprite();
	void IsHit();
	int GetPower() const;
	bool GetIsHit() const;
	void SetIsHit(bool isHit);
	void SetHoming(bool _homing);
	void HomingArrow();
	Vector2 GetSpeedXY() const;
	bool IsHoming() const;
private:
	float mSpeedX;
	float mSpeedY;
	float mPower;
	int mDrawX;
	int mDrawY;
	int mCount;
	int mLastMoveTime;
	int mMoveInterval;
	int mTurnRadian;
	system_clock::time_point mShootTime;
	bool mIsUse;
	bool mIsHit;
	bool mIsCharging;
	bool mIsHoming;
	Sprite8* mArrowSprite;
	RECT mArrowHitRect[8];
	GameObject* mTarget;
};

inline bool Arrow::IsUse() const
{
	return mIsUse;
}

inline bool Arrow::IsCharging() const
{
	return mIsCharging;
}

inline bool Arrow::IsSpeedZero() const
{
	return mSpeedX == 0 && mSpeedY == 0;
}

inline void Arrow::SetIsUse(bool use)
{
	mIsUse = use;
}

inline void Arrow::SetCharging(bool _charging)
{
	mIsCharging = _charging;
}

inline int Arrow::GetPower() const
{
	return static_cast<int>(mPower);
}

inline bool Arrow::GetIsHit() const
{
	return mIsHit;
}

inline void Arrow::SetIsHit(bool isHit)
{
	mIsHit = isHit;
}

inline void Arrow::SetHoming(bool _homing)
{
	mIsHoming = _homing;
}

inline Vector2 Arrow::GetSpeedXY() const
{
	return Vector2(mSpeedX, mSpeedY);
}

inline bool Arrow::IsHoming() const
{
	return mIsHoming;
}
