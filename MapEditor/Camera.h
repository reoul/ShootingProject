#pragma once
#include "Timer.h"
#include "Player.h"

struct ThreadData;

class Camera
{
public:
	Camera();
	~Camera() = default;
	int GetX() const;
	int GetY() const;
	int GetZoomInRange() const;
	void SetXY(int x, int y);
	void Expansion();
	float GetExpansion() const;
	bool GetIsExpansion() const;
	bool GetIsFirstAlpha() const;
	void SetIsFirstAlpha(bool alpha);
	void SetAlpha(int alpha);
	int GetAlpha() const;
	void SetExpansion(float expansion);
	void SetIsExpansion(bool isExpansion);
	bool IsExpansion() const;
	void InitExpansion();
	void Initialize();
	void CheckExpansion();
	void Left();
	void Right();
	void Up();
	void Down();
	void Move();
	void SetIsSkill(bool skill);
	int GetFinishX();
	int GetFinishY();
	int GetFinishX2() const;
	int GetFinishY2() const;
	void CheckFinishXY();
	void SetFinishXY(int x, int y);
	float GetExpansionSpeed() const;
	static void AlphaBlendingThreadWorker(ThreadData* data);
	void AlphaBlending(LPDIRECTDRAWSURFACE7 lpSurface);
private:
	system_clock::time_point mLastFrameTime;
	int mExpansionInterval;
	float mX;
	float mY;
	int mAlpha;
	bool mIsFirstAlpha;
	int mSpeedX;
	int mSpeedY;
	int mFinishX;
	int mFinishY;
	float mMoveSpeed;
	int mZoomInRange;
	float mExpansion;
	float mExpansionSpeed;
	bool mIsExpansion;
	bool mIsSkill;
};

inline int Camera::GetX() const
{
	return mX;
}

inline int Camera::GetY() const
{
	return mY;
}

inline int Camera::GetZoomInRange() const
{
	return mZoomInRange;
}

inline void Camera::SetXY(int x, int y)
{
	mX = x;
	mY = y;
}

inline float Camera::GetExpansion() const
{
	return mExpansion;
}

inline bool Camera::GetIsExpansion() const
{
	return mIsExpansion;
}

inline bool Camera::GetIsFirstAlpha() const
{
	return mIsFirstAlpha;
}

inline void Camera::SetIsFirstAlpha(bool alpha)
{
	mIsFirstAlpha = alpha;
}

inline void Camera::SetAlpha(int alpha)
{
	mAlpha = alpha;
}

inline int Camera::GetAlpha() const
{
	return mAlpha;
}

inline void Camera::SetExpansion(float expansion)
{
	mExpansion = expansion;
}

inline void Camera::SetIsExpansion(bool isExpansion)
{
	mIsExpansion = isExpansion;
}

inline void Camera::Expansion()
{
	mIsExpansion = true;
}

inline bool Camera::IsExpansion() const
{
	return mIsExpansion;
}

inline void Camera::SetIsSkill(bool skill)
{
	mIsSkill = skill;
}

inline void Camera::SetFinishXY(int x, int y)
{
	mFinishX = x;
	mFinishY = y;
}

inline float Camera::GetExpansionSpeed() const
{
	return mExpansionSpeed;
}
