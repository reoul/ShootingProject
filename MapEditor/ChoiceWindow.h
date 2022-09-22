#pragma once
#include <ddraw.h>

#include "Sprite.h"
#include "Vector2.h"

class ChoiceWindow
{
public:
	ChoiceWindow();
	~ChoiceWindow() = default;
	void Initialize();
	bool Drawing(LPDIRECTDRAWSURFACE7 pSurface) const;
	RECT GetRect() const;
	void CheckBlockClick(int x, int y); //에디터에서 블럭을 클릭했는지 확인하는 함수
	void SetCanMove(bool move);
	bool CanMove() const;
	void SetXY(int x, int y); //에디터 블럭창 위치 설정
private:
	int mX;
	int mY;
	Sprite* mWindowSpritePtr;
	RECT mWindowRect;
	bool mCanMove;
	Vector2 mMoveXY;
};

inline RECT ChoiceWindow::GetRect() const
{
	return { mX - mWindowRect.left, mY - mWindowRect.top, mX + mWindowRect.right, mY + mWindowRect.bottom };
}

inline void ChoiceWindow::SetCanMove(bool move)
{
	mCanMove = move;
}


inline bool ChoiceWindow::CanMove() const
{
	return mCanMove;
}

inline void ChoiceWindow::SetXY(int x, int y)
{
	mX = x + mWindowRect.left - mMoveXY.x;
	mY = y + mWindowRect.top - mMoveXY.y;
}
