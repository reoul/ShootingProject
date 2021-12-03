#ifndef __bmp_h__
#define __bmp_h__

#include <ddraw.h>
class CBMP
{
private:
	int m_nWidth;
	int m_nHeight;
	unsigned char *m_pBuffer;
public:
	CBMP();
	~CBMP();
	int GetWidth();
	int GetHeight();
	unsigned char* GetBMPBuffer();
	bool LoadBMPFile(char *filename);
	bool CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface);
	bool CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurfac, unsigned char* buffer);
	bool SaveWorldBMP(int x,int y,unsigned char *buffer,unsigned char *buffer2);
};

#endif
