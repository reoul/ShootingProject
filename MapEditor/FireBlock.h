#pragma once

#include "GameObject.h"
#include "Sprite.h"

class FireBlock : public GameObject
{
public:
	FireBlock();
	~FireBlock() = default;
	void Initialize(int x, int y, int frameInterval, Sprite* sprite);
	void Draw(LPDIRECTDRAWSURFACE7 surface);
private:
	int mDrawX;
	int mDrawY;
};
