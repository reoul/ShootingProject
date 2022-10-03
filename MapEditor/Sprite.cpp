#include "Sprite.h"
#include <Windows.h>
#include <string>

#include "Bmp.h"
#include "Camera.h"
#include "SettingData.h"
#include "Boss.h"

#define PI 3.141592

static RECT destRect;
static RECT destRect2;
extern Camera camera;
extern Boss boss;

Sprite::Sprite()
	: mFrame(0)
	, mWidth(0)
	, mHeight(0)
	, mDrawWidth(0)
	, mDrawHeight(0)
	, mSize(1)
	, mNumber(0)
	, mSurfacePtr(nullptr)
	, mDrawSurfacePtr(nullptr)
	, mBMPArray(nullptr)
{
	destRect.left = 0;
	destRect.top = 0;
	destRect.right = 800;
	destRect.bottom = 1200;

	destRect2.left = 0;
	destRect2.top = -1200;
	destRect2.right = 800;
	destRect2.bottom = 0;
}

bool Sprite::InitSprite(int frame, int width, int nHeight, int colorKey, LPDIRECTDRAW7 pDirectDraw)
{
	mSurfacePtr = make_unique<LPDIRECTDRAWSURFACE7[]>(frame);
	if (!mSurfacePtr)
		return false;

	mDrawSurfacePtr = make_unique<LPDIRECTDRAWSURFACE7[]>(frame);
	if (!mDrawSurfacePtr)
		return false;

	mBMPArray = make_unique<Bmp[]>(frame);
	if (!mBMPArray)
		return false;

	DDSURFACEDESC2 ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwHeight = nHeight;
	ddsd.dwWidth = width;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = colorKey;

	for (int i = 0; i < frame; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &mSurfacePtr[i], NULL)))
		{
			mSurfacePtr[i] = nullptr;
			return false;
		}
		else
		{
			mSurfacePtr[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}

		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &mDrawSurfacePtr[i], NULL)))
		{
			mDrawSurfacePtr[i] = nullptr;
			return false;
		}
		else
		{
			mDrawSurfacePtr[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}

	mWidth = width;
	mHeight = nHeight;
	mFrame = frame;
	mSize = 1;
	mNumber = 0;

	mDrawHeight = mHeight * mSize;
	mDrawWidth = mWidth * mSize;

	return true;
}

bool Sprite::LoadFrame(int frame, const char* filePath) const
{
	if (!mBMPArray[frame].LoadBMPFile(filePath))
		return false;
	if (!mBMPArray[frame].CopyBufferToSurface(mSurfacePtr[frame]))
		return false;
	if (!mBMPArray[frame].CopyBufferToSurface(mDrawSurfacePtr[frame]))
		return false;
	return true;
}

/**
 * 이미지를 표면에 그려준다
 * \param frame 그려줄 이미지 프레임
 * \param x x좌표
 * \param y y좌표
 * \param pSurface 그려줄 표면
 * \param bUsingColorKey 컬러키 사용 여부
 * \return Draw 성공 여부
 */
bool Sprite::Drawing(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey) const
{
	RECT destRect; //출력 영역 설정 변수
	RECT destRect2; //원본 이미지 영역 설정 변수

	destRect.left = x - (mDrawWidth >> 1);
	destRect.top = y - (mDrawHeight >> 1);
	destRect.right = destRect.left + mDrawWidth;
	destRect.bottom = destRect.top + mDrawHeight;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = mWidth;
	destRect2.bottom = mHeight;

	if (destRect.top < 0)
	{
		destRect2.top = -destRect.top / mSize;
		destRect.top = 0;
		if (destRect2.top > mHeight)
			return true;
	}
	if (destRect.left < 0)
	{
		destRect2.left = -destRect.left / mSize;
		destRect.left = 0;
		if (destRect2.left > mWidth)
			return true;
	}
	if (destRect.right > SCREEN_WIDTH)
	{
		destRect2.right = mWidth - (destRect.right - SCREEN_WIDTH) / mSize;
		destRect.right = SCREEN_WIDTH;
		if (destRect2.right < 0)
			return true;
	}
	if (destRect.bottom > SCREEN_HEIGHT)
	{
		destRect2.bottom = mHeight - (destRect.bottom - SCREEN_HEIGHT) / mSize;
		destRect.bottom = SCREEN_HEIGHT;
		if (destRect2.bottom < 0)
			return true;
	}

	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(
			hResult = pSurface->Blt(&destRect, mDrawSurfacePtr[frame], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}
			
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, mDrawSurfacePtr[frame], &destRect2, DDBLT_WAIT, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}
	}

	return true;
}

