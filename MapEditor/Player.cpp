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

extern EDirection curPlayerDirection;
extern EAction curPlayerAction;
extern EAction curBossAction;

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
	: GameObject()
	, mIsSkill(false)
	, mIsMove(false)
	, mIsUseBow(false)
	, mIsRoll(false)
	, mLastMoveTime(Timer::Now())
	, mMoveInterval(0)
	, mLastSkillTime(Timer::Now())
	, mLastRollTime(Timer::Now())
	, mSpeedX(0)
	, mSpeedY(0)
	, mOldX(0)
	, mOldY(0)
	, mMoveSpeedFold(2)
	, mDrawX(0)
	, mDrawY(0)
	, mHp(8)
	, mCurArrowPtr(nullptr)
	, mCurSpritePtr(nullptr)
	, mOldSpritePtr(nullptr)
	, mCurBowSpritePtr(nullptr)
	, mOldBowSpritePtr(nullptr)
	, mSkillSpritePtr(nullptr)
	, mWalkSpritePtr(nullptr)
	, mRollSpritePtr(nullptr)
	, mDeadSpritePtr(nullptr)
	, mBowWalkSpritePtr(nullptr)
	, mBowRollSpritePtr(nullptr)
	, mBowAttackSpritePtr(nullptr)
{
	curPlayerDirection = EDirection::Down;
	curPlayerAction = EAction::Faint;
	mCurState = EObjectType::Player;
	mMoveSpeed = 60;

	int a = 0, b = 0;
	SetRect(&mWalkHitRect[a++], 4, 12, 6, 10);
	SetRect(&mWalkHitRect[a++], 6, 12, 4, 12);
	SetRect(&mWalkHitRect[a++], 6, 12, 6, 12);
	SetRect(&mWalkHitRect[a++], 6, 12, 6, 12);
	SetRect(&mWalkHitRect[a++], 4, 12, 7, 12);
	SetRect(&mWalkHitRect[a++], 7, 12, 5, 12);
	SetRect(&mWalkHitRect[a++], 6, 12, 7, 12);
	SetRect(&mWalkHitRect[a], 7, 12, 6, 12);
	a = 0;
	SetRect(&mRollHitRect[a][b++], 4, 8, 4, 14);
	SetRect(&mRollHitRect[a][b++], 8, 2, 8, 10);
	SetRect(&mRollHitRect[a][b++], 6, 2, 6, 14);
	SetRect(&mRollHitRect[a][b++], 6, 0, 6, 14);
	SetRect(&mRollHitRect[a][b++], 8, 2, 10, 10);
	SetRect(&mRollHitRect[a][b], 4, 14, 4, 6);
	a++;
	b = 0;
	SetRect(&mRollHitRect[a][b++], 8, 8, 2, 12);
	SetRect(&mRollHitRect[a][b++], 8, 2, 8, 10);
	SetRect(&mRollHitRect[a][b++], 6, 2, 8, 16);
	SetRect(&mRollHitRect[a][b++], 8, -4, 6, 16);
	SetRect(&mRollHitRect[a][b++], 10, 2, 6, 10);
	SetRect(&mRollHitRect[a][b], 4, 14, 4, 6);
	a++;
	b = 0;
	SetRect(&mRollHitRect[a][b++], 6, 10, 6, 14);
	SetRect(&mRollHitRect[a][b++], 6, 6, 6, 12);
	SetRect(&mRollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&mRollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&mRollHitRect[a][b++], 6, 4, 6, 12);
	SetRect(&mRollHitRect[a][b], 8, 8, 6, 6);
	a++;
	b = 0;
	SetRect(&mRollHitRect[a][b++], 6, 12, 6, 14);
	SetRect(&mRollHitRect[a][b++], 6, 6, 6, 12);
	SetRect(&mRollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&mRollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&mRollHitRect[a][b++], 6, 4, 6, 10);
	SetRect(&mRollHitRect[a][b], 6, 10, 6, 6);
	a++;
	b = 0;
	SetRect(&mRollHitRect[a][b++], 8, 10, 4, 12);
	SetRect(&mRollHitRect[a][b++], 10, 2, 8, 10);
	SetRect(&mRollHitRect[a][b++], 8, -2, 4, 16);
	SetRect(&mRollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&mRollHitRect[a][b++], 7, -2, 8, 12);
	SetRect(&mRollHitRect[a][b], 6, 14, 4, 8);
	a++;
	b = 0;
	SetRect(&mRollHitRect[a][b++], 10, 10, 2, 12);
	SetRect(&mRollHitRect[a][b++], 8, 2, 12, 10);
	SetRect(&mRollHitRect[a][b++], 4, -2, 8, 16);
	SetRect(&mRollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&mRollHitRect[a][b++], 8, -2, 6, 12);
	SetRect(&mRollHitRect[a][b], 4, 14, 6, 8);
	a++;
	b = 0;
	SetRect(&mRollHitRect[a][b++], 6, 10, 6, 12);
	SetRect(&mRollHitRect[a][b++], 10, 4, 4, 10);
	SetRect(&mRollHitRect[a][b++], 8, -2, 4, 16);
	SetRect(&mRollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&mRollHitRect[a][b++], 2, 2, 8, 14);
	SetRect(&mRollHitRect[a][b], 8, 14, 6, 6);
	a++;
	b = 0;
	SetRect(&mRollHitRect[a][b++], 10, 8, 4, 12);
	SetRect(&mRollHitRect[a][b++], 6, 4, 12, 10);
	SetRect(&mRollHitRect[a][b++], 6, -4, 10, 14);
	SetRect(&mRollHitRect[a][b++], 6, 0, 6, 16);
	SetRect(&mRollHitRect[a][b++], 10, 0, 4, 14);
	SetRect(&mRollHitRect[a][b], 8, 14, 8, 6);
}

