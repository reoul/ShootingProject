#pragma once
#include <memory>

#include "GameEnum.h"

class Sprite;

class Sprite8
{
public:
	Sprite8() = default;
	~Sprite8() = default;
	void Init();
	Sprite* GetSprite(EDirection type) const;
	void SetSizeAll(float size) const;
	bool ReleaseAll() const;
	void ReStoreAll() const;
private:
	std::unique_ptr<Sprite> mLeftSpritePtr;
	std::unique_ptr<Sprite> mLeftUpSpritePtr;
	std::unique_ptr<Sprite> mLeftDownSpritePtr;
	std::unique_ptr<Sprite> mRightSpritePtr;
	std::unique_ptr<Sprite> mRightUpSpritePtr;
	std::unique_ptr<Sprite> mRightDownSpritePtr;
	std::unique_ptr<Sprite> mUpSpritePtr;
	std::unique_ptr<Sprite> mDownSpritePtr;
};
