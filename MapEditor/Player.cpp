#include "Player.h"
#include <dinput.h>
#include <vector>

#include "Arrow.h"
#include "Block.h"
#include "Camera.h"
#include "GameEnum.h"
#include "Map.h"
#include "Vector2.h"
#include "Gui.h"
#include "SettingData.h"
#include "Sprite8.h"

using namespace std;

extern Camera camera;

extern BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);
extern LPDIRECTINPUTDEVICE8 g_lpDirectInputKeyboard;

extern DIRECTION curPlayerDirection;
extern ACTION curPlayerAction;
extern ACTION curBossAction;

extern Arrow arrow[TOTAL_ARROW];
extern Vector2 attackDirection;
extern Block wall[139];
extern Map map;
extern vector<Vector2> wallBlock;

extern Gui playerHp1;
extern Gui playerHp2;
extern Gui playerHp3;
extern Gui playerHp4;

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(int x, int y, int currentFrame, int frameInterval, int moveInterval,
                        int skillFrameInterval)
{
	m_nHp = 8;
	draw_x = x;
	draw_y = y;
	m_nOldX = x;
	m_nOldY = y;
	curPlayerDirection = DIRECTION::DOWN;
	curPlayerAction = ACTION::FAINT;
	mCurState = OBJECT_TYPE::PLYAER;
	mMoveSpeed = 60;
	m_nMoveSpeedFold = 2;
	m_bIsSkill = false;
	m_bIsRoll = false;
	m_nSpeedX = 0;
	m_nSpeedY = 0;
	m_bIsUseBow = false;
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

	GameObject::Initialize(m_pCurSprite, x, y, currentFrame, frameInterval, skillFrameInterval);

	m_nMoveInterval = moveInterval;
}

void Player::SetWalkSprite(CSprite8* sprite)
{
	m_pWalkSprite = sprite;
}

void Player::SetRollSprite(CSprite8* sprite)
{
	m_pRollSprite = sprite;
}

void Player::SetDeadSprite(CSprite8* sprite)
{
	m_pDeadSprite = sprite;
}

/// <summary>
/// 처음 스프라이트랑 충돌 영역을 설정해준다.
/// </summary>
void Player::SetSpriteAndHitRect()
{
	m_pCurSprite = m_pWalkSprite->GetSprite(DIRECTION::DOWN);
	m_pOldSprite = m_pWalkSprite->GetSprite(DIRECTION::UP);
	SetSprite(m_pCurSprite);
	SetHitWallRect();
}

void Player::SetBowSprite(CSprite8* _walk, CSprite8* _roll, CSprite8* _attack)
{
	m_pBowWalkSprite = _walk;
	m_pBow_RollSprite = _roll;
	m_pBow_AttackSprite = _attack;
}

void Player::SetSkillSprite(Sprite* _skill)
{
	m_pSkillSprite = _skill;
}

void Player::SetArrow(Arrow* arrow)
{
	curArrow = arrow;
	if (!IsArrowNull())
		curArrow->SetIsUse(false);
}

Arrow* Player::GetCurArrow()
{
	return curArrow;
}

bool Player::IsArrowNull()
{
	for (int i = 0; i < TOTAL_ARROW; i++)
	{
		if (curArrow == &arrow[i])
			return false;
	}
	return true;
}

bool Player::IsUsingSkill()
{
	return m_bIsSkill;
}

