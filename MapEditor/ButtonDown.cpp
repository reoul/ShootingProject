#include "ButtonDown.h"

ButtonDown::ButtonDown()
{
	count = 0;
}

ButtonDown::~ButtonDown()
{
}

void ButtonDown::KeyDown()
{
	count++;
}

void ButtonDown::KeyInitDown()
{
	count = 0;
}

void ButtonDown::KeyCheckUp()
{
	if (count > 0)
		m_bIsDown = true;
	else
		m_bIsDown = false;
}

bool ButtonDown::KeyCheckClick() const
{
	return m_bIsDown == true ? false : true;
}
