#ifndef __sprite8_h_
#define __sprite8_h_

#include "sprite.h"
#include "gameEnum.h"

class CSprite8
{
private:
	CSprite *left;
	CSprite *leftup;
	CSprite *leftdown;
	CSprite *right;
	CSprite *rightup;
	CSprite *rightdown;
	CSprite *up;
	CSprite *down;
public:
	CSprite8();
	~CSprite8();
	void Init();
	CSprite* GetSprite(DISTANCE_TYPE type);
	CSprite* GetLeft();
	CSprite* GetLeftUp();
	CSprite* GetLeftDown();
	CSprite* GetRight();
	CSprite* GetRightUp();
	CSprite* GetRightDown();
	CSprite* GetUp();
	CSprite* GetDown();
	bool ReleaseAll();
	void ReStoreAll();
};
#endif