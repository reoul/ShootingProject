#include "myplayer.h"
#include "block.h"
#include "gameEnum.h"
#include "map.h"
#include "Vector2.h"
#include <vector>
#include "gui.h"

using namespace std;

extern CCamera camera;

extern DIRECTION curPlayerDirection;
extern ACTION curPlayerAction;
extern ACTION curBossAction;

extern CArrow arrow[TOTAL_ARROW];
extern CTimer g_Timer;
extern Vector2 attackDirection;
extern CBLOCK wall[139];
extern Map map;
extern vector<Vector2> wallBlock;

extern Gui playerHp1;
extern Gui playerHp2;
extern Gui playerHp3;
extern Gui playerHp4;

CPlayer::CPlayer()
{
}
CPlayer::~CPlayer()
{
}

void CPlayer::Initialize(int x, int y, CTimer* timer, int currentFrame, int frameInterval, int moveInterval, int skillFrameInterval)
{
	m_nHp = 8;
	draw_x = x;
	draw_y = y;
	m_nOldX = x;
	m_nOldY = y;
	curPlayerDirection = DIRECTION::DOWN;
	curPlayerAction = ACTION::FAINT;
	curState = OBJECT_TYPE::PLYAER;
	moveSpeed = 60;
	m_nMoveSpeedFold = 2;
	m_bIsSkill = false;
	m_bIsRoll = false;
	m_nSpeedX = 0;
	m_nSpeedY = 0;
	m_bIsUseBow = false;
	m_nLastSkillTime = 0;
	m_nSkillCoolTime = g_Timer.time()-20000;
	int a = 0, b = 0;
	SetRect(&walkHitRect[a++], 4, 12, 6, 10);
	SetRect(&walkHitRect[a++], 6, 12, 4, 12);
	SetRect(&walkHitRect[a++], 6, 12, 6, 12);
	SetRect(&walkHitRect[a++], 6, 12, 6, 12);
	SetRect(&walkHitRect[a++], 4, 12, 7, 12);
	SetRect(&walkHitRect[a++], 7, 12, 5, 12);
	SetRect(&walkHitRect[a++], 6, 12, 7, 12);
	SetRect(&walkHitRect[a], 7, 12, 6, 12);
	a = 0;
	SetRect(&rollHitRect[a][b++], 4, 8, 4, 14);
	SetRect(&rollHitRect[a][b++], 8, 2, 8, 10);
	SetRect(&rollHitRect[a][b++], 6, 2, 6, 14);
	SetRect(&rollHitRect[a][b++], 6, 0, 6, 14);
	SetRect(&rollHitRect[a][b++], 8, 2, 10, 10);
	SetRect(&rollHitRect[a][b], 4, 14, 4, 6);
	a++;
	b = 0;
	SetRect(&rollHitRect[a][b++], 8, 8, 2, 12);
	SetRect(&rollHitRect[a][b++], 8, 2, 8, 10);
	SetRect(&rollHitRect[a][b++], 6, 2, 8, 16);
	SetRect(&rollHitRect[a][b++], 8, -4, 6, 16);
	SetRect(&rollHitRect[a][b++], 10, 2, 6, 10);
	SetRect(&rollHitRect[a][b], 4, 14, 4, 6);
	a++;
	b = 0;
	SetRect(&rollHitRect[a][b++], 6, 10, 6, 14);
	SetRect(&rollHitRect[a][b++], 6, 6, 6, 12);
	SetRect(&rollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&rollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&rollHitRect[a][b++], 6, 4, 6, 12);
	SetRect(&rollHitRect[a][b], 8, 8, 6, 6);
	a++;
	b = 0;
	SetRect(&rollHitRect[a][b++], 6, 12, 6, 14);
	SetRect(&rollHitRect[a][b++], 6, 6, 6, 12);
	SetRect(&rollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&rollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&rollHitRect[a][b++], 6, 4, 6, 10);
	SetRect(&rollHitRect[a][b], 6, 10, 6, 6);
	a++;
	b = 0;
	SetRect(&rollHitRect[a][b++], 8, 10, 4, 12);
	SetRect(&rollHitRect[a][b++], 10, 2, 8, 10);
	SetRect(&rollHitRect[a][b++], 8, -2, 4, 16);
	SetRect(&rollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&rollHitRect[a][b++], 7, -2, 8, 12);
	SetRect(&rollHitRect[a][b], 6, 14, 4, 8);
	a++;
	b = 0;
	SetRect(&rollHitRect[a][b++], 10, 10, 2, 12);
	SetRect(&rollHitRect[a][b++], 8, 2, 12, 10);
	SetRect(&rollHitRect[a][b++], 4, -2, 8, 16);
	SetRect(&rollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&rollHitRect[a][b++], 8, -2, 6, 12);
	SetRect(&rollHitRect[a][b], 4, 14, 6, 8);
	a++;
	b = 0;
	SetRect(&rollHitRect[a][b++], 6, 10, 6, 12);
	SetRect(&rollHitRect[a][b++], 10, 4, 4, 10);
	SetRect(&rollHitRect[a][b++], 8, -2, 4, 16);
	SetRect(&rollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&rollHitRect[a][b++], 2, 2, 8, 14);
	SetRect(&rollHitRect[a][b], 8, 14, 6, 6);
	a++;
	b = 0;
	SetRect(&rollHitRect[a][b++], 10, 8, 4, 12);
	SetRect(&rollHitRect[a][b++], 6, 4, 12, 10);
	SetRect(&rollHitRect[a][b++], 6, -4, 10, 14);
	SetRect(&rollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&rollHitRect[a][b++], 10, 0, 4, 14);
	SetRect(&rollHitRect[a][b], 8, 14, 8, 6);
	a++;
	b = 0;

	CGObject::Initialize(m_pCurSprite, x, y, timer, currentFrame, frameInterval, skillFrameInterval);

	m_nMoveInterval = moveInterval;
}

