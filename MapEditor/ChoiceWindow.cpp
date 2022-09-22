#include "ChoiceWindow.h"
#include "Block.h"
#include "SettingData.h"
#include "GameEnum.h"

extern Sprite blockSprite[4][TOTAL_BLOCK_SPRITE];
extern Sprite blockSprite2[TOTAL_BLOCK_SPRITE_Y][TOTAL_BLOCK_SPRITE_X];
extern Block curBlock;
extern EEditWindow curEditWindow;

enum class EEditState;


ChoiceWindow::ChoiceWindow()
	: mX(700)
	, mY(600)
	, mWindowSpritePtr(&blockSprite[0][0])
	, mWindowRect{ 200,200,200,200 }
	, mCanMove(false)
{
}

void ChoiceWindow::Initialize()
{
	new (this) ChoiceWindow();
}

bool ChoiceWindow::Drawing(LPDIRECTDRAWSURFACE7 pSurface) const
{
	RECT rect;
	SetRect(&rect, mX - mWindowRect.left, mY - mWindowRect.top, mX + mWindowRect.right, mY + mWindowRect.bottom);
	mWindowSpritePtr->Drawing(rect, pSurface, false);
	if (curEditWindow == EEditWindow::BlockWindow1)
	{
		for (int j = 0; j < (TOTAL_BLOCK_SPRITE / 10) - 1; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				if (blockSprite[0][10 + i + j * 10].GetNumber() == -1)
					continue;
				SetRect(&rect, mX - mWindowRect.left + 10 + (i * 16) + i, mY - mWindowRect.top + 10 + (j * 16) + j,
					mX - mWindowRect.left + (i * 16) + i + 10 + 16,
					mY - mWindowRect.top + 10 + (j * 16) + j + 16);
				blockSprite[0][10 + i + j * 10].Drawing(rect, pSurface, false);
			}
		}
	}
	else if (curEditWindow == EEditWindow::BlockWindow2)
	{
		for (int y = 0; y < TOTAL_BLOCK_SPRITE_Y; y++)
		{
			for (int x = 0; x < TOTAL_BLOCK_SPRITE_X; x++)
			{
				SetRect(&rect, mX - mWindowRect.left + 10 + (x * 16) + x, mY - mWindowRect.top + 10 + (y * 16) + y,
					mX - mWindowRect.left + (x * 16) + x + 10 + 16,
					mY - mWindowRect.top + 10 + (y * 16) + y + 16);
				blockSprite2[y][x].Drawing(rect, pSurface, true);
			}
		}
	}
	return true;
}

void ChoiceWindow::CheckBlockClick(int x, int y)
{
	RECT rect;
	SetRect(&rect, mX - mWindowRect.left, mY - mWindowRect.top, mX - mWindowRect.left + 10, mY - mWindowRect.top + 10);
	if (rect.left < x && rect.top < y && rect.right > x && rect.bottom > y) //블럭창을 클릭했을때
	{
		mCanMove = true;
		mMoveXY.SetXY(rect.left - x, rect.top - y);
	}
	else
	{
		if (curEditWindow == EEditWindow::BlockWindow1)
		{
			for (int j = 0; j < (TOTAL_BLOCK_SPRITE / 10) - 1; j++)
			{
				for (int i = 0; i < 10; i++)
				{
					if (blockSprite[0][10 + i + j * 10].GetNumber() == -1)
						continue;
					SetRect(&rect, mX - mWindowRect.left + 10 + (i * 16) + i, mY - mWindowRect.top + 10 + (j * 16) + j,
						mX - mWindowRect.left + (i * 16) + i + 10 + 16,
						mY - mWindowRect.top + 10 + (j * 16) + j + 16);
					if (rect.left < x && rect.top < y && rect.right > x && rect.bottom > y)
					{
						curBlock.SetSprite(&blockSprite[0][10 + i + j * 10]);
					}
				}
			}
		}
		if (curEditWindow == EEditWindow::BlockWindow2)
		{
			for (int y = 0; y < TOTAL_BLOCK_SPRITE_Y; y++)
			{
				for (int x = 0; x < TOTAL_BLOCK_SPRITE_X; x++)
				{
					SetRect(&rect, mX - mWindowRect.left + 10 + (x * 16) + x,
						mY - mWindowRect.top + 10 + (y * 16) + y,
						mX - mWindowRect.left + (x * 16) + x + 10 + 16,
						mY - mWindowRect.top + 10 + (y * 16) + y + 16);
					if (rect.left < x && rect.top < y && rect.right > x && rect.bottom > y)
					{
						curBlock.SetSprite(&blockSprite2[y][x]);
					}
				}
			}
		}
	}
}
