#include <Windows.h>
#include <stdio.h>
#include <ddraw.h>
#include <dinput.h>
#include <dsound.h>

#include "Bmp.h"
#include "Timer.h"
#include "Sprite.h"
#include "SettingData.h"
#include "Block.h"
#include "Player.h"
#include "Camera.h"
#include "WorldMapBmp.h"
#include "IntroSprite.h"
#include "DSUTIL.h"
#include "GameEnum.h"


extern BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);
extern LPDIRECTDRAWSURFACE7 g_lpPrimarySurface;
extern LPDIRECTDRAWSURFACE7 g_lpSecondarySurface;
extern LPDIRECTDRAW7 g_lpDirectDrawObject;
extern LPDIRECTINPUT8 g_lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 g_lpDirectInputKeyboard;
extern Timer g_Timer;


extern Camera camera;
extern HDC hdc;
extern TCHAR buffer3[40];

extern WorldMap bossMap;
extern IntroSprite introSprite;
extern IntroSprite introButton;
extern HSNDOBJ Sound[10];

void InitIntro();

extern MOD curMod;

enum SELECT { START = 0, OPTION, QUIT };

SELECT curSelect;
int countSelect;
int SelectMod;
bool isStart;

extern BOOL m_bIntroFirst;
extern BOOL m_bGameFirst;
extern BOOL m_bEditorFirst;

void IntroMain(void)
{
	if (m_bIntroFirst)
	{
		InitIntro();
	}
	introSprite.Drawing(0, 0, 0, g_lpSecondarySurface, false);
	introButton.KeyInitDown();
	if (!isStart)
	{
		if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_W))
		{
			introButton.KeyDown();
			if (introButton.KeyCheckClick())
			{
				countSelect -= 2;
				if (countSelect < 0)
					countSelect = 0;
				switch (countSelect)
				{
				case 0:
					curSelect = START;
					break;
				case 1:
					curSelect = OPTION;
					break;
				case 2:
					curSelect = QUIT;
					break;
				}
			}
		}
		else if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_S))
		{
			introButton.KeyDown();
			if (introButton.KeyCheckClick())
			{
				countSelect += 2;
				if (countSelect > 2)
					countSelect = 2;
				switch (countSelect)
				{
				case 0:
					curSelect = START;
					break;
				case 1:
					curSelect = OPTION;
					break;
				case 2:
					curSelect = QUIT;
					break;
				}
			}
		}
		else if (DirectInputKeyboardDown(g_lpDirectInputKeyboard, DIK_SPACE))
		{
			switch (curSelect)
			{
			case START:
				isStart = true;
				break;
			case OPTION:
				SelectMod = 2;
				curMod = MOD::MOD_EDITOR;
				break;
			case QUIT:
				SelectMod = 3;
				curMod = MOD::MOD_QUIT;
				break;
			}
		}
	}
	introButton.KeyCheckUp();

	switch (curSelect)
	{
	case START:
		introButton.Drawing2(0, 605, 392, g_lpSecondarySurface, true);
		break;
	case OPTION:
		introButton.Drawing2(1, 605, 417, g_lpSecondarySurface, true);
		break;
	case QUIT:
		introButton.Drawing2(2, 605, 439, g_lpSecondarySurface, true);
		break;
	}

	if (isStart)
	{
		camera.AlphaBlending2(g_lpSecondarySurface);
		SetVolume(Sound[1]->Buffers[0], (255 - camera.GetAlpha()) * -20);
		if (camera.GetAlpha() <= 10)
		{
			SndObjStop(Sound[1]);
		}
	}

	HRESULT hResult;
	if (FAILED(hResult = g_lpPrimarySurface->Flip(NULL, DDFLIP_WAIT))) //더블버퍼링 화면전환
	{
		if (hResult == DDERR_SURFACELOST)
			g_lpPrimarySurface->Restore();
	}
}

void InitIntro()
{
	SndObjStop(Sound[0]);
	SndObjStop(Sound[1]);
	SndObjStop(Sound[4]);
	SndObjPlay(Sound[1], DSBPLAY_LOOPING);
	camera.Initialize(&g_Timer);
	camera.SetIsFirstAlpha(false);
	camera.SetAlpha(255);
	curSelect = START;
	SelectMod = 1;
	countSelect = 0;
	m_bIntroFirst = FALSE;
	m_bGameFirst = TRUE;
	m_bEditorFirst = TRUE;
	isStart = false;
}
