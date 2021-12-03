#ifndef __arrow_h__
#define __arrow_h__

#include "gobject.h"
#include "sprite8.h"
#include "define.h"
#include "Vector2.h"

#include <math.h>


class CArrow : public CGObject
{
private:
	float m_nSpeedX;
	float m_nSpeedY;
	float m_nPower;
	int draw_x;
	int draw_y;
	int m_nCount;
	int m_nLastMoveTime;
	int m_nMoveInterval;
	int m_nTurnRadian;
	bool m_bIsUse;
	bool m_bIsHit;
	bool m_bIsCharging;
	bool m_bIsHoming;
	CSprite8* m_pArrowSprite;
	RECT arrowHitRect[8];
	CGObject* m_pTarget;
public:
	CArrow();
	~CArrow();
	void Initialize(int x, int y,CTimer* timer ,int moveFrame,CSprite8* arrowSprite);
	void SetSpeedXY(float power,Vector2 direction);
	void CheckMove();
	void IsCharging();
	void Draw(LPDIRECTDRAWSURFACE7 surface);
	CArrow* FindNotUseArrow();
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
};

#endif
