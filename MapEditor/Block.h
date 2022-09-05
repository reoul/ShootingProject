#pragma once
#include "GameObject.h"

class Block : public GameObject
{
public:
	Block();
	~Block();
	void Initialize(Sprite* pSprite, int x, int y, int CurrentFrame, int FrameInterval);
	void Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface);
	void SetSprite(Sprite* sprite);
	void SetBlockNumber(int _number);
	int GetBlockNumber();
private:
	int number;
};
