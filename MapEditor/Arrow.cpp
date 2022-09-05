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

extern DIRECTION curPlayerDirection;

extern Arrow arrow[TOTAL_ARROW];
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
}

Arrow::~Arrow()
{
}

void Arrow::Initialize(int x, int y, int moveFrame, CSprite8* arrowSprite)
{
	mSpeedX = 0;
	mSpeedY = 0;
	mPower = 0;
	mCount = 0;
	mDrawX = 0;
	mDrawY = 0;
	mArrowSprite = arrowSprite;
	mLastMoveTime = 0;
	mMoveInterval = moveFrame;
	mIsCharging = false;
	mIsUse = false;
	mIsHit = false;
	int cnt = 0;
	SetRect(&mArrowHitRect[cnt++], 10, 4, -4, 4);
	SetRect(&mArrowHitRect[cnt++], -4, 4, 10, 4);
	SetRect(&mArrowHitRect[cnt++], 4, 10, 4, -4);
	SetRect(&mArrowHitRect[cnt++], 4, -4, 4, 10);
	SetRect(&mArrowHitRect[cnt++], 8, 8, 0, 0);
	SetRect(&mArrowHitRect[cnt++], 0, 8, 8, 0);
	SetRect(&mArrowHitRect[cnt++], 8, 0, 0, 8);
	SetRect(&mArrowHitRect[cnt], 0, 0, 8, 8);
	GameObject::Initialize(arrowSprite->GetSprite(DIRECTION::UP), x, y, 0, 0, 0);
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
	SetSprite(mArrowSprite->GetSprite(DIRECTION::RIGHT));
	SetHitRect(mArrowHitRect[(int)curPlayerDirection - 1]);
}

void Arrow::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	if (!mIsLive)
		return;
	mDrawX = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f));
	mDrawY = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f));
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

void Arrow::IsCharging()
{
	mIsCharging = true;
}

Arrow* Arrow::FindNotUseArrow()
{
	for (int i = 0; i < TOTAL_ARROW; i++)
	{
		if (!arrow[i].GetIsUse())
			return &arrow[i];
	}
	return NULL;
}

void Arrow::SetIsUse(bool use)
{
	mIsUse = use;
}

void Arrow::SetCharging(bool _charging)
{
	mIsCharging = _charging;
}

bool Arrow::GetIsUse()
{
	return mIsUse;
}

bool Arrow::GetIsCharging()
{
	return mIsCharging;
}

bool Arrow::GetChargingBow()
{
	for (int i = 0; i < TOTAL_ARROW; i++)
	{
		if (arrow[i].GetIsCharging())
			return true;
	}
	return false;
}

bool Arrow::IsZeroArrow()
{
	for (int i = 0; i < TOTAL_ARROW; i++)
	{
		if (!arrow[i].GetIsUse())
			return false;
	}
	return true;
}

bool Arrow::IsSpeedZero()
{
	if ((abs(mSpeedX) == 0) && (abs(mSpeedY) == 0))
		return true;
	return false;
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

int Arrow::GetPower()
{
	return (int)mPower;
}

bool Arrow::GetIsHit()
{
	return mIsHit;
}

void Arrow::SetIsHit(bool isHit)
{
	mIsHit = isHit;
}

void Arrow::SetHoming(bool _homing)
{
	mIsHoming = _homing;
}

void Arrow::HomingArrow()
{
	if (!mIsLive && !mIsHoming)
		return;
	Vector2 curPos;
	curPos.SetXY(GetPos().x, GetPos().y);
	Vector2 toDir = mTarget->GetPos() - curPos;
	toDir = toDir.normalize();
	float cross = (toDir.x * mDirection.y) - (toDir.y * mDirection.x); //어느방향으로 돌지 계산

	float tmp = (toDir.x * mDirection.x) + (toDir.y * mDirection.y);
	if (tmp < -1)
		tmp = -1;
	if (tmp > 1)
		tmp = 1;
	float angle = acosf(tmp); //두 벡터의 내각(0~180 양수)
	if (cross > 0) //외적 값이 음수면 시계방향으로 회전
		angle *= -1;

	mDirection.Rotate(angle * mTurnRadian); // 회전

	mTurnRadian += 1;

	mSpeedX = mDirection.x * 5 * mMoveSpeed * Timer::GetDeltaTime();
	mSpeedY = mDirection.y * 5 * mMoveSpeed * Timer::GetDeltaTime();
}

Vector2 Arrow::GetSpeedXY()
{
	return Vector2(mSpeedX, mSpeedY);
}

bool Arrow::IsHoming()
{
	return mIsHoming;
}
