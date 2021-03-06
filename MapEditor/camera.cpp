#include <vector>
#include <thread>
#include <stdio.h>
#include "camera.h"
#include "Vector2.h"
#include "gameEnum.h"

using namespace std;

extern MOD curMod;

extern DIRECTION curPlayerDirection;

extern ACTION curPlayerAction;

extern CPlayer player;

extern Vector2 attackDirection;

extern HDC hdc;
extern TCHAR buffer2[40];

CCamera::CCamera()
{
	_rgb = new unsigned char[SCREEN_HEIGHT * SCREEN_WIDTH * 4];
}
CCamera::~CCamera()
{
}

int CCamera::GetX()
{
	return m_x;
}

int CCamera::GetY()
{
	return m_y;
}

//int CCamera::GetDrawFinishX()
//{
//	return m_nFinishX;
//}
//
//int CCamera::GetDrawFinishY()
//{
//	return m_nFinishY;
//}

int CCamera::GetZoomInRange()
{
	return m_nZoomInRange;
}

void CCamera::SetXY(int x, int y)
{
	m_x = x;
	m_y = y;
}

float CCamera::GetExpansion()
{
	return m_expansion;
}

bool CCamera::GetIsExpansion()
{
	return m_bIsExpansion;
}

bool CCamera::GetIsFirstAlpha()
{
	return IsFirstAlpha;
}

void CCamera::SetIsFirstAlpha(bool alpha)
{
	IsFirstAlpha = alpha;
}

void CCamera::SetAlpha(int alpha)
{
	Alpha = alpha;
}

int CCamera::GetAlpha()
{
	return Alpha;
}

void CCamera::SetExpansion(float _expansion)
{
	m_expansion = _expansion;
}

void CCamera::SetIsExpansion(bool expantion)
{
	m_bIsExpansion = expantion;
}

void CCamera::Expansion()
{
	m_bIsExpansion = true;
}

bool CCamera::IsExpansion()
{
	return m_bIsExpansion;
}

void CCamera::CheckExpansion()
{
	if (m_pTimer->elapsed(m_nLastFrameTime, m_nExpantionInterval))
	{
		if (m_bIsExpansion)
		{
			m_expansion += m_expansionSpeed;
			if (m_expansion > 1.3f)
				m_expansion = 1.3f;
		}
		else
		{
			m_expansion -= 0.08f;
			if (m_expansion < 1)
				m_expansion = 1;
		}
	}
}

void CCamera::Initialize(CTimer* timer)
{
	m_nExpantionInterval = 4;
	m_nMoveInterval = 4;
	m_x = (curMod == MOD::MOD_GAME) ? player.GetPos().x : 1600;
	m_y = (curMod == MOD::MOD_GAME) ? player.GetPos().y : 1600;
	m_nSpeedX = 0;
	m_nSpeedY = 0;
	m_nFinishX = 0;
	m_nFinishY = 0;
	m_nZoomInRange = 600;
	m_nMoveSpeed = 0.5f;
	m_expansion = 1;
	m_expansionSpeed = 0.006f;
	m_bIsExpansion = false;
	m_bIsSkill = false;
	m_pTimer = timer;
	Alpha = 0;
	IsFirstAlpha = true;
	m_nLastFrameTime = m_pTimer->time();
}

void CCamera::InitExpantion()
{
	m_nSpeedX = 0;
	m_nSpeedY = 0;
	if (m_bIsSkill)
		m_expansionSpeed = 0.03f;
	else
		m_expansionSpeed = 0.006f;
	//m_bIsExpansion = false;
}

void CCamera::SetIsSkill(bool skill)
{
	m_bIsSkill = skill;
}

void CCamera::Left()
{
	m_x -= 5;
	if (m_x < SCREEN_WIDTH >> 1)
		m_x = SCREEN_WIDTH >> 1;
}

void CCamera::Right()
{
	m_x += 5;
	if (m_x > WORLDMAP_WIDTH - (SCREEN_WIDTH >> 1))
		m_x = WORLDMAP_WIDTH - (SCREEN_WIDTH >> 1);
}

void CCamera::Up()
{
	m_y -= 5;
	if (m_y < SCREEN_HEIGHT >> 1)
		m_y = SCREEN_HEIGHT >> 1;
}

