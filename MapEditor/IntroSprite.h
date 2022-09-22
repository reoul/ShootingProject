#pragma once
#include <ddraw.h>
#include <memory>

#include "ButtonDown.h"

class Bmp;

class IntroSprite : public ButtonDown
{
public:
	IntroSprite();
	~IntroSprite() = default;
	bool InitSprite(int nFrame, int nWidth, int nHeight, int nColorKey, LPDIRECTDRAW7 pDirectDraw);
	bool LoadBMPFile(const char* filePath);
	bool LoadFrame(int nFrame, const char* filePath);
	bool CopyBufferToSurface(LPDIRECTDRAW7 pDirectDraw);
	bool Drawing(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey) const;
	bool Drawing2(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey) const;
	bool ReleaseAll() const;
	void RestoreAll(LPDIRECTDRAW7 pDirectDraw);
private:
	int mWidth;
	int mHeight;
	int mFrame;
	std::unique_ptr<unsigned char[]> mBufferPtr;
	ButtonDown mButtonDown;
	std::unique_ptr<Bmp[]> mBmpPtr;
	std::unique_ptr<LPDIRECTDRAWSURFACE7[]> mSurfacePtr;
};
