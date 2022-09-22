#include "Bmp.h"
#include <Windows.h>
#include <windowsx.h>

extern bool g_bIsFirst;

Bmp::Bmp()
	: mWidth(0)
	, mHeight(0)
	, mBufferPtr(nullptr)
{
}

/**
 * \brief 비트맵의 정보를 불러와 버퍼에 색상정보를 담아주고 24bit이미지를 32bit이미지로 바꿔줌
 * \param filePath 파일 경로
 * \return BMP 파일 Load 성공 여부
 */
bool Bmp::LoadBMPFile(const char* filePath)
{
	DWORD actualRead;
	HANDLE hFile = CreateFile(TEXT(filePath), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		//DWORD ab = GetLastError();
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

	mBufferPtr = std::make_unique<unsigned char[]>(mHeight * mWidth * 4);
	memset(mBufferPtr.get(), 0, sizeof(mHeight * mWidth * 4));

	struct RBGstruct //비트맵은 RGB의 순서가 아닌 BGR순서로 저장됨
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	std::unique_ptr<RBGstruct[]> rgb24 = std::make_unique<RBGstruct[]>(mWidth);

	const int nStoredLine = (mWidth * 3 + 3) & ~3;
	unsigned char temp[4];
	const int nRemainder = nStoredLine - (mWidth * 3);

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

	for (int y = 0; y < nHeight; y++) //이미지의 높이픽셀수만큼
	{
		if (!ReadFile(hFile, rgb24.get(), 3 * mWidth, &actualRead, nullptr)) //파일에 3바이트 크기만큼 읽어와서 rgb24에 저장시켜줌
		{
			//파일을 못 읽어왔다면
			CloseHandle(hFile); //핸들을 반납해주고
			mBufferPtr = nullptr; //버퍼를 삭제해주고
			return false; //false를 반환해준다
		}
		for (int x = 0; x < mWidth; x++) //이미지의 가로픽셀수만큼
		{
			CopyMemory(mBufferPtr.get() + (x << 2) + nDestY * (mWidth << 2), rgb24.get() + x, 3); //각 픽셀마다 4바이트씩 나눠져있고 3바이트는 픽셀의 rgb값
			//*(mBufferPtr + (x << 2) + nDestY * (mWidth << 2) + 3) = 0; //마지막 1바이트는 알파값을 주어서 투명도를 조절
		}

		nDestY += nDeltaY; //현재라인을 다음라인으로 바꿔줌

		if (!ReadFile(hFile, temp, nRemainder, &actualRead, nullptr))
		{
			CloseHandle(hFile);
			mBufferPtr = nullptr;
			return false;
		}
	}

	CloseHandle(hFile);

	return true;
}

/**
 * \brief 버퍼에서 표면으로 복사하는 함수
 * \param lpSurface 복사되어지는 표면
 * \return 복사 성공 여부
 */
bool Bmp::CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface) const
{
	if (mBufferPtr == nullptr) //버퍼가 없으면
		return false; //false를 반환

	DDSURFACEDESC2 ddsd; //표면의 정보를 확인할수 있는 변수를 선언
	ZeroMemory(&ddsd, sizeof(ddsd)); //ddsd주소에서 ddsd크기만큼 메모리영역을 0으로 채워줌
	ddsd.dwSize = sizeof(ddsd); //dwSize를 ddsd크기만큼 초기화 해준다

	if (FAILED(lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) //표면에 접근하여 읽거나 쓰는 작업을 하기 위해서 표면은 잠근다
		return false; //만약 표면을 잠금하는데 실패할경우 false를 반환해준다

	unsigned char* pDest, * pSrc; //표면의주소를 담을수 있는 변수, 버퍼의 주소를 담을수 있는 변수

	pDest = (unsigned char*)ddsd.lpSurface; //표면의 주소값을 불러옴
	pSrc = mBufferPtr.get(); //버퍼의 주소를 불러옴

	for (int y = 0; y < mHeight; y++) //높이만큼 for문을 돌린다
	{
		CopyMemory(pDest, pSrc, mWidth << 2); //표면에다가 pSrc에서 가로길이 * 4한 크기만큼 메모리를 복사한다
		pDest += ddsd.lPitch; //pDest 시작주소를 다음라인시작주소로 바꾼다
		pSrc += mWidth << 2; //pSrc 시작주소를 다음라인시작주소로 바꾼다
	}

	lpSurface->Unlock(nullptr); //잠금을 해체해준다

	return true;
}

/**
 * \brief 버퍼에서 표면으로 복사하는 함수
 * \param lpSurface 복사되어지는 표면
 * \param buffer 원본 버퍼
 * \return Surface에 복사 성공 여부
 */
bool Bmp::CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface, unsigned char* buffer) const
{
	if (buffer == nullptr) //버퍼가 없으면
		return false; //false를 반환

	DDSURFACEDESC2 ddsd; //표면의 정보를 확인할수 있는 변수를 선언
	ZeroMemory(&ddsd, sizeof(ddsd)); //ddsd주소에서 ddsd크기만큼 메모리영역을 0으로 채워줌
	ddsd.dwSize = sizeof(ddsd); //dwSize를 ddsd크기만큼 초기화 해준다

	if (FAILED(lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) //표면에 접근하여 읽거나 쓰는 작업을 하기 위해서 표면은 잠근다
		return false; //만약 표면을 잠금하는데 실패할경우 false를 반환해준다

	unsigned char* pDest = static_cast<unsigned char*>(ddsd.lpSurface); //표면의주소를 담을수 있는 변수
	unsigned char* pSrc = buffer;

	for (int y = 0; y < mHeight; y++) //높이만큼 for문을 돌린다
	{
		CopyMemory(pDest, pSrc, mWidth << 2); //표면에다가 pSrc에서 가로길이 * 4한 크기만큼 메모리를 복사한다
		pDest += ddsd.lPitch; //pDest 시작주소를 다음라인시작주소로 바꾼다
		pSrc += mWidth << 2; //pSrc 시작주소를 다음라인시작주소로 바꾼다
	}

	lpSurface->Unlock(nullptr); //잠금을 해체해준다

	return true;
}

// todo : 사용안하면 삭제
bool Bmp::SaveWorldBMP(int _x, int _y, unsigned char* buffer, unsigned char* buffer2) const
{
	const char* filename = "image\\map\\bossmap.bmp";
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, (HANDLE)nullptr);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	BITMAPFILEHEADER bmpfilehead; //비트맵파일 자체의 정보를 저장할 변수 선언
	if (!ReadFile(hfile, &bmpfilehead, sizeof(bmpfilehead), &actualRead, nullptr)) //비트맵파일 자체의 정보를 읽을수 없다면
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
	if (!ReadFile(hfile, &bmpinfohead, sizeof(bmpinfohead), &actualRead, nullptr)) //비트맵영상 자체의 정보를 읽을수 없다면
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
	int nWidth = bmpinfohead.biWidth;

	int nDestY, nDeltaY, nDestY2, nDeltaY2; //현재 높이,이미지가 뒤집힘 상태에따라 다음라인으로 바꿀때 높이에 더해주는값

	nDestY = nHeight - 1;
	nDeltaY = -1;
	nDestY2 = 0; //0
	nDeltaY2 = 1; // 1

	const unsigned char* pBuffer = mBufferPtr.get();

	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			const unsigned char* p = pBuffer + (x << 2) + nDestY2 * (mWidth << 2);
			if (*p == 0 && *(p + 1) == 0 && *(p + 2) == 0)
			{
				continue;
			}
			CopyMemory(buffer2 + ((x + _x) << 2) + ((nDestY - _y) * (nWidth << 2)) - (x + _x + (nDestY - _y) * nWidth), p, 3);
		}

		nDestY += nDeltaY;
		nDestY2 += nDeltaY2;
	}
	CloseHandle(hfile);
	return true;
}
