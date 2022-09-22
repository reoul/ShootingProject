#include "GameObject.h"
#include "Player.h"
#include "Boss.h"
#include "Camera.h"
#include "GameEnum.h"

enum class EAction; //플레이어가 무슨 행동을 취하는지
extern EAction curPlayerAction;
extern EAction curBossAction;

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

void GameObject::Initialize(Sprite* pSprite, int x, int y, int curFrame, int frameInterval, int skillFrameInterval)
{
	mSprite = pSprite;
	mCurrentFrame = curFrame;
	mX = static_cast<float>(x);
	mY = static_cast<float>(y);
	mFrameInterval = frameInterval;
	mSkillFrameInterval = skillFrameInterval;
	mIsLive = true;
	mCurState = EObjectType::Player;
	mLastFrameTime = Timer::Now();
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
	case EObjectType::Player: //플레이어
		if (Timer::Elapsed(mLastFrameTime, mFrameInterval))
		{
			if ((curPlayerAction == EAction::Roll) && ((mCurrentFrame + 1) == mSprite->GetNumberOfFrame()))
			{
				curPlayerAction = EAction::Move;
				isMove = true;
				player.MoveANDCheckState();
				mCurrentFrame = mSprite->GetNumberOfFrame() - 1;
			}

			if (isMove)
				mCurrentFrame = ++mCurrentFrame % mSprite->GetNumberOfFrame();
			else
				mCurrentFrame = 0;
		}

		if (curPlayerAction != EAction::Dead)
		{
			if (mBowSprite != nullptr)
				mBowSprite->Drawing(mCurrentFrame, x, y, lpSurface);
		}
		break;
	case EObjectType::Arrow: //화살
		mSprite->Rotate(mDirection.VectorToAngle(), mCurrentFrame);
		break;
	case EObjectType::Boss: //보스
		if (Timer::Elapsed(mLastFrameTime, mFrameInterval))
		{
			if (!(curBossAction == EAction::Dead))
			{
				if (curBossAction == EAction::Attack && mCurrentFrame == 2)
					boss.Attack();

				mCurrentFrame = ++mCurrentFrame % mSprite->GetNumberOfFrame();

				if (curBossAction == EAction::Attack && mCurrentFrame == 0)
					boss.CheckDirectionState();
			}
			else
			{
				mCurrentFrame = ++mCurrentFrame;
				if (mCurrentFrame == mSprite->GetNumberOfFrame())
					mCurrentFrame = mSprite->GetNumberOfFrame() - 1;
			}
		}
		break;
	case EObjectType::Snowball: //눈덩이
		break;
	case EObjectType::Block: //블럭
		if (Timer::Elapsed(mLastFrameTime, mFrameInterval))
		{
			mCurrentFrame = ++mCurrentFrame % mSprite->GetNumberOfFrame();
		}
		break;
	case EObjectType::EditorBlock: //에디터 블록
		mSprite->BlockDrawing(x, y, lpSurface);
		return;
	case EObjectType::Enemy: //적
		if (Timer::Elapsed(mLastFrameTime, mFrameInterval))
			mCurrentFrame = ++mCurrentFrame % mSprite->GetNumberOfFrame();
		break;
	default:
		break;
	}
	if (mSprite != nullptr)
		mSprite->Drawing(mCurrentFrame, x, y, lpSurface);
}

void GameObject::SkillDraw(int x, int y, LPDIRECTDRAWSURFACE7 lpSurface)
{
	if (!mIsLive)
		return;
	if (Timer::Elapsed(mLastSkillFrameTime, mSkillFrameInterval))
		mCurrentFrame = ++mCurrentFrame % mSprite->GetNumberOfFrame();
	mSprite->Drawing(mCurrentFrame, x, y, lpSurface);
}

RECT GameObject::GetHitRect() const
{
	RECT rect;
	rect.left = GetPos().x - mHitRect.left;
	rect.top = GetPos().y - mHitRect.top;
	rect.right = GetPos().x + mHitRect.right;
	rect.bottom = GetPos().y + mHitRect.bottom;
	return rect;
}

RECT GameObject::GetHitRectImageRect() const
{
	RECT rect;
	rect.left = GetPos().x - (mSprite->GetWidth() >> 1);
	rect.top = GetPos().y - (mSprite->GetHeight() >> 1);
	rect.right = GetPos().x + (mSprite->GetWidth() >> 1);
	rect.bottom = GetPos().y + (mSprite->GetHeight() >> 1);
	return rect;
}

RECT GameObject::GetHitRectWallRect() const
{
	RECT rect;
	rect.left = GetPos().x - mHitWallRect.left;
	rect.top = GetPos().y - mHitWallRect.top;
	rect.right = GetPos().x + mHitWallRect.right;
	rect.bottom = GetPos().y + mHitWallRect.bottom;
	return rect;
}

bool GameObject::CheckHit(RECT rect) const
{
	RECT myHitRect = GetHitRect();
	RECT hitRect;
	if (IntersectRect(&hitRect, &myHitRect, &rect))
		return true;

	return false;
}

bool GameObject::CheckHitImageRect(RECT rect) const
{
	RECT myHitRect = GetHitRectImageRect();
	RECT hitRect;
	if (IntersectRect(&hitRect, &myHitRect, &rect))
		return true;

	return false;
}

bool GameObject::CheckHitWallRect(RECT rect) const
{
	RECT myHitRect = GetHitRectWallRect();
	RECT hitRect;
	if (IntersectRect(&hitRect, &myHitRect, &rect))
		return true;

	return false;
}

RECT GameObject::GetHitRect2(RECT rect) const
{
	RECT myHitRect = GetHitRectWallRect();
	RECT hitRect;
	IntersectRect(&hitRect, &myHitRect, &rect);
	return hitRect;
}
