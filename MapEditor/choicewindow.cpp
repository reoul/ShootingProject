#include "choicewindow.h"
#include "block.h"
#include "define.h"
#include "define.h"
#include "gameEnum.h"

extern CSprite blockSprite[4][TOTAL_BLOCK_SPRITE];
extern CSprite blockSprite2[TOTAL_BLOCK_SPRITE_Y][TOTAL_BLOCK_SPRITE_X];
extern CBLOCK curBlock;
extern EDIT_WINDOW curEditWindow;

extern enum EDIT_STATE;


CChoiceWindow::CChoiceWindow()
{

}

CChoiceWindow::~CChoiceWindow()
{

}

void CChoiceWindow::Initialize()
{
	m_x = 700;
	m_y = 600; 
	SetRect(&windowRect, 200, 200, 200, 200);
	windowSprite = &blockSprite[0][0];
	canMove = false;
}

bool CChoiceWindow::Drawing(LPDIRECTDRAWSURFACE7 pSurface)
{
	SetRect(&rect, m_x - windowRect.left, m_y - windowRect.top, m_x + windowRect.right, m_y + windowRect.bottom);
	windowSprite->Drawing(rect, pSurface, false);
	if (curEditWindow == EDIT_WINDOW::EDIT_BLOCK_WINDOW1)
	{
		for (int j = 0; j < (TOTAL_BLOCK_SPRITE / 10) - 1; j++)
		{
			/*if (j > 2)
			break;*/
			for (int i = 0; i < 10; i++)
			{
				if (blockSprite[0][10 + i + j * 10].GetNumber() == -1)
					continue;
				/*if (j>1 && i > 5)
				break;*/
				SetRect(&rect, m_x - windowRect.left + 10 + (i * 16) + i, m_y - windowRect.top + 10 + (j * 16) + j, m_x - windowRect.left + (i * 16) + i + 10 + 16,
					m_y - windowRect.top + 10 + (j * 16) + j + 16);
				blockSprite[0][10 + i + j * 10].Drawing(rect, pSurface, false);
			}
		}
	}
	else if (curEditWindow == EDIT_WINDOW::EDIT_BLOCK_WINDOW2)
	{
		for (int y = 0; y < TOTAL_BLOCK_SPRITE_Y; y++)
		{
			for (int x = 0; x < TOTAL_BLOCK_SPRITE_X; x++)
			{
				//if (blockSprite2[0][x + y * TOTAL_BLOCK_SPRITE_X].GetNumber() == -1)
				//	continue;
				/*if (j>1 && i > 5)
				break;*/
				SetRect(&rect, m_x - windowRect.left + 10 + (x * 16) + x, m_y - windowRect.top + 10 + (y * 16) + y, m_x - windowRect.left + (x * 16) + x + 10 + 16,
					m_y - windowRect.top + 10 + (y * 16) + y + 16);
				blockSprite2[y][x].Drawing(rect, pSurface, true);
			}
		}
	}
	return true;
}

RECT CChoiceWindow::GetRect()
{
	SetRect(&rect, m_x - windowRect.left, m_y - windowRect.top, m_x + windowRect.right, m_y + windowRect.bottom);
	return rect;
}

void CChoiceWindow::CheckBlockClick(int x, int y)
{
	RECT rect1;
	RECT rect2;
	RECT rect3;
	SetRect(&rect, m_x - windowRect.left, m_y - windowRect.top, m_x - windowRect.left + 10, m_y - windowRect.top + 10);
	if (rect.left < x && rect.top < y && rect.right > x && rect.bottom > y)		//블럭창을 클릭했을때
	{
		canMove = true;
		moveXY.SetRect(rect.left - x, rect.top - y);
	}
	else
	{
		if (curEditWindow == EDIT_WINDOW::EDIT_BLOCK_WINDOW1)
		{
			for (int j = 0; j < (TOTAL_BLOCK_SPRITE / 10) - 1; j++)
			{
				for (int i = 0; i < 10; i++)
				{
					if (blockSprite[0][10 + i + j * 10].GetNumber() == -1)
						continue;
					/*if (j>1 && i > 5)
					break;*/
					SetRect(&rect, m_x - windowRect.left + 10 + (i * 16) + i, m_y - windowRect.top + 10 + (j * 16) + j, 
						m_x - windowRect.left + (i * 16) + i + 10 + 16, m_y - windowRect.top + 10 + (j * 16) + j + 16);
					if (rect.left < x && rect.top < y && rect.right > x && rect.bottom > y)
					{
						curBlock.SetSprite(&blockSprite[0][10 + i + j * 10]);
					}
				}
			}
		}
		if (curEditWindow == EDIT_WINDOW::EDIT_BLOCK_WINDOW2)
		{
			for (int _y = 0; _y < TOTAL_BLOCK_SPRITE_Y; _y++)
			{
				for (int _x = 0; _x < TOTAL_BLOCK_SPRITE_X; _x++)
				{
					SetRect(&rect, m_x - windowRect.left + 10 + (_x * 16) + _x, m_y - windowRect.top + 10 + (_y * 16) + _y,
						m_x - windowRect.left + (_x * 16) + _x + 10 + 16, m_y - windowRect.top + 10 + (_y * 16) + _y + 16);
					if (rect.left < x && rect.top < y && rect.right > x && rect.bottom > y)
					{
						curBlock.SetSprite(&blockSprite2[_y][_x]);
					}
				}
			}
		}
	}
}

void CChoiceWindow::SetCanMove(bool move)
{
	canMove = move;
}


bool CChoiceWindow::GetCanMove()
{
	return canMove;
}

void CChoiceWindow::SetXY(int x, int y)
{
	m_x = x + windowRect.left - moveXY.x;
	m_y = y + windowRect.top - moveXY.y;
}