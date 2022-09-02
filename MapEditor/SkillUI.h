#pragma once
#include "Gui.h"

class Sprite;

class SkillUI : public Gui
{
public:
	SkillUI();
	~SkillUI();
	void SetBackGroundSprite(Sprite* sprite);
	void Drawing(float skillCoolTimePercent, LPDIRECTDRAWSURFACE7 lpsurface);
private:
	Sprite* m_pBackGroundSprite;
};
