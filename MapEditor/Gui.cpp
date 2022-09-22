#include "Gui.h"
#include "Player.h"

Gui::Gui()
	: mX(0)
	, mY(0)
	, mSpritePtr(nullptr)
	, mCurrentFrame(0)
	, mLastFrameTime(Timer::Now())
	, mFrameInterval(0)
{
}

void Gui::Initialize(int x, int y, Sprite* spritePtr, int frameInterval)
{
	mX = x;
	mY = y;
	mSpritePtr = spritePtr;
	mFrameInterval = frameInterval;
	mCurrentFrame = 0;
}

void Gui::Drawing(LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (mSpritePtr->GetNumberOfFrame() > 1 && Timer::Elapsed(mLastFrameTime, mFrameInterval))
		mCurrentFrame = ++mCurrentFrame % mSpritePtr->GetNumberOfFrame();

	mSpritePtr->Drawing(mCurrentFrame, mX, mY, lpSurface, true);
}

void Gui::DrawingBossHp(LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (Timer::Elapsed(mLastFrameTime, mFrameInterval))
		mCurrentFrame = ++mCurrentFrame % mSpritePtr->GetNumberOfFrame();

	mSpritePtr->DrawingBossHp(mCurrentFrame, mX, mY, lpSurface, true);
}

void Gui::DrawingPlayerHp(LPDIRECTDRAWSURFACE7 lpSurface) const
{
	mSpritePtr->Drawing(mCurrentFrame, mX, mY, lpSurface, true);
}
