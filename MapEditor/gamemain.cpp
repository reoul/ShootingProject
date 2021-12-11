#include <Windows.h>
#include <stdio.h>
#include <ddraw.h>
#include <dinput.h>
#include <dsound.h>
#include <math.h>
#include <vector>
#include <iostream>

#include "bmp.h"
#include "timer.h"
#include "sprite.h"
#include "define.h"
#include "block.h"
#include "myplayer.h"
#include "camera.h"
#include "worldmapBmp.h"
#include "sprite8.h"
#include "arrow.h"
#include "boss.h"
#include "DSUTIL.h"
#include "snowball.h"
#include "gui.h"
#include "map.h"
#include "gameEnum.h"
#include "Vector2.h"
#include "fireblock.h"
#include "enemy.h"

using namespace std;

extern BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);
extern LPDIRECTDRAWSURFACE7		g_lpPrimarySurface;
extern LPDIRECTDRAWSURFACE7		g_lpSecondarySurface;
extern LPDIRECTDRAW7			g_lpDirectDrawObject;
extern LPDIRECTINPUT			g_lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8		g_lpDirectInputKeyboard;
extern CTimer					g_Timer;

extern CSprite					grassSprite;
extern CSprite					player_skill;
extern CSprite					player_hp;
extern CSprite					boss_sleep;
extern CSprite					boss_snowball;
extern CSprite					boss_hp_window;
extern CSprite					boss_hp;
extern CSprite					enemy_idle_left;
extern CSprite					enemy_idle_right;
extern CSprite					enemy_up_left;
extern CSprite					enemy_up_right;
extern CSprite					enemy_hide_left;
extern CSprite					enemy_hide_right;
extern CSprite					enemy_attack_left;
extern CSprite					enemy_attack_right;
extern CSprite8					player_walk;
extern CSprite8					player_roll;
extern CSprite8					player_dead;
extern CSprite8					bow_walk;
extern CSprite8					bow_roll;
extern CSprite8					bow_attack;
extern CSprite8					arrowSprite;
extern CSprite8					boss_idle;
extern CSprite8					boss_roll;
extern CSprite8					boss_attack;
extern CSprite8					boss_dead;
extern CBLOCK					Block[BLOCK_Y][BLOCK_X];
extern CPlayer					player;
extern CBoss					boss;
extern CCamera					camera;
extern CWorldMap				baseMap;
extern CWorldMap				bossMap;
extern CWorldMap				bossMapRoof;
extern Map						map;
extern CSprite					fireSprite;
extern FireBlock				fireBlock;

extern DIRECTION		curPlayerDirection;
extern DIRECTION		curBossDirection;

extern ACTION		curPlayerAction;
extern ACTION		curBossAction;

extern CArrow					arrow[TOTAL_ARROW];
extern CSnowBall				snowball[TOTAL_SNOWBALL];
extern Enemy					enemy[TOTAL_ENEMY];

extern Vector2					attackDirection;

extern HDC hdc;

extern Gui						bossHpBarBack;
extern Gui						bossHpBar;
extern Gui						playerHp1;
extern Gui						playerHp2;
extern Gui						playerHp3;
extern Gui						playerHp4;

extern HSNDOBJ Sound[10];
extern void _Play(int num);
CBLOCK wall[139];
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
int time3 = 0;
int player_lastRollTime;
int cntVibes;
bool boss_HitWall;
bool IsPlayerNoHit;

int lastFrameTime = 0;
int lastSkillShootTime = 0;
int lastPlayerHitTime = 0;

