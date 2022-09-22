#pragma once
#include "GameObject.h"

class Block : public GameObject
{
public:
	Block();
	~Block() = default;
	void Initialize(Sprite* pSprite, int x, int y, int CurrentFrame, int FrameInterval);
	void Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface);
	void SetSprite(Sprite* sprite);
	void SetBlockNumber(int number);
	int GetBlockNumber() const;
private:
	int mNumber;
};

inline void Block::SetBlockNumber(int number)
{
	mNumber = number;
}

inline int Block::GetBlockNumber() const
{
	return mNumber;
}
