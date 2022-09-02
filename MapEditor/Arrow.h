#pragma once
#include "GameObject.h"
#include "Sprite8.h"
#include "SettingData.h"
#include "Vector2.h"

class Arrow : public GameObject
{
public:
	Arrow();
	~Arrow();
	void Initialize(int x, int y, Timer* timer, int moveFrame, CSprite8* arrowSprite);
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
	float m_nSpeedX;
	float m_nSpeedY;
	float m_nPower;
	int draw_x;
	int draw_y;
	int m_nCount;
	int m_nLastMoveTime;
	int m_nMoveInterval;
	int m_nTurnRadian;
	int m_nShootTime;
	bool m_bIsUse;
	bool m_bIsHit;
	bool m_bIsCharging;
	bool m_bIsHoming;
	CSprite8* m_pArrowSprite;
	RECT arrowHitRect[8];
	GameObject* m_pTarget;
};
