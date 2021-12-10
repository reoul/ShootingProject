#include "sprite8.h"

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
	left = new CSprite();
	leftup = new CSprite();
	leftdown = new CSprite();
	right = new CSprite();
	rightup	= new CSprite();
	rightdown = new CSprite();
	up	= new CSprite();
	down = new CSprite();
}

CSprite* CSprite8::GetSprite(DISTANCE_TYPE type)
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

CSprite* CSprite8::GetLeft()
{
	return left;
}
CSprite* CSprite8::GetLeftUp()
{
	return leftup;
}
CSprite* CSprite8::GetLeftDown()
{
	return leftdown;
}
CSprite* CSprite8::GetRight()
{
	return right;
}
CSprite* CSprite8::GetRightUp()
{
	return rightup;
}
CSprite* CSprite8::GetRightDown()
{
	return rightdown;
}
CSprite* CSprite8::GetUp()
{
	return up;
}
CSprite* CSprite8::GetDown()
{
	return down;
}