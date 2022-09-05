#pragma once

#include "GameObject.h"
#include "Sprite.h"

class FireBlock : public GameObject
{
private:
	int draw_x;
	int draw_y;

public:
	FireBlock();
	~FireBlock();
	void Initialize(int x, int y, int frameInterval, Sprite* sprite);
	void Draw(LPDIRECTDRAWSURFACE7 surface);
};
