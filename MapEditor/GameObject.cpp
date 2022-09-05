#include "GameObject.h"
#include "Player.h"
#include "Boss.h"
#include "Camera.h"
#include "GameEnum.h"

extern enum ACTION; //플레이어가 무슨 행동을 취하는지
extern ACTION curPlayerAction;
extern ACTION curBossAction;

extern Player player;
extern Boss boss;
extern Camera camera;

GameObject::GameObject()
	: mSprite(nullptr)
	, mBowSprite(nullptr)
	, mFrameInterval(0)
	, mCurrentFrame(0)
	, mLastFrameTime()
	, mSkillFrameInterval(0)
	, mLastSkillFrameTime()
	, mMoveSpeed(0)
	, mIsLive(false)
	, mCurState()
	, mHitRect()
	, mHitWallRect()
	, mDegree(0)
	, mX(0)
	, mY(0)
{
}

GameObject::~GameObject()
{
}

void GameObject::Initialize(Sprite* pSprite, int x, int y, int CurrentFrame, int FrameInterval,
	int SkillFrameInterval)
{
	mSprite = pSprite;
	mCurrentFrame = CurrentFrame;
	mX = static_cast<float>(x);
	mY = static_cast<float>(y);
	mFrameInterval = FrameInterval;
	mSkillFrameInterval = SkillFrameInterval;
	mIsLive = true;
	mCurState = OBJECT_TYPE::PLYAER;
	mLastFrameTime = Timer::Now();
}

Sprite* GameObject::GetSprite()
{
	return mSprite;
}

bool GameObject::IsLive()
{
	return mIsLive;
}

void GameObject::Kill()
{
	mIsLive = false;
}

void GameObject::SetState(OBJECT_TYPE state)
{
	mCurState = state;
}

void GameObject::SetSprite(Sprite* sprite)
{
	mCurrentFrame = 0;
	mSprite = sprite;
}

void GameObject::SetSprite(Sprite* sprite, Sprite* bowSprite)
{
	if (!player.IsUsingSkill())
		mCurrentFrame = 0;
	mSprite = sprite;
	mBowSprite = bowSprite;
}

