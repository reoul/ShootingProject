#include "Camera.h"
#include <vector>
#include <thread>

#include "Vector2.h"
#include "GameEnum.h"
#include "SettingData.h"

using namespace std;

extern MOD curMod;

extern DIRECTION curPlayerDirection;

extern ACTION curPlayerAction;

extern Player player;

extern Vector2 attackDirection;

extern HDC hdc;
extern TCHAR buffer2[40];

Camera::Camera()
{
	_rgb = new unsigned char[SCREEN_HEIGHT * SCREEN_WIDTH * 4];
}

Camera::~Camera()
{
}

int Camera::GetX()
{
	return m_x;
}

int Camera::GetY()
{
	return m_y;
}

//int Camera::GetDrawFinishX()
//{
//	return m_nFinishX;
//}
//
//int Camera::GetDrawFinishY()
//{
//	return m_nFinishY;
//}

int Camera::GetZoomInRange()
{
	return m_nZoomInRange;
}

void Camera::SetXY(int x, int y)
{
	m_x = x;
	m_y = y;
}

float Camera::GetExpansion()
{
	return m_expansion;
}

bool Camera::GetIsExpansion()
{
	return m_bIsExpansion;
}

bool Camera::GetIsFirstAlpha()
{
	return IsFirstAlpha;
}

void Camera::SetIsFirstAlpha(bool alpha)
{
	IsFirstAlpha = alpha;
}

void Camera::SetAlpha(int alpha)
{
	Alpha = alpha;
}

int Camera::GetAlpha()
{
	return Alpha;
}

void Camera::SetExpansion(float _expansion)
{
	m_expansion = _expansion;
}

void Camera::SetIsExpansion(bool expantion)
{
	m_bIsExpansion = expantion;
}

void Camera::Expansion()
{
	m_bIsExpansion = true;
}

bool Camera::IsExpansion()
{
	return m_bIsExpansion;
}

void Camera::CheckExpansion()
{
	if (Timer::Elapsed(m_nLastFrameTime, m_nExpantionInterval))
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

void Camera::Initialize()
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
	Alpha = 0;
	IsFirstAlpha = true;
	m_nLastFrameTime = Timer::Now();
}

void Camera::InitExpantion()
{
	m_nSpeedX = 0;
	m_nSpeedY = 0;
	if (m_bIsSkill)
		m_expansionSpeed = 0.03f;
	else
		m_expansionSpeed = 0.006f;
	//m_bIsExpansion = false;
}

void Camera::SetIsSkill(bool skill)
{
	m_bIsSkill = skill;
}

void Camera::Left()
{
	m_x -= 5;
	if (m_x < SCREEN_WIDTH >> 1)
		m_x = SCREEN_WIDTH >> 1;
}

void Camera::Right()
{
	m_x += 5;
	if (m_x > WORLDMAP_WIDTH - (SCREEN_WIDTH >> 1))
		m_x = WORLDMAP_WIDTH - (SCREEN_WIDTH >> 1);
}

void Camera::Up()
{
	m_y -= 5;
	if (m_y < SCREEN_HEIGHT >> 1)
		m_y = SCREEN_HEIGHT >> 1;
}

void Camera::Down()
{
	m_y += 5;
	if (m_y > WORLDMAP_HEIGHT - (SCREEN_HEIGHT >> 1))
		m_y = WORLDMAP_HEIGHT - (SCREEN_HEIGHT >> 1);
}

void Camera::Left2()
{
	m_nSpeedX -= 1;
}

void Camera::Right2()
{
	m_nSpeedX += 1;
}

void Camera::Up2()
{
	m_nSpeedY -= 1;
}

void Camera::Down2()
{
	m_nSpeedY += 1;
}

