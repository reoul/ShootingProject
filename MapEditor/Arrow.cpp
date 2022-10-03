#include "Arrow.h"

#include <Windows.h>
#include <vector>

#include "Camera.h"
#include "Player.h"
#include "Block.h"
#include "Boss.h"
#include "GameEnum.h"
#include "Map.h"
#include "SettingData.h"
#include "Vector2.h"

using namespace std;

extern EDirection curPlayerDirection;

extern Camera camera;
extern Player player;
extern Block wall[139];
extern Boss boss;
extern Map map;
extern vector<Vector2> wallBlock;

Arrow::Arrow()
	: GameObject()
	, mSpeedX(0)
	, mSpeedY(0)
	, mPower(0)
	, mDrawX(0)
	, mDrawY(0)
	, mCount(0)
	, mLastMoveTime(0)
	, mMoveInterval(0)
	, mTurnRadian(1)
	, mIsUse(false)
	, mIsHit(false)
	, mIsCharging(false)
	, mIsHoming(false)
	, mArrowSprite(nullptr)
	, mArrowHitRect{}
	, mTarget(&boss)
{
	mMoveSpeed = 70;
}

void Arrow::Initialize(int x, int y, int moveFrame, Sprite8* arrowSprite)
{
	mSpeedX = 0;
	mSpeedY = 0;
	mPower = 0;
	mDrawX = 0;
	mCount = 0;
	mDrawY = 0;
	mLastMoveTime = 0;
	mMoveInterval = moveFrame;
	mTurnRadian = 0;
	mIsUse = false;
	mIsHit = false;
	mIsCharging = false;
	mIsHoming = false;
	mArrowSprite = arrowSprite;
	SetRect(&mArrowHitRect[0], 10, 4, -4, 4);
	SetRect(&mArrowHitRect[1], -4, 4, 10, 4);
	SetRect(&mArrowHitRect[2], 4, 10, 4, -4);
	SetRect(&mArrowHitRect[3], 4, -4, 4, 10);
	SetRect(&mArrowHitRect[4], 8, 8, 0, 0);
	SetRect(&mArrowHitRect[5], 0, 8, 8, 0);
	SetRect(&mArrowHitRect[6], 8, 0, 0, 8);
	SetRect(&mArrowHitRect[7], 0, 0, 8, 8);
	GameObject::Initialize(arrowSprite->GetSprite(EDirection::Up), x, y, 0, 0, 0);
}

void Arrow::SetSpeedXY(float power, Vector2 direction)
{
	mCount = 0;
	mPower = power;
	mSpeedX = direction.x * power;
	mSpeedY = direction.y * power;
	mDirection = direction;
	mTurnRadian = 1;
	mIsLive = true;
	mShootTime = Timer::Now();
}

void Arrow::CheckSprite()
{
	SetSprite(mArrowSprite->GetSprite(EDirection::Right));
	SetHitRect(mArrowHitRect[(int)curPlayerDirection - 1]);
}

void Arrow::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	if (!mIsLive)
		return;

	mDrawX = static_cast<int>(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f));
	mDrawY = static_cast<int>(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f));
	GameObject::Draw(false, mDrawX, mDrawY, surface);
}

void Arrow::CheckMove()
{
	if (!mIsLive)
		return;

	if (Timer::Elapsed(mLastFrameTime, mMoveInterval))
	{
		if (mIsHoming)
			HomingArrow();
		SetX(GetPos().x + (mSpeedX * Timer::GetDeltaTime() * mMoveSpeed * (mIsHoming ? 4 : 1)));
		SetY(GetPos().y + (mSpeedY * Timer::GetDeltaTime() * mMoveSpeed * (mIsHoming ? 4 : 1)));
		RECT rect;
		if (!((GetPos().x > 1100) && (GetPos().x < 2100) && (GetPos().y > 1400) && (GetPos().y < 2100)))
		{
			for (int i = 0; i < 139; i++)
			{
				if (map.GetStageNum() == 0)
				{
					SetRect(&rect, wallBlock[i].x * 32, wallBlock[i].y * 32, wallBlock[i].x * 32 + 32,
						wallBlock[i].y * 32 + 32);
				}
				else
				{
					SetRect(&rect, wall[i].GetPos().x * 32, wall[i].GetPos().y * 32, wall[i].GetPos().x * 32 + 32,
						wall[i].GetPos().y * 32 + 32);
				}
				if (CheckHit(rect))
				{
					mIsUse = false;
					mIsLive = false;
					mIsHoming = false;
				}
			}
		}

		if (!mIsHoming)
		{
			mSpeedX -= mSpeedX * 0.02f * mCount * mMoveSpeed * Timer::GetDeltaTime();
			mSpeedY -= mSpeedY * 0.02f * mCount * mMoveSpeed * Timer::GetDeltaTime();
		}

		if ((abs(mSpeedX) < 0.1f) && (abs(mSpeedY) < 0.1f))
		{
			mIsUse = false;
			mIsLive = false;
			mIsHoming = false;
			mSpeedX = 0;
			mSpeedY = 0;
		}

		if (Timer::Elapsed(mShootTime, 2000))
		{
			mIsUse = false;
			mIsLive = false;
			mIsHoming = false;
			mSpeedX = 0;
			mSpeedY = 0;
		}

		mCount++;
	}
}

void Arrow::IsHit()
{
	mIsHit = true;
	mIsUse = false;
	mIsLive = false;
	mIsHoming = false;
	mSpeedX = 0;
	mSpeedY = 0;
}

/**
 * 타겟에게 향하는 방향과 속도를 계산
 */
void Arrow::HomingArrow()
{
	if (!mIsLive && !mIsHoming)
		return;

	const Vector2 toDir = (mTarget->GetPos() - GetPos()).Normalize();
	const float cross = toDir.x * mDirection.y - toDir.y * mDirection.x; //어느방향으로 돌지 계산

	float angle = toDir.x * mDirection.x + toDir.y * mDirection.y;
	if (angle < -1)
		angle = -1;
	if (angle > 1)
		angle = 1;

	angle = acosf(angle);	//두 벡터의 내각(0~180 양수)
	if (cross > 0)			//외적 값이 음수면 시계방향으로 회전
		angle *= -1;

	mDirection.Rotate(angle * mTurnRadian++);

	mSpeedX = mDirection.x * 5 * mMoveSpeed * Timer::GetDeltaTime();
	mSpeedY = mDirection.y * 5 * mMoveSpeed * Timer::GetDeltaTime();
}