void GameObject::Draw(bool isMove, int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!mIsLive)
		return;
	switch (mCurState)
	{
	case OBJECT_TYPE::PLYAER: //플레이어
		if (Timer::Elapsed(mLastFrameTime, mFrameInterval))
		{
			if ((curPlayerAction == ACTION::ROLL) && ((mCurrentFrame + 1) == mSprite->GetNumberOfFrame()))
			{
				curPlayerAction = ACTION::MOVE;
				isMove = true;
				player.MoveANDCheckState();
				mCurrentFrame = mSprite->GetNumberOfFrame() - 1;
			}
			if (isMove)
				mCurrentFrame = ++mCurrentFrame % mSprite->GetNumberOfFrame();
			else
				mCurrentFrame = 0;
		}
		mSprite->Drawing(mCurrentFrame, x, y, lpSurface);
		if (curPlayerAction != 6)
			mBowSprite->Drawing(mCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_TYPE::ARROW: //화살
		mSprite->Rotate(mDirection.VectorToAngle(), mCurrentFrame);
		mSprite->Drawing(mCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_TYPE::BOSS: //보스
		if (Timer::Elapsed(mLastFrameTime, mFrameInterval))
		{
			if (!(curBossAction == ACTION::DEAD))
			{
				if (curBossAction == ACTION::ATTACK)
				{
					if (mCurrentFrame == 2)
						boss.Attack();
				}
				mCurrentFrame = ++mCurrentFrame % mSprite->GetNumberOfFrame();
				if (curBossAction == ACTION::ATTACK)
					if (mCurrentFrame == 0)
						boss.CheckDirectionState();
			}
			else
			{
				mCurrentFrame = ++mCurrentFrame;
				if (mCurrentFrame == mSprite->GetNumberOfFrame())
					mCurrentFrame = mSprite->GetNumberOfFrame() - 1;
			}
		}
		mSprite->Drawing(mCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_TYPE::SNOWBALL: //눈덩이
		mSprite->Drawing(mCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_TYPE::BLOCK: //블럭
		if (Timer::Elapsed(mLastFrameTime, mFrameInterval))
		{
			mCurrentFrame = ++mCurrentFrame % mSprite->GetNumberOfFrame();
		}
		mSprite->Drawing(mCurrentFrame, x, y, lpSurface);
		break;
	case OBJECT_TYPE::EDITOR_BLOCK: //에디터 블록
		mSprite->BlockDrawing(x, y, lpSurface);
		break;
	case OBJECT_TYPE::ENEMY: //적
		if (Timer::Elapsed(mLastFrameTime, mFrameInterval))
		{
			mCurrentFrame = ++mCurrentFrame % mSprite->GetNumberOfFrame();
		}
		mSprite->Drawing(mCurrentFrame, x, y, lpSurface);
		break;
	default:
		mSprite->Drawing(mCurrentFrame, x, y, lpSurface);
		break;
	}
}

void GameObject::SkillDraw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!mIsLive)
		return;
	if (Timer::Elapsed(mLastSkillFrameTime, mSkillFrameInterval))
		mCurrentFrame = ++mCurrentFrame % mSprite->GetNumberOfFrame();
	mSprite->Drawing(mCurrentFrame, x, y, lpSurface);
}

Vector2 GameObject::GetPos()
{
	return Vector2(mX, mY);
}

void GameObject::SetX(float x)
{
	mX = x;
}

void GameObject::SetY(float y)
{
	mY = y;
}

void GameObject::SetXY(float x, float y)
{
	mX = x;
	mY = y;
}

void GameObject::SetFrameInterval(int frame)
{
	mFrameInterval = frame;
}

void GameObject::SetHitRect(int left, int top, int right, int bottom)
{
	SetRect(&mHitRect, left, top, right, bottom);
}

void GameObject::SetHitRect(RECT rect)
{
	SetRect(&mHitRect, rect.left, rect.top, rect.right, rect.bottom);
}

void GameObject::SetHitWallRect()
{
	SetRect(&mHitWallRect, mSprite->GetWidth() >> 1, mSprite->GetHeight() >> 1, mSprite->GetWidth() >> 1,
		mSprite->GetHeight() >> 1);
}

RECT GameObject::GetHitRect()
{
	RECT rect;
	rect.left = GetPos().x - mHitRect.left;
	rect.top = GetPos().y - mHitRect.top;
	rect.right = GetPos().x + mHitRect.right;
	rect.bottom = GetPos().y + mHitRect.bottom;
	return rect;
}

RECT GameObject::GetHitRectImageRect()
{
	RECT rect;
	rect.left = GetPos().x - (mSprite->GetWidth() >> 1);
	rect.top = GetPos().y - (mSprite->GetHeight() >> 1);
	rect.right = GetPos().x + (mSprite->GetWidth() >> 1);
	rect.bottom = GetPos().y + (mSprite->GetHeight() >> 1);
	return rect;
}

RECT GameObject::GetHitRectWallRect()
{
	RECT rect;
	rect.left = GetPos().x - mHitWallRect.left;
	rect.top = GetPos().y - mHitWallRect.top;
	rect.right = GetPos().x + mHitWallRect.right;
	rect.bottom = GetPos().y + mHitWallRect.bottom;
	return rect;
}

bool GameObject::CheckHit(RECT rect)
{
	RECT myHitRect = GetHitRect();
	RECT _hitrect;
	if (IntersectRect(&_hitrect, &myHitRect, &rect))
	{
		return true;
	}
	return false;
}

bool GameObject::CheckHitImageRect(RECT rect)
{
	RECT myHitRect = GetHitRectImageRect();
	RECT _hitrect;
	if (IntersectRect(&_hitrect, &myHitRect, &rect))
	{
		return true;
	}
	return false;
}

bool GameObject::CheckHitWallRect(RECT rect)
{
	RECT myHitRect = GetHitRectWallRect();
	RECT _hitrect;
	if (IntersectRect(&_hitrect, &myHitRect, &rect))
	{
		return true;
	}
	return false;
}

RECT GameObject::GetHitRect2(RECT rect)
{
	RECT myHitRect = GetHitRectWallRect();
	RECT _hitrect;
	IntersectRect(&_hitrect, &myHitRect, &rect);
	return _hitrect;
}

void GameObject::SetDirection(Vector2 _direction)
{
	mDirection = _direction;
}

Vector2 GameObject::GetDirection()
{
	return mDirection;
}