void Camera::Move()
{
	//mX += (m_nFinishX - mX)* !((int)curPlayerActionState == 4) ? 0.5f : 0.1f;
	//mY += (m_nFinishY - mY)* !((int)curPlayerActionState == 4) ? 0.5f : 0.1f;
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

void Camera::CheckFinishXY()
{
	m_nFinishX = GetFinishX2();
	m_nFinishY = GetFinishY2();
}

void Camera::SetFinishXY(int x, int y)
{
	m_nFinishX = x;
	m_nFinishY = y;
}

float Camera::GetExpansionSpeed()
{
	return m_expansionSpeed;
}

int Camera::GetFinishX()
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

int Camera::GetFinishY()
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

int Camera::GetFinishX2()
{
	int _x = 0;
	if ((curPlayerAction != ACTION::ATTACK) && (curPlayerAction != ACTION::ROLL)) //공격중이 아닐때
	{
		_x = (int)player.GetPos().x;
	}
	else
	{
		_x = (int)player.GetPos().x + (attackDirection.x * m_nZoomInRange * (m_expansion - 1) * 1.5f);
	}
	return _x;
}


int Camera::GetFinishY2()
{
	int _y;
	if ((curPlayerAction != ACTION::ATTACK) && (curPlayerAction != ACTION::ROLL)) //공격중이 아닐때
	{
		_y = (int)player.GetPos().y;
	}
	else
	{
		_y = (int)player.GetPos().y + (attackDirection.y * m_nZoomInRange * (m_expansion - 1) * 1.5f);
	}
	return _y;
}

void Camera::AlphaBlending(LPDIRECTDRAWSURFACE7 lpSurface) //버퍼에서 표면으로 복사하는 함수
{
	unsigned char* rgb = new unsigned char[SCREEN_HEIGHT * SCREEN_WIDTH * 4];
	DDSURFACEDESC2 ddsd; //표면의 정보를 확인할수 있는 변수를 선언
	ZeroMemory(&ddsd, sizeof(ddsd)); //ddsd주소에서 ddsd크기만큼 메모리영역을 0으로 채워줌
	ddsd.dwSize = sizeof(ddsd); //dwSize를 ddsd크기만큼 초기화 해준다

	FAILED(lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)); //만약 표면을 잠금하는데 실패할경우 false를 반환해준다

	//unsigned char *pDest, *pSrc;			//표면의주소를 담을수 있는 변수, 버퍼의 주소를 담을수 있는 변수
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
	pSrc = aaa;*/ //버퍼의 주소를 불러옴

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

void Camera::AlphaBlending2(LPDIRECTDRAWSURFACE7 lpSurface)
{
	DDSURFACEDESC2 ddsd; //표면의 정보를 확인할수 있는 변수를 선언
	ZeroMemory(&ddsd, sizeof(ddsd)); //ddsd주소에서 ddsd크기만큼 메모리영역을 0으로 채워줌
	ddsd.dwSize = sizeof(ddsd); //dwSize를 ddsd크기만큼 초기화 해준다

	lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL); //만약 표면을 잠금하는데 실패할경우 false를 반환해준다

	//unsigned char *pDest, *pSrc;			//표면의주소를 담을수 있는 변수, 버퍼의 주소를 담을수 있는 변수
	//unsigned char *aaa = new unsigned char[SCREEN_HEIGHT*SCREEN_WIDTH * 4];
	//CopyMemory(aaa, (unsigned char*)ddsd.lpSurface, SCREEN_HEIGHT*SCREEN_WIDTH * 4);
	unsigned char* surface = (unsigned char*)ddsd.lpSurface;
	//CopyMemory(_rgb, surface, SCREEN_HEIGHT * SCREEN_WIDTH * 4);

	for (int i = 0; i < ddsd.dwHeight; i++)
	{
		CopyMemory(_rgb + i * SCREEN_WIDTH * 4, surface + i * ddsd.lPitch, SCREEN_WIDTH * 4);
	}

	//const int num_pro = std::thread::hardware_concurrency();  //cpu 쓰레드 갯수 파악 함수
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
