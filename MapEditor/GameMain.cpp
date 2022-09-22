#include <Windows.h>
#include <stdio.h>
#include <ddraw.h>
#include <dinput.h>
#include <dsound.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "Bmp.h"
#include "Timer.h"
#include "Sprite.h"
#include "SettingData.h"
#include "Block.h"
#include "Player.h"
#include "Camera.h"
#include "WorldMapBmp.h"
#include "Sprite8.h"
#include "Arrow.h"
#include "Boss.h"
#include "DSUTIL.h"
#include "SnowBall.h"
#include "Gui.h"
#include "Map.h"
#include "GameEnum.h"
#include "Vector2.h"
#include "Fireblock.h"
#include "SkillUI.h"

using namespace std;

extern BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);
extern LPDIRECTDRAWSURFACE7 g_lpPrimarySurface;
extern LPDIRECTDRAWSURFACE7 g_lpSecondarySurface;
extern LPDIRECTDRAW7 g_lpDirectDrawObject;
extern LPDIRECTINPUT8 g_lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 g_lpDirectInputKeyboard;

extern Sprite grassSprite;
extern Sprite player_skill;
extern Sprite player_hp;
extern Sprite skill_background;
extern Sprite skill_roll;
extern Sprite skill_cyclone;
extern Sprite tutorialSprite;
extern Sprite boss_sleep;
extern Sprite boss_snowball;
extern Sprite boss_hp_window;
extern Sprite boss_hp;
extern Sprite enemy_idle_left;
extern Sprite enemy_idle_right;
extern Sprite enemy_up_left;
extern Sprite enemy_up_right;
extern Sprite enemy_hide_left;
extern Sprite enemy_hide_right;
extern Sprite enemy_attack_left;
extern Sprite enemy_attack_right;
extern Sprite8 player_walk;
extern Sprite8 player_roll;
extern Sprite8 player_dead;
extern Sprite8 bow_walk;
extern Sprite8 bow_roll;
extern Sprite8 bow_attack;
extern Sprite8 arrowSprite;
extern Sprite8 boss_idle;
extern Sprite8 boss_roll;
extern Sprite8 boss_attack;
extern Sprite8 boss_dead;
extern Block g_blocks[BLOCK_Y][BLOCK_X];
extern Player player;
extern Boss boss;
extern Camera camera;
extern WorldMap baseMap;
extern WorldMap bossMap;
extern WorldMap bossMapRoof;
extern Map map;
extern Sprite fireSprite;
extern FireBlock fireBlock;

extern EDirection curPlayerDirection;
extern EDirection curBossDirection;

extern EAction curPlayerAction;
extern EAction curBossAction;

extern Arrow arrow[TOTAL_ARROW];
extern SnowBall snowball[TOTAL_SNOWBALL];

extern Vector2 attackDirection;

extern HDC hdc;

extern Gui bossHpBarBack;
extern Gui bossHpBar;
extern Gui playerHp1;
extern Gui playerHp2;
extern Gui playerHp3;
extern Gui playerHp4;
extern Gui tutorial;
extern SkillUI skillRoll;
extern SkillUI skillCyclone;

extern HSNDOBJ Sound[10];
extern void _Play(int num);
Block wall[139];
vector<int> wallBlockData3;
vector<Vector2> wallBlock;

bool FrameRate();
void InitGame();
extern int cameraPositionX;
extern int cameraPositionY;
TCHAR buffer[100];
TCHAR buffer2[40];
TCHAR buffer3[80];
TCHAR buffer4[40];

extern BOOL m_bIntroFirst;
extern BOOL m_bGameFirst;
extern BOOL m_bEditorFirst;
bool ischeck = false;
bool isSound = false;
int player_lastRollTime;
int cntVibes;
bool boss_HitWall;
bool IsPlayerNoHit;
extern bool IsSkipTutorial;

system_clock::time_point lastFrameTime;
system_clock::time_point lastSkillShootTime;		// todo : 캐릭터한테 넣기
system_clock::time_point lastPlayerHitTime;

// todo : 플레이어 한테 넣기
Arrow* FindNotUseArrow()
{
	for (Arrow& arr : arrow)
	{
		if (!arr.IsUse())
			return &arr;
	}

	return nullptr;
}

