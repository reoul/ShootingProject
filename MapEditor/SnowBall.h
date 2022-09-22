#pragma once
#include "GameObject.h"
#include "Vector2.h"

class Sprite;

class SnowBall : public GameObject
{
public:
	SnowBall();
	~SnowBall() = default;
	void Initialize(int x, int y, int moveFrame, Sprite* spritePtr);
	void SetDirection(Vector2 direction);
	void CheckMove();
	void Draw(LPDIRECTDRAWSURFACE7 lpSurface);
	void SetUse(bool use);
	bool IsUse() const;
private:
	int mDrawX;
	int mDrawY;
	int mLastMoveTime;
	int mMoveInterval;
	bool mIsUse;
	RECT mSnowHitRect;
};

inline void SnowBall::SetDirection(Vector2 direction)
{
	GameObject::SetDirection(direction);
	mIsUse = true;
}

inline void SnowBall::SetUse(bool use)
{
	mIsUse = use;
}

inline bool SnowBall::IsUse() const
{
	return mIsUse;
}
