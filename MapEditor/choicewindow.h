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
	void CheckBlockClick(int x, int y);		//�����Ϳ��� ���� Ŭ���ߴ��� Ȯ���ϴ� �Լ�
	void SetCanMove(bool move);
	bool GetCanMove();
	void SetXY(int x,int y);		//������ ��â ��ġ ����
};
#endif