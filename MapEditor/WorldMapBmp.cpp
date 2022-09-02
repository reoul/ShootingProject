#include "WorldMapBmp.h"
#include "Camera.h"
#include "MapEditor.h"
#include "GameEnum.h"

extern Camera camera;
extern MAPEDITOR g_editor;
extern EDIT_STATE curEditState;

extern char* wallBlockData[BLOCK_X * BLOCK_Y * 2];
extern char* wallBlockData2[BLOCK_X * BLOCK_Y * 2];

extern MOD curMod; //현재 모드

WorldMap::WorldMap()
{
	//m_pBuffer = new unsigned char[1600 * 1600 * 3];
	m_pSurface = new LPDIRECTDRAWSURFACE7[1];
}

WorldMap::~WorldMap()
{
	free(m_pBuffer);
	free(m_pBuffer2);
}

bool WorldMap::LoadBMPFile(char* filename) //비트맵의 정보를 불러와 버퍼에 색상정보를 담아주고 24bit이미지를 32bit이미지로 바꿔줌
{
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
	                   FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

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

	if (m_pBuffer2)
		delete[] m_pBuffer2;

	m_pBuffer = new unsigned char[m_nHeight * m_nWidth * 4]; //새로운 버퍼를 32비트로 표현해주기위해서 총픽셀갯수*4를 해준다
	m_pBuffer2 = new unsigned char[m_nHeight * m_nWidth * 3];

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

	//if (bBottomUp)				//이미지가 뒤집어져있으면
	//{
	//	nDestY = nHeight - 1;
	//	nDeltaY = -1;
	//}
	//else
	//{
	//	nDestY = 0;
	//	nDeltaY = 1;
	//}

	nDestY = 0;
	nDeltaY = 1;

	for (int y = 0; y < m_nHeight; y++) //이미지의 높이픽셀수만큼
	{
		if (!ReadFile(hfile, rgb24, 3 * m_nWidth, &actualRead, NULL)) //파일에 3바이트 크기만큼 읽어와서 rgb24에 저장시켜줌
		{
			//파일을 못 읽어왔다면
			CloseHandle(hfile); //핸들을 반납해주고
			delete[] m_pBuffer; //버퍼를 삭제해주고
			return false; //false를 반환해준다
		}
		CopyMemory(m_pBuffer2 + nDestY * (m_nWidth * 3), rgb24, 3 * m_nWidth);
		for (int x = 0; x < m_nWidth; x++) //이미지의 가로픽셀수만큼
		{
			//CopyMemory(m_pBuffer + (x << 2) + nDestY*(m_nWidth << 2), &rgb24[x], 3);
			CopyMemory(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2), rgb24 + x, 3);
			*(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2) + 3) = 0;
		}

		nDestY += nDeltaY;
		if (!ReadFile(hfile, temp, nRemainder, &actualRead, NULL))
		{
			CloseHandle(hfile);
			delete[] m_pBuffer;
			return false;
		}
	}

	delete[] rgb24;
	rgb24 = NULL;

	CloseHandle(hfile);

	return true;
}

unsigned char* WorldMap::GetBMPBuffer()
{
	return m_pBuffer;
}

unsigned char* WorldMap::GetBMPBuffer2()
{
	return m_pBuffer2;
}

LPDIRECTDRAWSURFACE7* WorldMap::GetSurface()
{
	return m_pSurface;
}

bool WorldMap::ReleaseAll()
{
	if (!m_pSurface)
		return false;

	m_pSurface[0]->Release();
	return true;
}

