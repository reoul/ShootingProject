#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dsound.lib")

#include <Windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dinput.h>
#include <vector>
#include <assert.h>

#include "Bmp.h"
#include "Timer.h"
#include "Sprite.h"
#include "SettingData.h"
#include "Block.h"
#include "MapEditor.h"
#include "Player.h"
#include "Camera.h"
#include "WorldMapBmp.h"
#include "IntroSprite.h"
#include "Sprite8.h"
#include "Arrow.h"
#include "Boss.h"
#include "Vector2.h"
#include "SnowBall.h"
#include "ChoiceWindow.h"
#include "Gui.h"
#include "GameEnum.h"
#include "Map.h"
#include <string>
#include "Fireblock.h"
#include "SkillUI.h"

using namespace std;

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);
bool InitializeDirectX(void);
void IntroMain(void);
void GameMain(void);
void EditorMain(void);
void InitGame();
bool LoadBMPandInitSurface();
bool LoadWorldMapBlock();


HWND g_hwnd;
HINSTANCE g_hInstance;

HDC hdc;

LPDIRECTDRAW7 g_lpDirectDrawObject = nullptr;
LPDIRECTDRAWSURFACE7 g_lpPrimarySurface = nullptr;
LPDIRECTDRAWSURFACE7 g_lpSecondarySurface = nullptr;

LPDIRECTINPUT8 g_lpDirectInputObject = nullptr;
LPDIRECTINPUTDEVICE8 g_lpDirectInputKeyboard = nullptr;

Block g_blocks[BLOCK_Y][BLOCK_X];
Sprite blockSprite[4][TOTAL_BLOCK_SPRITE];
Sprite blockSprite2[TOTAL_BLOCK_SPRITE_Y][TOTAL_BLOCK_SPRITE_X];
Sprite fireSprite;
Player player;
Boss boss;
FireBlock fireBlock;

Camera camera;

Block curBlock;
Sprite grassSprite;

Sprite player_skill;

Sprite enemy_idle_left;
Sprite enemy_idle_right;
Sprite enemy_up_left;
Sprite enemy_up_right;
Sprite enemy_hide_left;
Sprite enemy_hide_right;
Sprite enemy_attack_left;
Sprite enemy_attack_right;

Sprite8 player_walk;
Sprite8 player_roll;
Sprite8 player_dead;
Sprite8 bow_walk;
Sprite8 bow_roll;
Sprite8 bow_attack;
Sprite8 arrowSprite;
Sprite8 boss_idle;
Sprite8 boss_roll;
Sprite8 boss_attack;
Sprite8 boss_dead;

Sprite boss_sleep;
Sprite boss_snowball;

Sprite boss_hp_window;
Sprite boss_hp;

Sprite player_hp;

Sprite skill_background;
Sprite skill_roll;
Sprite skill_cyclone;

Sprite tutorialSprite;

Arrow arrow[TOTAL_ARROW];
SnowBall snowball[TOTAL_SNOWBALL];

IntroSprite introSprite;
IntroSprite introButton;
WorldMap bossMap;
WorldMap bossMapRoof;
WorldMap baseMap;
WorldMap* curEditMap;
Map map;

Vector2 attackDirection;
Vector2 firstPosition;
Vector2 curMousePosition;

Gui bossHpBarBack;
Gui bossHpBar;
Gui playerHp1;
Gui playerHp2;
Gui playerHp3;
Gui playerHp4;
Gui tutorial;
SkillUI skillRoll;
SkillUI skillCyclone;

EMod curMod; //현재 모드

EDirection curPlayerDirection;
EDirection curBossDirection;

EAction curPlayerAction;
EAction curBossAction;

EEditState curEditState;

EEditWindow curEditWindow; //현재 에디터 창

MapEditor g_editor; //맵에디터 변수

bool g_bActiveApp = false;
bool g_bIsFirst;
bool g_bIsGame = true;
bool g_bIsQuit = false;
bool IsSkipTutorial = false;

int mouseX = 0, mouseY = 0;
int cameraPositionX;
int cameraPositionY;

int arrowDirection;
float x, y;
int StartTime = 0;

