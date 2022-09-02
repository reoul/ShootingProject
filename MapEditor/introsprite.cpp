#include "IntroSprite.h"

IntroSprite::IntroSprite()
{
}

IntroSprite::~IntroSprite()
{
	free(m_pBuffer);
}

bool IntroSprite::InitSprite(int nFrame, int nWidth, int nHeight, int nColorKey, LPDIRECTDRAW7 pDirectDraw)
{
	if (m_pSurface)
		delete[] m_pSurface;

	if (m_pBmp)
		delete[] m_pBmp;

	m_pSurface = new LPDIRECTDRAWSURFACE7[nFrame];

	if (!m_pSurface)
		return false;


	m_pBmp = new Bmp[nFrame];

	if (!m_pBmp)
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
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_pSurface[i], NULL)))
		{
			m_pSurface[i] = NULL;
			return false;
		}
		else
		{
			m_pSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nFrame = nFrame;

	return true;
}

bool IntroSprite::LoadFrame(int nFrame, char* filename)
{
	if (!m_pBmp[nFrame].LoadBMPFile(filename))
		return false;
	if (!m_pBmp[nFrame].CopyBufferToSurface(m_pSurface[nFrame]))
		return false;

	return true;
}

bool IntroSprite::ReleaseAll()
{
	if (!m_pSurface)
		return false;

	for (int i = 0; i < m_nFrame; i++)
	{
		if (m_pSurface[i])
			m_pSurface[i]->Release();
	}

	return true;
}

void IntroSprite::RestoreAll(LPDIRECTDRAW7 pDirectDraw)
{
	delete[] m_pSurface;
	m_pSurface = new LPDIRECTDRAWSURFACE7[m_nFrame];

	DDSURFACEDESC2 ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	ddsd.dwHeight = m_nHeight;

	ddsd.dwWidth = m_nWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = 0;

	for (int i = 0; i < m_nFrame; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_pSurface[i], NULL)))
		{
			m_pSurface[i] = NULL;
		}
		else
		{
			m_pSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}
	for (int i = 0; i < m_nFrame; i++)
	{
		m_pBmp[i].CopyBufferToSurface(m_pSurface[i]);
	}
}

