#include "myplayer.h"
#include "block.h"
#include "gameEnum.h"
#include "map.h"
#include "Vector2.h"
#include <vector>
#include "gui.h"

using namespace std;

extern CCamera camera;

extern DISTANCE_STATE curPlayerDistanceState;
extern ACTION_STATE curPlayerActionState;
extern ACTION_STATE curBossActionState;

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
	curPlayerDistanceState = DISTANCE_STATE::DOWN;
	curPlayerActionState = ACTION_STATE::FAINT;
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

void CPlayer::SetWalkSprite(CSprite* _walk_Left, CSprite* _walk_LeftUp, CSprite* _walk_LeftDown, CSprite* _walk_Right, CSprite* _walk_RightUp,
	CSprite* _walk_RightDown, CSprite* _walk_Up, CSprite* _walk_Down, CSprite8* _dead)
{
	m_pWalk_LeftSprite = _walk_Left;
	m_pWalk_LeftUpSprite = _walk_LeftUp;
	m_pWalk_LeftDownSprite = _walk_LeftDown;
	m_pWalk_RightSprite = _walk_Right;
	m_pWalk_RightUpSprite = _walk_RightUp;
	m_pWalk_RightDownSprite = _walk_RightDown;
	m_pWalk_UpSprite = _walk_Up;
	m_pWalk_DownSprite = _walk_Down;
	m_pCurSprite = m_pWalk_DownSprite;
	m_pOldSprite = m_pWalk_UpSprite;
	m_pDeadSprite = _dead;
	SetSprite(m_pCurSprite);
	SetHitWallRect();
}

void CPlayer::SetRollSprite(CSprite* _Roll_Left, CSprite* _Roll_LeftUp, CSprite* _Roll_LeftDown, CSprite* _Roll_Right, CSprite* _Roll_RightUp, CSprite* _Roll_RightDown,
	CSprite* _Roll_Up, CSprite* _Roll_Down)
{
	m_pRoll_LeftSprite = _Roll_Left;
	m_pRoll_LeftUpSprite = _Roll_LeftUp;
	m_pRoll_LeftDownSprite = _Roll_LeftDown;
	m_pRoll_RightSprite = _Roll_Right;
	m_pRoll_RightUpSprite = _Roll_RightUp;
	m_pRoll_RightDownSprite = _Roll_RightDown;
	m_pRoll_UpSprite = _Roll_Up;
	m_pRoll_DownSprite = _Roll_Down;
}

