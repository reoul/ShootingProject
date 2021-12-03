#include "block.h"
#include "define.h"

CBLOCK::CBLOCK()
{
}

CBLOCK::~CBLOCK()
{
}

void CBLOCK::Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval)
{
	CGObject::Initialize(pSprite, x, y, timer, CurrentFrame, FrameInterval, 0);
}

void CBLOCK::Draw(int x,int y,LPDIRECTDRAWSURFACE7 lpSurface)
{
	CGObject::Draw(false, (int)(-(x - GetPos().x) + (SCREEN_WIDTH * 0.5f)), (int)(-(y - GetPos().y) + (SCREEN_HEIGHT * 0.5f)), lpSurface);
}

void CBLOCK::SetSprite(CSprite* sprite)
{
	CGObject::SetSprite(sprite);
	number = sprite->GetNumber();
}

void CBLOCK::SetBlockNumber(int _number)
{
	number = _number;
}

int CBLOCK::GetBlockNumber()
{
	return number;
}