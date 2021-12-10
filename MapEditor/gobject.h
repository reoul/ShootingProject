#ifndef __gobject_h__
#define __gobject_h__
#include "sprite.h"
#include "timer.h"
#include <ddraw.h>
#include "Vector2.h"

extern enum OBJECT_TYPE;
class CGObject{
private:
	float m_x;
	float m_y;
protected:
	CSprite* m_pSprite;
	CSprite* m_pBowSprite;
	int m_nFrameInterval;
	int m_nCurrentFrame;
	int m_nLastFrameTime;
	int m_nSkillFrameInterval;
	int m_nLastSkillFrameTime;
	CTimer* m_pTimer;
	bool m_bIsLive;
	OBJECT_TYPE curState;
	RECT hitRect;		//물체 충돌 영역
	RECT hitWallRect;	//벽 충돌 영역
	double Degree; //회전 각도
	Vector2 m_direction; //현재 방향
	int moveSpeed;
public:
	CGObject();
	~CGObject();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval, int SkillFrameInterval);
	CSprite* GetSprite();
	void Draw(bool isMove, int x, int y, LPDIRECTDRAWSURFACE7 lpSurface);
	void SkillDraw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface);
	void SetSprite(CSprite* sprite);
	void SetSprite(CSprite* sprite, CSprite* bowSprite);
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
	RECT GetHitRectImageRect();		//자신의 이미지 영역으로 충돌영역 계산후 반환
	RECT GetHitRectWallRect();		//벽 영역으로 충돌영역 계산후 반환(플레이어 전용)
	RECT GetHitRect2(RECT rect);
	bool CheckHit(RECT rect);
	bool CheckHitImageRect(RECT rect);		//이미지 영역으로 충돌 계산
	bool CheckHitWallRect(RECT rect);		//벽 영역 충돌 계산
	void SetDirection(Vector2 _direction);
	Vector2 GetDirection();
};

#endif