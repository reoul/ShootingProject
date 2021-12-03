#ifndef __block_h__
#define __block_h__

#include "gobject.h"

class CBLOCK : public CGObject
{
private:
	int number;
public:
	CBLOCK();
	~CBLOCK();
	void Initialize(CSprite* pSprite, int x, int y, CTimer* timer, int CurrentFrame, int FrameInterval);
	void Draw(int x,int y,LPDIRECTDRAWSURFACE7 lpSurface);
	void SetSprite(CSprite* sprite);
	void SetBlockNumber(int _number);
	int GetBlockNumber();
};
#endif