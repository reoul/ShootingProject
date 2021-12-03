#ifndef __fireblock_h__
#define __fireblock_h__

#include "gobject.h"
#include "sprite.h"

class FireBlock : public CGObject
{
private:
	int draw_x;
	int draw_y;

public:
	FireBlock();
	~FireBlock();
	void Initialize(int x, int y, CTimer* timer, int frameInterval, CSprite* sprite);
	void Draw(LPDIRECTDRAWSURFACE7 surface);
};

#endif