void Player::CheckWallCollision()
{
	RECT blockRect;
	RECT collisionRect;
	bool isX = false;
	bool isY = false;
	int collisionCount = 0;
	int oldBlockX = 0;
	int oldBlockY = 0;
	if (map.GetStageNum() == 0) //기본 맵일때
	{
		for (int i = 0; i < wallBlock.size(); i++)
		{
			SetRect(&blockRect, wallBlock[i].x * 32, wallBlock[i].y * 32, wallBlock[i].x * 32 + 32,
			        wallBlock[i].y * 32 + 32); //해당 블록의 영역
			if (CheckHitWallRect(blockRect))
			{
				collisionCount++;
				if (collisionCount > 1)
				{
					if (oldBlockX != wallBlock[i].x && oldBlockY != wallBlock[i].y)
					{
						collisionRect = GetHitRect2(blockRect); //플레이어랑 블럭이랑 충돌된 영역
						int __left = abs(blockRect.left - collisionRect.left);
						int __right = abs(blockRect.right - collisionRect.right);
						int __top = abs(blockRect.top - collisionRect.top);
						int __bottom = abs(blockRect.bottom - collisionRect.bottom);

						if (__bottom + __top + __right == 0) //왼쪽 위 모서리
						{
							SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - __left));
						}
						else if (__left + __bottom + __top == 0) //오른쪽 아래 모서리
						{
							SetX(GetPos().x - (DEFAULT_BLOCK_SIZE - __right));
						}
						else if (__left + __bottom == 0) //왼쪽 위 모서리
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - __top));
						}
						else if (__left + __top == 0) //왼쪽 아래 모서리
						{
							SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
						}
						else if (__right + __top == 0)
						{
							SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - __left));
						}
						else if (__right + __bottom == 0) //오른쪽 위 모서리
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - __top));
						}
						continue;
					}
				}
				collisionRect = GetHitRect2(blockRect); //플레이어랑 블럭이랑 충돌된 영역
				int _left = abs(blockRect.left - collisionRect.left);
				int _right = abs(blockRect.right - collisionRect.right);
				int _top = abs(blockRect.top - collisionRect.top);
				int _bottom = abs(blockRect.bottom - collisionRect.bottom);

				oldBlockX = wallBlock[i].x;
				oldBlockY = wallBlock[i].y;

				SetX(m_nOldX + m_nSpeedX);
				SetY(m_nOldY);
				if (CheckHitWallRect(blockRect)) //x만 이동했을때 충돌하는지 체크
					isX = true;

				SetX(m_nOldX);
				SetY(m_nOldY + m_nSpeedY);
				if (CheckHitWallRect(blockRect)) //y만 이동했을때 충돌하는지 체크
					isY = true;

				if (!isX && !isY) //모서리에 부딛쳤을때
				{
					if (_left + _top == 0) //왼쪽 위 모서리
					{
						if (_right <= _bottom) //x축이 더 길때
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
					if (_top + _right == 0) //오른쪽 위 모서리
					{
						if (_left <= _bottom) //x축이 더 길때
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
					if (_right + _bottom == 0) //오른쪽 아래 모서리
					{
						if (_left <= _top) //x축이 더 길때
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
					if (_left + _bottom == 0) //왼쪽 아래 모서리
					{
						if (_right <= _top) //x축이 더 길때
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
				else if (isX) //x축으로 이동 후 충돌이 발생했을때
				{
					SetX(m_nOldX + m_nSpeedX + (DEFAULT_BLOCK_SIZE - (_left == 0 ? _right : _left)) * (
						_left == 0 ? -1 : 1));
					SetY(m_nOldY + m_nSpeedY);
				}
				else if (isY) //y축으로 이동 후 충돌이 발생했을때
				{
					SetX(m_nOldX + m_nSpeedX);
					SetY(m_nOldY + m_nSpeedY + (DEFAULT_BLOCK_SIZE - (_top == 0 ? _bottom : _top)) * (
						_top == 0 ? -1 : 1));
				}
			}
		}
	}
	else //보스 맵일때
	{
		for (int i = 0; i < 139; i++)
		{
			SetRect(&blockRect, wall[i].GetPos().x * 32, wall[i].GetPos().y * 32, wall[i].GetPos().x * 32 + 32,
			        wall[i].GetPos().y * 32 + 32); //해당 블록의 영역
			if (CheckHitWallRect(blockRect))
			{
				collisionCount++;
				if (collisionCount > 1)
				{
					if (oldBlockX != wall[i].GetPos().x && oldBlockY != wall[i].GetPos().y) //같은 x축
					{
						collisionRect = GetHitRect2(blockRect); //플레이어랑 블럭이랑 충돌된 영역
						int __left = abs(blockRect.left - collisionRect.left);
						int __right = abs(blockRect.right - collisionRect.right);
						int __top = abs(blockRect.top - collisionRect.top);
						int __bottom = abs(blockRect.bottom - collisionRect.bottom);

						if (__bottom + __top + __right == 0) //왼쪽 위 모서리
						{
							//SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
							SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - __left));
						}
						else if (__left + __bottom + __top == 0) //오른쪽 아래 모서리
						{
							//SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
							SetX(GetPos().x - (DEFAULT_BLOCK_SIZE - __right));
						}
						else if (__left + __bottom == 0) //왼쪽 위 모서리
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - __top));
						}
						else if (__left + __top == 0) //왼쪽 아래 모서리
						{
							SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - __bottom));
						}
						else if (__right + __bottom == 0) //오른쪽 위 모서리
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - __top));
						}
						continue;
					}
				}
				collisionRect = GetHitRect2(blockRect); //플레이어랑 블럭이랑 충돌된 영역
				int _left = abs(blockRect.left - collisionRect.left);
				int _right = abs(blockRect.right - collisionRect.right);
				int _top = abs(blockRect.top - collisionRect.top);
				int _bottom = abs(blockRect.bottom - collisionRect.bottom);

				oldBlockX = wall[i].GetPos().x;
				oldBlockY = wall[i].GetPos().y;

				SetX(m_nOldX + m_nSpeedX);
				SetY(m_nOldY);
				if (CheckHitWallRect(blockRect)) //x만 이동했을때 충돌하는지 체크
					isX = true;

				SetX(m_nOldX);
				SetY(m_nOldY + m_nSpeedY);
				if (CheckHitWallRect(blockRect)) //y만 이동했을때 충돌하는지 체크
					isY = true;

				if (!isX && !isY) //모서리에 부딛쳤을때
				{
					if (_left + _top == 0) //왼쪽 위 모서리
					{
						if (_right <= _bottom) //x축이 더 길때
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
					if (_top + _right == 0) //오른쪽 위 모서리
					{
						if (_left <= _bottom) //x축이 더 길때
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
					if (_right + _bottom == 0) //오른쪽 아래 모서리
					{
						if (_left <= _top) //x축이 더 길때
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
					if (_left + _bottom == 0) //왼쪽 아래 모서리
					{
						if (_right <= _top) //x축이 더 길때
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
				else if (isX) //x축으로 이동 후 충돌이 발생했을때
				{
					SetX(m_nOldX + m_nSpeedX + (DEFAULT_BLOCK_SIZE - (_left == 0 ? _right : _left)) * (
						_left == 0 ? -1 : 1));
					SetY(m_nOldY + m_nSpeedY);
				}
				else if (isY) //y축으로 이동 후 충돌이 발생했을때
				{
					SetX(m_nOldX + m_nSpeedX);
					SetY(m_nOldY + m_nSpeedY + (DEFAULT_BLOCK_SIZE - (_top == 0 ? _bottom : _top)) * (
						_top == 0 ? -1 : 1));
				}
			}
		}
	}
}

int Player::GetHp()
{
	return m_nHp;
}

void Player::CreateSkillArrow()
{
	curArrow->SetIsUse(true);
	curArrow->SetCharging(false);
	curArrow->SetX(GetPos().x - 1);
	curArrow->SetY(GetPos().y + 6);
	int randomX = 8 + (rand() % 5);
	int randomY = 8 + (rand() % 5);
	int PosiOrNegaX = rand() % 2; //랜덤한 값을 뽑아 x방향값을 양수 혹은 음수로 변환
	int PosiOrNegaY = rand() % 2; //랜덤한 값을 뽑아 y방향값을 양수 혹은 음수로 변환
	randomX = randomX * (PosiOrNegaX == 0 ? 1 : -1);
	randomY = randomY * (PosiOrNegaY == 0 ? 1 : -1);
	curArrow->SetSpeedXY(20, Vector2(0.1f * randomX, 0.1f * randomY));
	curArrow->CheckSprite();
	curArrow->SetHoming(map.GetStageNum());
	curArrow = NULL;
}

void Player::CheckKeyBoard()
{
	if (((int)curPlayerAction != ACTION::DEAD) && !camera.GetIsFirstAlpha())
	{
		if (!GetIsRoll())
		{
			if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_SPACE))
			{
				if (!IsUsingSkill())
					if (Timer::Elapsed(m_nLastRollTime, 1000))
					{
						m_nLastRollTime = Timer::Now();
						Roll();
					}
			}
			if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_Q))
			{
				Skill();
			}
		}
		if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_A))
		{
			Left();
		}
		if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_D))
		{
			Right();
		}
		if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_W))
		{
			Up();
		}
		if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_S))
		{
			Down();
		}
	}
}

