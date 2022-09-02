#include "Sprite8.h"
#include "Sprite.h"

CSprite8::CSprite8()
{
}

CSprite8::~CSprite8()
{
	delete left;
	delete leftup;
	delete leftdown;
	delete right;
	delete rightup;
	delete rightdown;
	delete up;
	delete down;
}

void CSprite8::Init()
{
	left = new Sprite();
	leftup = new Sprite();
	leftdown = new Sprite();
	right = new Sprite();
	rightup = new Sprite();
	rightdown = new Sprite();
	up = new Sprite();
	down = new Sprite();
}

Sprite* CSprite8::GetSprite(DIRECTION type)
{
	switch (type)
	{
	case LEFT:
		return left;
	case RIGHT:
		return right;
	case UP:
		return up;
	case DOWN:
		return down;
	case LEFTUP:
		return leftup;
	case RIGHTUP:
		return rightup;
	case LEFTDOWN:
		return leftdown;
	case RIGHTDOWN:
		return rightdown;
	default:
		return left;
	}
}

void CSprite8::SetSizeAll(float size)
{
	left->SetSize(size);
	leftup->SetSize(size);
	leftdown->SetSize(size);
	right->SetSize(size);
	rightup->SetSize(size);
	rightdown->SetSize(size);
	up->SetSize(size);
	down->SetSize(size);
}

bool CSprite8::ReleaseAll()
{
	if (!left->ReleaseAll())
		return false;
	if (!leftup->ReleaseAll())
		return false;
	if (!leftdown->ReleaseAll())
		return false;
	if (!right->ReleaseAll())
		return false;
	if (!rightup->ReleaseAll())
		return false;
	if (!rightdown->ReleaseAll())
		return false;
	if (!up->ReleaseAll())
		return false;
	if (!down->ReleaseAll())
		return false;

	return true;
}

void CSprite8::ReStoreAll()
{
	left->Restore();
	leftup->Restore();
	leftdown->Restore();
	right->Restore();
	rightup->Restore();
	rightdown->Restore();
	up->Restore();
	down->Restore();
}
