#include <Windows.h>
#include "sprite.h"
#include <string>
#include "camera.h"
#include "define.h"
#include "boss.h"

#define pi 3.141592

static RECT destRect;
static RECT destRect2;
extern CCamera camera;
extern CBoss boss;

CSprite::CSprite()
{
	destRect.left = 0;
	destRect.top = 0;
	destRect.right = 800;
	destRect.bottom = 1200;

	destRect2.left = 0;
	destRect2.top = -1200;
	destRect2.right = 800;
	destRect2.bottom = 0;

	m_ppSurface = NULL;	
	m_ppDrawSurface = NULL;
	m_nFrame = 0;
	m_pBMPArray = NULL;
}

CSprite::~CSprite()
{
	if (m_ppSurface)
		delete[] m_ppSurface;

	if (m_ppDrawSurface)
		delete[] m_ppDrawSurface;

	if (m_pBMPArray)
		delete[] m_pBMPArray;
}

CBMP *CSprite::GetBMP()
{
	return m_pBMPArray;
}

bool CSprite::InitSprite(int nFrame, int nWidth, int nHeight, int nColorKey, LPDIRECTDRAW7 pDirectDraw)
{
	if (m_ppSurface)
		delete[] m_ppSurface;

	if (m_ppDrawSurface)
		delete[] m_ppDrawSurface;

	if (m_pBMPArray)
		delete[] m_pBMPArray;

	m_ppSurface = new LPDIRECTDRAWSURFACE7[nFrame];

	if (!m_ppSurface)
		return false;

	m_ppDrawSurface = new LPDIRECTDRAWSURFACE7[nFrame];

	if (!m_ppDrawSurface)
		return false;

	m_pBMPArray = new CBMP[nFrame];

	if (!m_pBMPArray)
		return false;

	DDSURFACEDESC2 ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	ddsd.dwHeight = nHeight;

	ddsd.dwWidth = nWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = nColorKey;

	for (int i = 0; i < nFrame; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_ppSurface[i], NULL)))
		{
			m_ppSurface[i] = NULL;
			return false;
		}
		else
		{
			m_ppSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}

		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_ppDrawSurface[i], NULL)))
		{
			m_ppDrawSurface[i] = NULL;
			return false;
		}
		else
		{
			m_ppDrawSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nFrame = nFrame;
	m_nSize = 1;
	number = 0;

	draw_Height = m_nHeight*m_nSize;
	draw_Width = m_nWidth*m_nSize;

	/*if ((draw_Height % 2) == 0)
		IsDrawHeightEven = true;
	else
		IsDrawHeightEven = false;

	if ((draw_Width % 2) == 0)
		IsDrawWidthEven = true;
	else
		IsDrawWidthEven = false;*/

	return true;
}

bool CSprite::LoadFrame(int nFrame, char* filename)
{
	if (!m_pBMPArray[nFrame].LoadBMPFile(filename))
		return false;
	if (!m_pBMPArray[nFrame].CopyBufferToSurface(m_ppSurface[nFrame]))
		return false;
	if (!m_pBMPArray[nFrame].CopyBufferToSurface(m_ppDrawSurface[nFrame]))
		return false;
	return true;
}

bool CSprite::Drawing(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey)
{
	RECT destRect;					//출력 영역 설정 변수
	RECT destRect2;					//원본 이미지 영역 설정 변수

	//destRect.left = x - (m_nWidth >> 1) - (m_nWidth * (camera.GetExpansion() - 1));			//확대버전
	//destRect.top = y - (m_nHeight >> 1) - (m_nHeight * (camera.GetExpansion() - 1));
	//destRect.right = destRect.left + m_nWidth + 2*(m_nWidth * (camera.GetExpansion() - 1));
	//destRect.bottom = destRect.top + m_nHeight + 2*(m_nHeight * (camera.GetExpansion() - 1));

	/*draw_Height = m_nHeight*m_nSize;
	draw_Width = m_nWidth*m_nSize;*/

	/*if ((draw_Height % 2) == 0)
		IsDrawHeightEven = true;
	else
		IsDrawHeightEven = false;
	if ((draw_Width % 2) == 0)
		IsDrawWidthEven = true;
	else
		IsDrawWidthEven = false;*/

	destRect.left = x - (draw_Width >> 1);
	destRect.top = y - (draw_Height >> 1);
	//destRect.left = x - (draw_Width >> 1);
	//destRect.top = y - (draw_Height >> 1);
	destRect.right = destRect.left + draw_Width;
	destRect.bottom = destRect.top + draw_Height;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = m_nWidth;
	destRect2.bottom = m_nHeight;

	if (destRect.top < 0)
	{
		destRect2.top = -destRect.top/m_nSize;
		destRect.top = 0;
		if (destRect2.top > m_nHeight)
			return true;
	}
	if (destRect.left < 0)
	{
		destRect2.left = -destRect.left / m_nSize;
		destRect.left = 0;
		if (destRect2.left > m_nWidth)
			return true;
	}
	if (destRect.right > SCREEN_WIDTH)
	{
		destRect2.right = m_nWidth - (destRect.right - SCREEN_WIDTH) / m_nSize;
		destRect.right = SCREEN_WIDTH;
		if (destRect2.right < 0)
			return true;
	}
	if (destRect.bottom > SCREEN_HEIGHT)
	{
		destRect2.bottom = m_nHeight - (destRect.bottom - SCREEN_HEIGHT) / m_nSize;
		destRect.bottom = SCREEN_HEIGHT;
		if (destRect2.bottom < 0)
			return true;
	}

	HRESULT	hResult;
	if (bUsingColorKey)
	{

		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppDrawSurface[nFrame], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppDrawSurface[nFrame], &destRect2, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
	}

	return true;

}

