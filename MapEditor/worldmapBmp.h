#pragma once
#include <ddraw.h>
#include "Sprite.h"

class WorldMap
{
public:
	WorldMap();
	~WorldMap();
	unsigned char* GetBMPBuffer();
	unsigned char* GetBMPBuffer2();
	LPDIRECTDRAWSURFACE7* GetSurface();
	bool LoadBMPFile(char* filename);
	bool CopyBufferToSurface(LPDIRECTDRAW7 pDirectDraw);
	bool CopyBufferToSurface2(LPDIRECTDRAW7 pDirectDraw);
	bool CopyBufferToSurface3(LPDIRECTDRAW7 pDirectDraw);
	bool CopyBufferToSurface4(LPDIRECTDRAW7 pDirectDraw);
	void CopyMapData();
	void InitMapData();
	bool Drawing(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey, LPDIRECTDRAW7 pDirectDraw);
	bool Drawing2(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey, LPDIRECTDRAW7 pDirectDraw);
	bool ReleaseAll();
	void Restore();
private:
	int m_nWidth;
	int m_nHeight;
	unsigned char* m_pBuffer;
	unsigned char* m_pBuffer2;
	Sprite* m_pSprite;
	LPDIRECTDRAWSURFACE7* m_pSurface;
	bool isStart;
};