void GameMain(void)
{

	if (m_bGameFirst)
		InitGame();
	if (!player.IsArrowNull())
	{
		//sprintf_s(buffer, TEXT("player ( %4.2f , %4.2f )"), player.GetPos().x, player.GetPos().y);
		/*sprintf_s(buffer, TEXT("deltaTime ( %f )"), g_Timer.deltaTime);
		TextOut(hdc, 900, 100, buffer, 40);
		sprintf_s(buffer3, TEXT("boss hp : %d"), boss.GetHp());
		TextOut(hdc, 900, 200, buffer3, 80);*/
	}

	if (FrameRate())
	{
		int i, j;
		camera.InitExpantion();

		if (player.IsArrowNull())
			if (!arrow[0].IsZeroArrow())
				player.SetArrow(arrow[0].FindNotUseArrow());

		if (!IsPlayerNoHit)
		{
			if (g_Timer.elapsed(lastPlayerHitTime, 400))
			{
				IsPlayerNoHit = true;
			}
		}

		//if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_J))
		//{
		//	if ((int)curPlayerActionState != 3)
		//	{
		//		if (!player.IsArrowNull())
		//		{
		//			if (!(curPlayerActionState == (ACTION_STATE)6))
		//			{
		//				camera.Expansion();
		//				player.GetCurArrow()->IsCharging();
		//			}
		//			//else
		//				//camera.SetExpansion(1);
		//		}
		//	}
		//}
		//if (!arrow[9].IsZeroArrow())
		if (!player.IsArrowNull())
		{
			if (player.GetCurArrow()->GetIsCharging())
			{
				//활을 쐈을 때
				if (!camera.IsExpansion())
				{
					player.GetCurArrow()->SetIsUse(true);
					player.GetCurArrow()->SetCharging(false);
					player.GetCurArrow()->SetX(player.GetPos().x - 1);
					player.GetCurArrow()->SetY(player.GetPos().y + 6);
					/*player.GetCurArrow()->SetSpeedXY(2.5f * camera.GetExpansion()* camera.GetExpansion()* camera.GetExpansion()* camera.GetExpansion()* camera.GetExpansion()
						* camera.GetExpansion()* camera.GetExpansion()* camera.GetExpansion()* camera.GetExpansion()* camera.GetExpansion()* camera.GetExpansion()* camera.GetExpansion(),attackDirection);*/
					player.GetCurArrow()->SetSpeedXY(2.5f * powf(camera.GetExpansion(), 12), attackDirection);
					player.GetCurArrow()->CheckSprite();
					player.SetArrow(NULL);
					SndObjPlay(Sound[3], NULL);
					if (player.IsArrowNull())
						player.SetArrow(arrow[9].FindNotUseArrow());
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

		if (player.CanMove())		//이동
		{
			player.MoveInit();
			if (((int)curPlayerAction != ACTION::DEAD) && !camera.GetIsFirstAlpha())
			{
				if (!player.GetIsRoll())
				{
					if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_SPACE))
					{
						if (!player.IsUsingSkill())
							if (g_Timer.elapsed(player_lastRollTime, 1000))
							{
								player_lastRollTime = g_Timer.time();
								player.Roll();
							}
					}
					if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_Q))
					{
						player.Skill();
					}
				}
				if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_A))
				{
					player.Left();
				}
				if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_D))
				{
					player.Right();
				}
				if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_W))
				{
					player.Up();
				}
				if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_S))
				{
					player.Down();
				}
			}
			player.MoveANDCheckState();
			Vector2 curPlayerPos = player.GetPos();
			if (map.GetStageNum() == 0 && curPlayerPos.x > 1540 && curPlayerPos.x < 1630 && curPlayerPos.y > 680 && curPlayerPos.y < 712)
			{
				map.NextStage();
				player.SetXY(1600, 2500);
			}
		}

		if (!player.IsArrowNull())
		{
			if (player.GetCurArrow()->GetIsCharging())
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
				if (!(curPlayerAction == ACTION::ROLL) && (curPlayerAction != ACTION::DEAD))
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
			if (g_Timer.elapsed(lastSkillShootTime, 100))
			{
				player.GetCurArrow()->SetIsUse(true);
				player.GetCurArrow()->SetCharging(false);
				player.GetCurArrow()->SetX(player.GetPos().x - 1);
				player.GetCurArrow()->SetY(player.GetPos().y + 6);
				int randomX = 8 + (rand() % 5);
				int randomY = 8 + (rand() % 5);
				int PosiOrNegaX = rand() % 2;		//랜덤한 값을 뽑아 x방향값을 양수 혹은 음수로 변환
				int PosiOrNegaY = rand() % 2;		//랜덤한 값을 뽑아 y방향값을 양수 혹은 음수로 변환
				randomX = randomX * (PosiOrNegaX == 0 ? 1 : -1);
				randomY = randomY * (PosiOrNegaY == 0 ? 1 : -1);
				player.GetCurArrow()->SetSpeedXY(10, Vector2(0.1f * randomX, 0.1f * randomY));
				player.GetCurArrow()->CheckSprite();
				player.GetCurArrow()->SetHoming(true);
				player.SetArrow(NULL);
				SndObjPlay(Sound[3], NULL);
				if (player.IsArrowNull())
					player.SetArrow(arrow[9].FindNotUseArrow());
			}
		}

		//sprintf_s(buffer3, TEXT("%d"), camera.GetAlpha());
		//TextOut(hdc, 0, 0, buffer3, 40);

		/*if (!player.IsArrowNull())
		{
			if (player.GetCurArrow()->GetIsCharging())
			{
				player.GetCurArrow()->SetX(player.GetPos().x - 1);
				player.GetCurArrow()->SetY(player.GetPos().y + 6);
				player.GetCurArrow()->SetSpeedXY(20 - (30 * (camera.GetExpansion() - 1)), attackDirection);
				player.GetCurArrow()->CheckMove();
				player.GetCurArrow()->CheckSprite();
				if (!(curPlayerActionState == ACTION_STATE::ROLL))
					player.GetCurArrow()->Draw(g_lpSecondarySurface);
			}
		}*/

		for (i = 0; i < TOTAL_ARROW; i++)
		{
			if (arrow[i].GetIsUse())
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
						if (!arrow[i].GetIsHit() && !arrow[i].GetIsCharging())
						{
							if (arrow[i].CheckHit(boss.GetHitRect()) && !arrow[i].IsHoming())
							{
								arrow[i].IsHit();
								boss.Hit(arrow[i].GetPower());

								if (curBossAction == ACTION::SLEEP)
								{
									//boss.CheckDirectionState();
									boss.NextPattern();
									SndObjStop(Sound[4]);
									SndObjPlay(Sound[0], DSBPLAY_LOOPING);
								}
							}
						}
						if (abs((int)(boss.GetPos().x - arrow[i].GetPos().x)) < 50)
						{
							if (abs((int)(boss.GetPos().y - arrow[i].GetPos().y)) < 50)
							{
								arrow[i].IsHit();
								boss.Hit(arrow[i].GetPower());

								if ((int)curBossAction == 5)
									boss.NextPattern();
							}
						}
					}
				}
				arrow[i].CheckMove();
				arrow[i].Draw(g_lpSecondarySurface);
				//if (arrow[i].IsSpeedZero())
				//{
				//	if (player.CheckHit(arrow[i].GetHitRect()))
				//	{
				//		arrow[i].SetIsUse(false);
				//		arrow[i].SetIsHit(false);
				//	}
				//}
			}
		}

		//for (i = 0; i < TOTAL_ARROW; i++)
		//{
		//	if (arrow[i].GetIsUse())
		//	{
		//		if (arrow[i].IsSpeedZero())
		//		{
		//			if (player.CheckHit(arrow[i].GetHitRect()))
		//			{
		//				arrow[i].SetIsUse(false);
		//				arrow[i].SetIsHit(false);
		//			}
		//			/*if (abs((int)(player.GetX() - arrow[i].GetX())) < 16)
		//			{
		//				if (abs((int)(player.GetY() - arrow[i].GetY())) < 22)
		//				{
		//					arrow[i].SetIsUse(false);
		//				}
		//			}*/
		//		}
		//	}
		//}

		player.Draw(g_lpSecondarySurface);

		fireBlock.Draw(g_lpSecondarySurface);

		//보스가 구르기 패턴 공격을 할때
		if (curBossAction == ACTION::ROLL)
		{
			if (curPlayerAction != ACTION::DEAD)
			{
				//플레이어가 공격당하지 않았다면
				if (IsPlayerNoHit)
				{
					if (player.CheckHit(boss.GetHitRect()))
					{
						player.Hit();
						IsPlayerNoHit = false;
						lastPlayerHitTime = g_Timer.time();
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
				/*if (abs((int)(player.GetX() - snowball[i].GetX())) < 24)
				{
					if (abs((int)(player.GetY() - snowball[i].GetY())) < 24)
					{
						curPlayerActionState = (ACTION_STATE)6;
					}
				}*/
				snowball[i].Draw(g_lpSecondarySurface);
			}
		}

		if (map.GetStageNum() == 1)
		{
			if (boss.CanMove())
				boss.MoveANDCheckState();
			/*if (curBossActionState != 3)
			{
				boss.CheckDirectionState();
			}*/
			boss.CheckSprite();
			boss.Draw(g_lpSecondarySurface);
		}

		float distanceFromFireBlock = Vector2::Distance(player.GetPos(), Vector2(1152, 2656));
		SetVolume(Sound[8]->Buffers[0], -2000 - (int)distanceFromFireBlock * 4);//모닥불에서 플레이어가 멀어질수록 소리가 작아짐


		if (!player.IsArrowNull())
		{
			//sprintf_s(buffer, TEXT("%d, %d"), player.GetX(), player.GetY());
			//sprintf_s(buffer4, TEXT("%p"), player.GetCurArrow());
			//TextOut(hdc, 900, 250, buffer, 100);
			//wsprintf(buffer, TEXT("curArrow x = %d"), (int)player.GetCurArrow()->GetX());
			/*sprintf_s(buffer, TEXT("curArrow x = %d"), (int)player.GetCurArrow()->IsLive());
			TextOut(hdc, 900, 100, buffer, 40);
			sprintf_s(buffer2, TEXT("curArrow y = %d"), (int)player.GetCurArrow()->GetY());
			TextOut(hdc, 900, 150, buffer2, 40);*/
		}

		//wsprintf(buffer3, TEXT("%3.3f"), (int)boss.GetPlayerDirectionX());
		/*sprintf_s(buffer, TEXT("camera ( %4.2f , %4.2f )"), camera.GetX(), camera.GetY());
		TextOut(hdc, 900, 100, buffer, 40);*/
		//sprintf_s(buffer2, TEXT("boss ( %4.2f , %4.2f )"), boss.GetX(), boss.GetY());
		//TextOut(hdc, 900, 150, buffer2, 40);
		//int k = 0;
		//sprintf_s(buffer3, TEXT("arrow x : %d, y : %d"), attackDirection.normalize().x,attackDirection.normalize().y);
		//TextOut(hdc, 900, 200 + k * 30, buffer3, 80);
		//k++;
		/*sprintf_s(buffer3, TEXT("arrow[%d] x : %f, y : %f"), k, arrow[k].GetPos().x, arrow[k].GetPos().y);
		TextOut(hdc, 900, 200 + k * 30, buffer3, 80);
		k++;
		sprintf_s(buffer3, TEXT("arrow[%d] x : %f, y : %f"), k, arrow[k].GetPos().x, arrow[k].GetPos().y);
		TextOut(hdc, 900, 200 + k * 30, buffer3, 80);
		k++;
		sprintf_s(buffer3, TEXT("arrow[%d] x : %f, y : %f"), k, arrow[k].GetPos().x, arrow[k].GetPos().y);
		TextOut(hdc, 900, 200 + k * 30, buffer3, 80);
		k++;
		sprintf_s(buffer3, TEXT("arrow[%d] x : %f, y : %f"), k, arrow[k].GetPos().x, arrow[k].GetPos().y);
		TextOut(hdc, 900, 200 + k * 30, buffer3, 80);
		k++;
		sprintf_s(buffer3, TEXT("arrow[%d] x : %f, y : %f"), k, arrow[k].GetPos().x, arrow[k].GetPos().y);
		TextOut(hdc, 900, 200 + k * 30, buffer3, 80);
		k++;
		sprintf_s(buffer3, TEXT("arrow[%d] x : %f, y : %f"), k, arrow[k].GetPos().x, arrow[k].GetPos().y);
		TextOut(hdc, 900, 200 + k * 30, buffer3, 80);
		k++;
		sprintf_s(buffer3, TEXT("arrow[%d] x : %f, y : %f"), k, arrow[k].GetPos().x, arrow[k].GetPos().y);
		TextOut(hdc, 900, 200 + k * 30, buffer3, 80);
		k++;
		sprintf_s(buffer3, TEXT("arrow[%d] x : %f, y : %f"), k, arrow[k].GetPos().x, arrow[k].GetPos().y);
		TextOut(hdc, 900, 200 + k * 30, buffer3, 80);
		k++;
		sprintf_s(buffer3, TEXT("arrow[%d] x : %f, y : %f"), k, arrow[k].GetPos().x, arrow[k].GetPos().y);
		TextOut(hdc, 900, 200 + k * 30, buffer3, 80);
		k++;*/

		//sprintf_s(buffer4, TEXT("방향 x : %f, y : %f"), snowball[0].GetDirection().x, snowball[0].GetDirection().y);
		//TextOut(hdc, 900, 250, buffer4, 40);

		//worldmap2.Drawing2(camera.GetX(), camera.GetY(), g_lpSecondarySurface, true, g_lpDirectDrawObject);

		map.DrawingRoof(g_lpSecondarySurface, g_lpDirectDrawObject);

		//boss_hp_window.Drawing(0,1250,SCREEN_HEIGHT>>1,g_lpSecondarySurface,true);
		//boss_hp.Drawing(bbb,1250,SCREEN_HEIGHT>>1,g_lpSecondarySurface,true);

		if (map.GetStageNum() == 1)
		{
			bossHpBarBack.Drawing(g_lpSecondarySurface);
			bossHpBar.DrawingBossHp(g_lpSecondarySurface);
		}

		for (i = 0; i < TOTAL_ENEMY; i++)
		{
			enemy[i].Draw(g_lpSecondarySurface);
		}

		playerHp1.DrawingPlayerHp(g_lpSecondarySurface);
		playerHp2.DrawingPlayerHp(g_lpSecondarySurface);
		playerHp3.DrawingPlayerHp(g_lpSecondarySurface);
		playerHp4.DrawingPlayerHp(g_lpSecondarySurface);

		/*if (g_Timer.elapsed(ccc, 300))
		{
			bbb = ++bbb % boss_hp.GetNumberOfFrame();
		}*/
		if ((curPlayerAction == ACTION::DEAD) || (curBossAction == ACTION::DEAD) || camera.GetIsFirstAlpha())
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
		if (FAILED(hResult = g_lpPrimarySurface->Flip(NULL, DDFLIP_WAIT)))	//더블버퍼링 화면전환
		{
			if (hResult == DDERR_SURFACELOST)
				g_lpPrimarySurface->Restore();
		}
	}
}

