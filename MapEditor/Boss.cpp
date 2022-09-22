#include "Boss.h"
#include <dsound.h>
#include <math.h>

#include "Arrow.h"
#include "Timer.h"
#include "SnowBall.h"
#include "Block.h"
#include "DSUTIL.h"
#include "GameEnum.h"
#include "SettingData.h"

extern SnowBall snowball[TOTAL_SNOWBALL];
extern Block wall[139];
extern int cntVibes;
extern bool boss_HitWall;

extern HSNDOBJ Sound[10];

extern Camera camera;
extern Player player;

extern EDirection curPlayerDirection;
extern EDirection curBossDirection;
extern EAction curPlayerAction;
extern EAction curBossAction;

extern Arrow arrow[TOTAL_ARROW];

Boss::Boss()
	: GameObject()
	, mIsRoll(false)
	, mIsHitWall(false)
	, mIsRoar(false)
	, mLastMoveTime(Timer::Now())
	, mMoveInterval(0)
	, mSpeedX(0)
	, mSpeedY(0)
	, mMoveSpeedFold(1)
	, mDrawX(0)
	, mDrawY(0)
	, mHp(1000)
	, mPattern{}
	, mCurPatternIndex(0)
	, mAttackCnt(0)
	, mHitWallCnt(0)
	, mLastIdleTime(Timer::Now())
	, mIdleFrameInterval(300)
	, mPlayerDirection()
	, mIdleHitRect{}
	, mRollHitRect()
	, mAttackHitRect{}
	, mSleepHitRect()
	, mCurSpritePtr(nullptr)
	, mOldSpritePtr(nullptr)
	, mSleepSpritePtr(nullptr)
	, mIdleSpritePtr(nullptr)
	, mRollSpritePtr(nullptr)
	, mAttackSpritePtr(nullptr)
	, mDeadSpritePtr(nullptr)
{
	// 패턴 지정
	mPattern[0] = EAction::Sleep;
	mPattern[1] = EAction::Idle;
	mPattern[2] = EAction::Attack;
	mPattern[3] = EAction::Idle;
	mPattern[4] = EAction::Roll;
	mPattern[5] = EAction::Idle;
	mPattern[6] = EAction::Roll;
	mPattern[7] = EAction::Idle;
	mPattern[8] = EAction::Roll;

	curBossDirection = EDirection::Down;
	curBossAction = mPattern[mCurPatternIndex];
	mMoveSpeed = 1600;

	// 영역 지정
	SetRect(&mSleepHitRect, 67, 32, 64, 74);
	SetRect(&mRollHitRect, 38, 13, 39, 74);
	SetRect(&mIdleHitRect[0], 55, 33, 30, 64);
	SetRect(&mIdleHitRect[1], 37, 33, 52, 53);
	SetRect(&mIdleHitRect[2], 68, 28, 68, 64);
	SetRect(&mIdleHitRect[3], 65, 32, 66, 69);
	SetRect(&mIdleHitRect[4], 62, 30, 40, 62);
	SetRect(&mIdleHitRect[5], 40, 30, 64, 62);
	SetRect(&mIdleHitRect[6], 51, 28, 48, 55);
	SetRect(&mIdleHitRect[7], 50, 28, 58, 53);
	SetRect(&mAttackHitRect[0], 34, 56, 27, 43);
	SetRect(&mAttackHitRect[1], 36, 60, 34, 42);
	SetRect(&mAttackHitRect[2], 51, 54, 52, 51);
	SetRect(&mAttackHitRect[3], 34, 37, 38, 52);
	SetRect(&mAttackHitRect[4], 37, 49, 37, 39);
	SetRect(&mAttackHitRect[5], 44, 52, 35, 40);
	SetRect(&mAttackHitRect[6], 48, 55, 34, 45);
	SetRect(&mAttackHitRect[7], 41, 50, 46, 46);
}

void Boss::Initialize(int x, int y, int currentFrame, int frameInterval, int moveInterval)
{
	new (this) Boss();
	GameObject::Initialize(mCurSpritePtr, x, y, currentFrame, frameInterval, 0);
	mMoveInterval = moveInterval;
	mDrawX = x;
	mDrawY = y;
}

