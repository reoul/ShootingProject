#pragma once
#include "GameObject.h"
#include "Vector2.h"

class CSprite8;

class Arrow : public GameObject
{
public:
	Arrow();
	~Arrow();
	void Initialize(int x, int y, int moveFrame, CSprite8* arrowSprite);
	void SetSpeedXY(float power, Vector2 direction);
	void CheckMove();
	void IsCharging();
	void Draw(LPDIRECTDRAWSURFACE7 surface);
	Arrow* FindNotUseArrow();
	void SetIsUse(bool use);
	void SetCharging(bool _charging);
	bool GetIsUse();
	bool GetIsCharging();
	bool GetChargingBow();
	bool IsZeroArrow();
	bool IsSpeedZero();
	void CheckSprite();
	void IsHit();
	int GetPower();
	bool GetIsHit();
	void SetIsHit(bool isHit);
	void SetHoming(bool _homing);
	void HomingArrow();
	Vector2 GetSpeedXY();
	bool IsHoming();
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
	CSprite8* mArrowSprite;
	RECT mArrowHitRect[8];
	GameObject* mTarget;
};
