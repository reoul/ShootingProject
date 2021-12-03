#include "oneclick.h"

COneClick::COneClick()
{
	m_bIsClick = false;
}

COneClick::~COneClick()
{
}

void COneClick::Click()
{
	m_bIsClick = true;
}