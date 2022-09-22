#include "Fireblock.h"
#include "Camera.h"
#include "SettingData.h"

extern Camera camera;

FireBlock::FireBlock()
	: GameObject()
	, mDrawX(0)
	, mDrawY(0)
{
}

void FireBlock::Initialize(int x, int y, int frameInterval, Sprite* sprite)
{
	GameObject::Initialize(sprite, x, y, 0, frameInterval, 0);
	mDrawX = 0;
	mDrawY = 0;
}

void FireBlock::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	mDrawX = -(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f);
	mDrawY = -(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f);
	GameObject::Draw(false, mDrawX, mDrawY, surface);
}