bool IntroSprite::LoadBMPFile(char* filename) //비트맵의 정보를 불러와 버퍼에 색상정보를 담아주고 24bit이미지를 32bit이미지로 바꿔줌
{
	m_nFrame = 1;
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
	                   FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	m_pSurface = new LPDIRECTDRAWSURFACE7[m_nFrame];

	BITMAPFILEHEADER bmpfilehead; //비트맵파일 자체의 정보를 저장할 변수 선언
	if (!ReadFile(hfile, &bmpfilehead, sizeof(bmpfilehead), &actualRead, NULL)) //비트맵파일 자체의 정보를 읽을수 없다면
	{
		CloseHandle(hfile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}
	if (bmpfilehead.bfType != 0x4D42) //비트맵파일이 아닐경우
	{
		CloseHandle(hfile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	BITMAPINFOHEADER bmpinfohead; //비트맵영상 자체의 정보를 저장할 변수 선언
	if (!ReadFile(hfile, &bmpinfohead, sizeof(bmpinfohead), &actualRead, NULL)) //비트맵영상 자체의 정보를 읽을수 없다면
	{
		CloseHandle(hfile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	if (bmpinfohead.biBitCount != 24) //픽셀당 비트수가 24개가 아니면 
	{
		CloseHandle(hfile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	int nHeight = bmpinfohead.biHeight; //비트맵의 높이를 불러와 nHeight에 초기화
	bool bBottomUp; //이미지가 뒤집어져있는지 확인하는 bool형 변수, true면 이미지가 뒤집어져있음

	if (nHeight > 0) //비트맵의 높이가 0보다 크면
	{
		bBottomUp = true; //이미지가 뒤집어져있음
	}
	else //비트맵의 높이가 0보다 크지않을경우
	{
		bBottomUp = false; //이미지가 정상적으로 있음
		nHeight = -nHeight; //높이를 양수로 바꿔줌
	}

	m_nHeight = nHeight;
	m_nWidth = bmpinfohead.biWidth;

	if (m_pBuffer) //버퍼가 있다면
		delete[] m_pBuffer; //버퍼를 삭제해준다

	m_pBuffer = new unsigned char[m_nHeight * m_nWidth * 4]; //새로운 버퍼를 32비트로 표현해주기위해서 총픽셀갯수*4를 해준다

	struct RBGstruct //비트맵은 RGB의 순서가 아닌 BGR순서로 저장됨
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	struct RBGstruct* rgb24 = new RBGstruct[m_nWidth]; //픽셀색상 정보 담을 변수 선언

	int nStoredLine = (m_nWidth * 3 + 3) & ~3;
	unsigned char temp[4];
	int nRemainder = nStoredLine - (m_nWidth * 3);

	int nDestY, nDeltaY; //현재 높이,이미지가 뒤집힘 상태에따라 다음라인으로 바꿀때 높이에 더해주는값

	if (bBottomUp) //이미지가 뒤집어져있으면
	{
		nDestY = nHeight - 1;
		nDeltaY = -1;
	}
	else
	{
		nDestY = 0;
		nDeltaY = 1;
	}

	for (int y = 0; y < m_nHeight; y++) //이미지의 높이픽셀수만큼
	{
		if (!ReadFile(hfile, rgb24, m_nWidth * sizeof(RBGstruct), &actualRead, NULL)) //파일에 3바이트 크기만큼 읽어와서 rgb24에 저장시켜줌
		{
			//파일을 못 읽어왔다면
			CloseHandle(hfile); //핸들을 반납해주고
			delete[] m_pBuffer; //버퍼를 삭제해주고
			return false; //false를 반환해준다
		}

		for (int x = 0; x < m_nWidth; x++) //이미지의 가로픽셀수만큼
		{
			CopyMemory(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2), &rgb24[x], 3);
			//각 픽셀마다 4바이트씩 나눠져있고 3바이트는 픽셀의 rgb값
			*(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2) + 3) = 0; //마지막 1바이트는 알파값을 주어서 투명도를 조절
			if ((x == 0) || (x == (m_nWidth - 1)))
				bBottomUp = true;
		}

		nDestY += nDeltaY;

		if (!ReadFile(hfile, temp, nRemainder, &actualRead, NULL))
		{
			CloseHandle(hfile);
			delete[] m_pBuffer;
			return false;
		}
	}

	CloseHandle(hfile);

	return true;
}

bool IntroSprite::Drawing(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey)
{
	RECT destRect;
	RECT destRect2;

	destRect.left = x;
	destRect.top = y;
	destRect.right = m_nWidth;
	destRect.bottom = m_nHeight;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = m_nWidth;
	destRect2.bottom = m_nHeight;

	/*if (destRect.top < 0)
	{
		destRect2.top = -destRect.top;
		destRect.top = 0;
		if (destRect2.top > m_nHeight)
			return true;
	}
	if (destRect.left < 0)
	{
		destRect2.left = -destRect.left;
		destRect.left = 0;
		if (destRect2.left > m_nWidth)
			return true;
	}
	if (destRect.right > SCREEN_WIDTH)
	{
		destRect2.right = m_nWidth - (destRect.right - SCREEN_WIDTH);
		destRect.right = SCREEN_WIDTH;
		if (destRect2.right < 0)
			return true;
	}
	if (destRect.bottom > SCREEN_HEIGHT)
	{
		destRect2.bottom = m_nHeight - (destRect.bottom - SCREEN_HEIGHT);
		destRect.bottom = SCREEN_HEIGHT;
		if (destRect2.bottom < 0)
			return true;
	}*/

	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[frame], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return false;
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[frame], &destRect2, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return false;
			else
				return false;
	}

	return true;
}

bool IntroSprite::Drawing2(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey)
{
	RECT destRect;
	RECT destRect2;

	destRect.left = x;
	destRect.top = y;
	destRect.right = destRect.left + m_nWidth;
	destRect.bottom = destRect.top + m_nHeight;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = m_nWidth;
	destRect2.bottom = m_nHeight;

	if (destRect.top < 0)
	{
		destRect2.top = -destRect.top;
		destRect.top = 0;
		if (destRect2.top > m_nHeight)
			return true;
	}
	if (destRect.left < 0)
	{
		destRect2.left = -destRect.left;
		destRect.left = 0;
		if (destRect2.left > m_nWidth)
			return true;
	}
	if (destRect.right > SCREEN_WIDTH)
	{
		destRect2.right = m_nWidth - (destRect.right - SCREEN_WIDTH);
		destRect.right = SCREEN_WIDTH;
		if (destRect2.right < 0)
			return true;
	}
	if (destRect.bottom > SCREEN_HEIGHT)
	{
		destRect2.bottom = m_nHeight - (destRect.bottom - SCREEN_HEIGHT);
		destRect.bottom = SCREEN_HEIGHT;
		if (destRect2.bottom < 0)
			return true;
	}

	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[frame], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return false;
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[frame], &destRect2, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return false;
			else
				return false;
	}

	return true;
}

bool IntroSprite::CopyBufferToSurface(LPDIRECTDRAW7 pDirectDraw) //버퍼에서 표면으로 복사하는 함수
{
	if (!m_pBuffer) //버퍼가 없으면
		return false; //false를 반환

	DDSURFACEDESC2 ddsd; //표면의 정보를 확인할수 있는 변수를 선언
	ZeroMemory(&ddsd, sizeof(ddsd)); //ddsd주소에서 ddsd크기만큼 메모리영역을 0으로 채워줌
	ddsd.dwSize = sizeof(ddsd); //dwSize를 ddsd크기만큼 초기화 해준다

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	ddsd.dwHeight = m_nHeight;

	ddsd.dwWidth = m_nWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = 0;

	for (int i = 0; i < 1; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_pSurface[i], NULL)))
		{
			m_pSurface[i] = NULL;
			return false;
		}
		else
		{
			m_pSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}

	if (FAILED(m_pSurface[0]->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) //표면에 접근하여 읽거나 쓰는 작업을 하기 위해서 표면은 잠근다
		return false; //만약 표면을 잠금하는데 실패할경우 false를 반환해준다

	unsigned char *pDest, *pSrc; //표면의주소를 담을수 있는 변수, 버퍼의 주소를 담을수 있는 변수

	pDest = (unsigned char*)ddsd.lpSurface; //표면의 주소값을 불러옴
	pSrc = m_pBuffer; //버퍼의 주소를 불러옴

	for (int y = 0; y < m_nHeight; y++) //높이만큼 for문을 돌린다
	{
		CopyMemory(pDest, pSrc, m_nWidth << 2); //표면에다가 pSrc에서 가로길이 * 4한 크기만큼 메모리를 복사한다
		pDest += ddsd.lPitch; //pDest 시작주소를 다음라인시작주소로 바꾼다
		pSrc += (m_nWidth << 2); //pSrc 시작주소를 다음라인시작주소로 바꾼다
	}

	m_pSurface[0]->Unlock(NULL); //잠금을 해체해준다

	return true;
}


//void IntroSprite::KeyDown()
//{
//	buttondown.Down();
//}
//
//void IntroSprite::KeyDownInit()
//{
//	buttondown.InitDown();
//}
//
//void IntroSprite::KeyDownCheck()
//{
//	buttondown.CheckUp();
//}
//
//bool IntroSprite::CheckDown()
//{
//	return buttondown.CheckClick();
//}
