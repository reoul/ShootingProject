#pragma once
#include "Gui.h"

class Sprite;

class SkillUI : public Gui
{
public:
	SkillUI();
	~SkillUI() = default;
	void SetBackGroundSprite(Sprite* spritePtr);
	void Drawing(float skillCoolTimePercent, LPDIRECTDRAWSURFACE7 lpSurface) const;
private:
	Sprite* mBackGroundSpritePtr;
};

inline void SkillUI::SetBackGroundSprite(Sprite* spritePtr)
{
	mBackGroundSpritePtr = spritePtr;
}