void GameMain(void)
{
	if (m_bGameFirst)
		InitGame();

	if (FrameRate())
	{
		int i, j;
		camera.InitExpansion();

		// 사용할수 있는 화살이 있을때
		if (player.IsArrowNull())
			//if (!arrow[0].IsZeroArrow())
			if (true)
				player.SetArrow(FindNotUseArrow());

		if (!IsPlayerNoHit)
		{
			if (Timer::Elapsed(lastPlayerHitTime, 400))
			{
				IsPlayerNoHit = true;
			}
		}

		if (!player.IsArrowNull())
		{
			if (player.GetCurArrow()->IsCharging())
			{
				//활을 쐈을 때
				if (!camera.IsExpansion())
				{
					player.GetCurArrow()->SetIsUse(true);
					player.GetCurArrow()->SetCharging(false);
					player.GetCurArrow()->SetX(player.GetPos().x - 1);
					player.GetCurArrow()->SetY(player.GetPos().y + 6);
					player.GetCurArrow()->SetSpeedXY(2.5f * powf(camera.GetExpansion(), 12), attackDirection);
					player.GetCurArrow()->CheckSprite();
					player.SetArrow(nullptr);
					SndObjPlay(Sound[3], NULL);
					if (player.IsArrowNull())
						player.SetArrow(FindNotUseArrow());
				}
			}
		}

		camera.CheckExpansion();

		camera.CheckFinishXY();

		if (boss_HitWall && (cntVibes < 4))
		{
			camera.SetFinishXY(camera.GetFinishX(), camera.GetFinishY());
			cntVibes++;
		}

		camera.Move();

		map.SetPos(camera.GetX(), camera.GetY());
		map.Drawing(g_lpSecondarySurface, g_lpDirectDrawObject);

		if (player.CanMove()) //이동
		{
			player.MoveInit();
			player.CheckKeyBoard();
			player.MoveANDCheckState();
			Vector2 curPlayerPos = player.GetPos();
			if (map.GetStageNum() == 0 && curPlayerPos.x > 1540 && curPlayerPos.x < 1630 && curPlayerPos.y > 680 &&
				curPlayerPos.y < 712)
			{
				map.NextStage();
				player.SetXY(1600, 2500);
			}
		}

		if (!player.IsArrowNull())
		{
			if (player.GetCurArrow()->IsCharging())
			{
				if (!isSound)
				{
					SndObjPlay(Sound[2], NULL);
					isSound = true;
				}
				player.GetCurArrow()->SetX(player.GetPos().x - 1);
				player.GetCurArrow()->SetY(player.GetPos().y + 6);
				player.GetCurArrow()->SetSpeedXY(20 - (30 * (camera.GetExpansion() - 1)), attackDirection);
				player.GetCurArrow()->CheckMove();
				player.GetCurArrow()->CheckSprite();
				if (!(curPlayerAction == EAction::Roll) && (curPlayerAction != EAction::Dead))
					player.GetCurArrow()->Draw(g_lpSecondarySurface);
			}
		}

		if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_U))
		{
			IsPlayerNoHit = true;
		}

		player.CheckUseSkill();

		if (player.IsUsingSkill())
		{
			if (Timer::Elapsed(lastSkillShootTime, 100))
			{
				player.CreateSkillArrow();
				SndObjPlay(Sound[3], NULL);
				if (player.IsArrowNull())
					player.SetArrow(FindNotUseArrow());
			}
		}

		for (i = 0; i < TOTAL_ARROW; i++)
		{
			if (arrow[i].IsUse())
			{
				if (arrow[i].IsHoming())
				{
					if (map.GetStageNum() == 1)
					{
						if (abs(arrow[i].GetPos().x - boss.GetPos().x) < 45)
							if (abs(arrow[i].GetPos().y - boss.GetPos().y) < 45)
							{
								arrow[i].IsHit();
								boss.Hit(10);
							}
					}
				}
				if (!arrow[i].IsSpeedZero())
				{
					if (map.GetStageNum() == 1)
					{
						if (!arrow[i].GetIsHit() && !arrow[i].IsCharging())
						{
							if (arrow[i].CheckHit(boss.GetHitRect()) && !arrow[i].IsHoming())
							{
								arrow[i].IsHit();
								boss.Hit(arrow[i].GetPower());

								if (curBossAction == EAction::Sleep)
								{
									boss.NextPattern();
									SndObjStop(Sound[4]);
									SndObjPlay(Sound[0], DSBPLAY_LOOPING);
								}
							}
						}
						if (abs(static_cast<int>(boss.GetPos().x - arrow[i].GetPos().x)) < 50)
						{
							if (abs(static_cast<int>(boss.GetPos().y - arrow[i].GetPos().y)) < 50)
							{
								arrow[i].IsHit();
								boss.Hit(arrow[i].GetPower());

								if (curBossAction == EAction::Sleep)
								{
									boss.NextPattern();
									SndObjStop(Sound[4]);
									SndObjPlay(Sound[0], DSBPLAY_LOOPING);
								}
							}
						}
					}
				}
				arrow[i].CheckMove();
				arrow[i].Draw(g_lpSecondarySurface);
			}
		}

		player.Draw(g_lpSecondarySurface);

		fireBlock.Draw(g_lpSecondarySurface);

		//보스가 구르기 패턴 공격을 할때
		if (curBossAction == EAction::Roll)
		{
			if (curPlayerAction != EAction::Dead)
			{
				//플레이어가 공격당하지 않았다면
				if (IsPlayerNoHit)
				{
					if (player.CheckHit(boss.GetHitRect()))
					{
						player.Hit();
						IsPlayerNoHit = false;
						lastPlayerHitTime = Timer::Now();
					}
				}
			}
		}

		for (i = 0; i < TOTAL_SNOWBALL; i++)
		{
			if (snowball[i].IsUse())
			{
				snowball[i].CheckMove();
				if (IsPlayerNoHit)
				{
					if (player.CheckHit(snowball[i].GetHitRect()))
					{
						player.Hit();
						IsPlayerNoHit = false;
						snowball[i].SetUse(false);
					}
				}
				snowball[i].Draw(g_lpSecondarySurface);
			}
		}

		if (map.GetStageNum() == 1)
		{
			if (boss.CanMove())
				boss.MoveANDCheckState();

			boss.CheckSprite();
			boss.Draw(g_lpSecondarySurface);
		}

		if (map.GetStageNum() == 0)
		{
			float distanceFromFireBlock = Vector2::Distance(player.GetPos(), Vector2(1152, 2656));
			SetVolume(Sound[8]->Buffers[0], -2000 - (int)distanceFromFireBlock * 4); //모닥불에서 플레이어가 멀어질수록 소리가 작아짐
		}

		map.DrawingRoof(g_lpSecondarySurface, g_lpDirectDrawObject);

		if (map.GetStageNum() == 1)
		{
			bossHpBarBack.Drawing(g_lpSecondarySurface);
			bossHpBar.DrawingBossHp(g_lpSecondarySurface);
		}

		playerHp1.DrawingPlayerHp(g_lpSecondarySurface);
		playerHp2.DrawingPlayerHp(g_lpSecondarySurface);
		playerHp3.DrawingPlayerHp(g_lpSecondarySurface);
		playerHp4.DrawingPlayerHp(g_lpSecondarySurface);

		skillRoll.Drawing(player.GetRollCoolTimePercent(), g_lpSecondarySurface);
		skillCyclone.Drawing(player.GetSkillCoolTimePercent(), g_lpSecondarySurface);

		if (!IsSkipTutorial)
		{
			tutorial.Drawing(g_lpSecondarySurface);
		}

		if ((curPlayerAction == EAction::Dead) || (curBossAction == EAction::Dead) || camera.GetIsFirstAlpha())
		{
			camera.AlphaBlending2(g_lpSecondarySurface);
			if (!camera.GetIsFirstAlpha())
			{
				if (camera.GetAlpha() <= 10)
				{
					SndObjStop(Sound[0]);
					SndObjStop(Sound[6]);
					SndObjStop(Sound[7]);
				}
				SetVolume(Sound[0]->Buffers[0], (255 - camera.GetAlpha()) * -20);
				SetVolume(Sound[6]->Buffers[0], (255 - camera.GetAlpha()) * -20);
				SetVolume(Sound[7]->Buffers[0], (255 - camera.GetAlpha()) * -20);
			}
		}

		HRESULT hResult;
		if (FAILED(hResult = g_lpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) //더블버퍼링 화면전환
		{
			if (hResult == DDERR_SURFACELOST)
				g_lpPrimarySurface->Restore();
		}
	}
}