void CPlayer::SetWalkSprite(CSprite8* sprite)
{
	m_pWalkSprite = sprite;
}

void CPlayer::SetRollSprite(CSprite8* sprite)
{
	m_pRollSprite = sprite;
}

void CPlayer::SetDeadSprite(CSprite8* sprite)
{
	m_pDeadSprite = sprite;
}

/// <summary>
/// ?????? ?????????????????? ?????? ????????? ???????????????.
/// </summary>
void CPlayer::SetSpriteAndHitRect()
{
	m_pCurSprite = m_pWalkSprite->GetSprite(DIRECTION::DOWN);
	m_pOldSprite = m_pWalkSprite->GetSprite(DIRECTION::UP);
	SetSprite(m_pCurSprite);
	SetHitWallRect();
}

void CPlayer::SetBowSprite(CSprite8* _walk, CSprite8* _roll, CSprite8* _attack)
{
	m_pBowWalkSprite = _walk;
	m_pBow_RollSprite = _roll;
	m_pBow_AttackSprite = _attack;
}

void CPlayer::SetSkillSprite(CSprite* _skill)
{
	m_pSkillSprite = _skill;
}

void CPlayer::SetArrow(CArrow* arrow)
{
	curArrow = arrow;
	if (!IsArrowNull())
		curArrow->SetIsUse(false);
}

CArrow* CPlayer::GetCurArrow()
{
	return curArrow;
}

bool CPlayer::IsArrowNull()
{
	for (int i = 0; i < TOTAL_ARROW; i++)
	{
		if (curArrow == &arrow[i])
			return false;
	}
	return true;
}

bool CPlayer::IsUsingSkill()
{
	return m_bIsSkill;
}

