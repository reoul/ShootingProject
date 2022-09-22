#include "SkillUI.h"

SkillUI::SkillUI() : mBackGroundSpritePtr(nullptr)
{
}

void SkillUI::Drawing(float skillCoolTimePercent, LPDIRECTDRAWSURFACE7 lpSurface) const
{
	mBackGroundSpritePtr->Drawing(0, mX, mY, lpSurface, true);
	mSpritePtr->DrawingSkillCoolTime(skillCoolTimePercent, mX, mY, lpSurface, true);
}