bool Sprite::Drawing(RECT rect, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey) const
{
	RECT destRect2;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = mWidth;
	destRect2.bottom = mHeight;

	float size = (rect.right - rect.left) / destRect2.right;

	if (rect.top < 0)
	{
		destRect2.top = -rect.top / size;
		rect.top = 0;
		if (destRect2.top > mHeight)
			return true;
	}
	if (rect.left < 0)
	{
		destRect2.left = -rect.left / size;
		rect.left = 0;
		if (destRect2.left > mWidth)
			return true;
	}
	if (rect.right > SCREEN_WIDTH)
	{
		destRect2.right = mWidth - (rect.right - SCREEN_WIDTH) / size;
		rect.right = SCREEN_WIDTH;
		if (destRect2.right < 0)
			return true;
	}
	if (rect.bottom > SCREEN_HEIGHT)
	{
		destRect2.bottom = mHeight - (rect.bottom - SCREEN_HEIGHT) / size;
		rect.bottom = SCREEN_HEIGHT;
		if (destRect2.bottom < 0)
			return true;
	}

	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(hResult = pSurface->Blt(&rect, mDrawSurfacePtr[0], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&rect, mDrawSurfacePtr[0], &destRect2, DDBLT_WAIT, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}
	}

	return true;
}

bool Sprite::DrawingBossHp(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey) const
{
	RECT destRect; //출력 영역 설정 변수
	RECT destRect2; //원본 이미지 영역 설정 변수

	destRect.left = x - (mDrawWidth >> 1);
	destRect.top = y - (mDrawHeight >> 1) + (mDrawHeight * ((1000 - boss.GetHp()) * 0.001f)); // 보스 체력 깍인 만큼 체력 부분 줄여줌
	destRect.right = destRect.left + mDrawWidth;
	destRect.bottom = y - (mDrawHeight >> 1) + mDrawHeight;

	destRect2.left = 0;
	destRect2.top = mHeight * ((1000 - boss.GetHp()) * 0.001f);
	destRect2.right = mWidth;
	destRect2.bottom = mHeight;

	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(
			hResult = pSurface->Blt(&destRect, mDrawSurfacePtr[frame], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, mDrawSurfacePtr[frame], &destRect2, DDBLT_WAIT, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}
	}

	return true;
}

bool Sprite::Restore() const
{
	if (!mSurfacePtr)
		return false;
	if (!mDrawSurfacePtr)
		return false;

	for (int i = 0; i < mFrame; i++)
	{
		if (!mSurfacePtr[i])
			return false;

		if (!mDrawSurfacePtr[i])
			return false;

		if (FAILED(mSurfacePtr[i]->Restore()))
			return false;

		if (FAILED(mDrawSurfacePtr[i]->Restore()))
			return false;

		if (!mBMPArray[i].CopyBufferToSurface(mSurfacePtr[i]))
			return false;

		if (!mBMPArray[i].CopyBufferToSurface(mDrawSurfacePtr[i]))
			return false;
	}
	return true;
}

bool Sprite::ReleaseAll() const
{
	if (!mSurfacePtr)
		return false;

	if (!mDrawSurfacePtr)
		return false;

	for (int i = 0; i < mFrame; i++)
	{
		if (mSurfacePtr[i])
			mSurfacePtr[i]->Release();

		if (mDrawSurfacePtr[i])
			mDrawSurfacePtr[i]->Release();
	}

	return true;
}