void CPlayer::CheckWallCollision()
{
	RECT blockRect;
	RECT collisionRect;
	bool isX = false;
	bool isY = false;
	int collisionCount = 0;
	int oldBlockX = 0;
	int oldBlockY = 0;
	if (map.GetStageNum() == 0)		//?????? ?????????
	{
		for (int i = 0; i < wallBlock.size(); i++)
		{
			SetRect(&blockRect, wallBlock[i].x * 32, wallBlock[i].y * 32, wallBlock[i].x * 32 + 32, wallBlock[i].y * 32 + 32);	//?????? ????????? ??????
			if (CheckHitWallRect(blockRect))
			{
				collisionCount++;
				if (collisionCount > 1)
				{
					if (oldBlockX != wallBlock[i].x && oldBlockY != wallBlock[i].y)
					{
						collisionRect = GetHitRect2(blockRect);		//??????????????? ???????????? ????????? ??????
						int __left = abs(blockRect.left - collisionRect.left);
						int __right = abs(blockRect.right - collisionRect.right);
						int __top = abs(blockRect.top - collisionRect.top);
						int __bottom = abs(blockRect.bottom - collisionRect.bottom);

						if (__bottom + __top + __right == 0)		//?????? ??? ?????????
						{
							//SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
							SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - __left));
						}
						else if (__left + __bottom + __top == 0)		//????????? ?????? ?????????
						{
							//SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
							SetX(GetPos().x - (DEFAULT_BLOCK_SIZE - __right));
						}
						else if (__left + __bottom == 0)	//?????? ??? ?????????
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - __top));
						}
						else if (__left + __top == 0)	//?????? ?????? ?????????
						{
							SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
						}
						else if (__right + __top == 0)
						{
							SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - __left));
						}
						else if (__right + __bottom == 0)	//????????? ??? ?????????
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - __top));
						}
						continue;
						//SetX(m_nOldX + m_nSpeedX);
						//SetY(m_nOldY);
						//if (CheckHitWallRect(blockRect))		//x??? ??????????????? ??????????????? ??????
						//	isX = true;

						//SetX(m_nOldX);
						//SetY(m_nOldY + m_nSpeedY);
						//if (CheckHitWallRect(blockRect))		//y??? ??????????????? ??????????????? ??????
						//	isY = true;

						//if (isX)		//x????????? ?????? ??? ????????? ???????????????
						//{
						//	SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - (__left == 0 ? __right : __left)) * (__left == 0 ? -1 : 1));
						//	SetY(m_nOldY + m_nSpeedY);
						//}
						//else if (isY)		//y????????? ?????? ??? ????????? ???????????????
						//{
						//	SetX(m_nOldX + m_nSpeedX);
						//	SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - (__top == 0 ? __bottom : __top)) * (__top == 0 ? -1 : 1));
						//}
					}
				}
				collisionRect = GetHitRect2(blockRect);		//??????????????? ???????????? ????????? ??????
				int _left = abs(blockRect.left - collisionRect.left);
				int _right = abs(blockRect.right - collisionRect.right);
				int _top = abs(blockRect.top - collisionRect.top);
				int _bottom = abs(blockRect.bottom - collisionRect.bottom);

				oldBlockX = wallBlock[i].x;
				oldBlockY = wallBlock[i].y;

				SetX(m_nOldX + m_nSpeedX);
				SetY(m_nOldY);
				if (CheckHitWallRect(blockRect))		//x??? ??????????????? ??????????????? ??????
					isX = true;

				SetX(m_nOldX);
				SetY(m_nOldY + m_nSpeedY);
				if (CheckHitWallRect(blockRect))		//y??? ??????????????? ??????????????? ??????
					isY = true;

				if (!isX && !isY)		//???????????? ???????????????
				{
					if (_left + _top == 0)		//?????? ??? ?????????
					{
						if (_right <= _bottom)		//x?????? ??? ??????
						{
							SetX(m_nOldX + m_nSpeedX);
							SetY(m_nOldY + m_nSpeedY - (DEFAULT_BLOCK_SIZE - _bottom));
						}
						else
						{
							SetX(m_nOldX + m_nSpeedX - (DEFAULT_BLOCK_SIZE - _right));
							SetY(m_nOldY + m_nSpeedY);
						}
					}
					if (_top + _right == 0)		//????????? ??? ?????????
					{
						if (_left <= _bottom)		//x?????? ??? ??????
						{
							SetX(m_nOldX + m_nSpeedX);
							SetY(m_nOldY + m_nSpeedY - (DEFAULT_BLOCK_SIZE - _bottom));
						}
						else
						{
							SetX(m_nOldX + m_nSpeedX + (DEFAULT_BLOCK_SIZE - _left));
							SetY(m_nOldY + m_nSpeedY);
						}
					}
					if (_right + _bottom == 0)	//????????? ?????? ?????????
					{
						if (_left <= _top)		//x?????? ??? ??????
						{
							SetX(m_nOldX + m_nSpeedX);
							SetY(m_nOldY + m_nSpeedY + (DEFAULT_BLOCK_SIZE - _top));
						}
						else
						{
							SetX(m_nOldX + m_nSpeedX + (DEFAULT_BLOCK_SIZE - _left));
							SetY(m_nOldY + m_nSpeedY);
						}
					}
					if (_left + _bottom == 0)	//?????? ?????? ?????????
					{
						if (_right <= _top)		//x?????? ??? ??????
						{
							SetX(m_nOldX + m_nSpeedX);
							SetY(m_nOldY + m_nSpeedY + (DEFAULT_BLOCK_SIZE - _top));
						}
						else
						{
							SetX(m_nOldX + m_nSpeedX - (DEFAULT_BLOCK_SIZE - _right));
							SetY(m_nOldY + m_nSpeedY);
						}
					}
				}
				else if (isX)		//x????????? ?????? ??? ????????? ???????????????
				{
					SetX(m_nOldX + m_nSpeedX + (DEFAULT_BLOCK_SIZE - (_left == 0 ? _right : _left)) * (_left == 0 ? -1 : 1));
					SetY(m_nOldY + m_nSpeedY);
				}
				else if (isY)		//y????????? ?????? ??? ????????? ???????????????
				{
					SetX(m_nOldX + m_nSpeedX);
					SetY(m_nOldY + m_nSpeedY + (DEFAULT_BLOCK_SIZE - (_top == 0 ? _bottom : _top)) * (_top == 0 ? -1 : 1));
				}
			}
		}
	}
	else	//?????? ?????????
	{
		for (int i = 0; i < 139; i++)
		{
			SetRect(&blockRect, wall[i].GetPos().x * 32, wall[i].GetPos().y * 32, wall[i].GetPos().x * 32 + 32, wall[i].GetPos().y * 32 + 32);	//?????? ????????? ??????
			if (CheckHitWallRect(blockRect))
			{
				collisionCount++;
				if (collisionCount > 1)
				{
					if (oldBlockX != wall[i].GetPos().x && oldBlockY != wall[i].GetPos().y)		//?????? x???
					{
						collisionRect = GetHitRect2(blockRect);		//??????????????? ???????????? ????????? ??????
						int __left = abs(blockRect.left - collisionRect.left);
						int __right = abs(blockRect.right - collisionRect.right);
						int __top = abs(blockRect.top - collisionRect.top);
						int __bottom = abs(blockRect.bottom - collisionRect.bottom);

						if (__bottom + __top + __right == 0)		//?????? ??? ?????????
						{
							//SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
							SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - __left));
						}
						else if (__left + __bottom + __top == 0)		//????????? ?????? ?????????
						{
							//SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
							SetX(GetPos().x - (DEFAULT_BLOCK_SIZE - __right));
						}
						else if (__left + __bottom == 0)	//?????? ??? ?????????
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - __top));
						}
						else if (__left + __top == 0)	//?????? ?????? ?????????
						{
							SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
						}
						else if (__right + __bottom == 0)	//????????? ??? ?????????
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - __top));
						}
						continue;
						//SetX(m_nOldX + m_nSpeedX);
						//SetY(m_nOldY);
						//if (CheckHitWallRect(blockRect))		//x??? ??????????????? ??????????????? ??????
						//	isX = true;

						//SetX(m_nOldX);
						//SetY(m_nOldY + m_nSpeedY);
						//if (CheckHitWallRect(blockRect))		//y??? ??????????????? ??????????????? ??????
						//	isY = true;

						//if (isX)		//x????????? ?????? ??? ????????? ???????????????
						//{
						//	SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - (__left == 0 ? __right : __left)) * (__left == 0 ? -1 : 1));
						//	SetY(m_nOldY + m_nSpeedY);
						//}
						//else if (isY)		//y????????? ?????? ??? ????????? ???????????????
						//{
						//	SetX(m_nOldX + m_nSpeedX);
						//	SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - (__top == 0 ? __bottom : __top)) * (__top == 0 ? -1 : 1));
						//}
					}
				}
				collisionRect = GetHitRect2(blockRect);		//??????????????? ???????????? ????????? ??????
				int _left = abs(blockRect.left - collisionRect.left);
				int _right = abs(blockRect.right - collisionRect.right);
				int _top = abs(blockRect.top - collisionRect.top);
				int _bottom = abs(blockRect.bottom - collisionRect.bottom);

				oldBlockX = wall[i].GetPos().x;
				oldBlockY = wall[i].GetPos().y;

				SetX(m_nOldX + m_nSpeedX);
				SetY(m_nOldY);
				if (CheckHitWallRect(blockRect))		//x??? ??????????????? ??????????????? ??????
					isX = true;

				SetX(m_nOldX);
				SetY(m_nOldY + m_nSpeedY);
				if (CheckHitWallRect(blockRect))		//y??? ??????????????? ??????????????? ??????
					isY = true;

				if (!isX && !isY)		//???????????? ???????????????
				{
					if (_left + _top == 0)		//?????? ??? ?????????
					{
						if (_right <= _bottom)		//x?????? ??? ??????
						{
							SetX(m_nOldX + m_nSpeedX);
							SetY(m_nOldY + m_nSpeedY - (DEFAULT_BLOCK_SIZE - _bottom));
						}
						else
						{
							SetX(m_nOldX + m_nSpeedX - (DEFAULT_BLOCK_SIZE - _right));
							SetY(m_nOldY + m_nSpeedY);
						}
					}
					if (_top + _right == 0)		//????????? ??? ?????????
					{
						if (_left <= _bottom)		//x?????? ??? ??????
						{
							SetX(m_nOldX + m_nSpeedX);
							SetY(m_nOldY + m_nSpeedY - (DEFAULT_BLOCK_SIZE - _bottom));
						}
						else
						{
							SetX(m_nOldX + m_nSpeedX + (DEFAULT_BLOCK_SIZE - _left));
							SetY(m_nOldY + m_nSpeedY);
						}
					}
					if (_right + _bottom == 0)	//????????? ?????? ?????????
					{
						if (_left <= _top)		//x?????? ??? ??????
						{
							SetX(m_nOldX + m_nSpeedX);
							SetY(m_nOldY + m_nSpeedY + (DEFAULT_BLOCK_SIZE - _top));
						}
						else
						{
							SetX(m_nOldX + m_nSpeedX + (DEFAULT_BLOCK_SIZE - _left));
							SetY(m_nOldY + m_nSpeedY);
						}
					}
					if (_left + _bottom == 0)	//?????? ?????? ?????????
					{
						if (_right <= _top)		//x?????? ??? ??????
						{
							SetX(m_nOldX + m_nSpeedX);
							SetY(m_nOldY + m_nSpeedY + (DEFAULT_BLOCK_SIZE - _top));
						}
						else
						{
							SetX(m_nOldX + m_nSpeedX - (DEFAULT_BLOCK_SIZE - _right));
							SetY(m_nOldY + m_nSpeedY);
						}
					}
				}
				else if (isX)		//x????????? ?????? ??? ????????? ???????????????
				{
					SetX(m_nOldX + m_nSpeedX + (DEFAULT_BLOCK_SIZE - (_left == 0 ? _right : _left)) * (_left == 0 ? -1 : 1));
					SetY(m_nOldY + m_nSpeedY);
				}
				else if (isY)		//y????????? ?????? ??? ????????? ???????????????
				{
					SetX(m_nOldX + m_nSpeedX);
					SetY(m_nOldY + m_nSpeedY + (DEFAULT_BLOCK_SIZE - (_top == 0 ? _bottom : _top)) * (_top == 0 ? -1 : 1));
				}
			}
		}
	}
}

