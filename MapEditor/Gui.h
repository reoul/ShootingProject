#pragma once
#include "Sprite.h"
#include "Timer.h"

class Gui
{
public:
	Gui();
	~Gui() = default;
	void Initialize(int x, int y, Sprite* spritePtr, int frameInterval);
	virtual void Drawing(LPDIRECTDRAWSURFACE7 lpSurface);
	void DrawingBossHp(LPDIRECTDRAWSURFACE7 lpSurface);
	void DrawingPlayerHp(LPDIRECTDRAWSURFACE7 lpSurface) const;
	void SetFrame(int frame);
	int GetFrame() const;
protected:
	float mX;
	float mY;
	Sprite* mSpritePtr;
private:
	int mCurrentFrame;
	system_clock::time_point mLastFrameTime;
	int mFrameInterval;
};

inline void Gui::SetFrame(int frame)
{
	mCurrentFrame = frame;
}

inline int Gui::GetFrame() const
{
	return mCurrentFrame;
}
