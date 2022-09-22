#include "Sprite8.h"
#include "Sprite.h"

void Sprite8::Init()
{
	mLeftSpritePtr = std::make_unique<Sprite>();
	mLeftUpSpritePtr = std::make_unique<Sprite>();
	mLeftDownSpritePtr = std::make_unique<Sprite>();
	mRightSpritePtr = std::make_unique<Sprite>();
	mRightUpSpritePtr = std::make_unique<Sprite>();
	mRightDownSpritePtr = std::make_unique<Sprite>();
	mUpSpritePtr = std::make_unique<Sprite>();
	mDownSpritePtr = std::make_unique<Sprite>();
}

Sprite* Sprite8::GetSprite(EDirection type) const
{
	switch (type)
	{
	case EDirection::Left:
		return mLeftSpritePtr.get();
	case EDirection::Right:
		return mRightSpritePtr.get();
	case EDirection::Up:
		return mUpSpritePtr.get();
	case EDirection::Down:
		return mDownSpritePtr.get();
	case EDirection::LeftUp:
		return mLeftUpSpritePtr.get();
	case EDirection::RightUp:
		return mRightUpSpritePtr.get();
	case EDirection::LeftDown:
		return mLeftDownSpritePtr.get();
	case EDirection::RightDown:
		return mRightDownSpritePtr.get();
	default:
		return mLeftSpritePtr.get();
	}
}

void Sprite8::SetSizeAll(float size) const
{
	mLeftSpritePtr->SetSize(size);
	mLeftUpSpritePtr->SetSize(size);
	mLeftDownSpritePtr->SetSize(size);
	mRightSpritePtr->SetSize(size);
	mRightUpSpritePtr->SetSize(size);
	mRightDownSpritePtr->SetSize(size);
	mUpSpritePtr->SetSize(size);
	mDownSpritePtr->SetSize(size);
}

bool Sprite8::ReleaseAll() const
{
	if (!mLeftSpritePtr->ReleaseAll())
		return false;
	if (!mLeftUpSpritePtr->ReleaseAll())
		return false;
	if (!mLeftDownSpritePtr->ReleaseAll())
		return false;
	if (!mRightSpritePtr->ReleaseAll())
		return false;
	if (!mRightUpSpritePtr->ReleaseAll())
		return false;
	if (!mRightDownSpritePtr->ReleaseAll())
		return false;
	if (!mUpSpritePtr->ReleaseAll())
		return false;
	if (!mDownSpritePtr->ReleaseAll())
		return false;

	return true;
}

void Sprite8::ReStoreAll() const
{
	mLeftSpritePtr->Restore();
	mLeftUpSpritePtr->Restore();
	mLeftDownSpritePtr->Restore();
	mRightSpritePtr->Restore();
	mRightUpSpritePtr->Restore();
	mRightDownSpritePtr->Restore();
	mUpSpritePtr->Restore();
	mDownSpritePtr->Restore();
}