bool Sprite::Drawing2(int frame, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey) const
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
		if (FAILED(hResult = pSurface->Blt(&destRect, mDrawSurfacePtr[frame], NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect2, mDrawSurfacePtr[frame], NULL, DDBLT_WAIT, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}

		if (FAILED(hResult = pSurface->Blt(&destRect, mDrawSurfacePtr[frame], NULL, DDBLT_WAIT, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}
	}
	return true;
}

bool Sprite::DrawingSkillCoolTime(float coolTimePercent, int x, int y, LPDIRECTDRAWSURFACE7 pSurface,
	bool bUsingColorKey) const
{
	RECT destRect; //출력 영역 설정 변수
	RECT destRect2; //원본 이미지 영역 설정 변수

	destRect.left = x - (mDrawWidth >> 1);
	destRect.top = y - (mDrawHeight >> 1);
	destRect.right = destRect.left + mDrawWidth;
	destRect.bottom = destRect.top + mDrawHeight;
	destRect.top = y - (mDrawHeight >> 1) + mHeight - static_cast<int>(mHeight * coolTimePercent);

	destRect2.left = 0;
	destRect2.top = mHeight - static_cast<int>(mHeight * coolTimePercent);
	destRect2.right = mWidth;
	destRect2.bottom = mHeight;

	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, mDrawSurfacePtr[0], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, mDrawSurfacePtr[0], &destRect2, DDBLT_WAIT, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}
	}
	return true;
}

bool Sprite::BlockDrawing(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey) const
{
	RECT destRect; //출력 영역 설정 변수
	RECT destRect2; //원본 이미지 영역 설정 변수

	destRect.left = x;
	destRect.top = y;
	destRect.right = destRect.left + mDrawWidth;
	destRect.bottom = destRect.top + mDrawHeight;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = mWidth;
	destRect2.bottom = mHeight;

	if (destRect.top < 0)
	{
		destRect2.top = -destRect.top / mSize;
		destRect.top = 0;
		if (destRect2.top > mHeight)
			return true;
	}
	if (destRect.left < 0)
	{
		destRect2.left = -destRect.left / mSize;
		destRect.left = 0;
		if (destRect2.left > mWidth)
			return true;
	}
	if (destRect.right > SCREEN_WIDTH)
	{
		destRect2.right = mWidth - (destRect.right - SCREEN_WIDTH) / mSize;
		destRect.right = SCREEN_WIDTH;
		if (destRect2.right < 0)
			return true;
	}
	if (destRect.bottom > SCREEN_HEIGHT)
	{
		destRect2.bottom = mHeight - (destRect.bottom - SCREEN_HEIGHT) / mSize;
		destRect.bottom = SCREEN_HEIGHT;
		if (destRect2.bottom < 0)
			return true;
	}

	HRESULT hResult;
	if (bUsingColorKey)
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, mDrawSurfacePtr[0], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, mDrawSurfacePtr[0], &destRect2, DDBLT_WAIT, NULL)))
		{
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			return false;
		}
	}
	return true;
}

/**
 * 이미지를 회전시킨다
 * \param degree 각도
 * \param curFrame 현재 그려줄 프레임
 */
void Sprite::Rotate(double degree, int curFrame) const
{
	unique_ptr<unsigned char[]> pixel = make_unique<unsigned char[]>(3);

	const double radian = degree * PI / 180.0;

	const double cc = cos(radian);
	const double ss = sin(-radian);

	const double xCenter = static_cast<double>(mWidth) / 2.0;
	const double yCenter = static_cast<double>(mHeight) / 2.0;

	unique_ptr<unsigned char[]> buffer = make_unique<unsigned char[]>(mHeight * mWidth * 4);

	for (size_t y = 0; y < mHeight; y++)
	{
		for (size_t x = 0; x < mWidth; x++)
		{
			const int origX = static_cast<int>(xCenter + (static_cast<double>(y) - yCenter) 
					* ss + (static_cast<double>(x) - xCenter) * cc);
			const int origY = static_cast<int>(yCenter + (static_cast<double>(y) - yCenter) 
					* cc - (static_cast<double>(x) - xCenter) * ss);
			ZeroMemory(pixel.get(), 3);
			if ((origY >= 0 && origY < mHeight) && (origX >= 0 && origX < mWidth))
				CopyMemory(pixel.get(), mBMPArray.get()->GetBMPBuffer() + (origX << 2) + origY * (mWidth << 2), 3); //원본 픽셀 값 추출

			if ((origY >= 0 && origY < mHeight) && (origX >= 0 && origX < mWidth))
				CopyMemory(buffer.get() + (x << 2) + y * (mWidth << 2), pixel.get(), 3);
		}
	}

	mBMPArray.get()->CopyBufferToSurface(mDrawSurfacePtr[curFrame], buffer.get());
}
