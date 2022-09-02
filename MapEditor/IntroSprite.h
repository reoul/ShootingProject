#pragma once
#include <ddraw.h>
#include "SettingData.h"
#include "Sprite.h"
#include "ButtonDown.h"

class IntroSprite : public ButtonDown
{
public:
	IntroSprite();
	~IntroSprite();
	bool InitSprite(int nFrame, int nWidth, int nHeight, int nColorKey, LPDIRECTDRAW7 pDirectDraw);
	bool LoadBMPFile(char* filename);
	bool LoadFrame(int nFrame, char* filename);
	bool CopyBufferToSurface(LPDIRECTDRAW7 pDirectDraw);
	bool Drawing(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey);
	bool Drawing2(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey);
	bool ReleaseAll();
	void RestoreAll(LPDIRECTDRAW7 pDirectDraw);
private:
	int m_nWidth;
	int m_nHeight;
	int m_nFrame;
	unsigned char* m_pBuffer;
	ButtonDown buttondown;
	Sprite* m_pSprite;
	Bmp* m_pBmp;
	LPDIRECTDRAWSURFACE7* m_pSurface;
};
