#include "WorldMapBmp.h"
#include "Camera.h"
#include "MapEditor.h"
#include "GameEnum.h"

extern Camera camera;
extern MapEditor g_editor;
extern EEditState curEditState;

extern char* wallBlockData[BLOCK_X * BLOCK_Y * 2];
extern char* wallBlockData2[BLOCK_X * BLOCK_Y * 2];

extern EMod curMod; //현재 모드

WorldMap::WorldMap()
	: mWidth(0)
	, mHeight(0)
	, mBuffer(nullptr)
	, mBuffer2(nullptr)
{
	//mBuffer = new unsigned char[1600 * 1600 * 3];
	mSurface = make_unique<LPDIRECTDRAWSURFACE7>();
}

bool WorldMap::LoadBMPFile(const char* filePath) //비트맵의 정보를 불러와 버퍼에 색상정보를 담아주고 24bit이미지를 32bit이미지로 바꿔줌
{
	DWORD actualRead;
	HANDLE hFile = CreateFile(TEXT(filePath), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	BITMAPFILEHEADER bmpFileHead; //비트맵파일 자체의 정보를 저장할 변수 선언
	if (!ReadFile(hFile, &bmpFileHead, sizeof(bmpFileHead), &actualRead, nullptr)) //비트맵파일 자체의 정보를 읽을수 없다면
	{
		CloseHandle(hFile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}
	if (bmpFileHead.bfType != 0x4D42) //비트맵파일이 아닐경우
	{
		CloseHandle(hFile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	BITMAPINFOHEADER bmpInfoHead; //비트맵영상 자체의 정보를 저장할 변수 선언
	if (!ReadFile(hFile, &bmpInfoHead, sizeof(bmpInfoHead), &actualRead, nullptr)) //비트맵영상 자체의 정보를 읽을수 없다면
	{
		CloseHandle(hFile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	if (bmpInfoHead.biBitCount != 24) //픽셀당 비트수가 24개가 아니면 
	{
		CloseHandle(hFile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	int nHeight = bmpInfoHead.biHeight; //비트맵의 높이를 불러와 nHeight에 초기화
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

	mHeight = nHeight;
	mWidth = bmpInfoHead.biWidth;

	mBuffer = make_unique<unsigned char[]>(mHeight * mWidth * 4); //새로운 버퍼를 32비트로 표현해주기위해서 총픽셀갯수*4를 해준다
	mBuffer2 = make_unique<unsigned char[]>(mHeight * mWidth * 3);

	struct RBGStruct //비트맵은 RGB의 순서가 아닌 BGR순서로 저장됨
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	unique_ptr<RBGStruct[]> rgb24 = make_unique<RBGStruct[]>(mWidth); //픽셀색상 정보 담을 변수 선언
	int nStoredLine = (mWidth * 3 + 3) & ~3;
	unsigned char temp[4];
	int nRemainder = nStoredLine - (mWidth * 3);

	int nDestY, nDeltaY; //현재 높이,이미지가 뒤집힘 상태에따라 다음라인으로 바꿀때 높이에 더해주는값

	nDestY = 0;
	nDeltaY = 1;

	for (int y = 0; y < mHeight; y++) //이미지의 높이픽셀수만큼
	{
		if (!ReadFile(hFile, rgb24.get(), 3 * mWidth, &actualRead, nullptr)) //파일에 3바이트 크기만큼 읽어와서 rgb24에 저장시켜줌
		{
			//파일을 못 읽어왔다면
			CloseHandle(hFile); //핸들을 반납해주고
			mBuffer = nullptr; //버퍼를 삭제해주고
			return false; //false를 반환해준다
		}
		CopyMemory(mBuffer2.get() + nDestY * (mWidth * 3), rgb24.get(), 3 * mWidth);
		for (int x = 0; x < mWidth; x++) //이미지의 가로픽셀수만큼
		{
			CopyMemory(mBuffer.get() + (x << 2) + nDestY * (mWidth << 2), rgb24.get() + x, 3);
			*(mBuffer.get() + (x << 2) + nDestY * (mWidth << 2) + 3) = 0;
		}

		nDestY += nDeltaY;
		if (!ReadFile(hFile, temp, nRemainder, &actualRead, nullptr))
		{
			CloseHandle(hFile);
			mBuffer = nullptr;
			return false;
		}
	}

	rgb24 = nullptr;

	CloseHandle(hFile);

	return true;
}

bool WorldMap::ReleaseAll() const
{
	if (!mSurface)
		return false;

	(*mSurface)->Release();
	return true;
}

void WorldMap::Restore() const
{
	(*mSurface)->Restore();

	DDSURFACEDESC2 ddsd; //표면의 정보를 확인할수 있는 변수를 선언
	ZeroMemory(&ddsd, sizeof(ddsd)); //ddsd주소에서 ddsd크기만큼 메모리영역을 0으로 채워줌
	ddsd.dwSize = sizeof(ddsd); //dwSize를 ddsd크기만큼 초기화 해준다

	(*mSurface)->Lock(nullptr, &ddsd, DDLOCK_WAIT, nullptr); //만약 표면을 잠금하는데 실패할경우 false를 반환해준다

	unsigned char* pDest, * pSrc; //표면의주소를 담을수 있는 변수, 버퍼의 주소를 담을수 있는 변수

	pDest = static_cast<unsigned char*>(ddsd.lpSurface); //표면의 주소값을 불러옴
	pSrc = mBuffer.get(); //버퍼의 주소를 불러옴

	for (int y = 0; y < mHeight; y++) //높이만큼 for문을 돌린다
	{
		CopyMemory(pDest, pSrc, mWidth << 2); //표면에다가 pSrc에서 가로길이 * 4한 크기만큼 메모리를 복사한다
		pDest += ddsd.lPitch; //pDest 시작주소를 다음라인시작주소로 바꾼다
		pSrc += (mWidth << 2); //pSrc 시작주소를 다음라인시작주소로 바꾼다
	}

	(*mSurface)->Unlock(nullptr);
}

bool WorldMap::Drawing(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey, LPDIRECTDRAW7 pDirectDraw) const
{
	RECT destRect;
	RECT destRect2;

	destRect.left = x;
	destRect.top = y;
	destRect.right = destRect.left + mWidth;
	destRect.bottom = destRect.top + mHeight;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = mWidth;
	destRect2.bottom = mHeight;

	if (destRect.top < 0)
	{
		destRect2.top = -destRect.top;
		destRect.top = 0;
		if (destRect2.top > mHeight)
			return true;
	}
	if (destRect.left < 0)
	{
		destRect2.left = -destRect.left;
		destRect.left = 0;
		if (destRect2.left > mWidth)
			return true;
	}
	if (destRect.right > SCREEN_WIDTH)
	{
		destRect2.right = mWidth - (destRect.right - SCREEN_WIDTH);
		destRect.right = SCREEN_WIDTH;
		if (destRect2.right < 0)
			return true;
	}
	if (destRect.bottom > SCREEN_HEIGHT)
	{
		destRect2.bottom = mHeight - (destRect.bottom - SCREEN_HEIGHT);
		destRect.bottom = SCREEN_HEIGHT;
		if (destRect2.bottom < 0)
			return true;
	}

	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, *mSurface, &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				Restore();
			else
				return false;
		}

	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, *mSurface, &destRect2, DDBLT_WAIT, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				Restore();
			else
				return false;
		}
	}

	return true;
}

bool WorldMap::Drawing2(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey, LPDIRECTDRAW7 pDirectDraw) const
{
	RECT destRect;
	RECT destRect2;

	destRect.left = -x + (SCREEN_WIDTH >> 1); //-1600 + screen 640
	destRect.top = -y + (SCREEN_HEIGHT >> 1); //-1600 + 360
	destRect.right = destRect.left + mWidth;
	destRect.bottom = destRect.top + mHeight;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = mWidth;
	destRect2.bottom = mHeight;

	if (destRect.top < 0)
	{
		destRect2.top = -destRect.top;
		destRect.top = 0;
	}

	if (destRect.left < 0)
	{
		destRect2.left = -destRect.left;
		destRect.left = 0;
	}

	if (destRect.right > SCREEN_WIDTH)
	{
		destRect2.right = destRect2.left + SCREEN_WIDTH;
		destRect.right = SCREEN_WIDTH;
	}

	if (destRect.bottom > SCREEN_HEIGHT)
	{
		destRect2.bottom = destRect2.top + SCREEN_HEIGHT;
		destRect.bottom = SCREEN_HEIGHT;
	}

	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, *mSurface, &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				Restore();
			else
				return false;
		}
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, *mSurface, &destRect2, DDBLT_WAIT, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				Restore();
			else
				return false;
		}
	}

	return true;
}

bool WorldMap::CopyBufferToSurface(LPDIRECTDRAW7 pDirectDraw)
//버퍼에서 표면으로 복사하는 함수
{
	if (!mBuffer) //버퍼가 없으면
		return false; //false를 반환

	DDSURFACEDESC2 ddsd; //표면의 정보를 확인할수 있는 변수를 선언
	ZeroMemory(&ddsd, sizeof(ddsd)); //ddsd주소에서 ddsd크기만큼 메모리영역을 0으로 채워줌
	ddsd.dwSize = sizeof(ddsd); //dwSize를 ddsd크기만큼 초기화 해준다
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwHeight = mHeight;
	ddsd.dwWidth = mWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = 0;

	for (int i = 0; i < 1; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, mSurface.get(), NULL)))
		{
			mSurface = nullptr;
			return false;
		}
		else
		{
			(*mSurface)->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}

	if (FAILED((*mSurface)->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) //표면에 접근하여 읽거나 쓰는 작업을 하기 위해서 표면은 잠근다
		return false; //만약 표면을 잠금하는데 실패할경우 false를 반환해준다

	unsigned char* pDest, * pSrc; //표면의주소를 담을수 있는 변수, 버퍼의 주소를 담을수 있는 변수

	pDest = static_cast<unsigned char*>(ddsd.lpSurface); //표면의 주소값을 불러옴
	pSrc = mBuffer.get(); //버퍼의 주소를 불러옴

	for (int y = 0; y < mHeight; y++) //높이만큼 for문을 돌린다
	{
		CopyMemory(pDest, pSrc, mWidth << 2); //표면에다가 pSrc에서 가로길이 * 4한 크기만큼 메모리를 복사한다
		pDest += ddsd.lPitch; //pDest 시작주소를 다음라인시작주소로 바꾼다
		pSrc += (mWidth << 2); //pSrc 시작주소를 다음라인시작주소로 바꾼다
	}

	(*mSurface)->Unlock(nullptr); //잠금을 해체해준다

	return true;
}

bool WorldMap::CopyBufferToSurface2(LPDIRECTDRAW7 pDirectDraw)
//버퍼에서 표면으로 복사하는 함수
{
	const char* filePath = "image\\map\\bossmap.bmp";
	HANDLE hFile;
	DWORD actualRead;
	hFile = CreateFile(TEXT(filePath), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, (HANDLE)nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	BITMAPFILEHEADER bmpFileHead; //비트맵파일 자체의 정보를 저장할 변수 선언
	if (!ReadFile(hFile, &bmpFileHead, sizeof(bmpFileHead), &actualRead, nullptr)) //비트맵파일 자체의 정보를 읽을수 없다면
	{
		CloseHandle(hFile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}
	if (bmpFileHead.bfType != 0x4D42) //비트맵파일이 아닐경우
	{
		CloseHandle(hFile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	BITMAPINFOHEADER bmpInfoHead; //비트맵영상 자체의 정보를 저장할 변수 선언
	if (!ReadFile(hFile, &bmpInfoHead, sizeof(bmpInfoHead), &actualRead, nullptr)) //비트맵영상 자체의 정보를 읽을수 없다면
	{
		CloseHandle(hFile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	if (bmpInfoHead.biBitCount != 24) //픽셀당 비트수가 24개가 아니면 
	{
		CloseHandle(hFile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	BITMAPFILEHEADER bmpFileHead2 = bmpFileHead;
	BITMAPINFOHEADER bmpInfoHead2 = bmpInfoHead;

	CloseHandle(hFile);

	DWORD dwWritten;
	HANDLE hfile2;
	hfile2 = CreateFile(TEXT(filePath), GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr); //파일 생성

	WriteFile(hfile2, &bmpFileHead2, sizeof(bmpFileHead2), &dwWritten, nullptr); //파일에 데이터를 기록
	WriteFile(hfile2, &bmpInfoHead2, sizeof(bmpInfoHead2), &dwWritten, nullptr);
	WriteFile(hfile2, mBuffer2.get(), WORLDMAP_DATA_SIZE, &dwWritten, nullptr); //7680000
	CloseHandle(hfile2);

	if (!mBuffer) //버퍼가 없으면
		return false; //false를 반환

	DDSURFACEDESC2 ddsd; //표면의 정보를 확인할수 있는 변수를 선언
	ZeroMemory(&ddsd, sizeof(ddsd)); //ddsd주소에서 ddsd크기만큼 메모리영역을 0으로 채워줌
	ddsd.dwSize = sizeof(ddsd); //dwSize를 ddsd크기만큼 초기화 해준다
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwHeight = mHeight;
	ddsd.dwWidth = mWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = 0;

	for (int i = 0; i < 1; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, mSurface.get(), NULL)))
		{
			mSurface = nullptr;
			return false;
		}
		else
		{
			(*mSurface)->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}

	if (FAILED((*mSurface)->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) //표면에 접근하여 읽거나 쓰는 작업을 하기 위해서 표면은 잠근다
		return false; //만약 표면을 잠금하는데 실패할경우 false를 반환해준다

	int nDestY, nDeltaY, nDestY2, nDeltaY2; //현재 높이,이미지가 뒤집힘 상태에따라 다음라인으로 바꿀때 높이에 더해주는값
	nDestY = mHeight - 1;
	nDeltaY = -1;
	nDestY2 = 0;
	nDeltaY2 = 1;

	for (int y = 0; y < mHeight; y++) //이미지의 높이픽셀수만큼
	{
		for (int x = 0; x < mWidth; x++) //이미지의 가로픽셀수만큼
		{
			CopyMemory(mBuffer.get() + (x << 2) + nDestY * (mWidth << 2),
				mBuffer2.get() + (x << 2) + nDestY2 * (mWidth << 2) - (x + nDestY2 * mWidth), 3);
		}

		nDestY += nDeltaY;
		nDestY2 += nDeltaY2;
	}

	unsigned char* pDest, * pSrc; //표면의주소를 담을수 있는 변수, 버퍼의 주소를 담을수 있는 변수

	pDest = static_cast<unsigned char*>(ddsd.lpSurface); //표면의 주소값을 불러옴
	pSrc = mBuffer.get(); //버퍼의 주소를 불러옴

	for (int y = 0; y < mHeight; y++) //높이만큼 for문을 돌린다
	{
		CopyMemory(pDest, pSrc, mWidth << 2); //표면에다가 pSrc에서 가로길이 * 4한 크기만큼 메모리를 복사한다
		pDest += ddsd.lPitch; //pDest 시작주소를 다음라인시작주소로 바꾼다
		pSrc += (mWidth << 2); //pSrc 시작주소를 다음라인시작주소로 바꾼다
	}

	(*mSurface)->Unlock(nullptr); //잠금을 해체해준다

	return true;
}

bool WorldMap::CopyBufferToSurface3(LPDIRECTDRAW7 pDirectDraw)
//버퍼에서 표면으로 복사하는 함수
{
	const char* filePath = "image\\map\\bossmapRoof.bmp";
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filePath), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, (HANDLE)nullptr);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	BITMAPFILEHEADER bmpFileHead; //비트맵파일 자체의 정보를 저장할 변수 선언
	if (!ReadFile(hfile, &bmpFileHead, sizeof(bmpFileHead), &actualRead, nullptr)) //비트맵파일 자체의 정보를 읽을수 없다면
	{
		CloseHandle(hfile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}
	if (bmpFileHead.bfType != 0x4D42) //비트맵파일이 아닐경우
	{
		CloseHandle(hfile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	BITMAPINFOHEADER bmpInfoHead; //비트맵영상 자체의 정보를 저장할 변수 선언
	if (!ReadFile(hfile, &bmpInfoHead, sizeof(bmpInfoHead), &actualRead, nullptr)) //비트맵영상 자체의 정보를 읽을수 없다면
	{
		CloseHandle(hfile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	if (bmpInfoHead.biBitCount != 24) //픽셀당 비트수가 24개가 아니면 
	{
		CloseHandle(hfile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	CloseHandle(hfile);

	if (!mBuffer) //버퍼가 없으면
		return false; //false를 반환

	DDSURFACEDESC2 ddsd; //표면의 정보를 확인할수 있는 변수를 선언
	ZeroMemory(&ddsd, sizeof(ddsd)); //ddsd주소에서 ddsd크기만큼 메모리영역을 0으로 채워줌
	ddsd.dwSize = sizeof(ddsd); //dwSize를 ddsd크기만큼 초기화 해준다
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwHeight = mHeight;
	ddsd.dwWidth = mWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = 0;

	for (int i = 0; i < 1; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, mSurface.get(), NULL)))
		{
			mSurface = nullptr;
			return false;
		}
		else
		{
			(*mSurface)->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}

	if (FAILED((*mSurface)->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) //표면에 접근하여 읽거나 쓰는 작업을 하기 위해서 표면은 잠근다
		return false; //만약 표면을 잠금하는데 실패할경우 false를 반환해준다

	int nDestY, nDeltaY, nDestY2, nDeltaY2; //현재 높이,이미지가 뒤집힘 상태에따라 다음라인으로 바꿀때 높이에 더해주는값
	nDestY = mHeight - 1;
	nDeltaY = -1;
	nDestY2 = 0;
	nDeltaY2 = 1;

	for (int y = 0; y < mHeight; y++) //이미지의 높이픽셀수만큼
	{
		for (int x = 0; x < mWidth; x++) //이미지의 가로픽셀수만큼
		{
			CopyMemory(mBuffer.get() + (x << 2) + nDestY * (mWidth << 2),
				mBuffer2.get() + (x << 2) + nDestY2 * (mWidth << 2) - (x + nDestY2 * mWidth), 3);
		}

		nDestY += nDeltaY;
		nDestY2 += nDeltaY2;
	}

	unsigned char* pDest, * pSrc; //표면의주소를 담을수 있는 변수, 버퍼의 주소를 담을수 있는 변수

	pDest = static_cast<unsigned char*>(ddsd.lpSurface); //표면의 주소값을 불러옴
	pSrc = mBuffer.get(); //버퍼의 주소를 불러옴

	for (int y = 0; y < mHeight; y++) //높이만큼 for문을 돌린다
	{
		CopyMemory(pDest, pSrc, mWidth << 2); //표면에다가 pSrc에서 가로길이 * 4한 크기만큼 메모리를 복사한다
		pDest += ddsd.lPitch; //pDest 시작주소를 다음라인시작주소로 바꾼다
		pSrc += (mWidth << 2); //pSrc 시작주소를 다음라인시작주소로 바꾼다
	}

	(*mSurface)->Unlock(nullptr); //잠금을 해체해준다

	return true;
}

bool WorldMap::CopyBufferToSurface4(LPDIRECTDRAW7 pDirectDraw)
{
	const char* filePath = "image\\map\\basemap.bmp";
	HANDLE hFile;
	DWORD actualRead;
	hFile = CreateFile(TEXT(filePath), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, (HANDLE)nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	BITMAPFILEHEADER bmpFileHead; //비트맵파일 자체의 정보를 저장할 변수 선언
	if (!ReadFile(hFile, &bmpFileHead, sizeof(bmpFileHead), &actualRead, nullptr)) //비트맵파일 자체의 정보를 읽을수 없다면
	{
		CloseHandle(hFile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}
	if (bmpFileHead.bfType != 0x4D42) //비트맵파일이 아닐경우
	{
		CloseHandle(hFile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	BITMAPINFOHEADER bmpinfohead; //비트맵영상 자체의 정보를 저장할 변수 선언
	if (!ReadFile(hFile, &bmpinfohead, sizeof(bmpinfohead), &actualRead, nullptr)) //비트맵영상 자체의 정보를 읽을수 없다면
	{
		CloseHandle(hFile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	if (bmpinfohead.biBitCount != 24) //픽셀당 비트수가 24개가 아니면 
	{
		CloseHandle(hFile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	BITMAPFILEHEADER bmpfilehead2 = bmpFileHead;
	BITMAPINFOHEADER bmpinfohead2 = bmpinfohead;

	CloseHandle(hFile);

	DWORD dwWritten;
	HANDLE hfile2;
	hfile2 = CreateFile(TEXT(filePath), GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr); //파일 생성

	WriteFile(hfile2, &bmpfilehead2, sizeof(bmpfilehead2), &dwWritten, nullptr); //파일에 데이터를 기록
	WriteFile(hfile2, &bmpinfohead2, sizeof(bmpinfohead2), &dwWritten, nullptr);
	WriteFile(hfile2, mBuffer2.get(), WORLDMAP_DATA_SIZE, &dwWritten, nullptr); //7680000
	CloseHandle(hfile2);

	if (!mBuffer) //버퍼가 없으면
		return false; //false를 반환

	DDSURFACEDESC2 ddsd; //표면의 정보를 확인할수 있는 변수를 선언
	ZeroMemory(&ddsd, sizeof(ddsd)); //ddsd주소에서 ddsd크기만큼 메모리영역을 0으로 채워줌
	ddsd.dwSize = sizeof(ddsd); //dwSize를 ddsd크기만큼 초기화 해준다
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwHeight = mHeight;
	ddsd.dwWidth = mWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = 0;

	for (int i = 0; i < 1; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, mSurface.get(), NULL)))
		{
			mSurface = nullptr;
			return false;
		}
		else
		{
			(*mSurface)->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}

	if (FAILED((*mSurface)->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) //표면에 접근하여 읽거나 쓰는 작업을 하기 위해서 표면은 잠근다
		return false; //만약 표면을 잠금하는데 실패할경우 false를 반환해준다

	int nDestY, nDeltaY, nDestY2, nDeltaY2; //현재 높이,이미지가 뒤집힘 상태에따라 다음라인으로 바꿀때 높이에 더해주는값
	nDestY = mHeight - 1;
	nDeltaY = -1;
	nDestY2 = 0;
	nDeltaY2 = 1;

	for (int y = 0; y < mHeight; y++) //이미지의 높이픽셀수만큼
	{
		for (int x = 0; x < mWidth; x++) //이미지의 가로픽셀수만큼
		{
			CopyMemory(mBuffer.get() + (x << 2) + nDestY * (mWidth << 2),
				mBuffer2.get() + (x << 2) + nDestY2 * (mWidth << 2) - (x + nDestY2 * mWidth), 3);
		}

		nDestY += nDeltaY;
		nDestY2 += nDeltaY2;
	}

	unsigned char* pDest, * pSrc; //표면의주소를 담을수 있는 변수, 버퍼의 주소를 담을수 있는 변수

	pDest = static_cast<unsigned char*>(ddsd.lpSurface); //표면의 주소값을 불러옴
	pSrc = mBuffer.get(); //버퍼의 주소를 불러옴

	for (int y = 0; y < mHeight; y++) //높이만큼 for문을 돌린다
	{
		CopyMemory(pDest, pSrc, mWidth << 2); //표면에다가 pSrc에서 가로길이 * 4한 크기만큼 메모리를 복사한다
		pDest += ddsd.lPitch; //pDest 시작주소를 다음라인시작주소로 바꾼다
		pSrc += (mWidth << 2); //pSrc 시작주소를 다음라인시작주소로 바꾼다
	}

	(*mSurface)->Unlock(nullptr); //잠금을 해체해준다

	return true;
}

void WorldMap::CopyMapData()
{
	DWORD dwWritten;
	HANDLE hFile = CreateFile("data\\blockData.txt", GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr); //파일 생성

	WriteFile(hFile, g_editor.GetBlockData(), BLOCK_X * BLOCK_Y * 2, &dwWritten, nullptr); //파일에 데이터를 기록

	CloseHandle(hFile);
}

void WorldMap::InitMapData()
{
	DWORD dwWritten;
	HANDLE hFile;

	for (int i = 0; i < BLOCK_X * BLOCK_Y; i++)
	{
		const char* zero = "10";
		memcpy(g_editor.GetBlockData() + i * 2, zero, 2);
	}

	hFile = CreateFile("data\\blockData.txt", GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr); //파일 생성

	WriteFile(hFile, g_editor.GetBlockData(), BLOCK_X * BLOCK_Y * 2, &dwWritten, nullptr); //파일에 데이터를 기록

	CloseHandle(hFile);
}