void CPlayer::SetBowSprite(CSprite8* _walk, CSprite8* _roll, CSprite8* _attack)
{
	m_pBow_WalkSprite = _walk;
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
	if (map.GetStage() == 0)		//기본 맵일때
	{
		for (int i = 0; i < wallBlock.size(); i++)
		{
			SetRect(&blockRect, wallBlock[i].x * 32, wallBlock[i].y * 32, wallBlock[i].x * 32 + 32, wallBlock[i].y * 32 + 32);	//해당 블록의 영역
			if (CheckHitWallRect(blockRect))
			{
				collisionCount++;
				if (collisionCount > 1)
				{
					if (oldBlockX != wallBlock[i].x && oldBlockY != wallBlock[i].y)
					{
						collisionRect = GetHitRect2(blockRect);		//플레이어랑 블럭이랑 충돌된 영역
						int __left = abs(blockRect.left - collisionRect.left);
						int __right = abs(blockRect.right - collisionRect.right);
						int __top = abs(blockRect.top - collisionRect.top);
						int __bottom = abs(blockRect.bottom - collisionRect.bottom);

						if (__bottom + __top + __right == 0)		//왼쪽 위 모서리
						{
							//SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
							SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - __left));
						}
						else if (__left + __bottom + __top == 0)		//오른쪽 아래 모서리
						{
							//SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
							SetX(GetPos().x - (DEFAULT_BLOCK_SIZE - __right));
						}
						else if (__left + __bottom == 0)	//왼쪽 위 모서리
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - __top));
						}
						else if (__left + __top == 0)	//왼쪽 아래 모서리
						{
							SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
						}
						else if (__right + __top == 0)
						{
							SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - __left));
						}
						else if (__right + __bottom == 0)	//오른쪽 위 모서리
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - __top));
						}
						continue;
						//SetX(m_nOldX + m_nSpeedX);
						//SetY(m_nOldY);
						//if (CheckHitWallRect(blockRect))		//x만 이동했을때 충돌하는지 체크
						//	isX = true;

						//SetX(m_nOldX);
						//SetY(m_nOldY + m_nSpeedY);
						//if (CheckHitWallRect(blockRect))		//y만 이동했을때 충돌하는지 체크
						//	isY = true;

						//if (isX)		//x축으로 이동 후 충돌이 발생했을때
						//{
						//	SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - (__left == 0 ? __right : __left)) * (__left == 0 ? -1 : 1));
						//	SetY(m_nOldY + m_nSpeedY);
						//}
						//else if (isY)		//y축으로 이동 후 충돌이 발생했을때
						//{
						//	SetX(m_nOldX + m_nSpeedX);
						//	SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - (__top == 0 ? __bottom : __top)) * (__top == 0 ? -1 : 1));
						//}
					}
				}
				collisionRect = GetHitRect2(blockRect);		//플레이어랑 블럭이랑 충돌된 영역
				int _left = abs(blockRect.left - collisionRect.left);
				int _right = abs(blockRect.right - collisionRect.right);
				int _top = abs(blockRect.top - collisionRect.top);
				int _bottom = abs(blockRect.bottom - collisionRect.bottom);

				oldBlockX = wallBlock[i].x;
				oldBlockY = wallBlock[i].y;

				SetX(m_nOldX + m_nSpeedX);
				SetY(m_nOldY);
				if (CheckHitWallRect(blockRect))		//x만 이동했을때 충돌하는지 체크
					isX = true;

				SetX(m_nOldX);
				SetY(m_nOldY + m_nSpeedY);
				if (CheckHitWallRect(blockRect))		//y만 이동했을때 충돌하는지 체크
					isY = true;

				if (!isX && !isY)		//모서리에 부딛쳤을때
				{
					if (_left + _top == 0)		//왼쪽 위 모서리
					{
						if (_right <= _bottom)		//x축이 더 길때
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
					if (_top + _right == 0)		//오른쪽 위 모서리
					{
						if (_left <= _bottom)		//x축이 더 길때
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
					if (_right + _bottom == 0)	//오른쪽 아래 모서리
					{
						if (_left <= _top)		//x축이 더 길때
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
					if (_left + _bottom == 0)	//왼쪽 아래 모서리
					{
						if (_right <= _top)		//x축이 더 길때
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
				else if (isX)		//x축으로 이동 후 충돌이 발생했을때
				{
					SetX(m_nOldX + m_nSpeedX + (DEFAULT_BLOCK_SIZE - (_left == 0 ? _right : _left)) * (_left == 0 ? -1 : 1));
					SetY(m_nOldY + m_nSpeedY);
				}
				else if (isY)		//y축으로 이동 후 충돌이 발생했을때
				{
					SetX(m_nOldX + m_nSpeedX);
					SetY(m_nOldY + m_nSpeedY + (DEFAULT_BLOCK_SIZE - (_top == 0 ? _bottom : _top)) * (_top == 0 ? -1 : 1));
				}
			}
		}
	}
	else	//보스 맵일때
	{
		for (int i = 0; i < 139; i++)
		{
			SetRect(&blockRect, wall[i].GetPos().x * 32, wall[i].GetPos().y * 32, wall[i].GetPos().x * 32 + 32, wall[i].GetPos().y * 32 + 32);	//해당 블록의 영역
			if (CheckHitWallRect(blockRect))
			{
				collisionCount++;
				if (collisionCount > 1)
				{
					if (oldBlockX != wall[i].GetPos().x && oldBlockY != wall[i].GetPos().y)		//같은 x축
					{
						collisionRect = GetHitRect2(blockRect);		//플레이어랑 블럭이랑 충돌된 영역
						int __left = abs(blockRect.left - collisionRect.left);
						int __right = abs(blockRect.right - collisionRect.right);
						int __top = abs(blockRect.top - collisionRect.top);
						int __bottom = abs(blockRect.bottom - collisionRect.bottom);

						if (__bottom + __top + __right == 0)		//왼쪽 위 모서리
						{
							//SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
							SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - __left));
						}
						else if (__left + __bottom + __top == 0)		//오른쪽 아래 모서리
						{
							//SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
							SetX(GetPos().x - (DEFAULT_BLOCK_SIZE - __right));
						}
						else if (__left + __bottom == 0)	//왼쪽 위 모서리
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - __top));
						}
						else if (__left + __top == 0)	//왼쪽 아래 모서리
						{
							SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
						}
						else if (__right + __bottom == 0)	//오른쪽 위 모서리
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - __top));
						}
						continue;
						//SetX(m_nOldX + m_nSpeedX);
						//SetY(m_nOldY);
						//if (CheckHitWallRect(blockRect))		//x만 이동했을때 충돌하는지 체크
						//	isX = true;

						//SetX(m_nOldX);
						//SetY(m_nOldY + m_nSpeedY);
						//if (CheckHitWallRect(blockRect))		//y만 이동했을때 충돌하는지 체크
						//	isY = true;

						//if (isX)		//x축으로 이동 후 충돌이 발생했을때
						//{
						//	SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - (__left == 0 ? __right : __left)) * (__left == 0 ? -1 : 1));
						//	SetY(m_nOldY + m_nSpeedY);
						//}
						//else if (isY)		//y축으로 이동 후 충돌이 발생했을때
						//{
						//	SetX(m_nOldX + m_nSpeedX);
						//	SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - (__top == 0 ? __bottom : __top)) * (__top == 0 ? -1 : 1));
						//}
					}
				}
				collisionRect = GetHitRect2(blockRect);		//플레이어랑 블럭이랑 충돌된 영역
				int _left = abs(blockRect.left - collisionRect.left);
				int _right = abs(blockRect.right - collisionRect.right);
				int _top = abs(blockRect.top - collisionRect.top);
				int _bottom = abs(blockRect.bottom - collisionRect.bottom);

				oldBlockX = wall[i].GetPos().x;
				oldBlockY = wall[i].GetPos().y;

				SetX(m_nOldX + m_nSpeedX);
				SetY(m_nOldY);
				if (CheckHitWallRect(blockRect))		//x만 이동했을때 충돌하는지 체크
					isX = true;

				SetX(m_nOldX);
				SetY(m_nOldY + m_nSpeedY);
				if (CheckHitWallRect(blockRect))		//y만 이동했을때 충돌하는지 체크
					isY = true;

				if (!isX && !isY)		//모서리에 부딛쳤을때
				{
					if (_left + _top == 0)		//왼쪽 위 모서리
					{
						if (_right <= _bottom)		//x축이 더 길때
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
					if (_top + _right == 0)		//오른쪽 위 모서리
					{
						if (_left <= _bottom)		//x축이 더 길때
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
					if (_right + _bottom == 0)	//오른쪽 아래 모서리
					{
						if (_left <= _top)		//x축이 더 길때
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
					if (_left + _bottom == 0)	//왼쪽 아래 모서리
					{
						if (_right <= _top)		//x축이 더 길때
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
				else if (isX)		//x축으로 이동 후 충돌이 발생했을때
				{
					SetX(m_nOldX + m_nSpeedX + (DEFAULT_BLOCK_SIZE - (_left == 0 ? _right : _left)) * (_left == 0 ? -1 : 1));
					SetY(m_nOldY + m_nSpeedY);
				}
				else if (isY)		//y축으로 이동 후 충돌이 발생했을때
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
		CGObject::Draw(curPlayerActionState == ACTION_STATE::MOVE || curPlayerActionState == ACTION_STATE::RUN 
			|| curPlayerActionState == ACTION_STATE::ROLL, draw_x, draw_y, surface);
}

void CPlayer::Skill()
{
	if (curPlayerActionState != ACTION_STATE::DEAD)
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
	curPlayerActionState = ACTION_STATE::ROLL;
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
		curPlayerActionState = ACTION_STATE::DEAD;
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
	if (curPlayerActionState == ACTION_STATE::DEAD)
		m_bIsRoll = false;
	if (!(curPlayerActionState == ACTION_STATE::ROLL) && !(curPlayerActionState == ACTION_STATE::DEAD))
	{
		m_bIsRoll = false;
		if (camera.GetExpansion() == 1)			//화면이 확대가 안되어있을경우
		{
			if (curPlayerActionState != ACTION_STATE::FAINT)
			{
				curPlayerActionState = ACTION_STATE::IDLE;		//캐릭터의 행동을 기본으로 초기화
			}
		}
		else
			curPlayerActionState = ACTION_STATE::ATTACK;		//캐릭터의 행동을 공격중으로 초기화

		if (!(curPlayerActionState == ACTION_STATE::ATTACK) && (m_nSpeedX != 0 || m_nSpeedY != 0))
			curPlayerActionState = ACTION_STATE::MOVE;
	}

	if (m_bIsRoll)
	{
		m_nMoveSpeedFold = 4;
		switch (curPlayerDistanceState)
		{
		case DISTANCE_STATE::LEFT:
			m_nSpeedX = -(moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime);
			m_nSpeedY = 0;
			break;
		case DISTANCE_STATE::RIGHT:
			m_nSpeedX = moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
			m_nSpeedY = 0;
			break;
		case DISTANCE_STATE::UP:
			m_nSpeedX = 0;
			m_nSpeedY = -(moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime);
			break;
		case DISTANCE_STATE::DOWN:
			m_nSpeedX = 0;
			m_nSpeedY = moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
			break;
		case DISTANCE_STATE::LEFTUP:
			m_nSpeedX = -(moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime);
			m_nSpeedY = -(moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime);
			break;
		case DISTANCE_STATE::RIGHTUP:
			m_nSpeedX = moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
			m_nSpeedY = -(moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime);
			break;
		case DISTANCE_STATE::LEFTDOWN:
			m_nSpeedX = -(moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime);
			m_nSpeedY = moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
			break;
		case DISTANCE_STATE::RIGHTDOWN:
			m_nSpeedX = moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
			m_nSpeedY = moveSpeed * m_nMoveSpeedFold * g_Timer.deltaTime;
			break;
		}
	}

	if (!(curPlayerActionState == ACTION_STATE::ATTACK) && 
		!(curPlayerActionState == ACTION_STATE::IDLE) && !(curPlayerActionState == ACTION_STATE::DEAD))	//공격중이거나 기본상태가 아닐경우 캐릭터를 움직여준다
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
			if (m_nSpeedY < 0)		//up
				curPlayerDistanceState = DISTANCE_STATE::UP;
			else if (m_nSpeedY > 0)		//down
				curPlayerDistanceState = DISTANCE_STATE::DOWN;
		}
		else if (m_nSpeedX < 0)
		{
			if (m_nSpeedY == 0)				//left
				curPlayerDistanceState = DISTANCE_STATE::LEFT;
			else if (m_nSpeedY < 0)		//leftup
				curPlayerDistanceState = DISTANCE_STATE::LEFTUP;
			else if (m_nSpeedY > 0)		//leftdown
				curPlayerDistanceState = DISTANCE_STATE::LEFTDOWN;
		}
		else if (m_nSpeedX > 0)
		{
			if (m_nSpeedY == 0)				//right
				curPlayerDistanceState = DISTANCE_STATE::RIGHT;
			else if (m_nSpeedY < 0)		//rightup
				curPlayerDistanceState = DISTANCE_STATE::RIGHTUP;
			else if (m_nSpeedY > 0)		//rightdown
				curPlayerDistanceState = DISTANCE_STATE::RIGHTDOWN;
		}

		if (curPlayerActionState == 4)
		{
			if (attackDirection.y < 0)
			{
				if ((attackDirection.x > -0.3f) && (attackDirection.x < 0.3f))
					curPlayerDistanceState = DISTANCE_STATE::UP;
				else if (attackDirection.x < -0.3f)
					curPlayerDistanceState = DISTANCE_STATE::LEFTUP;
				else if (attackDirection.x > 0.3f)
					curPlayerDistanceState = DISTANCE_STATE::RIGHTUP;
			}
			else
			{
				if ((attackDirection.x > -0.3f) && (attackDirection.x < 0.3f))
					curPlayerDistanceState = DISTANCE_STATE::DOWN;
				else if (attackDirection.x < -0.3f)
					curPlayerDistanceState = DISTANCE_STATE::LEFTDOWN;
				else if (attackDirection.x > 0.3f)
					curPlayerDistanceState = DISTANCE_STATE::RIGHTDOWN;
			}
			if (attackDirection.x < 0)
			{
				if ((attackDirection.y > -0.3f) && (attackDirection.y < 0.3f))
					curPlayerDistanceState = DISTANCE_STATE::LEFT;
			}
			else
			{
				if ((attackDirection.y > -0.3f) && (attackDirection.y < 0.3f))
					curPlayerDistanceState = DISTANCE_STATE::RIGHT;
			}
		}


		switch (curPlayerDistanceState)
		{
		case DISTANCE_STATE::LEFT:
			m_pCurSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pWalk_LeftSprite : m_pRoll_LeftSprite;
			m_pCurBowSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pBow_WalkSprite->GetLeft() : m_pBow_RollSprite->GetLeft();
			if (curPlayerActionState == ACTION_STATE::ATTACK)
				m_pCurBowSprite = m_pBow_AttackSprite->GetLeft();
			break;
		case DISTANCE_STATE::RIGHT:
			m_pCurSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pWalk_RightSprite : m_pRoll_RightSprite;
			m_pCurBowSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pBow_WalkSprite->GetRight() : m_pBow_RollSprite->GetRight();
			if (curPlayerActionState == ACTION_STATE::ATTACK)
				m_pCurBowSprite = m_pBow_AttackSprite->GetRight();
			break;
		case DISTANCE_STATE::UP:
			m_pCurSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pWalk_UpSprite : m_pRoll_UpSprite;
			m_pCurBowSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pBow_WalkSprite->GetUp() : m_pBow_RollSprite->GetUp();
			if (curPlayerActionState == ACTION_STATE::ATTACK)
				m_pCurBowSprite = m_pBow_AttackSprite->GetUp();
			break;
		case DISTANCE_STATE::DOWN:
			m_pCurSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pWalk_DownSprite : m_pRoll_DownSprite;
			m_pCurBowSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pBow_WalkSprite->GetDown() : m_pBow_RollSprite->GetDown();
			if (curPlayerActionState == ACTION_STATE::ATTACK)
				m_pCurBowSprite = m_pBow_AttackSprite->GetDown();
			break;
		case DISTANCE_STATE::LEFTUP:
			m_pCurSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pWalk_LeftUpSprite : m_pRoll_LeftUpSprite;
			m_pCurBowSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pBow_WalkSprite->GetLeftUp() : m_pBow_RollSprite->GetLeftUp();
			if (curPlayerActionState == ACTION_STATE::ATTACK)
				m_pCurBowSprite = m_pBow_AttackSprite->GetLeftUp();
			break;
		case DISTANCE_STATE::RIGHTUP:
			m_pCurSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pWalk_RightUpSprite : m_pRoll_RightUpSprite;
			m_pCurBowSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pBow_WalkSprite->GetRightUp() : m_pBow_RollSprite->GetRightUp();
			if (curPlayerActionState == ACTION_STATE::ATTACK)
				m_pCurBowSprite = m_pBow_AttackSprite->GetRightUp();
			break;
		case DISTANCE_STATE::LEFTDOWN:
			m_pCurSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pWalk_LeftDownSprite : m_pRoll_LeftDownSprite;
			m_pCurBowSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pBow_WalkSprite->GetLeftDown() : m_pBow_RollSprite->GetLeftDown();
			if (curPlayerActionState == ACTION_STATE::ATTACK)
				m_pCurBowSprite = m_pBow_AttackSprite->GetLeftDown();
			break;
		case DISTANCE_STATE::RIGHTDOWN:
			m_pCurSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pWalk_RightDownSprite : m_pRoll_RightDownSprite;
			m_pCurBowSprite = !(curPlayerActionState == ACTION_STATE::ROLL) ? m_pBow_WalkSprite->GetRightDown() : m_pBow_RollSprite->GetRightDown();
			if (curPlayerActionState == ACTION_STATE::ATTACK)
				m_pCurBowSprite = m_pBow_AttackSprite->GetRightDown();
			break;
		}
	}

	SetHitRect((curPlayerActionState != ACTION_STATE::ROLL) ? walkHitRect[(int)curPlayerDistanceState-1] : rollHitRect[(int)curPlayerDistanceState-1][m_nCurrentFrame]);

	if (curPlayerActionState == ACTION_STATE::FAINT)
	{
		m_pCurSprite = m_pDeadSprite->GetRight();
	}

	if (curPlayerActionState == ACTION_STATE::DEAD)
	{
		switch (curPlayerDistanceState)
		{
		case DISTANCE_STATE::LEFT:
			m_pCurSprite = m_pDeadSprite->GetLeft();
			break;
		case DISTANCE_STATE::RIGHT:
			m_pCurSprite = m_pDeadSprite->GetRight();
			break;
		case DISTANCE_STATE::UP:
			m_pCurSprite = m_pDeadSprite->GetUp();
			break;
		case DISTANCE_STATE::DOWN:
			m_pCurSprite = m_pDeadSprite->GetDown();
			break;
		case DISTANCE_STATE::LEFTUP:
			m_pCurSprite = m_pDeadSprite->GetLeftUp();
			break;
		case DISTANCE_STATE::RIGHTUP:
			m_pCurSprite = m_pDeadSprite->GetRightUp();
			break;
		case DISTANCE_STATE::LEFTDOWN:
			m_pCurSprite = m_pDeadSprite->GetLeftDown();
			break;
		case DISTANCE_STATE::RIGHTDOWN:
			m_pCurSprite = m_pDeadSprite->GetRightDown();
			break;
		}
	}

	if(m_bIsSkill)
		m_pCurSprite = m_pSkillSprite;

	//if (!m_bIsRoll)
	if ((m_pOldSprite != m_pCurSprite && !m_bIsRoll)||(m_pOldBowSprite != m_pCurBowSprite))
	{
		m_pOldSprite = m_pCurSprite;
		m_pOldBowSprite = m_pCurBowSprite;
		CGObject::SetSprite(m_pCurSprite, m_pCurBowSprite);
		if (curPlayerActionState == ACTION_STATE::ROLL)
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
	if (curPlayerActionState != ACTION_STATE::ROLL)
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