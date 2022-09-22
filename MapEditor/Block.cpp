#include "Block.h"
#include "SettingData.h"

Block::Block() : mNumber(0)
{
}

void Block::Initialize(Sprite* pSprite, int x, int y, int CurrentFrame, int FrameInterval)
{
	GameObject::Initialize(pSprite, x, y, CurrentFrame, FrameInterval, 0);
}

void Block::Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	GameObject::Draw(false, -(x - mX) + (SCREEN_WIDTH * 0.5f), -(y - mY) + (SCREEN_HEIGHT * 0.5f), lpSurface);
}

void Block::SetSprite(Sprite* sprite)
{
	GameObject::SetSprite(sprite);
	mNumber = sprite->GetNumber();
}
