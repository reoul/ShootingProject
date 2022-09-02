#pragma once
#include "GameEnum.h"

class Sprite;

class CSprite8
{
public:
	CSprite8();
	~CSprite8();
	void Init();
	Sprite* GetSprite(DIRECTION type);
	void SetSizeAll(float size);
	bool ReleaseAll();
	void ReStoreAll();
private:
	Sprite* left;
	Sprite* leftup;
	Sprite* leftdown;
	Sprite* right;
	Sprite* rightup;
	Sprite* rightdown;
	Sprite* up;
	Sprite* down;
};