void WorldMap::Restore()
{
	m_pSurface[0]->Restore();

	DDSURFACEDESC2 ddsd; //표면의 정보를 확인할수 있는 변수를 선언
	ZeroMemory(&ddsd, sizeof(ddsd)); //ddsd주소에서 ddsd크기만큼 메모리영역을 0으로 채워줌
	ddsd.dwSize = sizeof(ddsd); //dwSize를 ddsd크기만큼 초기화 해준다

	m_pSurface[0]->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL); //만약 표면을 잠금하는데 실패할경우 false를 반환해준다

	unsigned char *pDest, *pSrc; //표면의주소를 담을수 있는 변수, 버퍼의 주소를 담을수 있는 변수

	pDest = (unsigned char*)ddsd.lpSurface; //표면의 주소값을 불러옴
	pSrc = m_pBuffer; //버퍼의 주소를 불러옴

	for (int y = 0; y < m_nHeight; y++) //높이만큼 for문을 돌린다
	{
		CopyMemory(pDest, pSrc, m_nWidth << 2); //표면에다가 pSrc에서 가로길이 * 4한 크기만큼 메모리를 복사한다
		pDest += ddsd.lPitch; //pDest 시작주소를 다음라인시작주소로 바꾼다
		pSrc += (m_nWidth << 2); //pSrc 시작주소를 다음라인시작주소로 바꾼다
	}

	m_pSurface[0]->Unlock(NULL);
}

bool WorldMap::Drawing(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey, LPDIRECTDRAW7 pDirectDraw)
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
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[0], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				Restore();
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[0], &destRect2, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				Restore();
			else
				return false;
	}

	return true;
}

bool WorldMap::Drawing2(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey, LPDIRECTDRAW7 pDirectDraw)
{
	RECT destRect;
	RECT destRect2;

	//destRect.left = x - (m_nWidth >> 1) - ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f);
	//destRect.top = y - (m_nHeight >> 1) - ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f);
	//destRect.left = x - ((m_nWidth >> 1) - ((camera.GetExpansion() - 1) *0.3f));
	//destRect.top = y - ((m_nHeight >> 1) - ((camera.GetExpansion() - 1) *0.3f));
	//destRect.right = destRect.left + m_nWidth + (2 * ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f));
	//destRect.bottom = destRect.top + m_nHeight + (2 * ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f));
	/*float tmpX = 0;
	float tmpY = 0;
	float tmpKX = camera.GetZoomInRange() * (camera.GetExpansion() - 1);
	float tmpKY = tmpKX;
	if (camera.GetDrawFinishX() < camera.GetX2())
	tmpX = -0.3;
	else if (camera.GetDrawFinishX() > camera.GetX2())
	tmpX = 0.3;
	if (camera.GetDrawFinishY() < camera.GetY2())
	tmpY = -0.3;
	else if (camera.GetDrawFinishY() > camera.GetY2())
	tmpY = 0.3;

	tmpKX *= tmpX;
	tmpKY *= tmpY;*/

	//destRect.left = x - (m_nWidth >> 1) + (tmpX * ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f)*(camera.GetExpansion() - 1));
	//destRect.top = y - (m_nHeight >> 1) + (tmpY * ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f)*0.56f*(camera.GetExpansion() - 1));
	destRect.left = -x + (SCREEN_WIDTH >> 1); //-1600 + screen 640
	destRect.top = -y + (SCREEN_HEIGHT >> 1); //-1600 + 360
	destRect.right = destRect.left + m_nWidth;
	destRect.bottom = destRect.top + m_nHeight;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = m_nWidth;
	destRect2.bottom = m_nHeight;

	if (destRect.top < 0)
	{
		//destRect2.top = -destRect.top + ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f)*0.56f + (tmpY * ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f)*0.56f*(camera.GetExpansion() - 1));
		//destRect2.top = -destRect.top + ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.6f)*0.5625f + (tmpY * (((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.6f)*0.5625f*(camera.GetExpansion() - 1)));
		//destRect2.top = -destRect.top + ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.6f)*0.5625f;
		//destRect2.top = -destRect.top + (SCREEN_HEIGHT >> 1)*(camera.GetExpansion() - 1);
		destRect2.top = -destRect.top;
		//destRect2.top = -destRect.top;
		destRect.top = 0;
		//if (destRect2.top > m_nHeight)
		//	return true;
	}

	if (destRect.left < 0)
	{
		//destRect2.left = -destRect.left + ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f) + (tmpX * ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f)*(camera.GetExpansion() - 1));
		//destRect2.left = -destRect.left + ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.6f) + (tmpX * (((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.6f)*(camera.GetExpansion() - 1)));
		//destRect2.left = -destRect.left + ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.6f);
		//destRect2.left = -destRect.left + (SCREEN_WIDTH >> 1)*(camera.GetExpansion() - 1);
		destRect2.left = -destRect.left;
		//destRect2.left = -destRect.left;
		destRect.left = 0;
		//if (destRect2.left > m_nWidth)
		//	return true;
	}

	if (destRect.right > SCREEN_WIDTH)
	{
		//destRect2.right = destRect2.left + SCREEN_WIDTH - ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f);
		//destRect2.right = m_nWidth - (destRect.right - SCREEN_WIDTH) - ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f);
		//destRect2.right = m_nWidth - (destRect.right - SCREEN_WIDTH);


		//destRect2.right = m_nWidth - (destRect.right - SCREEN_WIDTH) - (SCREEN_WIDTH >> 1)*(camera.GetExpansion() - 1);
		//destRect2.right = destRect2.left + SCREEN_WIDTH - (SCREEN_WIDTH >> 1)*(camera.GetExpansion() - 1);
		destRect2.right = destRect2.left + SCREEN_WIDTH;
		destRect.right = SCREEN_WIDTH;
		//if (destRect2.right < 0)
		//	return true;
	}
	if (destRect.bottom > SCREEN_HEIGHT)
	{
		//destRect2.bottom = destRect2.top + SCREEN_HEIGHT - ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f)*0.5625f;
		//destRect2.bottom = m_nHeight - (destRect.bottom - SCREEN_HEIGHT) - ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f)*0.5625f;
		//destRect2.bottom = m_nHeight - (destRect.bottom - SCREEN_HEIGHT);


		//destRect2.bottom = m_nHeight - (destRect.bottom - SCREEN_HEIGHT) - (SCREEN_HEIGHT >> 1)*(camera.GetExpansion() - 1);
		//destRect2.bottom = destRect2.top + SCREEN_HEIGHT - (SCREEN_HEIGHT >> 1)*(camera.GetExpansion() - 1);
		destRect2.bottom = destRect2.top + SCREEN_HEIGHT;
		destRect.bottom = SCREEN_HEIGHT;
		//if (destRect2.bottom < 0)
		//	return true;
	}

	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[0], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				Restore();
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[0], &destRect2, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				Restore();
			else
				return false;
	}

	return true;
}

