#include "Camera.h"

#include <cassert>
#include <vector>
#include <thread>

#include "Vector2.h"
#include "GameEnum.h"
#include "SettingData.h"

using namespace std;

extern EMod curMod;

extern EDirection curPlayerDirection;

extern EAction curPlayerAction;

extern Player player;

extern Vector2 attackDirection;

extern HDC hdc;
extern TCHAR buffer2[40];

Camera::Camera()
	: mLastFrameTime(Timer::Now())
	, mExpansionInterval(4)
	, mX(0)
	, mY(0)
	, mAlpha(0)
	, mIsFirstAlpha(true)
	, mSpeedX(0)
	, mSpeedY(0)
	, mFinishX(0)
	, mFinishY(0)
	, mMoveSpeed(0.5f)
	, mZoomInRange(600)
	, mExpansion(1)
	, mExpansionSpeed(0.006f)
	, mIsExpansion(false)
	, mIsSkill(false)
{
	mX = (curMod == EMod::Game) ? player.GetPos().x : 1600;
	mY = (curMod == EMod::Game) ? player.GetPos().y : 1600;
}

void Camera::CheckExpansion()
{
	if (Timer::Elapsed(mLastFrameTime, mExpansionInterval))
	{
		if (mIsExpansion)
		{
			mExpansion += mExpansionSpeed;
			if (mExpansion > 1.3f)
				mExpansion = 1.3f;
		}
		else
		{
			mExpansion -= 0.08f;
			if (mExpansion < 1)
				mExpansion = 1;
		}
	}
}

void Camera::Initialize()
{
	new (this) Camera();
	mX = (curMod == EMod::Game) ? player.GetPos().x : 1600;
	mY = (curMod == EMod::Game) ? player.GetPos().y : 1600;
}

void Camera::InitExpansion()
{
	mSpeedX = 0;
	mSpeedY = 0;
	if (mIsSkill)
		mExpansionSpeed = 0.03f;
	else
		mExpansionSpeed = 0.006f;
}

void Camera::Left()
{
	mX -= 5;
	if (mX < SCREEN_WIDTH >> 1)
		mX = SCREEN_WIDTH >> 1;
}

void Camera::Right()
{
	mX += 5;
	if (mX > WORLDMAP_WIDTH - (SCREEN_WIDTH >> 1))
		mX = WORLDMAP_WIDTH - (SCREEN_WIDTH >> 1);
}

void Camera::Up()
{
	mY -= 5;
	if (mY < SCREEN_HEIGHT >> 1)
		mY = SCREEN_HEIGHT >> 1;
}

void Camera::Down()
{
	mY += 5;
	if (mY > WORLDMAP_HEIGHT - (SCREEN_HEIGHT >> 1))
		mY = WORLDMAP_HEIGHT - (SCREEN_HEIGHT >> 1);
}

void Camera::Move()
{
	//mX += (mFinishX - mX)* !((int)curPlayerActionState == 4) ? 0.5f : 0.1f;
	//mY += (mFinishY - mY)* !((int)curPlayerActionState == 4) ? 0.5f : 0.1f;
	if (curPlayerAction == EAction::Attack)
		mMoveSpeed = 0.1f;
	else
		mMoveSpeed = 0.5f;
	if (curMod == EMod::Editor)
		mMoveSpeed = 1;
	mX += (mFinishX - mX) * mMoveSpeed;
	mY += (mFinishY - mY) * mMoveSpeed;
	if (mX < SCREEN_WIDTH >> 1)
		mX = SCREEN_WIDTH >> 1;
	else if (mX > WORLDMAP_WIDTH - (SCREEN_WIDTH >> 1))
		mX = WORLDMAP_WIDTH - (SCREEN_WIDTH >> 1);
	if (mY < SCREEN_HEIGHT >> 1)
		mY = SCREEN_HEIGHT >> 1;
	else if (mY > WORLDMAP_HEIGHT - (SCREEN_HEIGHT >> 1))
		mY = WORLDMAP_HEIGHT - (SCREEN_HEIGHT >> 1);
}

void Camera::CheckFinishXY()
{
	mFinishX = GetFinishX2();
	mFinishY = GetFinishY2();
}

int Camera::GetFinishX()
{
	int r = rand() % 4;
	switch (r)
	{
	case 0:
		r = 20;
		break;
	case 1:
		r = -20;
		break;
	case 2:
		r = 40;
		break;
	case 3:
		r = -40;
		break;
	default:
		assert(false);
	}

	return player.GetPos().x + r;
}