void Boss::SetSprite(Sprite* _sleep, Sprite8* _idle, Sprite8* _roll, Sprite8* _attack, Sprite8* _dead)
{
	mSleepSpritePtr = _sleep;
	mIdleSpritePtr = _idle;
	mRollSpritePtr = _roll;
	mAttackSpritePtr = _attack;
	mDeadSpritePtr = _dead;
	mCurSpritePtr = _sleep;
	mOldSpritePtr = _idle->GetSprite(EDirection::Left);
}

void Boss::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	GameObject::Draw(true, mDrawX, mDrawY, surface);
}

bool Boss::CanMove()
{
	if (mIsLive && Timer::Elapsed(mLastMoveTime, mMoveInterval))
		return true;
	return false;
}

void Boss::Attack()
{
	SndObjPlay(Sound[5], NULL);
	CheckPlayerDirection();
	snowball[mAttackCnt].SetDirection(mPlayerDirection);
	snowball[mAttackCnt].SetX(GetPos().x);
	snowball[mAttackCnt].SetY(GetPos().y);
	if (mAttackCnt >= 3)
	{
		NextPattern();
		CheckSprite();
	}
	mAttackCnt++;
}

void Boss::Hit(int damage)
{
	mHp -= damage;
	if ((mHp < 300) && !mIsRoar)
	{
		SndObjPlay(Sound[7], NULL);
		mIsRoar = true;
	}
	if (mHp < 0)
	{
		for (int i = 0; i < TOTAL_ARROW; i++)
		{
			arrow[i].IsHit();
		}
		SndObjPlay(Sound[7], NULL);
		curBossAction = EAction::Dead;
	}
}

void Boss::CheckPlayerDirection()
{
	float x = player.GetPos().x - GetPos().x;
	float y = player.GetPos().y - GetPos().y;
	int direction = (int)sqrtf(pow(x, 2) + pow(y, 2));

	mPlayerDirection.SetXY(x / direction, y / direction);
}

void Boss::CheckDirectionState()
{
	CheckPlayerDirection();
	if (mPlayerDirection.y < 0)
	{
		if ((mPlayerDirection.x > -0.3f) && (mPlayerDirection.x < 0.3f))
			curBossDirection = EDirection::Up;
		else if (mPlayerDirection.x < -0.3f)
			curBossDirection = EDirection::LeftUp;
		else if (mPlayerDirection.x > 0.3f)
			curBossDirection = EDirection::RightUp;
	}
	else
	{
		if ((mPlayerDirection.x > -0.3f) && (mPlayerDirection.x < 0.3f))
			curBossDirection = EDirection::Down;
		else if (mPlayerDirection.x < -0.3f)
			curBossDirection = EDirection::LeftDown;
		else if (mPlayerDirection.x > 0.3f)
			curBossDirection = EDirection::RightDown;
	}
	if (mPlayerDirection.x < 0)
	{
		if ((mPlayerDirection.y > -0.3f) && (mPlayerDirection.y < 0.3f))
			curBossDirection = EDirection::Left;
	}
	else
	{
		if ((mPlayerDirection.y > -0.3f) && (mPlayerDirection.y < 0.3f))
			curBossDirection = EDirection::Right;
	}
}

void Boss::CheckSpeedXY()
{
	mSpeedX = mPlayerDirection.x * mMoveSpeed * Timer::GetDeltaTime();
	mSpeedY = mPlayerDirection.y * mMoveSpeed * Timer::GetDeltaTime();
}

