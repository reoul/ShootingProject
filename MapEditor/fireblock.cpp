#include "fireblock.h"
#include "camera.h"

extern CCamera camera;

FireBlock::FireBlock()
{
	draw_x = 0;
	draw_y = 0;
}

FireBlock::~FireBlock()
{
}

void FireBlock::Initialize(int x, int y, CTimer* timer, int frameInterval, CSprite* sprite)
{
	draw_x = 0;
	draw_y = 0;
	CGObject::Initialize(sprite, x, y, timer, 0, frameInterval, 0);
}

void FireBlock::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	draw_x = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f));
	draw_y = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f));
	CGObject::Draw(false, draw_x, draw_y, surface);
}
