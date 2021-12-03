#ifndef __sprite_h_
#define __sprite_h_

#include <ddraw.h>

#include "bmp.h"

class CSprite
{
private:
	int m_nFrame;
	int m_nWidth;
	int m_nHeight;
	int draw_Width;
	int draw_Height;
	bool IsDrawWidthEven;		//짝수인지
	bool IsDrawHeightEven;		//짝수인지
	float m_nSize;
	int number;

	LPDIRECTDRAWSURFACE7* m_ppSurface;			//원본 이미지 surface
	LPDIRECTDRAWSURFACE7* m_ppDrawSurface;		//출력 이미지 surface

	CBMP* m_pBMPArray;
public:
	CSprite();
	~CSprite();
	int GetNumberOfFrame();
	CBMP* GetBMP();
	bool InitSprite(int nFrame, int nWidth, int Height, int nColorKey, LPDIRECTDRAW7 pDirectDraw);
	bool LoadFrame(int nFrame, char* filename);
	bool Drawing(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true);
	bool Drawing(RECT rect, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true);
	bool Drawing2(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true);
	bool BlockDrawing(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true);
	bool DrawingBossHp(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true);
	bool Restore();
	bool ReleaseAll();
	void SetSize(float _size);
	void SetNumber(int _number);
	float GetSize();
	int GetNumber();
	int GetWidth();
	int GetHeight();
	void Rotate(double Degree, int _curFrame);		//Degree는 회전값, 시계 반대방향으로 돈다
};
#endif