void CCamera::Down()
{
	m_y += 5;
	if (m_y > WORLDMAP_HEIGHT - (SCREEN_HEIGHT >> 1))
		m_y = WORLDMAP_HEIGHT - (SCREEN_HEIGHT >> 1);
}

void CCamera::Left2()
{
	m_nSpeedX -= 1;
}

void CCamera::Right2()
{
	m_nSpeedX += 1;
}

void CCamera::Up2()
{
	m_nSpeedY -= 1;
}

void CCamera::Down2()
{
	m_nSpeedY += 1;
}

void CCamera::Move()
{
	//m_x += (m_nFinishX - m_x)* !((int)curPlayerActionState == 4) ? 0.5f : 0.1f;
	//m_y += (m_nFinishY - m_y)* !((int)curPlayerActionState == 4) ? 0.5f : 0.1f;
	if ((int)curPlayerAction == ACTION::ATTACK)
		m_nMoveSpeed = 0.1f;
	else
		m_nMoveSpeed = 0.5f;
	if (curMod == MOD::MOD_EDITOR)
		m_nMoveSpeed = 1;
	m_x += (m_nFinishX - m_x) * m_nMoveSpeed;
	m_y += (m_nFinishY - m_y) * m_nMoveSpeed;
	if (m_x < SCREEN_WIDTH >> 1)
		m_x = SCREEN_WIDTH >> 1;
	else if (m_x > WORLDMAP_WIDTH - (SCREEN_WIDTH >> 1))
		m_x = WORLDMAP_WIDTH - (SCREEN_WIDTH >> 1);
	if (m_y < SCREEN_HEIGHT >> 1)
		m_y = SCREEN_HEIGHT >> 1;
	else if (m_y > WORLDMAP_HEIGHT - (SCREEN_HEIGHT >> 1))
		m_y = WORLDMAP_HEIGHT - (SCREEN_HEIGHT >> 1);
}

void CCamera::CheckFinishXY()
{
	m_nFinishX = GetFinishX2();
	m_nFinishY = GetFinishY2();
}

void CCamera::SetFinishXY(int x, int y)
{
	m_nFinishX = x;
	m_nFinishY = y;
}

float CCamera::GetExpansionSpeed()
{
	return m_expansionSpeed;
}

int CCamera::GetFinishX()
{
	int _x;

	int a = rand() % 4;
	switch (a)
	{
	case 0:
		a = 20;
		break;
	case 1:
		a = -20;
		break;
	case 2:
		a = 40;
		break;
	case 3:
		a = -40;
		break;
	}

	_x = player.GetPos().x + a;

	return _x;
}

int CCamera::GetFinishY()
{
	int _y;
	int a = rand() % 4;
	switch (a)
	{
	case 0:
		a = 20;
		break;
	case 1:
		a = -20;
		break;
	case 2:
		a = 40;
		break;
	case 3:
		a = -40;
		break;
	}
	_y = player.GetPos().y + a;
	return _y;
}

int CCamera::GetFinishX2()
{
	int _x = 0;
	if ((curPlayerAction != ACTION::ATTACK) && (curPlayerAction != ACTION::ROLL))		//???????????? ?????????
	{
		_x = (int)player.GetPos().x;
	}
	else
	{
		_x = (int)player.GetPos().x + (attackDirection.x * m_nZoomInRange * (m_expansion - 1) * 1.5f);
	}
	return _x;
}


int CCamera::GetFinishY2()
{
	int _y;
	if ((curPlayerAction != ACTION::ATTACK) && (curPlayerAction != ACTION::ROLL))		//???????????? ?????????
	{
		_y = (int)player.GetPos().y;
	}
	else
	{
		_y = (int)player.GetPos().y + (attackDirection.y * m_nZoomInRange * (m_expansion - 1) * 1.5f);
	}
	return _y;
}

