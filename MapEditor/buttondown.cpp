#include "buttondown.h"

CButtonDown::CButtonDown()
{
	count = 0;
}

CButtonDown::~CButtonDown()
{
}

void CButtonDown::KeyDown()
{
	count++;
}

void CButtonDown::KeyInitDown()
{
	count = 0;
}
void CButtonDown::KeyCheckUp()
{
	if (count > 0)
		m_bIsDown = true;
	else
		m_bIsDown = false;
}

bool CButtonDown::KeyCheckClick()
{
	return m_bIsDown == true ? false : true;
}