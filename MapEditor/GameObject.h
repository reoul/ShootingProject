#pragma once
#include <ddraw.h>
#include "Sprite.h"
#include "Timer.h"
#include "Vector2.h"

enum class EObjectType;

class GameObject
{
public:
	GameObject();
	~GameObject() = default;
	void Initialize(Sprite* pSprite, int x, int y, int curFrame, int frameInterval, int skillFrameInterval);
	Sprite* GetSprite() const;
	void Draw(bool isMove, int x, int y, LPDIRECTDRAWSURFACE7 lpSurface);
	void SkillDraw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface);
	void SetSprite(Sprite* sprite);
	void SetSprite(Sprite* sprite, Sprite* bowSprite);
	bool IsLive() const;
	void Kill();
	void SetState(EObjectType state);
	Vector2 GetPos() const;
	void SetX(float x);
	void SetY(float y);
	void SetXY(float x, float y);
	void SetFrameInterval(int frame);
	void SetHitRect(int left, int top, int right, int bottom);
	void SetHitRect(RECT rect);
	void SetHitWallRect();
	RECT GetHitRect() const;
	RECT GetHitRectImageRect() const; //자신의 이미지 영역으로 충돌영역 계산후 반환
	RECT GetHitRectWallRect() const; //벽 영역으로 충돌영역 계산후 반환(플레이어 전용)
	RECT GetHitRect2(RECT rect) const;
	bool CheckHit(RECT rect) const;
	bool CheckHitImageRect(RECT rect) const; //이미지 영역으로 충돌 계산
	bool CheckHitWallRect(RECT rect) const; //벽 영역 충돌 계산
	void SetDirection(Vector2 _direction);
	Vector2 GetDirection() const;
protected:
	Sprite* mSprite;
	Sprite* mBowSprite;
	int mFrameInterval;
	int mCurrentFrame;
	system_clock::time_point mLastFrameTime;
	int mSkillFrameInterval;
	system_clock::time_point mLastSkillFrameTime;
	int mMoveSpeed;
	bool mIsLive;
	EObjectType mCurState;
	RECT mHitRect; //물체 충돌 영역
	RECT mHitWallRect; //벽 충돌 영역
	double mDegree; //회전 각도
	Vector2 mDirection; //현재 방향
	float mX;
	float mY;
};

inline Sprite* GameObject::GetSprite() const
{
	return mSprite;
}

inline bool GameObject::IsLive() const
{
	return mIsLive;
}

inline void GameObject::Kill()
{
	mIsLive = false;
}

inline void GameObject::SetState(EObjectType state)
{
	mCurState = state;
}

inline void GameObject::SetSprite(Sprite* sprite)
{
	mCurrentFrame = 0;
	mSprite = sprite;
}

inline Vector2 GameObject::GetPos() const
{
	return Vector2(mX, mY);
}

inline void GameObject::SetX(float x)
{
	mX = x;
}

inline void GameObject::SetY(float y)
{
	mY = y;
}

inline void GameObject::SetXY(float x, float y)
{
	mX = x;
	mY = y;
}

inline void GameObject::SetFrameInterval(int frame)
{
	mFrameInterval = frame;
}

inline void GameObject::SetHitRect(int left, int top, int right, int bottom)
{
	SetRect(&mHitRect, left, top, right, bottom);
}

inline void GameObject::SetHitRect(RECT rect)
{
	SetRect(&mHitRect, rect.left, rect.top, rect.right, rect.bottom);
}

inline void GameObject::SetHitWallRect()
{
	SetRect(&mHitWallRect, mSprite->GetWidth() >> 1, mSprite->GetHeight() >> 1, mSprite->GetWidth() >> 1,
		mSprite->GetHeight() >> 1);
}

inline void GameObject::SetDirection(Vector2 _direction)
{
	mDirection = _direction;
}

inline Vector2 GameObject::GetDirection() const
{
	return mDirection;
}