int CPlayer::GetHp()
{
	return m_nHp;
}

void CPlayer::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	if (m_bIsSkill)
		CGObject::SkillDraw(draw_x, draw_y, surface);
	else
		CGObject::Draw(curPlayerAction == ACTION::MOVE || curPlayerAction == ACTION::RUN 
			|| curPlayerAction == ACTION::ROLL, draw_x, draw_y, surface);
}

void CPlayer::Skill()
{
	if (curPlayerAction != ACTION::DEAD)
	{
		if (g_Timer.elapsed(m_nSkillCoolTime, 10000))
		{
			if (m_bIsSkill == false)
			{
				m_bIsSkill = true;
				m_nLastSkillTime = g_Timer.time();
				camera.SetIsSkill(true);
			}
		}
	}
}

void CPlayer::CheckUseSkill()
{
	if (m_bIsSkill)
	{
		if (g_Timer.elapsed(m_nLastSkillTime, 5000))
		{
			m_bIsSkill = false;
			camera.SetIsSkill(false);
			m_nSkillCoolTime = g_Timer.time();
		}
	}
}

bool CPlayer::GetIsRoll()
{
	return m_bIsRoll;
}

void CPlayer::Roll()
{
	curPlayerAction = ACTION::ROLL;
	m_nFrameInterval = 70;
}