void Player::Initialize(int x, int y, int currentFrame, int frameInterval, int moveInterval,
	int skillFrameInterval)
{
	new (this) Player();

	mDrawX = x;
	mDrawY = y;
	mOldX = x;
	mOldY = y;

	GameObject::Initialize(mCurSpritePtr, x, y, currentFrame, frameInterval, skillFrameInterval);

	mMoveInterval = moveInterval;
}

/// <summary>
/// 처음 스프라이트랑 충돌 영역을 설정해준다.
/// </summary>
void Player::SetSpriteAndHitRect()
{
	mCurSpritePtr = mWalkSpritePtr->GetSprite(EDirection::Down);
	mOldSpritePtr = mWalkSpritePtr->GetSprite(EDirection::Up);
	SetSprite(mCurSpritePtr);
	SetHitWallRect();
}

void Player::SetBowSprite(Sprite8* _walk, Sprite8* _roll, Sprite8* _attack)
{
	mBowWalkSpritePtr = _walk;
	mBowRollSpritePtr = _roll;
	mBowAttackSpritePtr = _attack;
	SetSprite(nullptr, _walk->GetSprite(EDirection::Down));
}

void Player::SetArrow(Arrow* arrow)
{
	mCurArrowPtr = arrow;
	if (!IsArrowNull())
		mCurArrowPtr->SetIsUse(false);
}

