﻿#pragma once
#include "Timer.h"
#include "Player.h"

class Camera
{
public:
	Camera();
	~Camera();
	int GetX();
	int GetY();
	int GetZoomInRange();
	void SetXY(int x, int y);
	void Expansion();
	float GetExpansion();
	bool GetIsExpansion();
	bool GetIsFirstAlpha();
	void SetIsFirstAlpha(bool alpha);
	void SetAlpha(int alpha);
	int GetAlpha();
	void SetExpansion(float _expansion);
	void SetIsExpansion(bool expantion);
	bool IsExpansion();
	void InitExpantion();
	void Initialize();
	void CheckExpansion();
	void Left();
	void Right();
	void Up();
	void Down();
	void Left2();
	void Right2();
	void Up2();
	void Down2();
	void Move();
	void SetIsSkill(bool skill);
	int GetFinishX();
	int GetFinishY();
	int GetFinishX2();
	int GetFinishY2();
	void CheckFinishXY();
	void SetFinishXY(int x, int y);
	float GetExpansionSpeed();
	void AlphaBlending(LPDIRECTDRAWSURFACE7 lpSurface);
	void AlphaBlending2(LPDIRECTDRAWSURFACE7 lpSurface);
private:
	system_clock::time_point m_nLastFrameTime;
	int m_nExpantionInterval;
	int m_nMoveInterval;
	float m_x;
	float m_y;
	int Alpha;
	bool IsFirstAlpha;
	int m_nSpeedX;
	int m_nSpeedY;
	int m_nFinishX;
	int m_nFinishY;
	float m_nMoveSpeed;
	int m_nZoomInRange;
	float m_expansion;
	float m_expansionSpeed;
	bool m_bIsExpansion;
	bool m_bIsSkill;
	unsigned char* _rgb;
};
