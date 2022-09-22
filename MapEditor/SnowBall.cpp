#include "SnowBall.h"
#include "Camera.h"
#include "Block.h"
#include "SettingData.h"

extern Camera camera;
extern Block wall[139];

SnowBall::SnowBall()
	: mDrawX(0)
	, mDrawY(0)
	, mLastMoveTime(0)
	, mMoveInterval(0)
	, mIsUse(false)
	, mSnowHitRect({ 24,24,24,24 })
{
}

void SnowBall::Initialize(int x, int y, int moveFrame, Sprite* spritePtr)
{
	mDrawX = 0;
	mDrawY = 0;
	mLastMoveTime = 0;
	mMoveInterval = moveFrame;
	GameObject::Initialize(spritePtr, x, y, 0, 0, 0);
}

void SnowBall::Draw(LPDIRECTDRAWSURFACE7 lpSurface)
{
	mDrawX = -(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f);
	mDrawY = -(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f);
	GameObject::Draw(false, mDrawX, mDrawY, lpSurface);
}

void SnowBall::CheckMove()
{
	if (!mIsLive)
		return;

	if (Timer::Elapsed(mLastFrameTime, mMoveInterval))
	{
		SetX(GetPos().x + mDirection.x * 20);
		SetY(GetPos().y + mDirection.y * 20);
	}

	RECT rect;
	if (!((GetPos().x > 1200) && (GetPos().x < 2000) && (GetPos().y > 1300) && (GetPos().y < 2000)))
	{
		for (const Block& block : wall)
		{
			SetRect(&rect, block.GetPos().x * 32, block.GetPos().y * 32, block.GetPos().x * 32 + 32,
			        block.GetPos().y * 32 + 32);
			if (CheckHit(rect))
			{
				mIsUse = false;
			}
		}
	}

	SetHitRect(mSnowHitRect);
}