bool Player::IsArrowNull() const
{
	for (int i = 0; i < TOTAL_ARROW; i++)
	{
		if (mCurArrowPtr == &arrow[i])
			return false;
	}
	return true;
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
		for (const Vector2& blockPos : wallBlock)
		{
			SetRect(&blockRect, blockPos.x * 32, blockPos.y * 32, blockPos.x * 32 + 32,
			        blockPos.y * 32 + 32); //해당 블록의 영역
			if (CheckHitWallRect(blockRect))
			{
				collisionCount++;
				if (collisionCount > 1)
				{
					if (oldBlockX != blockPos.x && oldBlockY != blockPos.y)
					{
						collisionRect = GetHitRect2(blockRect); //플레이어랑 블럭이랑 충돌된 영역
						const int left = abs(blockRect.left - collisionRect.left);
						const int right = abs(blockRect.right - collisionRect.right);
						const int top = abs(blockRect.top - collisionRect.top);
						const int bottom = abs(blockRect.bottom - collisionRect.bottom);

						if (bottom + top + right == 0) //왼쪽 위 모서리
						{
							SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - left));
						}
						else if (left + bottom + top == 0) //오른쪽 아래 모서리
						{
							SetX(GetPos().x - (DEFAULT_BLOCK_SIZE - right));
						}
						else if (left + bottom == 0) //왼쪽 위 모서리
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - top));
						}
						else if (left + top == 0) //왼쪽 아래 모서리
						{
							SetY(GetPos().y - (DEFAULT_BLOCK_SIZE - bottom));
						}
						else if (right + top == 0)
						{
							SetX(GetPos().x + (DEFAULT_BLOCK_SIZE - left));
						}
						else if (right + bottom == 0) //오른쪽 위 모서리
						{
							SetY(GetPos().y + (DEFAULT_BLOCK_SIZE - top));
						}
						continue;
					}
				}
				collisionRect = GetHitRect2(blockRect); //플레이어랑 블럭이랑 충돌된 영역
				int left = abs(blockRect.left - collisionRect.left);
				int right = abs(blockRect.right - collisionRect.right);
				int top = abs(blockRect.top - collisionRect.top);
				int bottom = abs(blockRect.bottom - collisionRect.bottom);

				oldBlockX = blockPos.x;
				oldBlockY = blockPos.y;

				SetX(mOldX + mSpeedX);
				SetY(mOldY);
				if (CheckHitWallRect(blockRect)) //x만 이동했을때 충돌하는지 체크
					isX = true;

				SetX(mOldX);
				SetY(mOldY + mSpeedY);
				if (CheckHitWallRect(blockRect)) //y만 이동했을때 충돌하는지 체크
					isY = true;

				if (!isX && !isY) //모서리에 부딛쳤을때
				{
					if (left + top == 0) //왼쪽 위 모서리
					{
						if (right <= bottom) //x축이 더 길때
						{
							SetX(mOldX + mSpeedX);
							SetY(mOldY + mSpeedY - (DEFAULT_BLOCK_SIZE - bottom));
						}
						else
						{
							SetX(mOldX + mSpeedX - (DEFAULT_BLOCK_SIZE - right));
							SetY(mOldY + mSpeedY);
						}
					}
					if (top + right == 0) //오른쪽 위 모서리
					{
						if (left <= bottom) //x축이 더 길때
						{
							SetX(mOldX + mSpeedX);
							SetY(mOldY + mSpeedY - (DEFAULT_BLOCK_SIZE - bottom));
						}
						else
						{
							SetX(mOldX + mSpeedX + (DEFAULT_BLOCK_SIZE - left));
							SetY(mOldY + mSpeedY);
						}
					}
					if (right + bottom == 0) //오른쪽 아래 모서리
					{
						if (left <= top) //x축이 더 길때
						{
							SetX(mOldX + mSpeedX);
							SetY(mOldY + mSpeedY + (DEFAULT_BLOCK_SIZE - top));
						}
						else
						{
							SetX(mOldX + mSpeedX + (DEFAULT_BLOCK_SIZE - left));
							SetY(mOldY + mSpeedY);
						}
					}
					if (left + bottom == 0) //왼쪽 아래 모서리
					{
						if (right <= top) //x축이 더 길때
						{
							SetX(mOldX + mSpeedX);
							SetY(mOldY + mSpeedY + (DEFAULT_BLOCK_SIZE - top));
						}
						else
						{
							SetX(mOldX + mSpeedX - (DEFAULT_BLOCK_SIZE - right));
							SetY(mOldY + mSpeedY);
						}
					}
				}
				else if (isX) //x축으로 이동 후 충돌이 발생했을때
				{
					SetX(mOldX + mSpeedX + (DEFAULT_BLOCK_SIZE - (left == 0 ? right : left)) * (
						left == 0 ? -1 : 1));
					SetY(mOldY + mSpeedY);
				}
				else if (isY) //y축으로 이동 후 충돌이 발생했을때
				{
					SetX(mOldX + mSpeedX);
					SetY(mOldY + mSpeedY + (DEFAULT_BLOCK_SIZE - (top == 0 ? bottom : top)) * (
						top == 0 ? -1 : 1));
				}
			}
		}
	}
	else //보스 맵일때
	{
		for (const Block& block : wall)
		{
			SetRect(&blockRect, block.GetPos().x * 32, block.GetPos().y * 32, block.GetPos().x * 32 + 32,
			        block.GetPos().y * 32 + 32); //해당 블록의 영역
			if (CheckHitWallRect(blockRect))
			{
				collisionCount++;
				if (collisionCount > 1)
				{
					if (oldBlockX != block.GetPos().x && oldBlockY != block.GetPos().y) //같은 x축
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
				int left = abs(blockRect.left - collisionRect.left);
				int right = abs(blockRect.right - collisionRect.right);
				int top = abs(blockRect.top - collisionRect.top);
				int bottom = abs(blockRect.bottom - collisionRect.bottom);

				oldBlockX = block.GetPos().x;
				oldBlockY = block.GetPos().y;

				SetX(mOldX + mSpeedX);
				SetY(mOldY);
				if (CheckHitWallRect(blockRect)) //x만 이동했을때 충돌하는지 체크
					isX = true;

				SetX(mOldX);
				SetY(mOldY + mSpeedY);
				if (CheckHitWallRect(blockRect)) //y만 이동했을때 충돌하는지 체크
					isY = true;

				if (!isX && !isY) //모서리에 부딛쳤을때
				{
					if (left + top == 0) //왼쪽 위 모서리
					{
						if (right <= bottom) //x축이 더 길때
						{
							SetX(mOldX + mSpeedX);
							SetY(mOldY + mSpeedY - (DEFAULT_BLOCK_SIZE - bottom));
						}
						else
						{
							SetX(mOldX + mSpeedX - (DEFAULT_BLOCK_SIZE - right));
							SetY(mOldY + mSpeedY);
						}
					}
					if (top + right == 0) //오른쪽 위 모서리
					{
						if (left <= bottom) //x축이 더 길때
						{
							SetX(mOldX + mSpeedX);
							SetY(mOldY + mSpeedY - (DEFAULT_BLOCK_SIZE - bottom));
						}
						else
						{
							SetX(mOldX + mSpeedX + (DEFAULT_BLOCK_SIZE - left));
							SetY(mOldY + mSpeedY);
						}
					}
					if (right + bottom == 0) //오른쪽 아래 모서리
					{
						if (left <= top) //x축이 더 길때
						{
							SetX(mOldX + mSpeedX);
							SetY(mOldY + mSpeedY + (DEFAULT_BLOCK_SIZE - top));
						}
						else
						{
							SetX(mOldX + mSpeedX + (DEFAULT_BLOCK_SIZE - left));
							SetY(mOldY + mSpeedY);
						}
					}
					if (left + bottom == 0) //왼쪽 아래 모서리
					{
						if (right <= top) //x축이 더 길때
						{
							SetX(mOldX + mSpeedX);
							SetY(mOldY + mSpeedY + (DEFAULT_BLOCK_SIZE - top));
						}
						else
						{
							SetX(mOldX + mSpeedX - (DEFAULT_BLOCK_SIZE - right));
							SetY(mOldY + mSpeedY);
						}
					}
				}
				else if (isX) //x축으로 이동 후 충돌이 발생했을때
				{
					SetX(mOldX + mSpeedX + (DEFAULT_BLOCK_SIZE - (left == 0 ? right : left)) * (
						left == 0 ? -1 : 1));
					SetY(mOldY + mSpeedY);
				}
				else if (isY) //y축으로 이동 후 충돌이 발생했을때
				{
					SetX(mOldX + mSpeedX);
					SetY(mOldY + mSpeedY + (DEFAULT_BLOCK_SIZE - (top == 0 ? bottom : top)) * (
						top == 0 ? -1 : 1));
				}
			}
		}
	}
}