// todo : 프레임 제한 로직 변경하기
bool FrameRate()
{
	constexpr int frameDelta = 8;
	const system_clock::time_point now = Timer::Now();
	const milliseconds timeDelta = duration_cast<milliseconds>(now - lastFrameTime);
	if (timeDelta.count() > frameDelta)
	{
		lastFrameTime = now;
		return true;
	}

	return false;
}

void InitGame()
{
	SndObjStop(Sound[0]);
	SndObjPlay(Sound[4], DSBPLAY_LOOPING);
	SndObjPlay(Sound[8], DSBPLAY_LOOPING);
	SetVolume(Sound[4]->Buffers[0], -1000);
	SetVolume(Sound[7]->Buffers[0], -1000);
	SetVolume(Sound[8]->Buffers[0], -2000);
	ischeck = false;
	int i, j, m, m2;
	if (player.IsLive())
		player.Kill();
	player.Initialize(1122, 2664, 0, 140, 4, 90);
	player.SetWalkSprite(&player_walk);
	player.SetSpriteAndHitRect();
	player.SetRollSprite(&player_roll);
	player.SetDeadSprite(&player_dead);
	player.SetBowSprite(&bow_walk, &bow_roll, &bow_attack);
	player.SetSkillSprite(&player_skill);
	for (i = 0; i < TOTAL_ARROW; i++)
	{
		if (arrow[i].IsLive())
			arrow[i].Kill();
		arrow[i].Initialize(player.GetPos().x, player.GetPos().y, 2, &arrowSprite);
		arrow[i].SetState(EObjectType::Arrow);
	}
	player.SetArrow(&arrow[0]);

	if (boss.IsLive()) boss.Kill();
	boss.Initialize(1560, 1590, 0, 20, 4);
	boss.SetSprite(&boss_sleep, &boss_idle, &boss_roll, &boss_attack, &boss_dead);
	boss.SetState(EObjectType::Boss);

	fireBlock.Initialize(36 * DEFAULT_BLOCK_SIZE + 16, 83 * DEFAULT_BLOCK_SIZE, 130, &fireSprite);
	fireBlock.SetState(EObjectType::Block);


	for (i = 0; i < TOTAL_SNOWBALL; i++)
	{
		if (snowball[i].IsUse()) snowball[i].SetUse(false);
		snowball[i].Initialize(boss.GetPos().x, boss.GetPos().y, 4, &boss_snowball);
		snowball[i].SetState(EObjectType::Snowball);
	}

	bossHpBarBack.Initialize(1850, SCREEN_HEIGHT >> 1, &boss_hp_window, 0);
	bossHpBar.Initialize(1850, SCREEN_HEIGHT >> 1, &boss_hp, 300);

	playerHp1.Initialize(50, 43, &player_hp, 0);
	playerHp2.Initialize(110, 43, &player_hp, 0);
	playerHp3.Initialize(170, 43, &player_hp, 0);
	playerHp4.Initialize(230, 43, &player_hp, 0);

	tutorial.Initialize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, &tutorialSprite, 0);

	skillRoll.Initialize(50, 1040, &skill_roll, 0);
	skillRoll.SetBackGroundSprite(&skill_background);
	skillCyclone.Initialize(130, 1040, &skill_cyclone, 0);
	skillCyclone.SetBackGroundSprite(&skill_background);

	player_lastRollTime = 0;
	boss_HitWall = false;
	cntVibes = 0;
	IsPlayerNoHit = true;
	IsSkipTutorial = false;

	camera.Initialize();
	m_bIntroFirst = TRUE;
	m_bGameFirst = FALSE;
	m_bEditorFirst = TRUE;

	map.Init();
	map.SetWorldMap(&baseMap, &bossMap, &bossMapRoof);

