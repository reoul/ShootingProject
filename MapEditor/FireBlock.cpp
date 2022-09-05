#include "Fireblock.h"
#include "Camera.h"
#include "SettingData.h"

extern Camera camera;

FireBlock::FireBlock()
{
	draw_x = 0;
	draw_y = 0;
}

FireBlock::~FireBlock()
{
}

void FireBlock::Initialize(int x, int y, int frameInterval, Sprite* sprite)
{
	draw_x = 0;
	draw_y = 0;
	GameObject::Initialize(sprite, x, y, 0, frameInterval, 0);
}

void FireBlock::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	draw_x = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f));
	draw_y = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f));
	GameObject::Draw(false, draw_x, draw_y, surface);
}