void CCamera::AlphaBlending(LPDIRECTDRAWSURFACE7 lpSurface)		//???????????? ???????????? ???????????? ??????
{
	unsigned char* rgb = new unsigned char[SCREEN_HEIGHT * SCREEN_WIDTH * 4];
	DDSURFACEDESC2 ddsd;					//????????? ????????? ???????????? ?????? ????????? ??????
	ZeroMemory(&ddsd, sizeof(ddsd));		//ddsd???????????? ddsd???????????? ?????????????????? 0?????? ?????????
	ddsd.dwSize = sizeof(ddsd);				//dwSize??? ddsd???????????? ????????? ?????????

	FAILED(lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL));				//?????? ????????? ??????????????? ??????????????? false??? ???????????????

	//unsigned char *pDest, *pSrc;			//?????????????????? ????????? ?????? ??????, ????????? ????????? ????????? ?????? ??????
	//unsigned char *aaa = new unsigned char[SCREEN_HEIGHT*SCREEN_WIDTH * 4];
	//CopyMemory(aaa, (unsigned char*)ddsd.lpSurface, SCREEN_HEIGHT*SCREEN_WIDTH * 4);
	unsigned char* surface = (unsigned char*)ddsd.lpSurface;
	CopyMemory(rgb, surface, SCREEN_HEIGHT * SCREEN_WIDTH * 4);
	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		for (int j = 0; j < SCREEN_WIDTH; j++)
		{
			/*CopyMemory(&bgRed, aaa + i*SCREEN_WIDTH * 4 + j * 4+2, 1);
			CopyMemory(&bgGreen, aaa + i*SCREEN_WIDTH * 4 + j * 4+1, 1);
			CopyMemory(&bgBlue, aaa + i*SCREEN_WIDTH * 4 + j * 4, 1);*/
			//CopyMemory(&rgb, aaa + i*SCREEN_WIDTH * 4 + j * 4, 3);
			/*bgRed = (bgRed * Alpha) >> 8;
			bgGreen = (bgGreen * Alpha) >> 8;
			bgBlue = (bgBlue * Alpha) >> 8;*/
			rgb[(i * SCREEN_WIDTH * 4) + (j * 4)] = (rgb[(i * SCREEN_WIDTH * 4) + (j * 4)] * Alpha) >> 8;
			rgb[(i * SCREEN_WIDTH * 4) + (j * 4) + 1] = (rgb[(i * SCREEN_WIDTH * 4) + (j * 4) + 1] * Alpha) >> 8;
			rgb[(i * SCREEN_WIDTH * 4) + (j * 4) + 2] = (rgb[(i * SCREEN_WIDTH * 4) + (j * 4) + 2] * Alpha) >> 8;
			/*CopyMemory(aaa + i*SCREEN_WIDTH * 4 + j * 4 + 2, &bgRed, 1);
			CopyMemory(aaa + i*SCREEN_WIDTH * 4 + j * 4 + 1, &bgGreen, 1);
			CopyMemory(aaa + i*SCREEN_WIDTH * 4 + j * 4, &bgBlue, 1);*/
			//CopyMemory(aaa + i*SCREEN_WIDTH * 4 + j * 4, &rgb, 3);
		}
		//CopyMemory(aaa + i*SCREEN_WIDTH * 4, &rgb, SCREEN_WIDTH * 4);
	}
	CopyMemory(surface, rgb, SCREEN_HEIGHT * SCREEN_WIDTH * 4);
	//ZeroMemory(surface, sizeof(surface));

	/*pDest = (unsigned char*)ddsd.lpSurface;
	pSrc = aaa;*/						//????????? ????????? ?????????

	//CopyMemory(aaaa, surface, sizeof(surface));

	//CopyMemory(pDest, pSrc, SCREEN_HEIGHT*SCREEN_WIDTH * 4);
	//delete[] rgb;

	lpSurface->Unlock(NULL);

	if (!IsFirstAlpha)
	{
		Alpha -= 1;
		if (Alpha <= 0)
		{
			Alpha = 255;
			if ((int)curMod == 1)
				curMod = (MOD)0;
			else if ((int)curMod == 0)
				curMod = (MOD)1;
		}
	}
	else
	{
		Alpha += 1;
		if (Alpha >= 255)
		{
			Alpha = 255;
			IsFirstAlpha = false;
		}
	}
}

struct thread_data
{
	unsigned char* rgb;
	int start;
	int alpha;
	int threadCnt;
};

void worker(thread_data* data)
{
	int i, j, index;
	for (i = 0; i < SCREEN_HEIGHT / data->threadCnt; i++)
	{
		for (j = 0; j < SCREEN_WIDTH; j++)
		{
			index = (i * SCREEN_WIDTH * 4) + (j * 4) + data->start;
			*(data->rgb + index) = (*(data->rgb + index) * data->alpha) >> 8;
			*(data->rgb + index + 1) = (*(data->rgb + index + 1) * data->alpha) >> 8;
			*(data->rgb + index + 2) = (*(data->rgb + index + 2) * data->alpha) >> 8;


			//CopyMemory(surface + SCREEN_HEIGHT * SCREEN_WIDTH * 4 + 1, abc, 5 * SCREEN_WIDTH * 4 + 839 * 4);
		}
	}
}

