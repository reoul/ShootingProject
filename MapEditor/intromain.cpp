#include <Windows.h>
#include <stdio.h>
#include <ddraw.h>
#include <dinput.h>
#include <dsound.h>

#include "bmp.h"
#include "timer.h"
#include "sprite.h"
#include "define.h"
#include "block.h"
#include "myplayer.h"
#include "camera.h"
#include "worldmapBmp.h"
#include "introsprite.h"
#include "DSUTIL.h"
#include "gameEnum.h"


extern BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);
extern LPDIRECTDRAWSURFACE7		g_lpPrimarySurface;
extern LPDIRECTDRAWSURFACE7		g_lpSecondarySurface;
extern LPDIRECTDRAW7			g_lpDirectDrawObject;
extern LPDIRECTINPUT			g_lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8		g_lpDirectInputKeyboard;
extern CTimer					g_Timer;


extern CCamera					camera;
extern HDC hdc;
extern TCHAR buffer3[40];

extern CWorldMap				bossMap;
extern CIntroSprite				introSprite;
extern CIntroSprite				introButton;
extern HSNDOBJ					Sound[10];

void InitIntro();

extern MOD curMod;

enum SELECT {START = 0, OPTION, QUIT};
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
				countSelect -= 1;
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
				countSelect += 1;
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
		introButton.Drawing2(0, 601, 392, g_lpSecondarySurface, true);		//1280 = (567,368)     1152 = (512,440)
		break;
	case OPTION:
		introButton.Drawing2(1, 601, 417, g_lpSecondarySurface, true);		//1280 = (567,390)     1152 = (512,467)
		break;
	case QUIT:
		introButton.Drawing2(2, 601, 439, g_lpSecondarySurface, true);		//1280 = (567,411)     1152 = (512,492)
		break;
	}

	if (isStart)
	{
		camera.AlphaBlending2(g_lpSecondarySurface);
		SetVolume(Sound[1]->Buffers[0], (255 - camera.GetAlpha())*-20);
		if (camera.GetAlpha() <= 10)
		{
			SndObjStop(Sound[1]);
		}
	}

	HRESULT hResult;
	if (FAILED(hResult = g_lpPrimarySurface->Flip(NULL, DDFLIP_WAIT)))	//더블버퍼링 화면전환
	{
		if (hResult == DDERR_SURFACELOST)
			g_lpPrimarySurface->Restore();
	}

	//sprintf_s(buffer3, TEXT("%d"), camera.GetAlpha());
	//TextOut(hdc, 0, 0, buffer3, 40);
}

void InitIntro()
{
	SndObjStop(Sound[0]);
	SndObjStop(Sound[1]);
	SndObjStop(Sound[4]);
	//SndObjPlay(Sound[1], DSBPLAY_LOOPING);
	//SetVolume(Sound[1]->Buffers[0], SoundSize);
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