#pragma region wallInit
	int cnt = 0;
	int tmp = 0;
	int tmp2 = 0;
	tmp = 30;
	tmp2 = 45;
	for (i = 0; i < 26; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp2++;
	}
	tmp = 31;
	tmp2 = 71;
	for (i = 0; i < 5; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp++;
		tmp2++;
	}

	tmp = 35;
	tmp2 = 76;
	for (i = 0; i < 5; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp2++;
	}
	tmp = 36;
	tmp2 = 81;
	for (i = 0; i < 4; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp++;
		tmp2++;
	}
	tmp = 39;
	tmp2 = 84;
	for (i = 0; i < 22; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp++;
	}
	tmp = 60;
	tmp2 = 83;
	for (i = 0; i < 4; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp++;
		tmp2--;
	}
	tmp = 63;
	tmp2 = 79;
	for (i = 0; i < 5; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp2--;
	}
	tmp = 64;
	tmp2 = 74;
	for (i = 0; i < 5; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp++;
		tmp2--;
	}
	tmp = 68;
	tmp2 = 69;
	for (i = 0; i < 26; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp2--;
	}
	tmp = 67;
	tmp2 = 44;
	for (i = 0; i < 2; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp--;
		tmp2--;
	}
	wall[cnt].SetXY(65, 42);
	cnt++;
	wall[cnt].SetXY(64, 42);
	cnt++;
	tmp = 63;
	tmp2 = 41;
	for (i = 0; i < 7; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp--;
	}
	wall[cnt].SetXY(56, 41);
	cnt++;
	tmp = 55;
	tmp2 = 40;
	for (i = 0; i < 2; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp--;
	}
	tmp = 53;
	tmp2 = 40;
	for (i = 0; i < 3; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp--;
		tmp2--;
	}
	tmp = 50;
	tmp2 = 37;
	for (i = 0; i < 5; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp--;
	}
	tmp = 45;
	tmp2 = 38;
	for (i = 0; i < 5; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp--;
		tmp2++;
	}
	tmp = 40;
	tmp2 = 42;
	for (i = 0; i < 3; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp--;
	}
	wall[cnt].SetXY(37, 42);
	cnt++;
	tmp = 36;
	tmp2 = 41;
	for (i = 0; i < 2; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp--;
	}
	tmp = 34;
	tmp2 = 42;
	for (i = 0; i < 2; i++)
	{
		wall[cnt].SetXY(tmp, tmp2);
		cnt++;
		tmp--;
		tmp2++;
	}
	wall[cnt].SetXY(32, 43);
	cnt++;
	wall[cnt].SetXY(31, 44);
#pragma endregion
}