void Boss::CheckSprite()
{
	if (Timer::Elapsed(mLastIdleTime, mIdleFrameInterval) && (curBossAction == EAction::Idle) && !(curBossAction == EAction::Dead))
	{
		NextPattern();
	}

	if (curBossAction == EAction::Dead)
	{
		mCurSpritePtr = mDeadSpritePtr->GetSprite(curBossDirection);
	}
	else if (!mIsRoll)
	{
		if (curBossAction == EAction::Sleep)
		{
			mCurSpritePtr = mSleepSpritePtr;
		}
		else
		{
			mCurSpritePtr = !(curBossAction == EAction::Roll)
				? mIdleSpritePtr->GetSprite(curBossDirection)
				: mRollSpritePtr->GetSprite(curBossDirection);
			if (curBossAction == EAction::Attack)
				mCurSpritePtr = mAttackSpritePtr->GetSprite(curBossDirection);
		}
	}
	if (mOldSpritePtr != mCurSpritePtr && (!mIsRoll || (curBossAction == EAction::Dead)))
	{
		mOldSpritePtr = mCurSpritePtr;
		GameObject::SetSprite(mCurSpritePtr);
		switch (curBossAction)
		{
		case EAction::Idle:
			SetFrameInterval(300);
			break;
		case EAction::Roll:
			SetFrameInterval(50);
			break;
		case EAction::Attack:
			SetFrameInterval(100);
			break;
		case EAction::Dead:
			SetFrameInterval(300);
		}

		if (curBossAction == EAction::Roll)
			mIsRoll = true;
	}

	switch (curBossAction)
	{
	case EAction::Idle:
		SetHitRect(mIdleHitRect[mCurrentFrame]);
		break;
	case EAction::Roll:
		SetHitRect(mRollHitRect);
		break;
	case EAction::Attack:
		SetHitRect(mAttackHitRect[mCurrentFrame]);
		break;
	case EAction::Sleep:
		SetHitRect(mSleepHitRect);
		break;
	}
}

void Boss::MoveANDCheckState()
{
	if ((abs(static_cast<long>(mSpeedX)) < 0.1f) && (abs(static_cast<long>(mSpeedY)) < 0.1f) && mIsHitWall)
	{
		mSpeedX = 0;
		mSpeedY = 0;
		NextPattern();
		mIsHitWall = false;
		mHitWallCnt = 0;
	}

	if (mIsHitWall)
	{
		mSpeedX -= mSpeedX * (0.03f * mHitWallCnt);
		mSpeedY -= mSpeedY * (0.03f * mHitWallCnt);
		mHitWallCnt++;
	}

	if (curBossAction == EAction::Roll) //공격중이거나 기본상태가 아닐경우 캐릭터를 움직여준다
	{
		SetX(GetPos().x + mSpeedX);
		SetY(GetPos().y + mSpeedY);
	}

	RECT rect;
	if (!((GetPos().x > 1100) && (GetPos().x < 2100) && (GetPos().y > 1400) && (GetPos().y < 2100)))
	{
		for (const Block& block : wall)
		{
			SetRect(&rect, block.GetPos().x * 32, block.GetPos().y * 32,
				block.GetPos().x * 32 + 32, block.GetPos().y * 32 + 32);

			if (CheckHit(rect))
			{
				SetX(GetPos().x - mSpeedX);
				SetY(GetPos().y - mSpeedY);
				mSpeedX = -mSpeedX;
				mSpeedY = -mSpeedY;
				mIsHitWall = true;
				cntVibes = 0;
				boss_HitWall = true;
				SndObjPlay(Sound[6], NULL);
			}
		}
	}

	mDrawX = -(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f);
	mDrawY = -(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f);
}

void Boss::MoveInit()
{
	mSpeedX = 0;
	mSpeedY = 0;
}

void Boss::NextPattern()
{
	if ((EAction)mPattern[mCurPatternIndex] == EAction::Roll)
		mIdleFrameInterval = 1500;
	else
		mIdleFrameInterval = 300;

	if (++mCurPatternIndex % 9 == 0)
	{
		mCurPatternIndex = 1;
		mAttackCnt = 0;
	}

	switch (mPattern[mCurPatternIndex])
	{
	case EAction::Idle:
		mLastIdleTime = Timer::Now();
		mIsRoll = false;
		MoveInit();
		break;
	case EAction::Roll:
		CheckDirectionState();
		CheckSpeedXY();
		break;
	case EAction::Attack:
		CheckDirectionState();
		mIsRoll = false;
		break;
	}

	curBossAction = mPattern[mCurPatternIndex];
}