void CCamera::AlphaBlending2(LPDIRECTDRAWSURFACE7 lpSurface)
{
	DDSURFACEDESC2 ddsd;					//????????? ????????? ???????????? ?????? ????????? ??????
	ZeroMemory(&ddsd, sizeof(ddsd));		//ddsd???????????? ddsd???????????? ?????????????????? 0?????? ?????????
	ddsd.dwSize = sizeof(ddsd);				//dwSize??? ddsd???????????? ????????? ?????????

	lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);				//?????? ????????? ??????????????? ??????????????? false??? ???????????????

	//unsigned char *pDest, *pSrc;			//?????????????????? ????????? ?????? ??????, ????????? ????????? ????????? ?????? ??????
	//unsigned char *aaa = new unsigned char[SCREEN_HEIGHT*SCREEN_WIDTH * 4];
	//CopyMemory(aaa, (unsigned char*)ddsd.lpSurface, SCREEN_HEIGHT*SCREEN_WIDTH * 4);
	unsigned char* surface = (unsigned char*)ddsd.lpSurface;
	//CopyMemory(_rgb, surface, SCREEN_HEIGHT * SCREEN_WIDTH * 4);

	for (int i = 0; i < ddsd.dwHeight; i++)
	{
		CopyMemory(_rgb + i * SCREEN_WIDTH * 4, surface + i * ddsd.lPitch, SCREEN_WIDTH * 4);
	}

	//const int num_pro = std::thread::hardware_concurrency();  //cpu ????????? ?????? ?????? ??????
	//vector<std::thread> workers;
	//workers.resize(num_pro);

	const int threadCnt = 2;

	thread t[threadCnt];

	for (int b = 0; b < threadCnt; b++)
	{
		thread_data* data;
		data = (thread_data*)malloc(sizeof(thread_data));
		ZeroMemory(data, sizeof(thread_data));
		data->rgb = _rgb;
		data->start = SCREEN_HEIGHT * SCREEN_WIDTH * b * 4 / threadCnt;
		data->alpha = Alpha;
		data->threadCnt = threadCnt;
		t[b] = thread(worker, data);
	}
	for (int c = 0; c < threadCnt; c++)
	{
		t[c].join();
	}
	//unsigned char* abc = (unsigned char*)malloc(5 * SCREEN_WIDTH * 4 + 839 * 4);
	//ZeroMemory(abc, 5 * SCREEN_WIDTH * 4 + 839 * 4);

	//for (int i = 0; i < 6; i++)
	//{
	//	CopyMemory(surface + SCREEN_HEIGHT * SCREEN_WIDTH * 4 + i*SCREEN_WIDTH * 4, abc, SCREEN_WIDTH * 4);
	//}
	//for (int i = 0; i < 1; i++)
	//{
	//}

	//CopyMemory(surface, _rgb, (SCREEN_HEIGHT * SCREEN_WIDTH) << 2);

	for (int i = 0; i < ddsd.dwHeight; i++)
	{
		CopyMemory(surface + i * ddsd.lPitch, _rgb + i * SCREEN_WIDTH * 4, SCREEN_WIDTH * 4);
	}


	//memset(surface + SCREEN_HEIGHT * SCREEN_WIDTH * 4, 0, 5 * SCREEN_WIDTH * 4 + 839 * 4);

	//CopyMemory(abc, surface + SCREEN_HEIGHT * SCREEN_WIDTH * 4, 40);
	//sprintf_s(buffer2, TEXT("%d,, %d"), ddsd.dwWidth, ddsd.dwHeight);
	//TextOut(hdc, 0, 0, buffer2, 40);

	//delete[] _rgb;

	lpSurface->Unlock(NULL);

	if (!IsFirstAlpha)
	{
		Alpha -= 5;
		if (Alpha <= 0)
		{
			Alpha = 255;
			if ((int)curMod == 1)
				curMod = (MOD)0;
			else if ((int)curMod == 0)
				curMod = (MOD)1;
		}
	}
	else
	{
		Alpha += 5;
		if (Alpha >= 255)
		{
			Alpha = 255;
			IsFirstAlpha = false;
		}
	}
}