bool CSprite::Drawing(RECT rect, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey)
{
	RECT destRect2;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = m_nWidth;
	destRect2.bottom = m_nHeight;

	float size = (rect.right - rect.left) / destRect2.right;

	if (rect.top < 0)
	{
		destRect2.top = -rect.top / size;
		rect.top = 0;
		if (destRect2.top > m_nHeight)
			return true;
	}
	if (rect.left < 0)
	{
		destRect2.left = -rect.left / size;
		rect.left = 0;
		if (destRect2.left > m_nWidth)
			return true;
	}
	if (rect.right > SCREEN_WIDTH)
	{
		destRect2.right = m_nWidth - (rect.right - SCREEN_WIDTH) / size;
		rect.right = SCREEN_WIDTH;
		if (destRect2.right < 0)
			return true;
	}
	if (rect.bottom > SCREEN_HEIGHT)
	{
		destRect2.bottom = m_nHeight - (rect.bottom - SCREEN_HEIGHT) / size;
		rect.bottom = SCREEN_HEIGHT;
		if (destRect2.bottom < 0)
			return true;
	}

	HRESULT	hResult;
	if (bUsingColorKey)
	{

		if (FAILED(hResult = pSurface->Blt(&rect, m_ppDrawSurface[0], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&rect, m_ppDrawSurface[0], &destRect2, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
	}

	return true;

}

bool CSprite::DrawingBossHp(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey)
{
	RECT destRect;					//출력 영역 설정 변수
	RECT destRect2;					//원본 이미지 영역 설정 변수

	destRect.left = x - (draw_Width >> 1);
	destRect.top = y - (draw_Height >> 1) + (draw_Height * ((1000 - boss.m_nHp) * 0.001f));		// 보스 체력 깍인 만큼 체력 부분 줄여줌
	destRect.right = destRect.left + draw_Width;
	destRect.bottom = y - (draw_Height >> 1) + draw_Height;

	destRect2.left = 0;
	destRect2.top = m_nHeight * ((1000 - boss.m_nHp) * 0.001f);
	destRect2.right = m_nWidth;
	destRect2.bottom = m_nHeight;

	HRESULT	hResult;
	if (bUsingColorKey)
	{

		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppDrawSurface[nFrame], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppDrawSurface[nFrame], &destRect2, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
	}

	return true;

}

bool CSprite::Restore()
{
	if (!m_ppSurface)
		return false;
	if (!m_ppDrawSurface)
		return false;

	for (int i = 0; i < m_nFrame; i++)
	{
		if (!m_ppSurface[i])
			return false;

		if (!m_ppDrawSurface[i])
			return false;

		if (FAILED(m_ppSurface[i]->Restore()))
			return false;

		if (FAILED(m_ppDrawSurface[i]->Restore()))
			return false;

		if (!m_pBMPArray[i].CopyBufferToSurface(m_ppSurface[i]))
			return false;

		if (!m_pBMPArray[i].CopyBufferToSurface(m_ppDrawSurface[i]))
			return false;
	}
	return true;

}

int CSprite::GetNumberOfFrame()
{
	return m_nFrame;
}

void CSprite::SetSize(float _size)
{
	m_nSize = _size;

	draw_Height = m_nHeight*m_nSize;
	draw_Width = m_nWidth*m_nSize;

	/*if ((draw_Height % 2) == 0)
		IsDrawHeightEven = true;
	else
		IsDrawHeightEven = false;
	if ((draw_Width % 2) == 0)
		IsDrawWidthEven = true;
	else
		IsDrawWidthEven = false;*/
}

void CSprite::SetNumber(int _number)
{
	number = _number;
}

float CSprite::GetSize()
{
	return m_nSize;
}

int CSprite::GetNumber()
{
	return number;
}

int CSprite::GetWidth()
{
	return m_nWidth;
}

int CSprite::GetHeight()
{
	return m_nHeight;
}

bool CSprite::ReleaseAll()
{
	if (!m_ppSurface)
		return false;

	if (!m_ppDrawSurface)
		return false;

	for (int i = 0; i < m_nFrame; i++)
	{
		if (m_ppSurface[i])
			m_ppSurface[i]->Release();

		if (m_ppDrawSurface[i])
			m_ppDrawSurface[i]->Release();
	}

	return true;

}

bool CSprite::Drawing2(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey)
{
	if (destRect.top == 1200)
	{
		destRect.top = 0;
		destRect.bottom = 1200;

		destRect2.top = -1200;
		destRect2.bottom = 0;
	}

	destRect.top++;
	destRect.bottom++;

	destRect2.top++;
	destRect2.bottom++;

	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppDrawSurface[nFrame], NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect2, m_ppDrawSurface[nFrame], NULL, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;

		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppDrawSurface[nFrame], NULL, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
	}
	return true;
}

bool CSprite::BlockDrawing(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey)
{
	RECT destRect;					//출력 영역 설정 변수
	RECT destRect2;					//원본 이미지 영역 설정 변수

	destRect.left = x;
	destRect.top = y;
	destRect.right = destRect.left + draw_Width;
	destRect.bottom = destRect.top + draw_Height;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = m_nWidth;
	destRect2.bottom = m_nHeight;

	if (destRect.top < 0)
	{
		destRect2.top = -destRect.top / m_nSize;
		destRect.top = 0;
		if (destRect2.top > m_nHeight)
			return true;
	}
	if (destRect.left < 0)
	{
		destRect2.left = -destRect.left / m_nSize;
		destRect.left = 0;
		if (destRect2.left > m_nWidth)
			return true;
	}
	if (destRect.right > SCREEN_WIDTH)
	{
		destRect2.right = m_nWidth - (destRect.right - SCREEN_WIDTH) / m_nSize;
		destRect.right = SCREEN_WIDTH;
		if (destRect2.right < 0)
			return true;
	}
	if (destRect.bottom > SCREEN_HEIGHT)
	{
		destRect2.bottom = m_nHeight - (destRect.bottom - SCREEN_HEIGHT) / m_nSize;
		destRect.bottom = SCREEN_HEIGHT;
		if (destRect2.bottom < 0)
			return true;
	}

	HRESULT	hResult;
	if (bUsingColorKey)
	{

		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppDrawSurface[0], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppDrawSurface[0], &destRect2, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
	}

	return true;
}

void CSprite::Rotate(double Degree, int _curFrame)
{

	int x, y;

	int orig_x, orig_y;

	unsigned char* pixel = new unsigned char[3];

	double radian = Degree * pi / 180.0; // (1)		

	double cc = cos(radian), ss = sin(-radian);

	double xcenter = (double)m_nWidth / 2.0, ycenter = (double)m_nHeight / 2.0; // (2)


	unsigned char* buffer = new unsigned char[m_nHeight * m_nWidth * 4];
	ZeroMemory(buffer, m_nHeight * m_nWidth * 4);

	for (y = 0; y < m_nHeight; y++)
	{
		for (x = 0; x < m_nWidth; x++)
		{
			for (int i = 0; i < 3; i++)
			{
				//if (*(m_pBuffer + (x << 2) + nDestY2 * (m_nWidth << 2)) == 0)
				//	if (*(m_pBuffer + (x << 2) + nDestY2 * (m_nWidth << 2) + 1) == 0)
				//		if (*(m_pBuffer + (x << 2) + nDestY2 * (m_nWidth << 2) + 2) == 0)
				//			continue;
				//CopyMemory(buffer2 + ((x + _x) << 2) + ((nDestY - _y) * (nWidth << 2)) - (x + _x + (nDestY - _y) * nWidth), m_pBuffer + (x << 2) + nDestY2 * (m_nWidth << 2), 3);
			}
			orig_x = (int)(xcenter + ((double)y - ycenter) * ss + ((double)x - xcenter) * cc);
			orig_y = (int)(ycenter + ((double)y - ycenter) * cc - ((double)x - xcenter) * ss);
			ZeroMemory(pixel, 3);
			if ((orig_y >= 0 && orig_y < m_nHeight) && (orig_x >= 0 && orig_x < m_nWidth))
				CopyMemory(pixel, m_pBMPArray->GetBMPBuffer() + (orig_x << 2) + orig_y * (m_nWidth << 2), 3);		//원본 픽셀 값 추출
			//char b = *(m_pBMPArray->GetBMPBuffer() + (orig_x << 2) + orig_y * (m_nWidth << 2));
			//char g = *(m_pBMPArray->GetBMPBuffer() + (orig_x << 2) + orig_y * (m_nWidth << 2)+1);
			//char r = *(m_pBMPArray->GetBMPBuffer() + (orig_x << 2) + orig_y * (m_nWidth << 2)+2);

			if((orig_y >= 0 && orig_y < m_nHeight) && (orig_x >= 0 && orig_x < m_nWidth)) // (4)
				CopyMemory(buffer + (x << 2) + y * (m_nWidth << 2), pixel, 3);
				//CopyMemory(buffer + (m_pBMPArray->GetBMPBuffer() + (orig_x << 2) + orig_y * (m_nWidth << 2)), *pixel, 3);
				//pixel = InputImage[orig_y][orig_x]; // (5)
		} // x-loop
		
	} // y-loop

	m_pBMPArray->CopyBufferToSurface(m_ppDrawSurface[_curFrame], buffer);

	delete[] pixel;
	delete[] buffer;


}