int Camera::GetFinishY()
{
	int r = rand() % 4;
	switch (r)
	{
	case 0:
		r = 20;
		break;
	case 1:
		r = -20;
		break;
	case 2:
		r = 40;
		break;
	case 3:
		r = -40;
		break;
	default:
		assert(false);
	}
	return player.GetPos().y + r;
}

int Camera::GetFinishX2() const
{
	int x;
	if ((curPlayerAction != EAction::Attack) && (curPlayerAction != EAction::Roll)) //공격중이 아닐때
		x = player.GetPos().x;
	else
		x = player.GetPos().x + (attackDirection.x * mZoomInRange * (mExpansion - 1) * 1.5f);
	return x;
}


int Camera::GetFinishY2() const
{
	int y;
	if ((curPlayerAction != EAction::Attack) && (curPlayerAction != EAction::Roll)) //공격중이 아닐때
		y = player.GetPos().y;
	else
		y = player.GetPos().y + (attackDirection.y * mZoomInRange * (mExpansion - 1) * 1.5f);
	return y;
}

struct ThreadData
{
	unsigned char* rgbArr;
	int start;
	int end;
	int alpha;
	int threadCnt;
};

void Camera::AlphaBlendingThreadWorker(ThreadData* data)
{
	if (data->end >= SCREEN_HEIGHT)
		data->end = SCREEN_HEIGHT;

	for (int height = data->start; height < data->end; height++)
	{
		for (int width = 0; width < SCREEN_WIDTH; width++)
		{
			unsigned char* p = data->rgbArr + (height * SCREEN_WIDTH * 4) + (width * 4);
			*p = (*p * data->alpha) >> 8;
			*(p + 1) = (*(p + 1) * data->alpha) >> 8;
			*(p + 2) = (*(p + 2) * data->alpha) >> 8;
		}
	}
	delete data;
}

void Camera::AlphaBlending2(LPDIRECTDRAWSURFACE7 lpSurface)
{
	DDSURFACEDESC2 ddsd; //표면의 정보를 확인할수 있는 변수를 선언
	ZeroMemory(&ddsd, sizeof(ddsd)); //ddsd주소에서 ddsd크기만큼 메모리영역을 0으로 채워줌
	ddsd.dwSize = sizeof(ddsd); //dwSize를 ddsd크기만큼 초기화 해준다

	lpSurface->Lock(nullptr, &ddsd, DDLOCK_WAIT, nullptr); //만약 표면을 잠금하는데 실패할경우 false를 반환해준다

	unsigned char* surface = (unsigned char*)ddsd.lpSurface;

	unique_ptr<unsigned char[]> rgbArr = make_unique<unsigned char[]>(SCREEN_HEIGHT * SCREEN_WIDTH * 4);

	for (int i = 0; i < ddsd.dwHeight; i++)
	{
		CopyMemory(rgbArr.get() + i * SCREEN_WIDTH * 4, surface + i * ddsd.lPitch, SCREEN_WIDTH * 4);
	}

	const int threadCnt = thread::hardware_concurrency();

	vector<thread> threads;

	int height = SCREEN_HEIGHT / threadCnt;
	for (int i = 0; i < threadCnt; i++)
	{
		ThreadData* data = new ThreadData;
		ZeroMemory(data, sizeof(ThreadData));
		data->rgbArr = rgbArr.get();
		data->start = height * i;
		data->end = height * i + height;
		data->alpha = mAlpha;
		data->threadCnt = threadCnt;
		threads.emplace_back(thread(AlphaBlendingThreadWorker, data));
	}

	for (thread& t : threads)
	{
		t.join();
	}

	for (int i = 0; i < ddsd.dwHeight; i++)
	{
		CopyMemory(surface + i * ddsd.lPitch, rgbArr.get() + i * SCREEN_WIDTH * 4, SCREEN_WIDTH * 4);
	}

	lpSurface->Unlock(nullptr);

	if (!mIsFirstAlpha)
	{
		mAlpha -= 5;
		if (mAlpha <= 0)
		{
			mAlpha = 255;
			if (curMod == EMod::Game)
				curMod = EMod::Intro;
			else if (curMod == EMod::Intro)
				curMod = EMod::Game;
		}
	}
	else
	{
		mAlpha += 5;
		if (mAlpha >= 255)
		{
			mAlpha = 255;
			mIsFirstAlpha = false;
		}
	}
}