bool WorldMap::CopyBufferToSurface(LPDIRECTDRAW7 pDirectDraw) //버퍼에서 표면으로 복사하는 함수
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

bool WorldMap::CopyBufferToSurface2(LPDIRECTDRAW7 pDirectDraw) //버퍼에서 표면으로 복사하는 함수
{
	char* filename = "image\\map\\bossmap.bmp";
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
	                   FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

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

	BITMAPFILEHEADER bmpfilehead2 = bmpfilehead;
	BITMAPINFOHEADER bmpinfohead2 = bmpinfohead;

	CloseHandle(hfile);

	DWORD dwWritten;
	HANDLE hfile2;
	hfile2 = CreateFile(TEXT(filename), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //파일 생성

	WriteFile(hfile2, &bmpfilehead2, sizeof(bmpfilehead2), &dwWritten, NULL); //파일에 데이터를 기록
	WriteFile(hfile2, &bmpinfohead2, sizeof(bmpinfohead2), &dwWritten, NULL);
	WriteFile(hfile2, m_pBuffer2, WORLDMAP_DATA_SIZE, &dwWritten, NULL); //7680000
	CloseHandle(hfile2);

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

	int nDestY, nDeltaY, nDestY2, nDeltaY2; //현재 높이,이미지가 뒤집힘 상태에따라 다음라인으로 바꿀때 높이에 더해주는값
	nDestY = m_nHeight - 1;
	nDeltaY = -1;
	//nDestY = 0;
	//nDeltaY = 1;

	//nDestY2 = m_nHeight - 1;
	//nDeltaY2 = -1;
	nDestY2 = 0;
	nDeltaY2 = 1;

	for (int y = 0; y < m_nHeight; y++) //이미지의 높이픽셀수만큼
	{
		for (int x = 0; x < m_nWidth; x++) //이미지의 가로픽셀수만큼
		{
			CopyMemory(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2),
			           m_pBuffer2 + (x << 2) + nDestY2 * (m_nWidth << 2) - (x + nDestY2 * m_nWidth), 3);
		}

		nDestY += nDeltaY;
		nDestY2 += nDeltaY2;
	}

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

bool WorldMap::CopyBufferToSurface3(LPDIRECTDRAW7 pDirectDraw) //버퍼에서 표면으로 복사하는 함수
{
	char* filename = "image\\map\\bossmapRoof.bmp";
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
	                   FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

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

	CloseHandle(hfile);

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

	int nDestY, nDeltaY, nDestY2, nDeltaY2; //현재 높이,이미지가 뒤집힘 상태에따라 다음라인으로 바꿀때 높이에 더해주는값
	nDestY = m_nHeight - 1;
	nDeltaY = -1;
	//nDestY = 0;
	//nDeltaY = 1;

	//nDestY2 = m_nHeight - 1;
	//nDeltaY2 = -1;
	nDestY2 = 0;
	nDeltaY2 = 1;

	for (int y = 0; y < m_nHeight; y++) //이미지의 높이픽셀수만큼
	{
		for (int x = 0; x < m_nWidth; x++) //이미지의 가로픽셀수만큼
		{
			CopyMemory(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2),
			           m_pBuffer2 + (x << 2) + nDestY2 * (m_nWidth << 2) - (x + nDestY2 * m_nWidth), 3);
		}

		nDestY += nDeltaY;
		nDestY2 += nDeltaY2;
	}

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

bool WorldMap::CopyBufferToSurface4(LPDIRECTDRAW7 pDirectDraw)
{
	char* filename = "image\\map\\basemap.bmp";
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
	                   FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

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

	BITMAPFILEHEADER bmpfilehead2 = bmpfilehead;
	BITMAPINFOHEADER bmpinfohead2 = bmpinfohead;

	CloseHandle(hfile);

	DWORD dwWritten;
	HANDLE hfile2;
	hfile2 = CreateFile(TEXT(filename), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //파일 생성

	WriteFile(hfile2, &bmpfilehead2, sizeof(bmpfilehead2), &dwWritten, NULL); //파일에 데이터를 기록
	WriteFile(hfile2, &bmpinfohead2, sizeof(bmpinfohead2), &dwWritten, NULL);
	WriteFile(hfile2, m_pBuffer2, WORLDMAP_DATA_SIZE, &dwWritten, NULL); //7680000
	CloseHandle(hfile2);

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

	int nDestY, nDeltaY, nDestY2, nDeltaY2; //현재 높이,이미지가 뒤집힘 상태에따라 다음라인으로 바꿀때 높이에 더해주는값
	nDestY = m_nHeight - 1;
	nDeltaY = -1;
	//nDestY = 0;
	//nDeltaY = 1;

	//nDestY2 = m_nHeight - 1;
	//nDeltaY2 = -1;
	nDestY2 = 0;
	nDeltaY2 = 1;

	for (int y = 0; y < m_nHeight; y++) //이미지의 높이픽셀수만큼
	{
		for (int x = 0; x < m_nWidth; x++) //이미지의 가로픽셀수만큼
		{
			CopyMemory(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2),
			           m_pBuffer2 + (x << 2) + nDestY2 * (m_nWidth << 2) - (x + nDestY2 * m_nWidth), 3);
		}

		nDestY += nDeltaY;
		nDestY2 += nDeltaY2;
	}

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

void WorldMap::CopyMapData()
{
	DWORD dwWritten;
	HANDLE hfile;
	hfile = CreateFile("data\\blockData.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //파일 생성

	WriteFile(hfile, g_editor.GetBlockData(), BLOCK_X * BLOCK_Y * 2, &dwWritten, NULL); //파일에 데이터를 기록

	CloseHandle(hfile);
}

void WorldMap::InitMapData()
{
	DWORD dwWritten;
	HANDLE hfile;

	char* zero = "10";

	for (int i = 0; i < BLOCK_X * BLOCK_Y; i++)
	{
		memcpy(g_editor.GetBlockData() + i * 2, zero, 2);
	}

	hfile = CreateFile("data\\blockData.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); //파일 생성

	WriteFile(hfile, g_editor.GetBlockData(), BLOCK_X * BLOCK_Y * 2, &dwWritten, NULL); //파일에 데이터를 기록

	CloseHandle(hfile);
}
