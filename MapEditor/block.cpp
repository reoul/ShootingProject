#include "Block.h"
#include "SettingData.h"

Block::Block()
{
}

Block::~Block()
{
}

void Block::Initialize(Sprite* pSprite, int x, int y, Timer* timer, int CurrentFrame, int FrameInterval)
{
	GameObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval, 0);
}

void Block::Draw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	GameObject::Draw(false, (int)(-(x - GetPos().x) + (SCREEN_WIDTH * 0.5f)),
	                 (int)(-(y - GetPos().y) + (SCREEN_HEIGHT * 0.5f)), lpSurface);
}

void Block::SetSprite(Sprite* sprite)
{
	GameObject::SetSprite(sprite);
	number = sprite->GetNumber();
}

void Block::SetBlockNumber(int _number)
{
	number = _number;
}

int Block::GetBlockNumber()
{
	return number;
}
