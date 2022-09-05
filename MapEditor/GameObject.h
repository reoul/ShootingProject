#pragma once
#include <ddraw.h>
#include "Sprite.h"
#include "Timer.h"
#include "Vector2.h"

extern enum OBJECT_TYPE;

class GameObject
{
public:
	GameObject();
	~GameObject();
	void Initialize(Sprite* pSprite, int x, int y, int CurrentFrame, int FrameInterval, int SkillFrameInterval);
	Sprite* GetSprite();
	void Draw(bool isMove, int x, int y, LPDIRECTDRAWSURFACE7 lpSurface);
	void SkillDraw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface);
	void SetSprite(Sprite* sprite);
	void SetSprite(Sprite* sprite, Sprite* bowSprite);
	bool IsLive();
	void Kill();
	void SetState(OBJECT_TYPE state);
	Vector2 GetPos();
	void SetX(float x);
	void SetY(float y);
	void SetXY(float x, float y);
	void SetFrameInterval(int frame);
	void SetHitRect(int left, int top, int right, int bottom);
	void SetHitRect(RECT rect);
	void SetHitWallRect();
	RECT GetHitRect();
	RECT GetHitRectImageRect(); //자신의 이미지 영역으로 충돌영역 계산후 반환
	RECT GetHitRectWallRect(); //벽 영역으로 충돌영역 계산후 반환(플레이어 전용)
	RECT GetHitRect2(RECT rect);
	bool CheckHit(RECT rect);
	bool CheckHitImageRect(RECT rect); //이미지 영역으로 충돌 계산
	bool CheckHitWallRect(RECT rect); //벽 영역 충돌 계산
	void SetDirection(Vector2 _direction);
	Vector2 GetDirection();
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
	OBJECT_TYPE mCurState;
	RECT mHitRect; //물체 충돌 영역
	RECT mHitWallRect; //벽 충돌 영역
	double mDegree; //회전 각도
	Vector2 mDirection; //현재 방향
private:
	float mX;
	float mY;
};
