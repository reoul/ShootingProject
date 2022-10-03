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

/**
 * 게임 플레이 로직
 */
void GameMain(void)
{
	if (m_bGameFirst)	// 처음 시작했을 때
		InitGame();		// 초기화

	if (FrameRate())
	{
		int i, j;
		camera.InitExpansion();	// 카메라 확대 초기화

		// 사용할수 있는 화살이 있을때
		if (player.IsArrowNull())
				player.SetArrow(FindNotUseArrow());

		if (!IsPlayerNoHit)		// 플레이어 피격시 일정시간 무적
		{
			if (Timer::Elapsed(lastPlayerHitTime, 400))
			{
				IsPlayerNoHit = true;
			}
		}

		if (!player.IsArrowNull())
		{
			if (player.GetCurArrow()->IsCharging())	// 차징중 일때
			{
				if (!camera.IsExpansion())	//활을 쐈으면
				{
					player.GetCurArrow()->SetIsUse(true);
					player.GetCurArrow()->SetCharging(false);
					player.GetCurArrow()->SetX(player.GetPos().x - 1);
					player.GetCurArrow()->SetY(player.GetPos().y + 6);
					player.GetCurArrow()->SetSpeedXY(2.5f * powf(camera.GetExpansion(), 12), attackDirection);
					player.GetCurArrow()->CheckSprite();
					player.SetArrow(nullptr);
					SndObjPlay(Sound[3], NULL);		// 활 효과음 재생
					if (player.IsArrowNull())
						player.SetArrow(FindNotUseArrow());		// 사용되지 않은 화살 장착
				}
			}
		}

		camera.CheckExpansion();	// 카메라 확대 체크

		camera.CheckFinishXY();		// 카메라 위치 체크

		if (boss_HitWall && (cntVibes < 4))		// 보스가 벽에 부딪쳤을 때 화면 흔들림 효과
		{
			camera.SetFinishXY(camera.GetFinishX(), camera.GetFinishY());
			cntVibes++;
		}

		camera.Move();	// 카메라 이동

		map.SetPos(camera.GetX(), camera.GetY());	// 카메라 위치에 따른 맵 위치 이동
		map.Drawing(g_lpSecondarySurface, g_lpDirectDrawObject);	// 맵 바닥 화면에 그리기

		if (player.CanMove()) //이동
		{
			player.MoveInit();
			player.CheckKeyBoard();
			player.MoveANDCheckState();
			Vector2 curPlayerPos = player.GetPos();
			if (map.GetStageNum() == 0 && curPlayerPos.x > 1540 && curPlayerPos.x < 1630 && curPlayerPos.y > 680 &&
				curPlayerPos.y < 712)	// 다음 스테이지 입구에 도착했을 때
			{
				map.NextStage();
				player.SetXY(1600, 2500);
			}
		}

		if (!player.IsArrowNull())
		{
			if (player.GetCurArrow()->IsCharging())	// 활 차징중일때
			{
				if (!isSound)	// 활 처음에 차징했을 때만 소리 나오게
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

		player.CheckUseSkill();		// 스킬 지속 시간 체크

		if (player.IsUsingSkill())	// 스킬 사용 했으면
		{
			if (Timer::Elapsed(lastSkillShootTime, 100))		// 0.1초마다 랜덤한 방향으로 화살 발사
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
				if (arrow[i].IsHoming())	// 유도 화살의 경우
				{
					if (map.GetStageNum() == 1)		// 보스방일때 화살과 보스 충돌 체크
					{
						if (abs(arrow[i].GetPos().x - boss.GetPos().x) < 45)
							if (abs(arrow[i].GetPos().y - boss.GetPos().y) < 45)
							{
								arrow[i].IsHit();
								boss.Hit(10);
							}
					}
				}
				if (!arrow[i].IsSpeedZero())	// 화살이 움직일 수 있다면
				{
					if (map.GetStageNum() == 1)		// 보스방일때
					{
						if (!arrow[i].GetIsHit() && !arrow[i].IsCharging())		// 화살과 보스 충돌 체크
						{
							if (arrow[i].CheckHit(boss.GetHitRect()) && !arrow[i].IsHoming())
							{
								arrow[i].IsHit();
								boss.Hit(arrow[i].GetPower());

								if (curBossAction == EAction::Sleep)	// 보스가 자는 상태일때 깨운다
								{
									boss.NextPattern();
									SndObjStop(Sound[4]);
									SndObjPlay(Sound[0], DSBPLAY_LOOPING);		// 전투 브금 재생
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
				arrow[i].CheckMove();					// 화살 이동
				arrow[i].Draw(g_lpSecondarySurface);	// 화살 화면에 그려줌
			}
		}

		player.Draw(g_lpSecondarySurface);		// 플레이어 그려줌

		fireBlock.Draw(g_lpSecondarySurface);	// 처음 장소에 있는 불블럭 그려줌

		//보스가 구르기 패턴 공격을 할때
		if (curBossAction == EAction::Roll)
		{
			if (curPlayerAction != EAction::Dead)
			{
				if (IsPlayerNoHit)	// 플레이어가 무적상태가 아닐때
				{
					if (player.CheckHit(boss.GetHitRect()))		// 플레이어가 보스가 충돌했다면
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
				snowball[i].CheckMove();	// 눈덩이 이동
				if (IsPlayerNoHit)
				{
					if (player.CheckHit(snowball[i].GetHitRect()))		// 눈덩이랑 플레이어랑 충돌 체크
					{
						player.Hit();
						IsPlayerNoHit = false;
						snowball[i].SetUse(false);
					}
				}
				snowball[i].Draw(g_lpSecondarySurface);		// 눈덩이 그려줌
			}
		}

		if (map.GetStageNum() == 1)				// 보스방일때
		{
			if (boss.CanMove())					// 보스가 이동이 가능하면
				boss.MoveANDCheckState();		// 이동과 상태를 업데이트 시켜준다

			boss.CheckSprite();					// 보스 상태에 맞는 이미지를 바꿔준다
			boss.Draw(g_lpSecondarySurface);	// 보스를 그려줌
		}
		else   // 처음 모작불 장소일때
		{
			float distanceFromFireBlock = Vector2::Distance(player.GetPos(), Vector2(1152, 2656));	// 모닥불과 플레이어 거리 계산
			SetVolume(Sound[8]->Buffers[0], -2000 - static_cast<int>(distanceFromFireBlock) * 4); // 거리가 멀어질수록 소리가 작아짐
		}

		map.DrawingRoof(g_lpSecondarySurface, g_lpDirectDrawObject);	// 맵 지붕 그리기

		if (map.GetStageNum() == 1)		// 보스방일때
		{
			bossHpBarBack.Drawing(g_lpSecondarySurface);		// 보스 체력바 그리기
			bossHpBar.DrawingBossHp(g_lpSecondarySurface);
		}

		playerHp1.DrawingPlayerHp(g_lpSecondarySurface);		// 플레이어 체력 그리기
		playerHp2.DrawingPlayerHp(g_lpSecondarySurface);
		playerHp3.DrawingPlayerHp(g_lpSecondarySurface);
		playerHp4.DrawingPlayerHp(g_lpSecondarySurface);

		skillRoll.Drawing(player.GetRollCoolTimePercent(), g_lpSecondarySurface);		// 구르기 쿨타임 그리기
		skillCyclone.Drawing(player.GetSkillCoolTimePercent(), g_lpSecondarySurface);	// 스킬 쿨타임 그리기

		if (!IsSkipTutorial)	// 튜토리얼 스킵을 안했으면
		{
			tutorial.Drawing(g_lpSecondarySurface);	// 튜토리얼 계속 그리기
		}

		// 플레이어 죽거나 보스가 죽거나 게임 시작했을 때
		if ((curPlayerAction == EAction::Dead) || (curBossAction == EAction::Dead) || camera.GetIsFirstAlpha())
		{
			camera.AlphaBlending(g_lpSecondarySurface);		// 전체화면 알파 블렌딩
			if (!camera.GetIsFirstAlpha())
			{
				if (camera.GetAlpha() <= 10)	// 인트로 화면에서 알파가 10이하가 된면
				{
					SndObjStop(Sound[0]);		// 배경음악 끝기
					SndObjStop(Sound[6]);
					SndObjStop(Sound[7]);
				}
				SetVolume(Sound[0]->Buffers[0], (255 - camera.GetAlpha()) * -20);		// 배경음악 점점 줄이기
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
