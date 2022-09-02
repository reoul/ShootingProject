#pragma once
#include <ddraw.h>

class Bmp
{
public:
	Bmp();
	~Bmp();
	int GetWidth();
	int GetHeight();
	unsigned char* GetBMPBuffer();
	bool LoadBMPFile(char* filename);
	bool CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface);
	bool CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurfac, unsigned char* buffer);
	bool SaveWorldBMP(int x, int y, unsigned char* buffer, unsigned char* buffer2);
private:
	int m_nWidth;
	int m_nHeight;
	unsigned char* m_pBuffer;
};
