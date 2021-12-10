#include <Windows.h>
#include <stdio.h>
#include <ddraw.h>
#include <dinput.h>

#include "bmp.h"
#include "timer.h"
#include "sprite.h"
#include "define.h"
#include "block.h"
#include "myplayer.h"
#include "camera.h"
#include "worldmapBmp.h"
#include "mapeditor.h"
#include "gameEnum.h"


extern BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);
extern LPDIRECTDRAWSURFACE7		g_lpPrimarySurface;
extern LPDIRECTDRAWSURFACE7		g_lpSecondarySurface;
extern LPDIRECTDRAW7			g_lpDirectDrawObject;
extern LPDIRECTINPUT			g_lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8		g_lpDirectInputKeyboard;
extern CTimer					g_Timer;

extern CSprite					grassSprite;
extern CBLOCK					Block[BLOCK_Y][BLOCK_X];
extern CPlayer					player;
extern CCamera					camera;
extern CWorldMap				bossMap;
extern CWorldMap				bossMapRoof;
extern CWorldMap				baseMap;
extern CWorldMap*				curEditMap;
extern MAPEDITOR				g_editor;

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
			Block[y][x].Draw(camera.GetX(), camera.GetY(), g_lpSecondarySurface);
		}
	}
	g_editor.GetChoiceWindow()->Drawing(g_lpSecondarySurface);

	HRESULT hResult;
	if (FAILED(hResult = g_lpPrimarySurface->Flip(NULL, DDFLIP_WAIT)))	//더블버퍼링 화면전환
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