extern BOOL _InitDirectSound(void);
BOOL m_bEditorFirst = TRUE;
BOOL m_bGameFirst = TRUE;
BOOL m_bIntroFirst = TRUE;
extern bool isSound;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WinProc;
	wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = CLASS_NAME;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (RegisterClassEx(&wndclass) == 0)
		return 0;
	g_hwnd = CreateWindowEx(WS_EX_TOPMOST, CLASS_NAME, CLASS_NAME, WS_POPUP | WS_VISIBLE,
		GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hInstance, nullptr);
	
	if (g_hwnd == nullptr) return 0;
	g_hInstance = hInstance;
	cameraPositionX = 0;
	cameraPositionY = 0;
	attackDirection.SetXY(0, 0);

	SetFocus(g_hwnd);

	if (!InitializeDirectX())
		return 0;

	if (!_InitDirectSound())
		return 0;

	Timer::Start();

	if (!LoadBMPandInitSurface())
		return 0;

	if (!LoadWorldMapBlock())
		return 0;

	hdc = GetDC(g_hwnd);

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, nullptr, 0, 0))
				return (int)msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (g_bActiveApp)
		{
			Timer::UpdateDeltaTime();
			switch (curMod)
			{
			case EMod::Intro:
				IntroMain();
				break;
			case EMod::Game:
				GameMain();
				break;
			case EMod::Editor:
				EditorMain();
				break;
			case EMod::Quit:
				break;
			default:
				assert(false);
			}
		}
		else WaitMessage();
	}
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (curMod == EMod::Quit && !g_bIsQuit)
	{
		g_bIsQuit = true;
		DestroyWindow(hwnd);
	}
	switch (message)
	{
	case WM_ACTIVATEAPP:
		if (wParam)
		{
			if (g_bActiveApp == false)
			{
				if (StartTime > 0)
				{
					introSprite.CopyBufferToSurface(g_lpDirectDrawObject);
					introButton.RestoreAll(g_lpDirectDrawObject);
					baseMap.CopyBufferToSurface4(g_lpDirectDrawObject);
					bossMap.CopyBufferToSurface2(g_lpDirectDrawObject);
					bossMapRoof.CopyBufferToSurface3(g_lpDirectDrawObject);

					grassSprite.Restore();
					player_walk.ReStoreAll();
					player_roll.ReStoreAll();
					player_dead.ReStoreAll();
					bow_walk.ReStoreAll();
					bow_roll.ReStoreAll();
					bow_attack.ReStoreAll();
					arrowSprite.ReStoreAll();
					boss_idle.ReStoreAll();
					boss_roll.ReStoreAll();
					boss_attack.ReStoreAll();
					boss_sleep.Restore();
					boss_snowball.Restore();
				}
				StartTime++;
			}
			g_bActiveApp = true;
		}
		else
			g_bActiveApp = false;
		break;
	case WM_KEYDOWN:
		if (wParam == 'Y') curMod = EMod::Intro;
		if (wParam == VK_RETURN) IsSkipTutorial = true;
		if (wParam == VK_ESCAPE) DestroyWindow(hwnd);
		break;
	case WM_LBUTTONDOWN:
		switch (curMod)
		{
		case EMod::Intro:
			break;
		case EMod::Game:
			if (!(curPlayerAction == EAction::Roll))
				if (!player.IsArrowNull() && !(curPlayerAction == EAction::Dead) && !camera.GetIsFirstAlpha())
				{
					firstPosition.SetXY(LOWORD(lParam) + camera.GetX() - (SCREEN_WIDTH >> 1),
						HIWORD(lParam) + camera.GetY() - (SCREEN_HEIGHT >> 1));
					curMousePosition.SetXY(LOWORD(lParam) + camera.GetX() - (SCREEN_WIDTH >> 1),
						HIWORD(lParam) + camera.GetY() - (SCREEN_HEIGHT >> 1));
					camera.SetIsExpansion(true);
					player.GetCurArrow()->SetCharging(true);
					x = curMousePosition.x - player.GetPos().x;
					y = curMousePosition.y - player.GetPos().y;
					arrowDirection = (int)sqrtf(pow(x, 2) + pow(y, 2));
					attackDirection.SetXY(x / arrowDirection, y / arrowDirection);
				}
			break;
		case EMod::Editor:
			g_editor.CheckChoiceWindow(LOWORD(lParam), HIWORD(lParam));
			if (!g_editor.IsChoiceWindow())
				g_editor.SetStartXY(LOWORD(lParam) + camera.GetX() - (SCREEN_WIDTH >> 1),
					HIWORD(lParam) + camera.GetY() - (SCREEN_HEIGHT >> 1));
			else
				g_editor.GetChoiceWindow()->CheckBlockClick(LOWORD(lParam), HIWORD(lParam));
			break;
		case EMod::Quit:
			break;
		}
		break;
	case WM_LBUTTONUP:
		switch (curMod)
		{
		case EMod::Intro:
			break;
		case EMod::Game:
			if (!camera.GetIsFirstAlpha())
			{
				isSound = false;
				curMousePosition.SetXY(LOWORD(lParam) + camera.GetX() - (SCREEN_WIDTH >> 1),
					HIWORD(lParam) + camera.GetY() - (SCREEN_HEIGHT >> 1));
				camera.SetIsExpansion(false);
				x = curMousePosition.x - player.GetPos().x;
				y = curMousePosition.y - player.GetPos().y;
				arrowDirection = (int)sqrtf(pow(x, 2) + pow(y, 2));
				attackDirection.SetXY(x / arrowDirection, y / arrowDirection);
			}
			break;
		case EMod::Editor:
			if (!g_editor.IsChoiceWindow())
			{
				g_editor.SetEndXY(LOWORD(lParam) + camera.GetX() - (SCREEN_WIDTH >> 1),
					HIWORD(lParam) + camera.GetY() - (SCREEN_HEIGHT >> 1));
				g_editor.SetBlock(g_blocks, curBlock.GetSprite());
				g_editor.SaveWallData();
			}
			else
			{
				g_editor.GetChoiceWindow()->SetCanMove(false);
			}
			break;
		case EMod::Quit:
			break;
		}
		break;
	case WM_MOUSEMOVE:
		switch (curMod)
		{
		case EMod::Intro:
			break;
		case EMod::Game:
			if (camera.GetIsExpansion() && !camera.GetIsFirstAlpha())
			{
				curMousePosition.SetXY(LOWORD(lParam) + camera.GetX() - (SCREEN_WIDTH >> 1),
					HIWORD(lParam) + camera.GetY() - (SCREEN_HEIGHT >> 1));
				x = curMousePosition.x - player.GetPos().x;
				y = curMousePosition.y - player.GetPos().y;
				arrowDirection = (int)sqrtf(pow(x, 2) + pow(y, 2));
				attackDirection.SetXY(x / arrowDirection, y / arrowDirection);
			}
			break;
		case EMod::Editor:
			if (g_editor.GetChoiceWindow()->CanMove())
				g_editor.GetChoiceWindow()->SetXY(LOWORD(lParam), HIWORD(lParam));
			break;
		case EMod::Quit:
			break;
		}
		break;
	case WM_DESTROY:
		if (g_lpDirectDrawObject)
		{
			if (g_lpSecondarySurface)
				g_lpSecondarySurface->Release();
			if (g_lpPrimarySurface)
				g_lpPrimarySurface->Release();

			grassSprite.ReleaseAll();

			bow_walk.ReleaseAll();
			bow_roll.ReleaseAll();
			bow_attack.ReleaseAll();
			arrowSprite.ReleaseAll();
			boss_idle.ReleaseAll();
			boss_roll.ReleaseAll();
			boss_attack.ReleaseAll();

			boss_sleep.ReleaseAll();
			boss_snowball.ReleaseAll();

			baseMap.ReleaseAll();
			bossMap.ReleaseAll();
			bossMapRoof.ReleaseAll();
			introSprite.ReleaseAll();
			introButton.ReleaseAll();

			player_walk.ReleaseAll();
			player_roll.ReleaseAll();
			player_dead.ReleaseAll();
			boss_dead.ReleaseAll();
			fireSprite.ReleaseAll();

			boss_hp_window.ReleaseAll();
			boss_hp.ReleaseAll();

			player_hp.ReleaseAll();

			g_lpDirectDrawObject->Release();
		}
		if (g_lpDirectInputObject)
		{
			if (g_lpDirectInputKeyboard)
			{
				g_lpDirectInputKeyboard->Unacquire();
				g_lpDirectInputKeyboard->Release();
			}
			g_lpDirectInputObject->Release();
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode)
{
	char KeyBuffer[256];
	HRESULT hResult;

	if (lpKeyboard->GetDeviceState(256, KeyBuffer) == DIERR_INPUTLOST)
	{
		while (hResult = lpKeyboard->Acquire() == DIERR_INPUTLOST)
			hResult = lpKeyboard->Acquire();
		lpKeyboard->GetDeviceState(256, KeyBuffer);
	}

	return (KeyBuffer[dikcode] & 0x80);
}

bool LoadWorldMapBlock()
{
	HANDLE hfile;
	DWORD actualRead;

	hfile = CreateFile(TEXT("data\\blockData.txt"), GENERIC_READ, FILE_SHARE_READ, nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	unsigned char* MapData;

	MapData = new unsigned char[BLOCK_X * BLOCK_Y * 2];

	if (!ReadFile(hfile, MapData, BLOCK_X * BLOCK_Y * 2, &actualRead, nullptr)) //비트맵파일 자체의 정보를 읽을수 없다면
	{
		CloseHandle(hfile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}

	char tmp[2];
	int tmp2;
	for (int i = 0; i < BLOCK_Y; i++)
	{
		for (int j = 0; j < BLOCK_X; j++)
		{
			memcpy(tmp, MapData + (i * BLOCK_X * 2) + j * 2, 2);
			tmp2 = atoi(tmp);
			g_blocks[i][j].SetBlockNumber(tmp2);
		}
	}
	memcpy(g_editor.GetBlockData(), MapData, BLOCK_X * BLOCK_Y * 2);

	delete[] MapData;

	CloseHandle(hfile);

	hfile = CreateFile(TEXT("data\\blockData2.txt"), GENERIC_READ, FILE_SHARE_READ, nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	MapData = new unsigned char[BLOCK_X * BLOCK_Y * 2];

	if (!ReadFile(hfile, MapData, BLOCK_X * BLOCK_Y * 2, &actualRead, nullptr)) //비트맵파일 자체의 정보를 읽을수 없다면
	{
		CloseHandle(hfile); //핸들을 반납해주고
		return false; //false를 반환해준다
	}
	for (int i = 0; i < BLOCK_Y; i++)
	{
		for (int j = 0; j < BLOCK_X; j++)
		{
			memcpy(tmp, MapData + (i * BLOCK_X * 2) + j * 2, 2);
			tmp2 = atoi(tmp);
			g_blocks[i][j].SetBlockNumber(tmp2);
		}
	}
	memcpy(g_editor.GetBlockData2(), MapData, BLOCK_X * BLOCK_Y * 2);

	delete[] MapData;

	CloseHandle(hfile);

	return true;
}

bool LoadBMPandInitSurface()
{
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < TOTAL_BLOCK_SPRITE; i++)
			blockSprite[j][i].SetNumber(-1);
	}

	g_bIsFirst = true;
#pragma region intro
	if (!introSprite.LoadBMPFile("image\\intro\\background.bmp"))
		return false;
	if (!introSprite.CopyBufferToSurface(g_lpDirectDrawObject))
		return false;

	if (!introButton.InitSprite(3, 156, 18, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!introButton.LoadFrame(0, "image\\intro\\active_start.bmp"))
		return false;
	if (!introButton.LoadFrame(1, "image\\intro\\active_option.bmp"))
		return false;
	if (!introButton.LoadFrame(2, "image\\intro\\active_quit.bmp"))
		return false;
#pragma endregion

#pragma region map_editor_block
	if (!blockSprite[0][0].InitSprite(1, 200, 200, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][0].LoadFrame(0, "image\\mapblock\\windowsprite.bmp"))
		return false;
	blockSprite[0][0].SetNumber(0);
	for (int i = 0; i < 4; i++)
	{
		if (!blockSprite[i][1].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
			return false;
		if (!blockSprite[i][1].LoadFrame(0, "1.bmp"))
			return false;
		if (!blockSprite[i][10].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
			return false;
		if (!blockSprite[i][10].LoadFrame(0, "image\\mapblock\\backgroundblock.bmp"))
			return false;
		blockSprite[i][10].SetNumber(10);
		if (!blockSprite[i][30].InitSprite(1, 16, 16, COLOR_KEY, g_lpDirectDrawObject))
			return false;
		if (!blockSprite[i][30].LoadFrame(0, "image\\mapblock\\boss_button.bmp"))
			return false;
		blockSprite[i][30].SetNumber(30);
		if (!blockSprite[i][40].InitSprite(1, 16, 16, COLOR_KEY, g_lpDirectDrawObject))
			return false;
		if (!blockSprite[i][40].LoadFrame(0, "image\\mapblock\\forest_button.bmp"))
			return false;
		blockSprite[i][40].SetNumber(40);
		if (!blockSprite[i][50].InitSprite(1, 16, 16, COLOR_KEY, g_lpDirectDrawObject))
			return false;
		if (!blockSprite[i][50].LoadFrame(0, "image\\mapblock\\edit_button.bmp"))
			return false;
		blockSprite[i][50].SetNumber(50);
	}

	if (!blockSprite[2][11].InitSprite(1, 16, 16, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[2][11].LoadFrame(0, "image\\mapblock\\edit_button.bmp"))
		return false;
	blockSprite[2][11].SetNumber(11);
	if (!blockSprite[3][11].InitSprite(1, 16, 16, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[3][11].LoadFrame(0, "image\\mapblock\\edit_button.bmp"))
		return false;
	blockSprite[3][11].SetNumber(11);
	if (!blockSprite[0][11].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][11].LoadFrame(0, "image\\mapblock\\ice\\00.bmp"))
		return false;
	blockSprite[0][11].SetNumber(11);
	if (!blockSprite[0][12].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][12].LoadFrame(0, "image\\mapblock\\ice\\01.bmp"))
		return false;
	blockSprite[0][12].SetNumber(12);
	if (!blockSprite[0][13].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][13].LoadFrame(0, "image\\mapblock\\ice\\02.bmp"))
		return false;
	blockSprite[0][13].SetNumber(13);
	if (!blockSprite[0][14].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][14].LoadFrame(0, "image\\mapblock\\ice\\03.bmp"))
		return false;
	blockSprite[0][14].SetNumber(14);
	if (!blockSprite[0][21].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][21].LoadFrame(0, "image\\mapblock\\ice\\04.bmp"))
		return false;
	blockSprite[0][21].SetNumber(21);
	if (!blockSprite[0][22].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][22].LoadFrame(0, "image\\mapblock\\ice\\05.bmp"))
		return false;
	blockSprite[0][22].SetNumber(22);
	if (!blockSprite[0][23].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][23].LoadFrame(0, "image\\mapblock\\ice\\06.bmp"))
		return false;
	blockSprite[0][23].SetNumber(23);
	if (!blockSprite[0][24].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][24].LoadFrame(0, "image\\mapblock\\ice\\07.bmp"))
		return false;
	blockSprite[0][24].SetNumber(24);
	if (!blockSprite[0][31].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][31].LoadFrame(0, "image\\mapblock\\ice\\08.bmp"))
		return false;
	blockSprite[0][31].SetNumber(31);
	if (!blockSprite[0][20].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][20].LoadFrame(0, "image\\mapblock\\block00.bmp"))
		return false;
	blockSprite[0][20].SetNumber(20);
	if (!blockSprite[0][32].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][32].LoadFrame(0, "image\\mapblock\\ice\\09.bmp"))
		return false;
	blockSprite[0][32].SetNumber(32);
	if (!blockSprite[0][33].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][33].LoadFrame(0, "image\\mapblock\\ice\\10.bmp"))
		return false;
	blockSprite[0][33].SetNumber(33);
	if (!blockSprite[0][34].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][34].LoadFrame(0, "image\\mapblock\\ice\\11.bmp"))
		return false;
	blockSprite[0][34].SetNumber(34);
	if (!blockSprite[0][15].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][15].LoadFrame(0, "image\\mapblock\\ice_walltop\\00.bmp"))
		return false;
	blockSprite[0][15].SetNumber(15);
	if (!blockSprite[0][16].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][16].LoadFrame(0, "image\\mapblock\\ice_walltop\\01.bmp"))
		return false;
	blockSprite[0][16].SetNumber(16);
	if (!blockSprite[0][17].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][17].LoadFrame(0, "image\\mapblock\\ice_walltop\\02.bmp"))
		return false;
	blockSprite[0][17].SetNumber(17);
	if (!blockSprite[0][25].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][25].LoadFrame(0, "image\\mapblock\\ice_walltop\\03.bmp"))
		return false;
	blockSprite[0][25].SetNumber(25);
	if (!blockSprite[0][26].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][26].LoadFrame(0, "image\\mapblock\\ice_walltop\\04.bmp"))
		return false;
	blockSprite[0][26].SetNumber(26);
	if (!blockSprite[0][27].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][27].LoadFrame(0, "image\\mapblock\\ice_walltop\\05.bmp"))
		return false;
	blockSprite[0][27].SetNumber(27);
	if (!blockSprite[0][35].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][35].LoadFrame(0, "image\\mapblock\\ice_walltop\\06.bmp"))
		return false;
	blockSprite[0][35].SetNumber(35);
	if (!blockSprite[0][36].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][36].LoadFrame(0, "image\\mapblock\\ice_walltop\\07.bmp"))
		return false;
	blockSprite[0][36].SetNumber(36);
	if (!blockSprite[0][37].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][37].LoadFrame(0, "image\\mapblock\\ice_walltop\\08.bmp"))
		return false;
	blockSprite[0][37].SetNumber(37);
	if (!blockSprite[0][18].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][18].LoadFrame(0, "image\\mapblock\\ice_walltop\\09.bmp"))
		return false;
	blockSprite[0][18].SetNumber(18);
	if (!blockSprite[0][19].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][19].LoadFrame(0, "image\\mapblock\\ice_walltop\\10.bmp"))
		return false;
	blockSprite[0][19].SetNumber(19);
	if (!blockSprite[0][28].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][28].LoadFrame(0, "image\\mapblock\\ice_walltop\\11.bmp"))
		return false;
	blockSprite[0][28].SetNumber(28);
	if (!blockSprite[0][29].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][29].LoadFrame(0, "image\\mapblock\\ice_walltop\\12.bmp"))
		return false;
	blockSprite[0][29].SetNumber(29);
	if (!blockSprite[0][41].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][41].LoadFrame(0, "image\\mapblock\\block2\\00.bmp"))
		return false;
	blockSprite[0][41].SetNumber(41);
	if (!blockSprite[0][42].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][42].LoadFrame(0, "image\\mapblock\\block2\\01.bmp"))
		return false;
	blockSprite[0][42].SetNumber(42);
	if (!blockSprite[0][43].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][43].LoadFrame(0, "image\\mapblock\\block2\\02.bmp"))
		return false;
	blockSprite[0][43].SetNumber(43);
	if (!blockSprite[0][44].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][44].LoadFrame(0, "image\\mapblock\\block2\\03.bmp"))
		return false;
	blockSprite[0][44].SetNumber(44);
	if (!blockSprite[0][51].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][51].LoadFrame(0, "image\\mapblock\\block2\\04.bmp"))
		return false;
	blockSprite[0][51].SetNumber(51);
	if (!blockSprite[0][52].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][52].LoadFrame(0, "image\\mapblock\\block2\\05.bmp"))
		return false;
	blockSprite[0][52].SetNumber(52);
	if (!blockSprite[0][53].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][53].LoadFrame(0, "image\\mapblock\\block2\\06.bmp"))
		return false;
	blockSprite[0][53].SetNumber(53);
	if (!blockSprite[0][54].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][54].LoadFrame(0, "image\\mapblock\\block2\\07.bmp"))
		return false;
	blockSprite[0][54].SetNumber(54);
	if (!blockSprite[0][61].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][61].LoadFrame(0, "image\\mapblock\\block2\\08.bmp"))
		return false;
	blockSprite[0][61].SetNumber(61);
	if (!blockSprite[0][62].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][62].LoadFrame(0, "image\\mapblock\\block2\\09.bmp"))
		return false;
	blockSprite[0][62].SetNumber(62);
	if (!blockSprite[0][63].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][63].LoadFrame(0, "image\\mapblock\\block2\\10.bmp"))
		return false;
	blockSprite[0][63].SetNumber(63);
	if (!blockSprite[0][64].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][64].LoadFrame(0, "image\\mapblock\\block2\\11.bmp"))
		return false;
	blockSprite[0][64].SetNumber(64);
	if (!blockSprite[0][71].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][71].LoadFrame(0, "image\\mapblock\\block2\\12.bmp"))
		return false;
	blockSprite[0][71].SetNumber(71);
	if (!blockSprite[0][72].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][72].LoadFrame(0, "image\\mapblock\\block2\\13.bmp"))
		return false;
	blockSprite[0][72].SetNumber(72);
	if (!blockSprite[0][73].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][73].LoadFrame(0, "image\\mapblock\\block2\\14.bmp"))
		return false;
	blockSprite[0][73].SetNumber(73);
	if (!blockSprite[0][74].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][74].LoadFrame(0, "image\\mapblock\\block2\\15.bmp"))
		return false;
	blockSprite[0][74].SetNumber(74);
	if (!blockSprite[0][38].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][38].LoadFrame(0, "image\\mapblock\\block3\\00.bmp"))
		return false;
	blockSprite[0][38].SetNumber(38);
	if (!blockSprite[0][39].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][39].LoadFrame(0, "image\\mapblock\\block3\\01.bmp"))
		return false;
	blockSprite[0][39].SetNumber(39);
	if (!blockSprite[0][58].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][58].LoadFrame(0, "image\\mapblock\\block3\\02.bmp"))
		return false;
	blockSprite[0][58].SetNumber(58);
	if (!blockSprite[0][59].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][59].LoadFrame(0, "image\\mapblock\\block3\\03.bmp"))
		return false;
	blockSprite[0][59].SetNumber(59);
	if (!blockSprite[0][48].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][48].LoadFrame(0, "image\\mapblock\\block3\\04.bmp"))
		return false;
	blockSprite[0][48].SetNumber(48);
	if (!blockSprite[0][49].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][49].LoadFrame(0, "image\\mapblock\\block3\\05.bmp"))
		return false;
	blockSprite[0][49].SetNumber(49);
	if (!blockSprite[0][68].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][68].LoadFrame(0, "image\\mapblock\\block3\\06.bmp"))
		return false;
	blockSprite[0][68].SetNumber(68);
	if (!blockSprite[0][69].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][69].LoadFrame(0, "image\\mapblock\\block3\\07.bmp"))
		return false;
	blockSprite[0][69].SetNumber(69);
	if (!blockSprite[0][76].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][76].LoadFrame(0, "image\\mapblock\\block3\\08.bmp"))
		return false;
	blockSprite[0][76].SetNumber(76);
	if (!blockSprite[0][77].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][77].LoadFrame(0, "image\\mapblock\\block3\\09.bmp"))
		return false;
	blockSprite[0][77].SetNumber(77);
	if (!blockSprite[0][78].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][78].LoadFrame(0, "image\\mapblock\\block3\\10.bmp"))
		return false;
	blockSprite[0][78].SetNumber(78);
	if (!blockSprite[0][79].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][79].LoadFrame(0, "image\\mapblock\\block3\\11.bmp"))
		return false;
	blockSprite[0][79].SetNumber(79);
	if (!blockSprite[0][45].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][45].LoadFrame(0, "image\\mapblock\\ice_wall\\00.bmp"))
		return false;
	blockSprite[0][45].SetNumber(45);
	if (!blockSprite[0][46].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][46].LoadFrame(0, "image\\mapblock\\ice_wall\\01.bmp"))
		return false;
	blockSprite[0][46].SetNumber(46);
	if (!blockSprite[0][47].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][47].LoadFrame(0, "image\\mapblock\\ice_wall\\02.bmp"))
		return false;
	blockSprite[0][47].SetNumber(47);
	if (!blockSprite[0][55].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][55].LoadFrame(0, "image\\mapblock\\ice_wall\\03.bmp"))
		return false;
	blockSprite[0][55].SetNumber(55);
	if (!blockSprite[0][56].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][56].LoadFrame(0, "image\\mapblock\\ice_wall\\04.bmp"))
		return false;
	blockSprite[0][56].SetNumber(56);
	if (!blockSprite[0][57].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][57].LoadFrame(0, "image\\mapblock\\ice_wall\\05.bmp"))
		return false;
	blockSprite[0][57].SetNumber(57);
	if (!blockSprite[0][65].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][65].LoadFrame(0, "image\\mapblock\\ice_wall\\06.bmp"))
		return false;
	blockSprite[0][65].SetNumber(65);
	if (!blockSprite[0][66].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][66].LoadFrame(0, "image\\mapblock\\ice_wall\\07.bmp"))
		return false;
	blockSprite[0][66].SetNumber(66);
	if (!blockSprite[0][67].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][67].LoadFrame(0, "image\\mapblock\\ice_wall\\08.bmp"))
		return false;
	blockSprite[0][67].SetNumber(67);
	int cnt = 102;
	if (!blockSprite[0][75].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][75].LoadFrame(0, "image\\mapblock\\block4\\00.bmp"))
		return false;
	blockSprite[0][75].SetNumber(75);
	if (!blockSprite[0][81].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][81].LoadFrame(0, "image\\mapblock\\block4\\02.bmp"))
		return false;
	blockSprite[0][81].SetNumber(81);
	if (!blockSprite[0][82].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][82].LoadFrame(0, "image\\mapblock\\block4\\03.bmp"))
		return false;
	blockSprite[0][82].SetNumber(82);
	if (!blockSprite[0][83].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][83].LoadFrame(0, "image\\mapblock\\block4\\04.bmp"))
		return false;
	blockSprite[0][83].SetNumber(83);
	if (!blockSprite[0][91].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][91].LoadFrame(0, "image\\mapblock\\block4\\05.bmp"))
		return false;
	blockSprite[0][91].SetNumber(91);
	if (!blockSprite[0][92].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][92].LoadFrame(0, "image\\mapblock\\block4\\06.bmp"))
		return false;
	blockSprite[0][92].SetNumber(92);
	if (!blockSprite[0][93].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][93].LoadFrame(0, "image\\mapblock\\block4\\07.bmp"))
		return false;
	blockSprite[0][93].SetNumber(93);
	if (!blockSprite[0][84].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][84].LoadFrame(0, "image\\mapblock\\block4\\08.bmp"))
		return false;
	blockSprite[0][84].SetNumber(84);
	if (!blockSprite[0][85].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][85].LoadFrame(0, "image\\mapblock\\block4\\09.bmp"))
		return false;
	blockSprite[0][85].SetNumber(85);
	if (!blockSprite[0][86].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][86].LoadFrame(0, "image\\mapblock\\block4\\10.bmp"))
		return false;
	blockSprite[0][86].SetNumber(86);
	if (!blockSprite[0][87].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][87].LoadFrame(0, "image\\mapblock\\block4\\11.bmp"))
		return false;
	blockSprite[0][87].SetNumber(87);
	if (!blockSprite[0][88].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][88].LoadFrame(0, "image\\mapblock\\block4\\12.bmp"))
		return false;
	blockSprite[0][88].SetNumber(88);
	if (!blockSprite[0][94].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][94].LoadFrame(0, "image\\mapblock\\block4\\13.bmp"))
		return false;
	blockSprite[0][94].SetNumber(94);
	if (!blockSprite[0][95].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][95].LoadFrame(0, "image\\mapblock\\block4\\14.bmp"))
		return false;
	blockSprite[0][95].SetNumber(95);
	if (!blockSprite[0][96].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][96].LoadFrame(0, "image\\mapblock\\block4\\15.bmp"))
		return false;
	blockSprite[0][96].SetNumber(96);
	if (!blockSprite[0][98].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][98].LoadFrame(0, "image\\mapblock\\block4\\17.bmp"))
		return false;
	blockSprite[0][98].SetNumber(98);
	if (!blockSprite[0][99].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][99].LoadFrame(0, "image\\mapblock\\block4\\19.bmp"))
		return false;
	blockSprite[0][99].SetNumber(99);
	if (!blockSprite[0][97].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][97].LoadFrame(0, "image\\mapblock\\block4\\20.bmp"))
		return false;
	blockSprite[0][97].SetNumber(97);
	if (!blockSprite[0][89].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!blockSprite[0][89].LoadFrame(0, "image\\mapblock\\block4\\22.bmp"))
		return false;
	blockSprite[0][89].SetNumber(89);

	if (!fireSprite.InitSprite(10, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!fireSprite.LoadFrame(0, "image\\mapblock\\fire\\fire_01.bmp"))
		return false;
	if (!fireSprite.LoadFrame(1, "image\\mapblock\\fire\\fire_02.bmp"))
		return false;
	if (!fireSprite.LoadFrame(2, "image\\mapblock\\fire\\fire_03.bmp"))
		return false;
	if (!fireSprite.LoadFrame(3, "image\\mapblock\\fire\\fire_04.bmp"))
		return false;
	if (!fireSprite.LoadFrame(4, "image\\mapblock\\fire\\fire_05.bmp"))
		return false;
	if (!fireSprite.LoadFrame(5, "image\\mapblock\\fire\\fire_06.bmp"))
		return false;
	if (!fireSprite.LoadFrame(6, "image\\mapblock\\fire\\fire_07.bmp"))
		return false;
	if (!fireSprite.LoadFrame(7, "image\\mapblock\\fire\\fire_08.bmp"))
		return false;
	if (!fireSprite.LoadFrame(8, "image\\mapblock\\fire\\fire_09.bmp"))
		return false;
	if (!fireSprite.LoadFrame(9, "image\\mapblock\\fire\\fire_10.bmp"))
		return false;

	string path = "";
	char paths[40];
	string _index = "";
	string _bmp = ".bmp";
	for (int y = 0; y < TOTAL_BLOCK_SPRITE_Y; y++)
	{
		for (int x = 0; x < TOTAL_BLOCK_SPRITE_X; x++)
		{
			if (!blockSprite2[y][x].InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
				return false;
			_index = std::to_string(x + y * TOTAL_BLOCK_SPRITE_X);
			path = "image\\mapblock\\mapblock2\\";
			path.append(_index);
			path.append(_bmp);
			char* c = new char[path.size() + 1]; //string을 char로 바꿔주는 과정
		std:copy(path.begin(), path.end(), c);
			c[path.size()] = '\0';
			if (!blockSprite2[y][x].LoadFrame(0, c))
				return false;
			delete[] c;
			blockSprite2[y][x].SetNumber(x + y * TOTAL_BLOCK_SPRITE_X);
		}
	}

	curBlock.SetSprite(&blockSprite[0][10]);
#pragma endregion

	curMod = EMod::Intro;

	baseMap.LoadBMPFile("image\\map\\basemap.bmp");
	baseMap.CopyBufferToSurface4(g_lpDirectDrawObject);

	bossMap.LoadBMPFile("image\\map\\bossmap.bmp");
	bossMap.CopyBufferToSurface2(g_lpDirectDrawObject);

	bossMapRoof.LoadBMPFile("image\\map\\bossmapRoof.bmp");
	bossMapRoof.CopyBufferToSurface3(g_lpDirectDrawObject);

	curEditMap = &baseMap;
	curEditWindow = EEditWindow::BlockWindow2;

	player_walk.Init();
	player_roll.Init();
	player_dead.Init();
	bow_walk.Init();
	bow_roll.Init();
	bow_attack.Init();
	arrowSprite.Init();
	boss_idle.Init();
	boss_roll.Init();
	boss_attack.Init();
	boss_dead.Init();

#pragma region player_sprite

#pragma region player_walk
	if (!player_walk.GetSprite(EDirection::Left)->InitSprite(6, 20, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_walk.GetSprite(EDirection::Left)->LoadFrame(0, "image\\me\\walk\\me_walk_left00.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Left)->LoadFrame(1, "image\\me\\walk\\me_walk_left01.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Left)->LoadFrame(2, "image\\me\\walk\\me_walk_left02.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Left)->LoadFrame(3, "image\\me\\walk\\me_walk_left03.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Left)->LoadFrame(4, "image\\me\\walk\\me_walk_left04.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Left)->LoadFrame(5, "image\\me\\walk\\me_walk_left05.bmp"))
		return false;

	if (!player_walk.GetSprite(EDirection::LeftUp)->InitSprite(6, 20, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_walk.GetSprite(EDirection::LeftUp)->LoadFrame(0, "image\\me\\walk\\me_walk_leftup00.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::LeftUp)->LoadFrame(1, "image\\me\\walk\\me_walk_leftup01.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::LeftUp)->LoadFrame(2, "image\\me\\walk\\me_walk_leftup02.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::LeftUp)->LoadFrame(3, "image\\me\\walk\\me_walk_leftup03.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::LeftUp)->LoadFrame(4, "image\\me\\walk\\me_walk_leftup04.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::LeftUp)->LoadFrame(5, "image\\me\\walk\\me_walk_leftup05.bmp"))
		return false;

	if (!player_walk.GetSprite(EDirection::LeftDown)->InitSprite(6, 20, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_walk.GetSprite(EDirection::LeftDown)->LoadFrame(0, "image\\me\\walk\\me_walk_leftdown00.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::LeftDown)->LoadFrame(1, "image\\me\\walk\\me_walk_leftdown01.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::LeftDown)->LoadFrame(2, "image\\me\\walk\\me_walk_leftdown02.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::LeftDown)->LoadFrame(3, "image\\me\\walk\\me_walk_leftdown03.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::LeftDown)->LoadFrame(4, "image\\me\\walk\\me_walk_leftdown04.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::LeftDown)->LoadFrame(5, "image\\me\\walk\\me_walk_leftdown05.bmp"))
		return false;

	if (!player_walk.GetSprite(EDirection::Right)->InitSprite(6, 20, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_walk.GetSprite(EDirection::Right)->LoadFrame(0, "image\\me\\walk\\me_walk_right00.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Right)->LoadFrame(1, "image\\me\\walk\\me_walk_right01.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Right)->LoadFrame(2, "image\\me\\walk\\me_walk_right02.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Right)->LoadFrame(3, "image\\me\\walk\\me_walk_right03.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Right)->LoadFrame(4, "image\\me\\walk\\me_walk_right04.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Right)->LoadFrame(5, "image\\me\\walk\\me_walk_right05.bmp"))
		return false;

	if (!player_walk.GetSprite(EDirection::RightUp)->InitSprite(6, 20, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_walk.GetSprite(EDirection::RightUp)->LoadFrame(0, "image\\me\\walk\\me_walk_rightup00.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::RightUp)->LoadFrame(1, "image\\me\\walk\\me_walk_rightup01.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::RightUp)->LoadFrame(2, "image\\me\\walk\\me_walk_rightup02.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::RightUp)->LoadFrame(3, "image\\me\\walk\\me_walk_rightup03.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::RightUp)->LoadFrame(4, "image\\me\\walk\\me_walk_rightup04.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::RightUp)->LoadFrame(5, "image\\me\\walk\\me_walk_rightup05.bmp"))
		return false;

	if (!player_walk.GetSprite(EDirection::RightDown)->InitSprite(6, 20, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_walk.GetSprite(EDirection::RightDown)->LoadFrame(0, "image\\me\\walk\\me_walk_rightdown00.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::RightDown)->LoadFrame(1, "image\\me\\walk\\me_walk_rightdown01.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::RightDown)->LoadFrame(2, "image\\me\\walk\\me_walk_rightdown02.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::RightDown)->LoadFrame(3, "image\\me\\walk\\me_walk_rightdown03.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::RightDown)->LoadFrame(4, "image\\me\\walk\\me_walk_rightdown04.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::RightDown)->LoadFrame(5, "image\\me\\walk\\me_walk_rightdown05.bmp"))
		return false;

	if (!player_walk.GetSprite(EDirection::Up)->InitSprite(6, 20, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_walk.GetSprite(EDirection::Up)->LoadFrame(0, "image\\me\\walk\\me_walk_up00.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Up)->LoadFrame(1, "image\\me\\walk\\me_walk_up01.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Up)->LoadFrame(2, "image\\me\\walk\\me_walk_up02.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Up)->LoadFrame(3, "image\\me\\walk\\me_walk_up03.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Up)->LoadFrame(4, "image\\me\\walk\\me_walk_up04.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Up)->LoadFrame(5, "image\\me\\walk\\me_walk_up05.bmp"))
		return false;

	if (!player_walk.GetSprite(EDirection::Down)->InitSprite(6, 20, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_walk.GetSprite(EDirection::Down)->LoadFrame(0, "image\\me\\walk\\me_walk_down00.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Down)->LoadFrame(1, "image\\me\\walk\\me_walk_down01.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Down)->LoadFrame(2, "image\\me\\walk\\me_walk_down02.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Down)->LoadFrame(3, "image\\me\\walk\\me_walk_down03.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Down)->LoadFrame(4, "image\\me\\walk\\me_walk_down04.bmp"))
		return false;
	if (!player_walk.GetSprite(EDirection::Down)->LoadFrame(5, "image\\me\\walk\\me_walk_down05.bmp"))
		return false;
#pragma endregion

#pragma region player_roll
	if (!player_roll.GetSprite(EDirection::Left)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_roll.GetSprite(EDirection::Left)->LoadFrame(0, "image\\me\\roll\\left\\left_00.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Left)->LoadFrame(1, "image\\me\\roll\\left\\left_01.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Left)->LoadFrame(2, "image\\me\\roll\\left\\left_02.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Left)->LoadFrame(3, "image\\me\\roll\\left\\left_03.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Left)->LoadFrame(4, "image\\me\\roll\\left\\left_04.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Left)->LoadFrame(5, "image\\me\\roll\\left\\left_05.bmp"))
		return false;

	if (!player_roll.GetSprite(EDirection::LeftUp)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_roll.GetSprite(EDirection::LeftUp)->LoadFrame(0, "image\\me\\roll\\leftup\\leftup_00.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::LeftUp)->LoadFrame(1, "image\\me\\roll\\leftup\\leftup_01.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::LeftUp)->LoadFrame(2, "image\\me\\roll\\leftup\\leftup_02.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::LeftUp)->LoadFrame(3, "image\\me\\roll\\leftup\\leftup_03.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::LeftUp)->LoadFrame(4, "image\\me\\roll\\leftup\\leftup_04.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::LeftUp)->LoadFrame(5, "image\\me\\roll\\leftup\\leftup_05.bmp"))
		return false;

	if (!player_roll.GetSprite(EDirection::LeftDown)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_roll.GetSprite(EDirection::LeftDown)->LoadFrame(0, "image\\me\\roll\\leftdown\\leftdown_00.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::LeftDown)->LoadFrame(1, "image\\me\\roll\\leftdown\\leftdown_01.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::LeftDown)->LoadFrame(2, "image\\me\\roll\\leftdown\\leftdown_02.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::LeftDown)->LoadFrame(3, "image\\me\\roll\\leftdown\\leftdown_03.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::LeftDown)->LoadFrame(4, "image\\me\\roll\\leftdown\\leftdown_04.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::LeftDown)->LoadFrame(5, "image\\me\\roll\\leftdown\\leftdown_05.bmp"))
		return false;

	if (!player_roll.GetSprite(EDirection::Right)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_roll.GetSprite(EDirection::Right)->LoadFrame(0, "image\\me\\roll\\right\\right_00.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Right)->LoadFrame(1, "image\\me\\roll\\right\\right_01.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Right)->LoadFrame(2, "image\\me\\roll\\right\\right_02.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Right)->LoadFrame(3, "image\\me\\roll\\right\\right_03.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Right)->LoadFrame(4, "image\\me\\roll\\right\\right_04.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Right)->LoadFrame(5, "image\\me\\roll\\right\\right_05.bmp"))
		return false;

	if (!player_roll.GetSprite(EDirection::RightUp)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_roll.GetSprite(EDirection::RightUp)->LoadFrame(0, "image\\me\\roll\\rightup\\rightup_00.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::RightUp)->LoadFrame(1, "image\\me\\roll\\rightup\\rightup_01.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::RightUp)->LoadFrame(2, "image\\me\\roll\\rightup\\rightup_02.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::RightUp)->LoadFrame(3, "image\\me\\roll\\rightup\\rightup_03.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::RightUp)->LoadFrame(4, "image\\me\\roll\\rightup\\rightup_04.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::RightUp)->LoadFrame(5, "image\\me\\roll\\rightup\\rightup_05.bmp"))
		return false;

	if (!player_roll.GetSprite(EDirection::RightDown)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_roll.GetSprite(EDirection::RightDown)->LoadFrame(0, "image\\me\\roll\\rightdown\\rightdown_00.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::RightDown)->LoadFrame(1, "image\\me\\roll\\rightdown\\rightdown_01.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::RightDown)->LoadFrame(2, "image\\me\\roll\\rightdown\\rightdown_02.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::RightDown)->LoadFrame(3, "image\\me\\roll\\rightdown\\rightdown_03.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::RightDown)->LoadFrame(4, "image\\me\\roll\\rightdown\\rightdown_04.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::RightDown)->LoadFrame(5, "image\\me\\roll\\rightdown\\rightdown_05.bmp"))
		return false;

	if (!player_roll.GetSprite(EDirection::Up)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_roll.GetSprite(EDirection::Up)->LoadFrame(0, "image\\me\\roll\\up\\up_00.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Up)->LoadFrame(1, "image\\me\\roll\\up\\up_01.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Up)->LoadFrame(2, "image\\me\\roll\\up\\up_02.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Up)->LoadFrame(3, "image\\me\\roll\\up\\up_03.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Up)->LoadFrame(4, "image\\me\\roll\\up\\up_04.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Up)->LoadFrame(5, "image\\me\\roll\\up\\up_05.bmp"))
		return false;

	if (!player_roll.GetSprite(EDirection::Down)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_roll.GetSprite(EDirection::Down)->LoadFrame(0, "image\\me\\roll\\down\\down_00.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Down)->LoadFrame(1, "image\\me\\roll\\down\\down_01.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Down)->LoadFrame(2, "image\\me\\roll\\down\\down_02.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Down)->LoadFrame(3, "image\\me\\roll\\down\\down_03.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Down)->LoadFrame(4, "image\\me\\roll\\down\\down_04.bmp"))
		return false;
	if (!player_roll.GetSprite(EDirection::Down)->LoadFrame(5, "image\\me\\roll\\down\\down_05.bmp"))
		return false;
#pragma endregion

#pragma region player_dead
	if (!player_dead.GetSprite(EDirection::Left)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_dead.GetSprite(EDirection::Left)->LoadFrame(0, "image\\me\\dead\\left.bmp"))
		return false;
	if (!player_dead.GetSprite(EDirection::LeftUp)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_dead.GetSprite(EDirection::LeftUp)->LoadFrame(0, "image\\me\\dead\\leftup.bmp"))
		return false;
	if (!player_dead.GetSprite(EDirection::LeftDown)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_dead.GetSprite(EDirection::LeftDown)->LoadFrame(0, "image\\me\\dead\\leftdown.bmp"))
		return false;
	if (!player_dead.GetSprite(EDirection::Right)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_dead.GetSprite(EDirection::Right)->LoadFrame(0, "image\\me\\dead\\right.bmp"))
		return false;
	if (!player_dead.GetSprite(EDirection::RightUp)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_dead.GetSprite(EDirection::RightUp)->LoadFrame(0, "image\\me\\dead\\rightup.bmp"))
		return false;
	if (!player_dead.GetSprite(EDirection::RightDown)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_dead.GetSprite(EDirection::RightDown)->LoadFrame(0, "image\\me\\dead\\rightdown.bmp"))
		return false;
	if (!player_dead.GetSprite(EDirection::Up)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_dead.GetSprite(EDirection::Up)->LoadFrame(0, "image\\me\\dead\\up.bmp"))
		return false;
	if (!player_dead.GetSprite(EDirection::Down)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_dead.GetSprite(EDirection::Down)->LoadFrame(0, "image\\me\\dead\\down.bmp"))
		return false;
#pragma endregion

#pragma region player_skill
	if (!player_skill.InitSprite(11, 64, 64, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_skill.LoadFrame(0, "image\\skill\\skill_01.bmp"))
		return false;
	if (!player_skill.LoadFrame(1, "image\\skill\\skill_02.bmp"))
		return false;
	if (!player_skill.LoadFrame(2, "image\\skill\\skill_03.bmp"))
		return false;
	if (!player_skill.LoadFrame(3, "image\\skill\\skill_04.bmp"))
		return false;
	if (!player_skill.LoadFrame(4, "image\\skill\\skill_05.bmp"))
		return false;
	if (!player_skill.LoadFrame(5, "image\\skill\\skill_06.bmp"))
		return false;
	if (!player_skill.LoadFrame(6, "image\\skill\\skill_07.bmp"))
		return false;
	if (!player_skill.LoadFrame(7, "image\\skill\\skill_08.bmp"))
		return false;
	if (!player_skill.LoadFrame(8, "image\\skill\\skill_09.bmp"))
		return false;
	if (!player_skill.LoadFrame(9, "image\\skill\\skill_10.bmp"))
		return false;
	if (!player_skill.LoadFrame(10, "image\\skill\\skill_11.bmp"))
		return false;
#pragma endregion

#pragma endregion

#pragma region bow_arrow_sprite

#pragma region bow_walk
	if (!bow_walk.GetSprite(EDirection::Left)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_walk.GetSprite(EDirection::Left)->LoadFrame(0, "image\\bow\\walk\\left\\bow_walk_left_00.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Left)->LoadFrame(1, "image\\bow\\walk\\left\\bow_walk_left_01.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Left)->LoadFrame(2, "image\\bow\\walk\\left\\bow_walk_left_02.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Left)->LoadFrame(3, "image\\bow\\walk\\left\\bow_walk_left_03.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Left)->LoadFrame(4, "image\\bow\\walk\\left\\bow_walk_left_04.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Left)->LoadFrame(5, "image\\bow\\walk\\left\\bow_walk_left_05.bmp"))
		return false;

	if (!bow_walk.GetSprite(EDirection::LeftUp)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_walk.GetSprite(EDirection::LeftUp)->LoadFrame(0, "image\\bow\\walk\\leftup\\bow_walk_leftup_00.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::LeftUp)->LoadFrame(1, "image\\bow\\walk\\leftup\\bow_walk_leftup_01.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::LeftUp)->LoadFrame(2, "image\\bow\\walk\\leftup\\bow_walk_leftup_02.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::LeftUp)->LoadFrame(3, "image\\bow\\walk\\leftup\\bow_walk_leftup_03.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::LeftUp)->LoadFrame(4, "image\\bow\\walk\\leftup\\bow_walk_leftup_04.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::LeftUp)->LoadFrame(5, "image\\bow\\walk\\leftup\\bow_walk_leftup_05.bmp"))
		return false;

	if (!bow_walk.GetSprite(EDirection::LeftDown)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_walk.GetSprite(EDirection::LeftDown)->LoadFrame(0, "image\\bow\\walk\\leftdown\\bow_walk_leftdown_00.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::LeftDown)->LoadFrame(1, "image\\bow\\walk\\leftdown\\bow_walk_leftdown_01.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::LeftDown)->LoadFrame(2, "image\\bow\\walk\\leftdown\\bow_walk_leftdown_02.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::LeftDown)->LoadFrame(3, "image\\bow\\walk\\leftdown\\bow_walk_leftdown_03.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::LeftDown)->LoadFrame(4, "image\\bow\\walk\\leftdown\\bow_walk_leftdown_04.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::LeftDown)->LoadFrame(5, "image\\bow\\walk\\leftdown\\bow_walk_leftdown_05.bmp"))
		return false;

	if (!bow_walk.GetSprite(EDirection::Right)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_walk.GetSprite(EDirection::Right)->LoadFrame(0, "image\\bow\\walk\\right\\bow_walk_right_00.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Right)->LoadFrame(1, "image\\bow\\walk\\right\\bow_walk_right_01.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Right)->LoadFrame(2, "image\\bow\\walk\\right\\bow_walk_right_02.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Right)->LoadFrame(3, "image\\bow\\walk\\right\\bow_walk_right_03.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Right)->LoadFrame(4, "image\\bow\\walk\\right\\bow_walk_right_04.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Right)->LoadFrame(5, "image\\bow\\walk\\right\\bow_walk_right_05.bmp"))
		return false;

	if (!bow_walk.GetSprite(EDirection::RightUp)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_walk.GetSprite(EDirection::RightUp)->LoadFrame(0, "image\\bow\\walk\\rightup\\bow_walk_rightup_00.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::RightUp)->LoadFrame(1, "image\\bow\\walk\\rightup\\bow_walk_rightup_01.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::RightUp)->LoadFrame(2, "image\\bow\\walk\\rightup\\bow_walk_rightup_02.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::RightUp)->LoadFrame(3, "image\\bow\\walk\\rightup\\bow_walk_rightup_03.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::RightUp)->LoadFrame(4, "image\\bow\\walk\\rightup\\bow_walk_rightup_04.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::RightUp)->LoadFrame(5, "image\\bow\\walk\\rightup\\bow_walk_rightup_05.bmp"))
		return false;

	if (!bow_walk.GetSprite(EDirection::RightDown)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_walk.GetSprite(EDirection::RightDown)->LoadFrame(
		0, "image\\bow\\walk\\rightdown\\bow_walk_rightdown_00.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::RightDown)->LoadFrame(
		1, "image\\bow\\walk\\rightdown\\bow_walk_rightdown_01.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::RightDown)->LoadFrame(
		2, "image\\bow\\walk\\rightdown\\bow_walk_rightdown_02.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::RightDown)->LoadFrame(
		3, "image\\bow\\walk\\rightdown\\bow_walk_rightdown_03.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::RightDown)->LoadFrame(
		4, "image\\bow\\walk\\rightdown\\bow_walk_rightdown_04.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::RightDown)->LoadFrame(
		5, "image\\bow\\walk\\rightdown\\bow_walk_rightdown_05.bmp"))
		return false;

	if (!bow_walk.GetSprite(EDirection::Up)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_walk.GetSprite(EDirection::Up)->LoadFrame(0, "image\\bow\\walk\\up\\bow_walk_up_00.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Up)->LoadFrame(1, "image\\bow\\walk\\up\\bow_walk_up_01.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Up)->LoadFrame(2, "image\\bow\\walk\\up\\bow_walk_up_02.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Up)->LoadFrame(3, "image\\bow\\walk\\up\\bow_walk_up_03.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Up)->LoadFrame(4, "image\\bow\\walk\\up\\bow_walk_up_04.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Up)->LoadFrame(5, "image\\bow\\walk\\up\\bow_walk_up_05.bmp"))
		return false;

	if (!bow_walk.GetSprite(EDirection::Down)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_walk.GetSprite(EDirection::Down)->LoadFrame(0, "image\\bow\\walk\\down\\bow_walk_down_00.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Down)->LoadFrame(1, "image\\bow\\walk\\down\\bow_walk_down_01.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Down)->LoadFrame(2, "image\\bow\\walk\\down\\bow_walk_down_02.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Down)->LoadFrame(3, "image\\bow\\walk\\down\\bow_walk_down_03.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Down)->LoadFrame(4, "image\\bow\\walk\\down\\bow_walk_down_04.bmp"))
		return false;
	if (!bow_walk.GetSprite(EDirection::Down)->LoadFrame(5, "image\\bow\\walk\\down\\bow_walk_down_05.bmp"))
		return false;
#pragma endregion

#pragma region bow_roll
	if (!bow_roll.GetSprite(EDirection::Left)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_roll.GetSprite(EDirection::Left)->LoadFrame(0, "image\\bow\\roll\\left\\bow_roll_left_00.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Left)->LoadFrame(1, "image\\bow\\roll\\left\\bow_roll_left_01.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Left)->LoadFrame(2, "image\\bow\\roll\\left\\bow_roll_left_02.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Left)->LoadFrame(3, "image\\bow\\roll\\left\\bow_roll_left_03.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Left)->LoadFrame(4, "image\\bow\\roll\\left\\bow_roll_left_04.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Left)->LoadFrame(5, "image\\bow\\roll\\left\\bow_roll_left_05.bmp"))
		return false;

	if (!bow_roll.GetSprite(EDirection::LeftUp)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_roll.GetSprite(EDirection::LeftUp)->LoadFrame(0, "image\\bow\\roll\\leftup\\bow_roll_leftup_00.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::LeftUp)->LoadFrame(1, "image\\bow\\roll\\leftup\\bow_roll_leftup_01.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::LeftUp)->LoadFrame(2, "image\\bow\\roll\\leftup\\bow_roll_leftup_02.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::LeftUp)->LoadFrame(3, "image\\bow\\roll\\leftup\\bow_roll_leftup_03.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::LeftUp)->LoadFrame(4, "image\\bow\\roll\\leftup\\bow_roll_leftup_04.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::LeftUp)->LoadFrame(5, "image\\bow\\roll\\leftup\\bow_roll_leftup_05.bmp"))
		return false;

	if (!bow_roll.GetSprite(EDirection::LeftDown)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_roll.GetSprite(EDirection::LeftDown)->LoadFrame(0, "image\\bow\\roll\\leftdown\\bow_roll_leftdown_00.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::LeftDown)->LoadFrame(1, "image\\bow\\roll\\leftdown\\bow_roll_leftdown_01.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::LeftDown)->LoadFrame(2, "image\\bow\\roll\\leftdown\\bow_roll_leftdown_02.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::LeftDown)->LoadFrame(3, "image\\bow\\roll\\leftdown\\bow_roll_leftdown_03.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::LeftDown)->LoadFrame(4, "image\\bow\\roll\\leftdown\\bow_roll_leftdown_04.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::LeftDown)->LoadFrame(5, "image\\bow\\roll\\leftdown\\bow_roll_leftdown_05.bmp"))
		return false;

	if (!bow_roll.GetSprite(EDirection::Right)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_roll.GetSprite(EDirection::Right)->LoadFrame(0, "image\\bow\\roll\\right\\bow_roll_right_00.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Right)->LoadFrame(1, "image\\bow\\roll\\right\\bow_roll_right_01.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Right)->LoadFrame(2, "image\\bow\\roll\\right\\bow_roll_right_02.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Right)->LoadFrame(3, "image\\bow\\roll\\right\\bow_roll_right_03.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Right)->LoadFrame(4, "image\\bow\\roll\\right\\bow_roll_right_04.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Right)->LoadFrame(5, "image\\bow\\roll\\right\\bow_roll_right_05.bmp"))
		return false;

	if (!bow_roll.GetSprite(EDirection::RightUp)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_roll.GetSprite(EDirection::RightUp)->LoadFrame(0, "image\\bow\\roll\\rightup\\bow_roll_rightup_00.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::RightUp)->LoadFrame(1, "image\\bow\\roll\\rightup\\bow_roll_rightup_01.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::RightUp)->LoadFrame(2, "image\\bow\\roll\\rightup\\bow_roll_rightup_02.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::RightUp)->LoadFrame(3, "image\\bow\\roll\\rightup\\bow_roll_rightup_03.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::RightUp)->LoadFrame(4, "image\\bow\\roll\\rightup\\bow_roll_rightup_04.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::RightUp)->LoadFrame(5, "image\\bow\\roll\\rightup\\bow_roll_rightup_05.bmp"))
		return false;

	if (!bow_roll.GetSprite(EDirection::RightDown)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_roll.GetSprite(EDirection::RightDown)->LoadFrame(
		0, "image\\bow\\roll\\rightdown\\bow_roll_rightdown_00.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::RightDown)->LoadFrame(
		1, "image\\bow\\roll\\rightdown\\bow_roll_rightdown_01.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::RightDown)->LoadFrame(
		2, "image\\bow\\roll\\rightdown\\bow_roll_rightdown_02.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::RightDown)->LoadFrame(
		3, "image\\bow\\roll\\rightdown\\bow_roll_rightdown_03.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::RightDown)->LoadFrame(
		4, "image\\bow\\roll\\rightdown\\bow_roll_rightdown_04.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::RightDown)->LoadFrame(
		5, "image\\bow\\roll\\rightdown\\bow_roll_rightdown_05.bmp"))
		return false;

	if (!bow_roll.GetSprite(EDirection::Up)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_roll.GetSprite(EDirection::Up)->LoadFrame(0, "image\\bow\\roll\\up\\bow_roll_up_00.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Up)->LoadFrame(1, "image\\bow\\roll\\up\\bow_roll_up_01.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Up)->LoadFrame(2, "image\\bow\\roll\\up\\bow_roll_up_02.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Up)->LoadFrame(3, "image\\bow\\roll\\up\\bow_roll_up_03.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Up)->LoadFrame(4, "image\\bow\\roll\\up\\bow_roll_up_04.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Up)->LoadFrame(5, "image\\bow\\roll\\up\\bow_roll_up_05.bmp"))
		return false;

	if (!bow_roll.GetSprite(EDirection::Down)->InitSprite(6, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_roll.GetSprite(EDirection::Down)->LoadFrame(0, "image\\bow\\roll\\down\\bow_roll_down_00.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Down)->LoadFrame(1, "image\\bow\\roll\\down\\bow_roll_down_01.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Down)->LoadFrame(2, "image\\bow\\roll\\down\\bow_roll_down_02.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Down)->LoadFrame(3, "image\\bow\\roll\\down\\bow_roll_down_03.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Down)->LoadFrame(4, "image\\bow\\roll\\down\\bow_roll_down_04.bmp"))
		return false;
	if (!bow_roll.GetSprite(EDirection::Down)->LoadFrame(5, "image\\bow\\roll\\down\\bow_roll_down_05.bmp"))
		return false;
#pragma endregion

#pragma region bow_attack
	if (!bow_attack.GetSprite(EDirection::Left)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_attack.GetSprite(EDirection::Left)->LoadFrame(0, "image\\bow\\attack\\bow_attack_left.bmp"))
		return false;

	if (!bow_attack.GetSprite(EDirection::LeftUp)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_attack.GetSprite(EDirection::LeftUp)->LoadFrame(0, "image\\bow\\attack\\bow_attack_leftup.bmp"))
		return false;

	if (!bow_attack.GetSprite(EDirection::LeftDown)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_attack.GetSprite(EDirection::LeftDown)->LoadFrame(0, "image\\bow\\attack\\bow_attack_leftdown.bmp"))
		return false;

	if (!bow_attack.GetSprite(EDirection::Right)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_attack.GetSprite(EDirection::Right)->LoadFrame(0, "image\\bow\\attack\\bow_attack_right.bmp"))
		return false;

	if (!bow_attack.GetSprite(EDirection::RightUp)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_attack.GetSprite(EDirection::RightUp)->LoadFrame(0, "image\\bow\\attack\\bow_attack_rightup.bmp"))
		return false;

	if (!bow_attack.GetSprite(EDirection::RightDown)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_attack.GetSprite(EDirection::RightDown)->LoadFrame(0, "image\\bow\\attack\\bow_attack_rightdown.bmp"))
		return false;

	if (!bow_attack.GetSprite(EDirection::Up)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_attack.GetSprite(EDirection::Up)->LoadFrame(0, "image\\bow\\attack\\bow_attack_up.bmp"))
		return false;

	if (!bow_attack.GetSprite(EDirection::Down)->InitSprite(1, 32, 32, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!bow_attack.GetSprite(EDirection::Down)->LoadFrame(0, "image\\bow\\attack\\bow_attack_down.bmp"))
		return false;
#pragma endregion

#pragma region arrow
	if (!arrowSprite.GetSprite(EDirection::Left)->InitSprite(1, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!arrowSprite.GetSprite(EDirection::Left)->LoadFrame(0, "image\\arrow\\arrow_left.bmp"))
		return false;

	if (!arrowSprite.GetSprite(EDirection::LeftUp)->InitSprite(1, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!arrowSprite.GetSprite(EDirection::LeftUp)->LoadFrame(0, "image\\arrow\\arrow_leftup.bmp"))
		return false;

	if (!arrowSprite.GetSprite(EDirection::LeftDown)->InitSprite(1, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!arrowSprite.GetSprite(EDirection::LeftDown)->LoadFrame(0, "image\\arrow\\arrow_leftdown.bmp"))
		return false;

	if (!arrowSprite.GetSprite(EDirection::Right)->InitSprite(1, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!arrowSprite.GetSprite(EDirection::Right)->LoadFrame(0, "image\\arrow\\arrow_right.bmp"))
		return false;

	if (!arrowSprite.GetSprite(EDirection::RightUp)->InitSprite(1, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!arrowSprite.GetSprite(EDirection::RightUp)->LoadFrame(0, "image\\arrow\\arrow_rightup.bmp"))
		return false;

	if (!arrowSprite.GetSprite(EDirection::RightDown)->InitSprite(1, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!arrowSprite.GetSprite(EDirection::RightDown)->LoadFrame(0, "image\\arrow\\arrow_rightdown.bmp"))
		return false;

	if (!arrowSprite.GetSprite(EDirection::Up)->InitSprite(1, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!arrowSprite.GetSprite(EDirection::Up)->LoadFrame(0, "image\\arrow\\arrow_up.bmp"))
		return false;

	if (!arrowSprite.GetSprite(EDirection::Down)->InitSprite(1, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!arrowSprite.GetSprite(EDirection::Down)->LoadFrame(0, "image\\arrow\\arrow_down.bmp"))
		return false;
#pragma endregion

#pragma endregion

#pragma region boss_sprite

#pragma region boss_idle
	if (!boss_sleep.InitSprite(1, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_sleep.LoadFrame(0, "image\\boss\\idle\\sleep00.bmp"))
		return false;

	if (!boss_snowball.InitSprite(1, 24, 24, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_snowball.LoadFrame(0, "image\\boss\\weapon\\snowball.bmp"))
		return false;

	if (!boss_idle.GetSprite(EDirection::Left)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_idle.GetSprite(EDirection::Left)->LoadFrame(0, "image\\boss\\idle\\left\\00.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::Left)->LoadFrame(1, "image\\boss\\idle\\left\\01.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::Left)->LoadFrame(2, "image\\boss\\idle\\left\\02.bmp"))
		return false;

	if (!boss_idle.GetSprite(EDirection::LeftUp)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_idle.GetSprite(EDirection::LeftUp)->LoadFrame(0, "image\\boss\\idle\\leftup\\00.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::LeftUp)->LoadFrame(1, "image\\boss\\idle\\leftup\\01.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::LeftUp)->LoadFrame(2, "image\\boss\\idle\\leftup\\02.bmp"))
		return false;

	if (!boss_idle.GetSprite(EDirection::LeftDown)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_idle.GetSprite(EDirection::LeftDown)->LoadFrame(0, "image\\boss\\idle\\leftdown\\00.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::LeftDown)->LoadFrame(1, "image\\boss\\idle\\leftdown\\01.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::LeftDown)->LoadFrame(2, "image\\boss\\idle\\leftdown\\02.bmp"))
		return false;

	if (!boss_idle.GetSprite(EDirection::Right)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_idle.GetSprite(EDirection::Right)->LoadFrame(0, "image\\boss\\idle\\right\\00.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::Right)->LoadFrame(1, "image\\boss\\idle\\right\\01.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::Right)->LoadFrame(2, "image\\boss\\idle\\right\\02.bmp"))
		return false;

	if (!boss_idle.GetSprite(EDirection::RightUp)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_idle.GetSprite(EDirection::RightUp)->LoadFrame(0, "image\\boss\\idle\\rightup\\00.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::RightUp)->LoadFrame(1, "image\\boss\\idle\\rightup\\01.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::RightUp)->LoadFrame(2, "image\\boss\\idle\\rightup\\02.bmp"))
		return false;

	if (!boss_idle.GetSprite(EDirection::RightDown)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_idle.GetSprite(EDirection::RightDown)->LoadFrame(0, "image\\boss\\idle\\rightdown\\00.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::RightDown)->LoadFrame(1, "image\\boss\\idle\\rightdown\\01.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::RightDown)->LoadFrame(2, "image\\boss\\idle\\rightdown\\02.bmp"))
		return false;

	if (!boss_idle.GetSprite(EDirection::Up)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_idle.GetSprite(EDirection::Up)->LoadFrame(0, "image\\boss\\idle\\up\\00.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::Up)->LoadFrame(1, "image\\boss\\idle\\up\\01.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::Up)->LoadFrame(2, "image\\boss\\idle\\up\\02.bmp"))
		return false;

	if (!boss_idle.GetSprite(EDirection::Down)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_idle.GetSprite(EDirection::Down)->LoadFrame(0, "image\\boss\\idle\\down\\00.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::Down)->LoadFrame(1, "image\\boss\\idle\\down\\01.bmp"))
		return false;
	if (!boss_idle.GetSprite(EDirection::Down)->LoadFrame(2, "image\\boss\\idle\\down\\02.bmp"))
		return false;
#pragma endregion

#pragma region boss_roll
	if (!boss_roll.GetSprite(EDirection::Left)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_roll.GetSprite(EDirection::Left)->LoadFrame(0, "image\\boss\\roll\\left\\00.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::Left)->LoadFrame(1, "image\\boss\\roll\\left\\01.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::Left)->LoadFrame(2, "image\\boss\\roll\\left\\02.bmp"))
		return false;

	if (!boss_roll.GetSprite(EDirection::LeftUp)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_roll.GetSprite(EDirection::LeftUp)->LoadFrame(0, "image\\boss\\roll\\leftup\\00.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::LeftUp)->LoadFrame(1, "image\\boss\\roll\\leftup\\01.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::LeftUp)->LoadFrame(2, "image\\boss\\roll\\leftup\\02.bmp"))
		return false;

	if (!boss_roll.GetSprite(EDirection::LeftDown)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_roll.GetSprite(EDirection::LeftDown)->LoadFrame(0, "image\\boss\\roll\\leftdown\\00.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::LeftDown)->LoadFrame(1, "image\\boss\\roll\\leftdown\\01.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::LeftDown)->LoadFrame(2, "image\\boss\\roll\\leftdown\\02.bmp"))
		return false;

	if (!boss_roll.GetSprite(EDirection::Right)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_roll.GetSprite(EDirection::Right)->LoadFrame(0, "image\\boss\\roll\\right\\00.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::Right)->LoadFrame(1, "image\\boss\\roll\\right\\01.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::Right)->LoadFrame(2, "image\\boss\\roll\\right\\02.bmp"))
		return false;

	if (!boss_roll.GetSprite(EDirection::RightUp)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_roll.GetSprite(EDirection::RightUp)->LoadFrame(0, "image\\boss\\roll\\rightup\\00.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::RightUp)->LoadFrame(1, "image\\boss\\roll\\rightup\\01.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::RightUp)->LoadFrame(2, "image\\boss\\roll\\rightup\\02.bmp"))
		return false;

	if (!boss_roll.GetSprite(EDirection::RightDown)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_roll.GetSprite(EDirection::RightDown)->LoadFrame(0, "image\\boss\\roll\\rightdown\\00.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::RightDown)->LoadFrame(1, "image\\boss\\roll\\rightdown\\01.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::RightDown)->LoadFrame(2, "image\\boss\\roll\\rightdown\\02.bmp"))
		return false;

	if (!boss_roll.GetSprite(EDirection::Up)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_roll.GetSprite(EDirection::Up)->LoadFrame(0, "image\\boss\\roll\\up\\00.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::Up)->LoadFrame(1, "image\\boss\\roll\\up\\01.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::Up)->LoadFrame(2, "image\\boss\\roll\\up\\02.bmp"))
		return false;

	if (!boss_roll.GetSprite(EDirection::Down)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_roll.GetSprite(EDirection::Down)->LoadFrame(0, "image\\boss\\roll\\down\\00.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::Down)->LoadFrame(1, "image\\boss\\roll\\down\\01.bmp"))
		return false;
	if (!boss_roll.GetSprite(EDirection::Down)->LoadFrame(2, "image\\boss\\roll\\down\\02.bmp"))
		return false;
#pragma endregion

#pragma region boss_attack
	if (!boss_attack.GetSprite(EDirection::Left)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_attack.GetSprite(EDirection::Left)->LoadFrame(0, "image\\boss\\attack\\left\\00.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::Left)->LoadFrame(1, "image\\boss\\attack\\left\\01.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::Left)->LoadFrame(2, "image\\boss\\attack\\left\\02.bmp"))
		return false;

	if (!boss_attack.GetSprite(EDirection::LeftUp)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_attack.GetSprite(EDirection::LeftUp)->LoadFrame(0, "image\\boss\\attack\\leftup\\00.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::LeftUp)->LoadFrame(1, "image\\boss\\attack\\leftup\\01.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::LeftUp)->LoadFrame(2, "image\\boss\\attack\\leftup\\02.bmp"))
		return false;

	if (!boss_attack.GetSprite(EDirection::LeftDown)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_attack.GetSprite(EDirection::LeftDown)->LoadFrame(0, "image\\boss\\attack\\leftdown\\00.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::LeftDown)->LoadFrame(1, "image\\boss\\attack\\leftdown\\01.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::LeftDown)->LoadFrame(2, "image\\boss\\attack\\leftdown\\02.bmp"))
		return false;

	if (!boss_attack.GetSprite(EDirection::Right)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_attack.GetSprite(EDirection::Right)->LoadFrame(0, "image\\boss\\attack\\right\\00.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::Right)->LoadFrame(1, "image\\boss\\attack\\right\\01.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::Right)->LoadFrame(2, "image\\boss\\attack\\right\\02.bmp"))
		return false;

	if (!boss_attack.GetSprite(EDirection::RightUp)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_attack.GetSprite(EDirection::RightUp)->LoadFrame(0, "image\\boss\\attack\\rightup\\00.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::RightUp)->LoadFrame(1, "image\\boss\\attack\\rightup\\01.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::RightUp)->LoadFrame(2, "image\\boss\\attack\\rightup\\02.bmp"))
		return false;

	if (!boss_attack.GetSprite(EDirection::RightDown)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_attack.GetSprite(EDirection::RightDown)->LoadFrame(0, "image\\boss\\attack\\rightdown\\00.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::RightDown)->LoadFrame(1, "image\\boss\\attack\\rightdown\\01.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::RightDown)->LoadFrame(2, "image\\boss\\attack\\rightdown\\02.bmp"))
		return false;

	if (!boss_attack.GetSprite(EDirection::Up)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_attack.GetSprite(EDirection::Up)->LoadFrame(0, "image\\boss\\attack\\up\\00.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::Up)->LoadFrame(1, "image\\boss\\attack\\up\\01.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::Up)->LoadFrame(2, "image\\boss\\attack\\up\\02.bmp"))
		return false;

	if (!boss_attack.GetSprite(EDirection::Down)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_attack.GetSprite(EDirection::Down)->LoadFrame(0, "image\\boss\\attack\\down\\00.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::Down)->LoadFrame(1, "image\\boss\\attack\\down\\01.bmp"))
		return false;
	if (!boss_attack.GetSprite(EDirection::Down)->LoadFrame(2, "image\\boss\\attack\\down\\02.bmp"))
		return false;
#pragma endregion

#pragma region boss_dead
	if (!boss_dead.GetSprite(EDirection::Left)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_dead.GetSprite(EDirection::Left)->LoadFrame(0, "image\\boss\\dead\\left\\00.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::Left)->LoadFrame(1, "image\\boss\\dead\\left\\01.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::Left)->LoadFrame(2, "image\\boss\\dead\\left\\02.bmp"))
		return false;

	if (!boss_dead.GetSprite(EDirection::LeftUp)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_dead.GetSprite(EDirection::LeftUp)->LoadFrame(0, "image\\boss\\dead\\leftup\\00.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::LeftUp)->LoadFrame(1, "image\\boss\\dead\\leftup\\01.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::LeftUp)->LoadFrame(2, "image\\boss\\dead\\leftup\\02.bmp"))
		return false;

	if (!boss_dead.GetSprite(EDirection::LeftDown)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_dead.GetSprite(EDirection::LeftDown)->LoadFrame(0, "image\\boss\\dead\\leftdown\\00.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::LeftDown)->LoadFrame(1, "image\\boss\\dead\\leftdown\\01.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::LeftDown)->LoadFrame(2, "image\\boss\\dead\\leftdown\\02.bmp"))
		return false;

	if (!boss_dead.GetSprite(EDirection::Right)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_dead.GetSprite(EDirection::Right)->LoadFrame(0, "image\\boss\\dead\\right\\00.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::Right)->LoadFrame(1, "image\\boss\\dead\\right\\01.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::Right)->LoadFrame(2, "image\\boss\\dead\\right\\02.bmp"))
		return false;

	if (!boss_dead.GetSprite(EDirection::RightUp)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_dead.GetSprite(EDirection::RightUp)->LoadFrame(0, "image\\boss\\dead\\rightup\\00.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::RightUp)->LoadFrame(1, "image\\boss\\dead\\rightup\\01.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::RightUp)->LoadFrame(2, "image\\boss\\dead\\rightup\\02.bmp"))
		return false;

	if (!boss_dead.GetSprite(EDirection::RightDown)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_dead.GetSprite(EDirection::RightDown)->LoadFrame(0, "image\\boss\\dead\\rightdown\\00.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::RightDown)->LoadFrame(1, "image\\boss\\dead\\rightdown\\01.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::RightDown)->LoadFrame(2, "image\\boss\\dead\\rightdown\\02.bmp"))
		return false;

	if (!boss_dead.GetSprite(EDirection::Up)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_dead.GetSprite(EDirection::Up)->LoadFrame(0, "image\\boss\\dead\\up\\00.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::Up)->LoadFrame(1, "image\\boss\\dead\\up\\01.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::Up)->LoadFrame(2, "image\\boss\\dead\\up\\02.bmp"))
		return false;

	if (!boss_dead.GetSprite(EDirection::Down)->InitSprite(3, 192, 192, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_dead.GetSprite(EDirection::Down)->LoadFrame(0, "image\\boss\\dead\\down\\00.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::Down)->LoadFrame(1, "image\\boss\\dead\\down\\01.bmp"))
		return false;
	if (!boss_dead.GetSprite(EDirection::Down)->LoadFrame(2, "image\\boss\\dead\\down\\02.bmp"))
		return false;
#pragma endregion

#pragma endregion

#pragma region boss_hp_bar
	if (!boss_hp_window.InitSprite(1, 36, 552, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_hp_window.LoadFrame(0, "image\\gui\\boss_hp_bar\\boss_hp_window.bmp"))
		return false;

	if (!boss_hp.InitSprite(4, 18, 450, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!boss_hp.LoadFrame(0, "image\\gui\\boss_hp_bar\\boss_hp0.bmp"))
		return false;
	if (!boss_hp.LoadFrame(1, "image\\gui\\boss_hp_bar\\boss_hp1.bmp"))
		return false;
	if (!boss_hp.LoadFrame(2, "image\\gui\\boss_hp_bar\\boss_hp2.bmp"))
		return false;
	if (!boss_hp.LoadFrame(3, "image\\gui\\boss_hp_bar\\boss_hp3.bmp"))
		return false;
#pragma endregion

#pragma region player_hp
	if (!player_hp.InitSprite(3, 50, 43, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!player_hp.LoadFrame(0, "image\\gui\\player_hp\\hp_0.bmp"))
		return false;
	if (!player_hp.LoadFrame(1, "image\\gui\\player_hp\\hp_1.bmp"))
		return false;
	if (!player_hp.LoadFrame(2, "image\\gui\\player_hp\\hp_2.bmp"))
		return false;
#pragma endregion

#pragma region skill_backgroud
	if (!skill_background.InitSprite(1, 65, 65, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!skill_background.LoadFrame(0, "image\\gui\\skill\\background.bmp"))
		return false;
#pragma endregion

#pragma region skill_roll
	if (!skill_roll.InitSprite(1, 45, 45, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!skill_roll.LoadFrame(0, "image\\gui\\skill\\roll.bmp"))
		return false;
#pragma endregion

#pragma region skill_cyclone
	if (!skill_cyclone.InitSprite(1, 45, 45, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!skill_cyclone.LoadFrame(0, "image\\gui\\skill\\cyclone.bmp"))
		return false;
#pragma endregion

	if (!tutorialSprite.InitSprite(1, 1024, 596, 10, g_lpDirectDrawObject))
		return false;
	if (!tutorialSprite.LoadFrame(0, "image\\gui\\tutorial.bmp"))
		return false;


#pragma region enemy_idle
	if (!enemy_idle_left.InitSprite(6, 24 * 2, 21 * 2, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!enemy_idle_left.LoadFrame(0, "image\\enemy\\idle\\left\\00.bmp"))
		return false;
	if (!enemy_idle_left.LoadFrame(1, "image\\enemy\\idle\\left\\01.bmp"))
		return false;
	if (!enemy_idle_left.LoadFrame(2, "image\\enemy\\idle\\left\\02.bmp"))
		return false;
	if (!enemy_idle_left.LoadFrame(3, "image\\enemy\\idle\\left\\03.bmp"))
		return false;
	if (!enemy_idle_left.LoadFrame(4, "image\\enemy\\idle\\left\\04.bmp"))
		return false;
	if (!enemy_idle_left.LoadFrame(5, "image\\enemy\\idle\\left\\05.bmp"))
		return false;

	if (!enemy_idle_right.InitSprite(6, 24 * 2, 21 * 2, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!enemy_idle_right.LoadFrame(0, "image\\enemy\\idle\\right\\00.bmp"))
		return false;
	if (!enemy_idle_right.LoadFrame(1, "image\\enemy\\idle\\right\\01.bmp"))
		return false;
	if (!enemy_idle_right.LoadFrame(2, "image\\enemy\\idle\\right\\02.bmp"))
		return false;
	if (!enemy_idle_right.LoadFrame(3, "image\\enemy\\idle\\right\\03.bmp"))
		return false;
	if (!enemy_idle_right.LoadFrame(4, "image\\enemy\\idle\\right\\04.bmp"))
		return false;
	if (!enemy_idle_right.LoadFrame(5, "image\\enemy\\idle\\right\\05.bmp"))
		return false;
#pragma endregion

#pragma region enemy_up
	if (!enemy_up_left.InitSprite(5, 36 * 2, 21 * 2, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!enemy_up_left.LoadFrame(0, "image\\enemy\\up\\left\\00.bmp"))
		return false;
	if (!enemy_up_left.LoadFrame(1, "image\\enemy\\up\\left\\01.bmp"))
		return false;
	if (!enemy_up_left.LoadFrame(2, "image\\enemy\\up\\left\\02.bmp"))
		return false;
	if (!enemy_up_left.LoadFrame(3, "image\\enemy\\up\\left\\03.bmp"))
		return false;
	if (!enemy_up_left.LoadFrame(4, "image\\enemy\\up\\left\\04.bmp"))
		return false;

	if (!enemy_up_right.InitSprite(5, 36 * 2, 21 * 2, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!enemy_up_right.LoadFrame(0, "image\\enemy\\up\\right\\00.bmp"))
		return false;
	if (!enemy_up_right.LoadFrame(1, "image\\enemy\\up\\right\\01.bmp"))
		return false;
	if (!enemy_up_right.LoadFrame(2, "image\\enemy\\up\\right\\02.bmp"))
		return false;
	if (!enemy_up_right.LoadFrame(3, "image\\enemy\\up\\right\\03.bmp"))
		return false;
	if (!enemy_up_right.LoadFrame(4, "image\\enemy\\up\\right\\04.bmp"))
		return false;
#pragma endregion

#pragma region enemy_hide
	if (!enemy_hide_left.InitSprite(6, 20 * 2, 21 * 2, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!enemy_hide_left.LoadFrame(0, "image\\enemy\\hide\\left\\00.bmp"))
		return false;
	if (!enemy_hide_left.LoadFrame(1, "image\\enemy\\hide\\left\\01.bmp"))
		return false;
	if (!enemy_hide_left.LoadFrame(2, "image\\enemy\\hide\\left\\02.bmp"))
		return false;
	if (!enemy_hide_left.LoadFrame(3, "image\\enemy\\hide\\left\\03.bmp"))
		return false;
	if (!enemy_hide_left.LoadFrame(4, "image\\enemy\\hide\\left\\04.bmp"))
		return false;
	if (!enemy_hide_left.LoadFrame(5, "image\\enemy\\hide\\left\\05.bmp"))
		return false;

	if (!enemy_hide_right.InitSprite(6, 20 * 2, 21 * 2, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!enemy_hide_right.LoadFrame(0, "image\\enemy\\hide\\right\\00.bmp"))
		return false;
	if (!enemy_hide_right.LoadFrame(1, "image\\enemy\\hide\\right\\01.bmp"))
		return false;
	if (!enemy_hide_right.LoadFrame(2, "image\\enemy\\hide\\right\\02.bmp"))
		return false;
	if (!enemy_hide_right.LoadFrame(3, "image\\enemy\\hide\\right\\03.bmp"))
		return false;
	if (!enemy_hide_right.LoadFrame(4, "image\\enemy\\hide\\right\\04.bmp"))
		return false;
	if (!enemy_hide_right.LoadFrame(5, "image\\enemy\\hide\\right\\05.bmp"))
		return false;
#pragma endregion

#pragma region enemy_attack
	if (!enemy_attack_left.InitSprite(10, 22 * 2, 21 * 2, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!enemy_attack_left.LoadFrame(0, "image\\enemy\\attack\\left\\00.bmp"))
		return false;
	if (!enemy_attack_left.LoadFrame(1, "image\\enemy\\attack\\left\\01.bmp"))
		return false;
	if (!enemy_attack_left.LoadFrame(2, "image\\enemy\\attack\\left\\02.bmp"))
		return false;
	if (!enemy_attack_left.LoadFrame(3, "image\\enemy\\attack\\left\\03.bmp"))
		return false;
	if (!enemy_attack_left.LoadFrame(4, "image\\enemy\\attack\\left\\04.bmp"))
		return false;
	if (!enemy_attack_left.LoadFrame(5, "image\\enemy\\attack\\left\\05.bmp"))
		return false;
	if (!enemy_attack_left.LoadFrame(6, "image\\enemy\\attack\\left\\06.bmp"))
		return false;
	if (!enemy_attack_left.LoadFrame(7, "image\\enemy\\attack\\left\\07.bmp"))
		return false;
	if (!enemy_attack_left.LoadFrame(8, "image\\enemy\\attack\\left\\08.bmp"))
		return false;
	if (!enemy_attack_left.LoadFrame(9, "image\\enemy\\attack\\left\\09.bmp"))
		return false;

	if (!enemy_attack_right.InitSprite(10, 22 * 2, 21 * 2, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!enemy_attack_right.LoadFrame(0, "image\\enemy\\attack\\right\\00.bmp"))
		return false;
	if (!enemy_attack_right.LoadFrame(1, "image\\enemy\\attack\\right\\01.bmp"))
		return false;
	if (!enemy_attack_right.LoadFrame(2, "image\\enemy\\attack\\right\\02.bmp"))
		return false;
	if (!enemy_attack_right.LoadFrame(3, "image\\enemy\\attack\\right\\03.bmp"))
		return false;
	if (!enemy_attack_right.LoadFrame(4, "image\\enemy\\attack\\right\\04.bmp"))
		return false;
	if (!enemy_attack_right.LoadFrame(5, "image\\enemy\\attack\\right\\05.bmp"))
		return false;
	if (!enemy_attack_right.LoadFrame(6, "image\\enemy\\attack\\right\\06.bmp"))
		return false;
	if (!enemy_attack_right.LoadFrame(7, "image\\enemy\\attack\\right\\07.bmp"))
		return false;
	if (!enemy_attack_right.LoadFrame(8, "image\\enemy\\attack\\right\\08.bmp"))
		return false;
	if (!enemy_attack_right.LoadFrame(9, "image\\enemy\\attack\\right\\09.bmp"))
		return false;
#pragma endregion

	float arrowSize = 0.15f;
	arrowSprite.SetSizeAll(arrowSize);

	boss_snowball.SetSize(3);

	return true;
}
