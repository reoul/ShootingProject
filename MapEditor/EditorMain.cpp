#include <Windows.h>
#include <stdio.h>
#include <ddraw.h>
#include <dinput.h>

#include "Bmp.h"
#include "Timer.h"
#include "Sprite.h"
#include "SettingData.h"
#include "Block.h"
#include "Player.h"
#include "Camera.h"
#include "WorldMapBmp.h"
#include "MapEditor.h"
#include "GameEnum.h"


extern BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);
extern LPDIRECTDRAWSURFACE7 g_lpPrimarySurface;
extern LPDIRECTDRAWSURFACE7 g_lpSecondarySurface;
extern LPDIRECTDRAW7 g_lpDirectDrawObject;
extern LPDIRECTINPUT g_lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 g_lpDirectInputKeyboard;
extern Timer g_Timer;

extern Sprite grassSprite;
extern Block g_blocks[BLOCK_Y][BLOCK_X];
extern Player player;
extern Camera camera;
extern WorldMap bossMap;
extern WorldMap bossMapRoof;
extern WorldMap baseMap;
extern WorldMap* curEditMap;
extern MAPEDITOR g_editor;

extern EDIT_STATE curEditState;

void InitEditor();
extern int cameraPositionX;
extern int cameraPositionY;

extern BOOL m_bIntroFirst;
extern BOOL m_bGameFirst;
extern BOOL m_bEditorFirst;

void EditorMain(void)
{
	if (m_bEditorFirst)
		InitEditor();
	int i, j;

	camera.InitExpantion();

	//화면이동
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_A))
	{
		camera.Left();
	}
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_D))
	{
		camera.Right();
	}
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_W))
	{
		camera.Up();
	}
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_S))
	{
		camera.Down();
	}
	if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_P))
	{
		curEditMap->InitMapData();
	}

	curEditMap->Drawing2(camera.GetX(), camera.GetY(), g_lpSecondarySurface, false, g_lpDirectDrawObject);
	for (int y = 0; y < BLOCK_Y; y++)
	{
		for (int x = 0; x < BLOCK_X; x++)
		{
			g_blocks[y][x].Draw(camera.GetX(), camera.GetY(), g_lpSecondarySurface);
		}
	}
	g_editor.GetChoiceWindow()->Drawing(g_lpSecondarySurface);

	HRESULT hResult;
	if (FAILED(hResult = g_lpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) //더블버퍼링 화면전환
	{
		if (hResult == DDERR_SURFACELOST)
			g_lpPrimarySurface->Restore();
	}
}

void InitEditor()
{
	camera.Initialize(&g_Timer);
	g_editor.Initialize();
	m_bIntroFirst = TRUE;
	m_bGameFirst = TRUE;
	m_bEditorFirst = FALSE;
	//curEditMap = &baseMap;
	curEditState = EDIT_STATE::EDIT_BOSS;
}