bool FrameRate()
{
	int frameDelta = 8;
	int timeDelta = timeGetTime() - lastFrameTime;
	if (timeDelta > frameDelta)
	{
		g_Timer.deltaTime = timeDelta * 0.001f;
		lastFrameTime = timeGetTime();
		return true;
	}
	else
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
	//player.Initialize(1560, 1984, &g_Timer, 0, 140, 4, 90);		//학교 컴기준     집컴기준  140,20
	player.Initialize(1122, 2664, &g_Timer, 0, 140, 4, 90);		//학교 컴기준     집컴기준  140,20
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
		arrow[i].Initialize(player.GetPos().x, player.GetPos().y, &g_Timer, 2, &arrowSprite);
		arrow[i].SetState(OBJECT_TYPE::ARROW);
	}
	player.SetArrow(&arrow[0]);

	if (boss.IsLive()) boss.Kill();
	boss.Initialize(1560, 1590, &g_Timer, 0, 20, 4);
	boss.SetSprite(&boss_sleep, &boss_idle, &boss_roll, &boss_attack, &boss_dead);
	boss.SetState(OBJECT_TYPE::BOSS);

	fireBlock.Initialize(36 * DEFAULT_BLOCK_SIZE + 16, 83 * DEFAULT_BLOCK_SIZE, &g_Timer, 130, &fireSprite);
	fireBlock.SetState(OBJECT_TYPE::BLOCK);


	for (i = 0; i < TOTAL_SNOWBALL; i++)
	{
		if (snowball[i].IsUse()) snowball[i].SetUse(false);
		snowball[i].Initialize(boss.GetPos().x, boss.GetPos().y, &g_Timer, 4, &boss_snowball);
		snowball[i].SetState(OBJECT_TYPE::SNOWBALL);
	}

	bossHpBarBack.Initialize(1250, SCREEN_HEIGHT >> 1, &boss_hp_window, &g_Timer, 0);
	bossHpBar.Initialize(1250, SCREEN_HEIGHT >> 1, &boss_hp, &g_Timer, 300);

	playerHp1.Initialize(50, 43, &player_hp, &g_Timer, 0);
	playerHp2.Initialize(110, 43, &player_hp, &g_Timer, 0);
	playerHp3.Initialize(170, 43, &player_hp, &g_Timer, 0);
	playerHp4.Initialize(230, 43, &player_hp, &g_Timer, 0);

	time3 = g_Timer.time();

	player_lastRollTime = 0;
	boss_HitWall = false;
	cntVibes = 0;
	IsPlayerNoHit = true;

	camera.Initialize(&g_Timer);
	m_bIntroFirst = TRUE;
	m_bGameFirst = FALSE;
	m_bEditorFirst = TRUE;


	/*for (i = 0; i < TOTAL_ENEMY; i++)
	{
		enemy[i].Initialize(1300 + i * 50, 2600, &g_Timer, 0, 200, 50);
		enemy[i].SetSprite(&enemy_idle_left, &enemy_idle_right, &enemy_up_left, &enemy_up_right,
			&enemy_hide_left, &enemy_hide_right, &enemy_attack_left, &enemy_attack_right);
		enemy[i].SetState(OBJECT_TYPE::ENEMY);
	}*/

	map.SetWorldMap(&baseMap, &bossMap, &bossMapRoof);
	map.SetStage(0);

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


	//SndObjPlay(Sound[0], DSBPLAY_LOOPING);
}