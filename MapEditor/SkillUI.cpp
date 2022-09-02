#include "SkillUI.h"

SkillUI::SkillUI()
{
	m_pBackGroundSprite = NULL;
}

SkillUI::~SkillUI()
{
}

void SkillUI::SetBackGroundSprite(Sprite* sprite)
{
	m_pBackGroundSprite = sprite;
}

void SkillUI::Drawing(float skillCoolTimePercent, LPDIRECTDRAWSURFACE7 lpsurface)
{
	m_pBackGroundSprite->Drawing(0, m_x, m_y, lpsurface, true);
	m_pSprite->DrawingSkillCoolTime(skillCoolTimePercent, m_x, m_y, lpsurface, true);
}