void Player::CreateSkillArrow()
{
	mCurArrowPtr->SetIsUse(true);
	mCurArrowPtr->SetCharging(false);
	mCurArrowPtr->SetX(GetPos().x - 1);
	mCurArrowPtr->SetY(GetPos().y + 6);
	int randomX = 8 + (rand() % 5);
	int randomY = 8 + (rand() % 5);
	int PosiOrNegaX = rand() % 2; //랜덤한 값을 뽑아 x방향값을 양수 혹은 음수로 변환
	int PosiOrNegaY = rand() % 2; //랜덤한 값을 뽑아 y방향값을 양수 혹은 음수로 변환
	randomX = randomX * (PosiOrNegaX == 0 ? 1 : -1);
	randomY = randomY * (PosiOrNegaY == 0 ? 1 : -1);
	mCurArrowPtr->SetSpeedXY(20, Vector2(0.1f * randomX, 0.1f * randomY));
	mCurArrowPtr->CheckSprite();
	mCurArrowPtr->SetHoming(map.GetStageNum());
	mCurArrowPtr = nullptr;
}

void Player::CheckKeyBoard()
{
	if ((curPlayerAction != EAction::Dead) && !camera.GetIsFirstAlpha())
	{
		if (!GetIsRoll())
		{
			if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_SPACE))
			{
				if (!IsUsingSkill())
					if (Timer::Elapsed(mLastRollTime, RollCoolTime))
					{
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
	if (mIsSkill)
		GameObject::SkillDraw(mDrawX, mDrawY, surface);
	else
		GameObject::Draw(curPlayerAction == EAction::Move || curPlayerAction == EAction::Run
			|| curPlayerAction == EAction::Roll, mDrawX, mDrawY, surface);
}

void Player::Skill()
{
	if (curPlayerAction != EAction::Dead)
	{
		if (Timer::Elapsed(mLastSkillTime, SkillCoolTime))
		{
			if (mIsSkill == false)
			{
				mIsSkill = true;
				camera.SetIsSkill(true);
			}
		}
	}
}

void Player::CheckUseSkill()
{
	if (mIsSkill)
	{
		if (Timer::Elapsed(mLastSkillTime, 5000))
		{
			mIsSkill = false;
			camera.SetIsSkill(false);
			//m_nSkillCoolTime = Timer::Now();
		}
	}
}

void Player::Roll()
{
	curPlayerAction = EAction::Roll;
	mFrameInterval = 70;
}

void Player::Hit()
{
	mHp -= 1;
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
	if (mHp == 0)
	{
		curPlayerAction = EAction::Dead;
	}
}

bool Player::CanMove()
{
	if (!mIsLive)
		return false;

	if (Timer::Elapsed(mLastMoveTime, mMoveInterval))
		return true;
	return false;
}

float Player::GetSkillCoolTimePercent() const
{
	float percent = static_cast<float>(duration_cast<milliseconds>(Timer::Now() - mLastSkillTime).count()) / SkillCoolTime;
	percent = (percent > 1) ? 1 : percent;
	return percent;
}

float Player::GetRollCoolTimePercent() const
{
	float percent = static_cast<float>(duration_cast<milliseconds>(Timer::Now() - mLastRollTime).count()) / RollCoolTime;
	percent = (percent > 1) ? 1 : percent;
	return percent;
}

void Player::MoveANDCheckState()
{
	if (curPlayerAction == EAction::Dead)
		mIsRoll = false;
	if (!(curPlayerAction == EAction::Roll) && !(curPlayerAction == EAction::Dead))
	{
		mIsRoll = false;
		if (camera.GetExpansion() == 1) //화면이 확대가 안되어있을경우
		{
			if (curPlayerAction != EAction::Faint)
			{
				curPlayerAction = EAction::Idle; //캐릭터의 행동을 기본으로 초기화
			}
		}
		else
			curPlayerAction = EAction::Attack; //캐릭터의 행동을 공격중으로 초기화

		if (!(curPlayerAction == EAction::Attack) && (mSpeedX != 0 || mSpeedY != 0))
			curPlayerAction = EAction::Move;
	}

	if (mIsRoll)
	{
		mMoveSpeedFold = 4;
		switch (curPlayerDirection)
		{
		case EDirection::Left:
			mSpeedX = -(mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime());
			mSpeedY = 0;
			break;
		case EDirection::Right:
			mSpeedX = mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime();
			mSpeedY = 0;
			break;
		case EDirection::Up:
			mSpeedX = 0;
			mSpeedY = -(mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime());
			break;
		case EDirection::Down:
			mSpeedX = 0;
			mSpeedY = mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime();
			break;
		case EDirection::LeftUp:
			mSpeedX = -(mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime());
			mSpeedY = -(mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime());
			break;
		case EDirection::RightUp:
			mSpeedX = mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime();
			mSpeedY = -(mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime());
			break;
		case EDirection::LeftDown:
			mSpeedX = -(mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime());
			mSpeedY = mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime();
			break;
		case EDirection::RightDown:
			mSpeedX = mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime();
			mSpeedY = mMoveSpeed * mMoveSpeedFold * Timer::GetDeltaTime();
			break;
		}
	}

	if (!(curPlayerAction == EAction::Attack) &&
		!(curPlayerAction == EAction::Idle) && !(curPlayerAction == EAction::Dead)) //공격중이거나 기본상태가 아닐경우 캐릭터를 움직여준다
	{
		mOldX = GetPos().x;
		mOldY = GetPos().y;
		SetX(GetPos().x + mSpeedX);
		SetY(GetPos().y + mSpeedY);
	}

	CheckWallCollision();

	mDrawX = (int)(-(camera.GetX() - GetPos().x) + (SCREEN_WIDTH * 0.5f));
	mDrawY = (int)(-(camera.GetY() - GetPos().y) + (SCREEN_HEIGHT * 0.5f));

	if (!mIsRoll)
	{
		if (mSpeedX == 0)
		{
			if (mSpeedY < 0)
				curPlayerDirection = EDirection::Up;
			else if (mSpeedY > 0)
				curPlayerDirection = EDirection::Down;
		}
		else if (mSpeedX < 0)
		{
			if (mSpeedY == 0)
				curPlayerDirection = EDirection::Left;
			else if (mSpeedY < 0)
				curPlayerDirection = EDirection::LeftUp;
			else if (mSpeedY > 0)
				curPlayerDirection = EDirection::LeftDown;
		}
		else if (mSpeedX > 0)
		{
			if (mSpeedY == 0)
				curPlayerDirection = EDirection::Right;
			else if (mSpeedY < 0)
				curPlayerDirection = EDirection::RightUp;
			else if (mSpeedY > 0)
				curPlayerDirection = EDirection::RightDown;
		}

		if (curPlayerAction == EAction::Attack)
		{
			if (attackDirection.y < 0)
			{
				if ((attackDirection.x > -0.3f) && (attackDirection.x < 0.3f))
					curPlayerDirection = EDirection::Up;
				else if (attackDirection.x < -0.3f)
					curPlayerDirection = EDirection::LeftUp;
				else if (attackDirection.x > 0.3f)
					curPlayerDirection = EDirection::RightUp;
			}
			else
			{
				if ((attackDirection.x > -0.3f) && (attackDirection.x < 0.3f))
					curPlayerDirection = EDirection::Down;
				else if (attackDirection.x < -0.3f)
					curPlayerDirection = EDirection::LeftDown;
				else if (attackDirection.x > 0.3f)
					curPlayerDirection = EDirection::RightDown;
			}
			if (attackDirection.x < 0)
			{
				if ((attackDirection.y > -0.3f) && (attackDirection.y < 0.3f))
					curPlayerDirection = EDirection::Left;
			}
			else
			{
				if ((attackDirection.y > -0.3f) && (attackDirection.y < 0.3f))
					curPlayerDirection = EDirection::Right;
			}
		}

		mCurSpritePtr = !(curPlayerAction == EAction::Roll)
			? mWalkSpritePtr->GetSprite(curPlayerDirection)
			: mRollSpritePtr->GetSprite(curPlayerDirection);
		mCurBowSpritePtr = !(curPlayerAction == EAction::Roll)
			? mBowWalkSpritePtr->GetSprite(curPlayerDirection)
			: mBowRollSpritePtr->GetSprite(curPlayerDirection);

		if (curPlayerAction == EAction::Attack)
			mCurBowSpritePtr = mBowAttackSpritePtr->GetSprite(curPlayerDirection);
	}

	SetHitRect((curPlayerAction != EAction::Roll)
		? mWalkHitRect[(int)curPlayerDirection - 1]
		: mRollHitRect[(int)curPlayerDirection - 1][mCurrentFrame]);

	if (curPlayerAction == EAction::Faint)
	{
		mCurSpritePtr = mDeadSpritePtr->GetSprite(EDirection::Right);
	}

	if (curPlayerAction == EAction::Dead)
	{
		mCurSpritePtr = mDeadSpritePtr->GetSprite(curPlayerDirection);
	}

	if (mIsSkill)
		mCurSpritePtr = mSkillSpritePtr;

	if ((mOldSpritePtr != mCurSpritePtr && !mIsRoll) || (mOldBowSpritePtr != mCurBowSpritePtr))
	{
		mOldSpritePtr = mCurSpritePtr;
		mOldBowSpritePtr = mCurBowSpritePtr;
		SetSprite(mCurSpritePtr, mCurBowSpritePtr);
		if (curPlayerAction == EAction::Roll)
		{
			mIsRoll = true;
		}
	}
}

void Player::MoveInit()
{
	mSpeedX = 0;
	mSpeedY = 0;
	if (curPlayerAction != EAction::Roll)
	{
		mMoveSpeedFold = 2;
		mFrameInterval = 140;
	}
	mIsUseBow = false;
}

bool Player::IsArrowReady() const
{
	if (mCurArrowPtr == nullptr)
		return false;
	return true;
}
