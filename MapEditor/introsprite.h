#ifndef __introsprite_h__
#define __introsprite_h__

#include <ddraw.h>
#include "define.h"
#include "sprite.h"
#include "buttondown.h"

class CIntroSprite : public CButtonDown
{
private:
	int m_nWidth;
	int m_nHeight;
	int m_nFrame;
	unsigned char *m_pBuffer;
	CButtonDown buttondown;
	CSprite *m_pSprite;
	CBMP *m_pBmp;
	LPDIRECTDRAWSURFACE7 *m_pSurface;

public:
	CIntroSprite();
	~CIntroSprite();
	bool InitSprite(int nFrame, int nWidth, int nHeight, int nColorKey, LPDIRECTDRAW7 pDirectDraw);
	bool LoadBMPFile(char *filename);
	bool LoadFrame(int nFrame, char* filename);
	bool CopyBufferToSurface(LPDIRECTDRAW7 pDirectDraw);
	bool Drawing(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey);
	bool Drawing2(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey);
	bool ReleaseAll();
	void RestoreAll(LPDIRECTDRAW7 pDirectDraw);
	//void KeyDown();
	//void KeyDownInit();
	//void KeyDownCheck();
	//bool CheckDown();
};

#endif