void CPlayer::Left()
{
	m_nSpeedX -= moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
}
void CPlayer::Right()
{
	m_nSpeedX += moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
}
void CPlayer::Up()
{
	m_nSpeedY -= moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
}
void CPlayer::Down()
{
	m_nSpeedY += moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
}

void CPlayer::Hit()
{
	m_nHp -= 1;
	if (playerHp4.GetFrame() != 2)
	{
		playerHp4.SetFrame(playerHp4.GetFrame() + 1);
	}
	else if (playerHp3.GetFrame() != 2)
	{
		playerHp3.SetFrame(playerHp3.GetFrame() + 1);
	}
	else if (playerHp2.GetFrame() != 2)
	{
		playerHp2.SetFrame(playerHp2.GetFrame() + 1);
	}
	else if (playerHp1.GetFrame() != 2)
	{
		playerHp1.SetFrame(playerHp1.GetFrame() + 1);
	}
	if (m_nHp == 0)
	{
		curPlayerAction = ACTION::DEAD;
	}
}

bool CPlayer::CanMove()
{
	if (!m_bIsLive)
		return false;

	if (m_pTimer->elapsed(m_nLastMoveTime, m_nMoveInterval))
		return true;
	return false;
}

float CPlayer::GetSpeedX()
{
	return m_nSpeedX;
}
float CPlayer::GetSpeedY()
{
	return m_nSpeedY;
}

