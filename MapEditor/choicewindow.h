#ifndef __choicewindow_h_
#define __choicewindow_h_

#include <ddraw.h>

#include "sprite.h"
#include "Vector2.h"

class CChoiceWindow
{
private:
	int m_x;
	int m_y;
	CSprite* windowSprite;
	RECT windowRect;
	RECT rect;
	bool canMove;
	Vector2 moveXY;
public:
	CChoiceWindow();
	~CChoiceWindow();
	void Initialize();
	bool Drawing(LPDIRECTDRAWSURFACE7 pSurface);
	RECT GetRect();
	void CheckBlockClick(int x, int y);		//에디터에서 블럭을 클릭했는지 확인하는 함수
	void SetCanMove(bool move);
	bool GetCanMove();
	void SetXY(int x,int y);		//에디터 블럭창 위치 설정
};
#endif