void Player::Draw(LPDIRECTDRAWSURFACE7 surface)
{
	if (m_bIsSkill)
		GameObject::SkillDraw(draw_x, draw_y, surface);
	else
		GameObject::Draw(curPlayerAction == ACTION::MOVE || curPlayerAction == ACTION::RUN
		                 || curPlayerAction == ACTION::ROLL, draw_x, draw_y, surface);
}

void Player::Skill()
{
	if (curPlayerAction != ACTION::DEAD)
	{
		if (Timer::Elapsed(m_nLastSkillTime, SkillCoolTime))
		{
			if (m_bIsSkill == false)
			{
				m_bIsSkill = true;
				m_nLastSkillTime = Timer::Now();
				camera.SetIsSkill(true);
			}
		}
	}
}

void Player::CheckUseSkill()
{
	if (m_bIsSkill)
	{
		if (Timer::Elapsed(m_nLastSkillTime, 5000))
		{
			m_bIsSkill = false;
			camera.SetIsSkill(false);
			//m_nSkillCoolTime = Timer::Now();
		}
	}
}

bool Player::GetIsRoll()
{
	return m_bIsRoll;
}

void Player::Roll()
{
	curPlayerAction = ACTION::ROLL;
	mFrameInterval = 70;
}

void Player::Left()
{
	m_nSpeedX -= mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime;
}