void CPlayer::MoveANDCheckState()
{
	if (curPlayerAction == ACTION::DEAD)
		m_bIsRoll = false;
	if (!(curPlayerAction == ACTION::ROLL) && !(curPlayerAction == ACTION::DEAD))
	{
		m_bIsRoll = false;
		if (camera.GetExpansion() == 1)			//????????? ????????? ?????????????????????
		{
			if (curPlayerAction != ACTION::FAINT)
			{
				curPlayerAction = ACTION::IDLE;		//???????????? ????????? ???????????? ?????????
			}
		}
		else
			curPlayerAction = ACTION::ATTACK;		//???????????? ????????? ??????????????? ?????????

		if (!(curPlayerAction == ACTION::ATTACK) && (m_nSpeedX != 0 || m_nSpeedY != 0))
			curPlayerAction = ACTION::MOVE;
	}

	if (m_bIsRoll)
	{
		m_nMoveSpeedFold = 4;
		switch (curPlayerDirection)
		{
		case DIRECTION::LEFT:
			m_nSpeedX = -(moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime);
			m_nSpeedY = 0;
			break;
		case DIRECTION::RIGHT:
			m_nSpeedX = moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
			m_nSpeedY = 0;
			break;
		case DIRECTION::UP:
			m_nSpeedX = 0;
			m_nSpeedY = -(moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime);
			break;
		case DIRECTION::DOWN:
			m_nSpeedX = 0;
			m_nSpeedY = moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
			break;
		case DIRECTION::LEFTUP:
			m_nSpeedX = -(moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime);
			m_nSpeedY = -(moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime);
			break;
		case DIRECTION::RIGHTUP:
			m_nSpeedX = moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
			m_nSpeedY = -(moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime);
			break;
		case DIRECTION::LEFTDOWN:
			m_nSpeedX = -(moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime);
			m_nSpeedY = moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
			break;
		case DIRECTION::RIGHTDOWN:
			m_nSpeedX = moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
			m_nSpeedY = moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
			break;
		}
	}

	if (!(curPlayerAction == ACTION::ATTACK) && 
		!(curPlayerAction == ACTION::IDLE) && !(curPlayerAction == ACTION::DEAD))	//?????????????????? ??????????????? ???????????? ???????????? ???????????????
	{
		m_nOldX = GetPos().x;
		m_nOldY = GetPos().y;
		SetX(GetPos().x + m_nSpeedX);
		SetY(GetPos().y + m_nSpeedY);
	}

	CheckWallCollision();


	/*if (GetX() < 1110)
	{
		SetX(1111);
	}
	if (GetX() > 2090)
	{
		SetX(2089);
	}
	if (GetY() < 1116)
	{
		SetY(1117);
	}
	if (GetY() > 2084)
	{
		SetY(2083);
	}*/

	draw_x = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH*0.5f));
	draw_y = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT*0.5f));

	if (!m_bIsRoll)
	{
		if (m_nSpeedX == 0)
		{
			if (m_nSpeedY < 0)
				curPlayerDirection = DIRECTION::UP;
			else if (m_nSpeedY > 0)
				curPlayerDirection = DIRECTION::DOWN;
		}
		else if (m_nSpeedX < 0)
		{
			if (m_nSpeedY == 0)
				curPlayerDirection = DIRECTION::LEFT;
			else if (m_nSpeedY < 0)
				curPlayerDirection = DIRECTION::LEFTUP;
			else if (m_nSpeedY > 0)
				curPlayerDirection = DIRECTION::LEFTDOWN;
		}
		else if (m_nSpeedX > 0)
		{
			if (m_nSpeedY == 0)
				curPlayerDirection = DIRECTION::RIGHT;
			else if (m_nSpeedY < 0)
				curPlayerDirection = DIRECTION::RIGHTUP;
			else if (m_nSpeedY > 0)
				curPlayerDirection = DIRECTION::RIGHTDOWN;
		}

		if (curPlayerAction == 4)
		{
			if (attackDirection.y < 0)
			{
				if ((attackDirection.x > -0.3f) && (attackDirection.x < 0.3f))
					curPlayerDirection = DIRECTION::UP;
				else if (attackDirection.x < -0.3f)
					curPlayerDirection = DIRECTION::LEFTUP;
				else if (attackDirection.x > 0.3f)
					curPlayerDirection = DIRECTION::RIGHTUP;
			}
			else
			{
				if ((attackDirection.x > -0.3f) && (attackDirection.x < 0.3f))
					curPlayerDirection = DIRECTION::DOWN;
				else if (attackDirection.x < -0.3f)
					curPlayerDirection = DIRECTION::LEFTDOWN;
				else if (attackDirection.x > 0.3f)
					curPlayerDirection = DIRECTION::RIGHTDOWN;
			}
			if (attackDirection.x < 0)
			{
				if ((attackDirection.y > -0.3f) && (attackDirection.y < 0.3f))
					curPlayerDirection = DIRECTION::LEFT;
			}
			else
			{
				if ((attackDirection.y > -0.3f) && (attackDirection.y < 0.3f))
					curPlayerDirection = DIRECTION::RIGHT;
			}
		}

		m_pCurSprite = !(curPlayerAction == ACTION::ROLL) ? m_pWalkSprite->GetSprite(curPlayerDirection) : m_pRollSprite->GetSprite(curPlayerDirection);
		m_pCurBowSprite = !(curPlayerAction == ACTION::ROLL) ? m_pBowWalkSprite->GetSprite(curPlayerDirection) : m_pBow_RollSprite->GetSprite(curPlayerDirection);

		if (curPlayerAction == ACTION::ATTACK)
			m_pCurBowSprite = m_pBow_AttackSprite->GetSprite(curPlayerDirection);
	}

	SetHitRect((curPlayerAction != ACTION::ROLL) ? walkHitRect[(int)curPlayerDirection-1] : rollHitRect[(int)curPlayerDirection-1][m_nCurrentFrame]);

	if (curPlayerAction == ACTION::FAINT)
	{
		m_pCurSprite = m_pDeadSprite->GetSprite(DIRECTION::RIGHT);
	}

	if (curPlayerAction == ACTION::DEAD)
	{
		m_pCurSprite = m_pDeadSprite->GetSprite(curPlayerDirection);
	}

	if(m_bIsSkill)
		m_pCurSprite = m_pSkillSprite;

	//if (!m_bIsRoll)
	if ((m_pOldSprite != m_pCurSprite && !m_bIsRoll)||(m_pOldBowSprite != m_pCurBowSprite))
	{
		m_pOldSprite = m_pCurSprite;
		m_pOldBowSprite = m_pCurBowSprite;
		CGObject::SetSprite(m_pCurSprite, m_pCurBowSprite);
		if (curPlayerAction == ACTION::ROLL)
		{
			m_bIsRoll = true;
			//curBossActionState = (ACTION_STATE)3;
		}
	}
}

void CPlayer::MoveInit()
{
	m_nSpeedX = 0;
	m_nSpeedY = 0;
	if (curPlayerAction != ACTION::ROLL)
	{
		m_nMoveSpeedFold = 2;
		m_nFrameInterval = 140;
	}
	m_bIsUseBow = false;
}

bool CPlayer::IsArrowReady()
{
	if (curArrow == NULL)
		return false;
	return true;
}