void Player::Right()
{
	m_nSpeedX += mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime;
}

void Player::Up()
{
	m_nSpeedY -= mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime;
}

void Player::Down()
{
	m_nSpeedY += mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime;
}

void Player::Hit()
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

bool Player::CanMove()
{
	if (!mIsLive)
		return false;

	if (Timer::Elapsed(m_nLastMoveTime, m_nMoveInterval))
		return true;
	return false;
}

float Player::GetSpeedX()
{
	return m_nSpeedX;
}

float Player::GetSpeedY()
{
	return m_nSpeedY;
}

float Player::GetSkillCoolTimePercent()
{
	float percent = (Timer::Now() - m_nLastSkillTime).count() / SkillCoolTime;
	percent = percent - ((percent > 1) ? percent - 1 : 0);
	return percent;
}

float Player::GetRollCoolTimePercent()
{
	float percent = (Timer::Now() - m_nLastRollTime).count() / RollCoolTime;
	percent = percent - ((percent > 1) ? percent - 1 : 0);
	return percent;
}

void Player::MoveANDCheckState()
{
	if (curPlayerAction == ACTION::DEAD)
		m_bIsRoll = false;
	if (!(curPlayerAction == ACTION::ROLL) && !(curPlayerAction == ACTION::DEAD))
	{
		m_bIsRoll = false;
		if (camera.GetExpansion() == 1) //화면이 확대가 안되어있을경우
		{
			if (curPlayerAction != ACTION::FAINT)
			{
				curPlayerAction = ACTION::IDLE; //캐릭터의 행동을 기본으로 초기화
			}
		}
		else
			curPlayerAction = ACTION::ATTACK; //캐릭터의 행동을 공격중으로 초기화

		if (!(curPlayerAction == ACTION::ATTACK) && (m_nSpeedX != 0 || m_nSpeedY != 0))
			curPlayerAction = ACTION::MOVE;
	}

	if (m_bIsRoll)
	{
		m_nMoveSpeedFold = 4;
		switch (curPlayerDirection)
		{
		case DIRECTION::LEFT:
			m_nSpeedX = -(mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime);
			m_nSpeedY = 0;
			break;
		case DIRECTION::RIGHT:
			m_nSpeedX = mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime;
			m_nSpeedY = 0;
			break;
		case DIRECTION::UP:
			m_nSpeedX = 0;
			m_nSpeedY = -(mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime);
			break;
		case DIRECTION::DOWN:
			m_nSpeedX = 0;
			m_nSpeedY = mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime;
			break;
		case DIRECTION::LEFTUP:
			m_nSpeedX = -(mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime);
			m_nSpeedY = -(mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime);
			break;
		case DIRECTION::RIGHTUP:
			m_nSpeedX = mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime;
			m_nSpeedY = -(mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime);
			break;
		case DIRECTION::LEFTDOWN:
			m_nSpeedX = -(mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime);
			m_nSpeedY = mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime;
			break;
		case DIRECTION::RIGHTDOWN:
			m_nSpeedX = mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime;
			m_nSpeedY = mMoveSpeed * m_nMoveSpeedFold * Timer::deltaTime;
			break;
		}
	}

	if (!(curPlayerAction == ACTION::ATTACK) &&
		!(curPlayerAction == ACTION::IDLE) && !(curPlayerAction == ACTION::DEAD)) //공격중이거나 기본상태가 아닐경우 캐릭터를 움직여준다
	{
		m_nOldX = GetPos().x;
		m_nOldY = GetPos().y;
		SetX(GetPos().x + m_nSpeedX);
		SetY(GetPos().y + m_nSpeedY);
	}

	CheckWallCollision();

	draw_x = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f));
	draw_y = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f));

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

		m_pCurSprite = !(curPlayerAction == ACTION::ROLL)
			               ? m_pWalkSprite->GetSprite(curPlayerDirection)
			               : m_pRollSprite->GetSprite(curPlayerDirection);
		m_pCurBowSprite = !(curPlayerAction == ACTION::ROLL)
			                  ? m_pBowWalkSprite->GetSprite(curPlayerDirection)
			                  : m_pBow_RollSprite->GetSprite(curPlayerDirection);

		if (curPlayerAction == ACTION::ATTACK)
			m_pCurBowSprite = m_pBow_AttackSprite->GetSprite(curPlayerDirection);
	}

	SetHitRect((curPlayerAction != ACTION::ROLL)
		           ? walkHitRect[(int)curPlayerDirection - 1]
		           : rollHitRect[(int)curPlayerDirection - 1][mCurrentFrame]);

	if (curPlayerAction == ACTION::FAINT)
	{
		m_pCurSprite = m_pDeadSprite->GetSprite(DIRECTION::RIGHT);
	}

	if (curPlayerAction == ACTION::DEAD)
	{
		m_pCurSprite = m_pDeadSprite->GetSprite(curPlayerDirection);
	}

	if (m_bIsSkill)
		m_pCurSprite = m_pSkillSprite;

	if ((m_pOldSprite != m_pCurSprite && !m_bIsRoll) || (m_pOldBowSprite != m_pCurBowSprite))
	{
		m_pOldSprite = m_pCurSprite;
		m_pOldBowSprite = m_pCurBowSprite;
		GameObject::SetSprite(m_pCurSprite, m_pCurBowSprite);
		if (curPlayerAction == ACTION::ROLL)
		{
			m_bIsRoll = true;
		}
	}
}

void Player::MoveInit()
{
	m_nSpeedX = 0;
	m_nSpeedY = 0;
	if (curPlayerAction != ACTION::ROLL)
	{
		m_nMoveSpeedFold = 2;
		mFrameInterval = 140;
	}
	m_bIsUseBow = false;
}

bool Player::IsArrowReady()
{
	if (curArrow == NULL)
		return false;
	return true;
}
