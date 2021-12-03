#include "worldmapBmp.h"
#include "camera.h"
#include "mapeditor.h"
#include "gameEnum.h"

extern CCamera camera;
extern MAPEDITOR g_editor;
extern EDIT_STATE curEditState;

extern char *wallBlockData[BLOCK_X*BLOCK_Y * 2];
extern char *wallBlockData2[BLOCK_X*BLOCK_Y * 2];

extern MOD curMod;			//���� ���

CWorldMap::CWorldMap()
{
	//m_pBuffer = new unsigned char[1600 * 1600 * 3];
	m_pSurface = new LPDIRECTDRAWSURFACE7[1];
}

CWorldMap::~CWorldMap()
{
	free(m_pBuffer);
	free(m_pBuffer2);
}


bool CWorldMap::LoadBMPFile(char *filename)		//��Ʈ���� ������ �ҷ��� ���ۿ� ���������� ����ְ� 24bit�̹����� 32bit�̹����� �ٲ���
{
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	BITMAPFILEHEADER bmpfilehead;		//��Ʈ������ ��ü�� ������ ������ ���� ����
	if (!ReadFile(hfile, &bmpfilehead, sizeof(bmpfilehead), &actualRead, NULL))		//��Ʈ������ ��ü�� ������ ������ ���ٸ�
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}
	if (bmpfilehead.bfType != 0x4D42)		//��Ʈ�������� �ƴҰ��
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}

	BITMAPINFOHEADER bmpinfohead;		//��Ʈ�ʿ��� ��ü�� ������ ������ ���� ����
	if (!ReadFile(hfile, &bmpinfohead, sizeof(bmpinfohead), &actualRead, NULL))		//��Ʈ�ʿ��� ��ü�� ������ ������ ���ٸ�
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}

	if (bmpinfohead.biBitCount != 24)		//�ȼ��� ��Ʈ���� 24���� �ƴϸ� 
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}

	int nHeight = bmpinfohead.biHeight;		//��Ʈ���� ���̸� �ҷ��� nHeight�� �ʱ�ȭ
	bool bBottomUp;			//�̹����� ���������ִ��� Ȯ���ϴ� bool�� ����, true�� �̹����� ������������

	if (nHeight > 0)		//��Ʈ���� ���̰� 0���� ũ��
	{
		bBottomUp = true;	//�̹����� ������������
	}
	else					//��Ʈ���� ���̰� 0���� ũ���������
	{
		bBottomUp = false;		//�̹����� ���������� ����
		nHeight = -nHeight;		//���̸� ����� �ٲ���
	}

	m_nHeight = nHeight;
	m_nWidth = bmpinfohead.biWidth;

	if (m_pBuffer)				//���۰� �ִٸ�
		delete[] m_pBuffer;		//���۸� �������ش�

	if (m_pBuffer2)
		delete[] m_pBuffer2;

	m_pBuffer = new unsigned char[m_nHeight * m_nWidth * 4];		//���ο� ���۸� 32��Ʈ�� ǥ�����ֱ����ؼ� ���ȼ�����*4�� ���ش�
	m_pBuffer2 = new unsigned char[m_nHeight * m_nWidth * 3];

	struct RBGstruct			//��Ʈ���� RGB�� ������ �ƴ� BGR������ �����
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	struct RBGstruct *rgb24 = new RBGstruct[m_nWidth];		//�ȼ����� ���� ���� ���� ����

	int nDestY, nDeltaY;		//���� ����,�̹����� ������ ���¿����� ������������ �ٲܶ� ���̿� �����ִ°�

	//if (bBottomUp)				//�̹����� ��������������
	//{
	//	nDestY = nHeight - 1;
	//	nDeltaY = -1;
	//}
	//else
	//{
	//	nDestY = 0;
	//	nDeltaY = 1;
	//}

	nDestY = 0;
	nDeltaY = 1;

	for (int y = 0; y < m_nHeight; y++)				//�̹����� �����ȼ�����ŭ
	{
		if (!ReadFile(hfile, rgb24, 3*m_nWidth, &actualRead, NULL))		//���Ͽ� 3����Ʈ ũ�⸸ŭ �о�ͼ� rgb24�� ���������
		{														//������ �� �о�Դٸ�
			CloseHandle(hfile);									//�ڵ��� �ݳ����ְ�
			delete[] m_pBuffer;									//���۸� �������ְ�
			return false;										//false�� ��ȯ���ش�
		}
		CopyMemory(m_pBuffer2+ nDestY*(m_nWidth*3), rgb24, 3*m_nWidth);
		for (int x = 0; x < m_nWidth; x++)			//�̹����� �����ȼ�����ŭ
		{
			//CopyMemory(m_pBuffer + (x << 2) + nDestY*(m_nWidth << 2), &rgb24[x], 3);
			CopyMemory(m_pBuffer + (x << 2) + nDestY*(m_nWidth << 2), rgb24+x, 3);
			*(m_pBuffer + (x << 2) + nDestY*(m_nWidth << 2) + 3) = 0;
		}

		nDestY += nDeltaY;
	}

	delete[] rgb24;
	rgb24 = NULL;

	CloseHandle(hfile);

	return true;
}

unsigned char* CWorldMap::GetBMPBuffer()
{
	return m_pBuffer;
}

unsigned char* CWorldMap::GetBMPBuffer2()
{
	return m_pBuffer2;
}

LPDIRECTDRAWSURFACE7* CWorldMap::GetSurface()
{
	return m_pSurface;
}

bool CWorldMap::ReleaseAll()
{
	if (!m_pSurface)
		return false;

	m_pSurface[0]->Release();
	return true;
}

void CWorldMap::Restore()
{
	m_pSurface[0]->Restore();

	DDSURFACEDESC2 ddsd;					//ǥ���� ������ Ȯ���Ҽ� �ִ� ������ ����
	ZeroMemory(&ddsd, sizeof(ddsd));		//ddsd�ּҿ��� ddsdũ�⸸ŭ �޸𸮿����� 0���� ä����
	ddsd.dwSize = sizeof(ddsd);				//dwSize�� ddsdũ�⸸ŭ �ʱ�ȭ ���ش�

	m_pSurface[0]->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);					//���� ǥ���� ����ϴµ� �����Ұ�� false�� ��ȯ���ش�

	unsigned char *pDest, *pSrc;			//ǥ�����ּҸ� ������ �ִ� ����, ������ �ּҸ� ������ �ִ� ����

	pDest = (unsigned char*)ddsd.lpSurface;		//ǥ���� �ּҰ��� �ҷ���
	pSrc = m_pBuffer;						//������ �ּҸ� �ҷ���

	for (int y = 0; y < m_nHeight; y++)			//���̸�ŭ for���� ������
	{
		CopyMemory(pDest, pSrc, m_nWidth << 2);	//ǥ�鿡�ٰ� pSrc���� ���α��� * 4�� ũ�⸸ŭ �޸𸮸� �����Ѵ�
		pDest += ddsd.lPitch;					//pDest �����ּҸ� �������ν����ּҷ� �ٲ۴�
		pSrc += (m_nWidth << 2);				//pSrc �����ּҸ� �������ν����ּҷ� �ٲ۴�
	}

	m_pSurface[0]->Unlock(NULL);
}

bool CWorldMap::Drawing(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey, LPDIRECTDRAW7 pDirectDraw)
{
	RECT destRect;
	RECT destRect2;

	destRect.left = x;
	destRect.top = y;
	destRect.right = destRect.left + m_nWidth;
	destRect.bottom = destRect.top + m_nHeight;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = m_nWidth;
	destRect2.bottom = m_nHeight;

	if (destRect.top < 0)
	{
	destRect2.top = -destRect.top;
	destRect.top = 0;
	if (destRect2.top > m_nHeight)
	return true;
	}
	if (destRect.left < 0)
	{
	destRect2.left = -destRect.left;
	destRect.left = 0;
	if (destRect2.left > m_nWidth)
	return true;
	}
	if (destRect.right > SCREEN_WIDTH)
	{
	destRect2.right = m_nWidth - (destRect.right - SCREEN_WIDTH);
	destRect.right = SCREEN_WIDTH;
	if (destRect2.right < 0)
	return true;
	}
	if (destRect.bottom > SCREEN_HEIGHT)
	{
	destRect2.bottom = m_nHeight - (destRect.bottom - SCREEN_HEIGHT);
	destRect.bottom = SCREEN_HEIGHT;
	if (destRect2.bottom < 0)
	return true;
	}

	HRESULT	hResult;
	if (bUsingColorKey)
	{

		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[0], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				Restore();
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[0], &destRect2, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				Restore();
			else
				return false;
	}

	return true;

}

bool CWorldMap::Drawing2(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey, LPDIRECTDRAW7 pDirectDraw)
{
	RECT destRect;
	RECT destRect2;

	//destRect.left = x - (m_nWidth >> 1) - ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f);
	//destRect.top = y - (m_nHeight >> 1) - ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f);
	//destRect.left = x - ((m_nWidth >> 1) - ((camera.GetExpansion() - 1) *0.3f));
	//destRect.top = y - ((m_nHeight >> 1) - ((camera.GetExpansion() - 1) *0.3f));
	//destRect.right = destRect.left + m_nWidth + (2 * ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f));
	//destRect.bottom = destRect.top + m_nHeight + (2 * ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f));
	/*float tmpX = 0;
	float tmpY = 0;
	float tmpKX = camera.GetZoomInRange() * (camera.GetExpansion() - 1);
	float tmpKY = tmpKX;
	if (camera.GetDrawFinishX() < camera.GetX2())
	tmpX = -0.3;
	else if (camera.GetDrawFinishX() > camera.GetX2())
	tmpX = 0.3;
	if (camera.GetDrawFinishY() < camera.GetY2())
	tmpY = -0.3;
	else if (camera.GetDrawFinishY() > camera.GetY2())
	tmpY = 0.3;

	tmpKX *= tmpX;
	tmpKY *= tmpY;*/

	//destRect.left = x - (m_nWidth >> 1) + (tmpX * ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f)*(camera.GetExpansion() - 1));
	//destRect.top = y - (m_nHeight >> 1) + (tmpY * ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f)*0.56f*(camera.GetExpansion() - 1));
	destRect.left = -x + (SCREEN_WIDTH>>1);			//-1600 + screen 640
	destRect.top = -y + (SCREEN_HEIGHT>>1);			//-1600 + 360
	destRect.right = destRect.left + m_nWidth;
	destRect.bottom = destRect.top + m_nHeight;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = m_nWidth;
	destRect2.bottom = m_nHeight;

	if (destRect.top < 0)
	{
		//destRect2.top = -destRect.top + ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f)*0.56f + (tmpY * ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f)*0.56f*(camera.GetExpansion() - 1));
		//destRect2.top = -destRect.top + ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.6f)*0.5625f + (tmpY * (((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.6f)*0.5625f*(camera.GetExpansion() - 1)));
		//destRect2.top = -destRect.top + ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.6f)*0.5625f;
		//destRect2.top = -destRect.top + (SCREEN_HEIGHT >> 1)*(camera.GetExpansion() - 1);
		destRect2.top = -destRect.top;
		//destRect2.top = -destRect.top;
		destRect.top = 0;
		//if (destRect2.top > m_nHeight)
		//	return true;
	}

	if (destRect.left < 0)
	{
		//destRect2.left = -destRect.left + ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f) + (tmpX * ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f)*(camera.GetExpansion() - 1));
		//destRect2.left = -destRect.left + ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.6f) + (tmpX * (((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.6f)*(camera.GetExpansion() - 1)));
		//destRect2.left = -destRect.left + ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.6f);
		//destRect2.left = -destRect.left + (SCREEN_WIDTH >> 1)*(camera.GetExpansion() - 1);
		destRect2.left = -destRect.left;
		//destRect2.left = -destRect.left;
		destRect.left = 0;
		//if (destRect2.left > m_nWidth)
		//	return true;
	}

	if (destRect.right > SCREEN_WIDTH)
	{
		//destRect2.right = destRect2.left + SCREEN_WIDTH - ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f);
		//destRect2.right = m_nWidth - (destRect.right - SCREEN_WIDTH) - ((m_nWidth >> 1)*(camera.GetExpansion() - 1)*0.3f);
		//destRect2.right = m_nWidth - (destRect.right - SCREEN_WIDTH);


		//destRect2.right = m_nWidth - (destRect.right - SCREEN_WIDTH) - (SCREEN_WIDTH >> 1)*(camera.GetExpansion() - 1);
		//destRect2.right = destRect2.left + SCREEN_WIDTH - (SCREEN_WIDTH >> 1)*(camera.GetExpansion() - 1);
		destRect2.right = destRect2.left + SCREEN_WIDTH;
		destRect.right = SCREEN_WIDTH;
		//if (destRect2.right < 0)
		//	return true;
	}
	if (destRect.bottom > SCREEN_HEIGHT)
	{
		//destRect2.bottom = destRect2.top + SCREEN_HEIGHT - ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f)*0.5625f;
		//destRect2.bottom = m_nHeight - (destRect.bottom - SCREEN_HEIGHT) - ((m_nHeight >> 1)*(camera.GetExpansion() - 1)*0.3f)*0.5625f;
		//destRect2.bottom = m_nHeight - (destRect.bottom - SCREEN_HEIGHT);



		//destRect2.bottom = m_nHeight - (destRect.bottom - SCREEN_HEIGHT) - (SCREEN_HEIGHT >> 1)*(camera.GetExpansion() - 1);
		//destRect2.bottom = destRect2.top + SCREEN_HEIGHT - (SCREEN_HEIGHT >> 1)*(camera.GetExpansion() - 1);
		destRect2.bottom = destRect2.top + SCREEN_HEIGHT;
		destRect.bottom = SCREEN_HEIGHT;
		//if (destRect2.bottom < 0)
		//	return true;
	}

	HRESULT	hResult;
	if (bUsingColorKey)
	{

		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[0], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
		if (hResult == DDERR_SURFACELOST)
			Restore();
		else
			return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[0], &destRect2, DDBLT_WAIT, NULL)))
		if (hResult == DDERR_SURFACELOST)
			Restore();
		else
			return false;
	}

	return true;

}

bool CWorldMap::CopyBufferToSurface(LPDIRECTDRAW7 pDirectDraw)		//���ۿ��� ǥ������ �����ϴ� �Լ�
{
	if (!m_pBuffer)			//���۰� ������
		return false;		//false�� ��ȯ

	DDSURFACEDESC2 ddsd;					//ǥ���� ������ Ȯ���Ҽ� �ִ� ������ ����
	ZeroMemory(&ddsd, sizeof(ddsd));		//ddsd�ּҿ��� ddsdũ�⸸ŭ �޸𸮿����� 0���� ä����
	ddsd.dwSize = sizeof(ddsd);				//dwSize�� ddsdũ�⸸ŭ �ʱ�ȭ ���ش�

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	ddsd.dwHeight = m_nHeight;

	ddsd.dwWidth = m_nWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = 0;

	for (int i = 0; i < 1; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_pSurface[i], NULL)))
		{
			m_pSurface[i] = NULL;
			return false;
		}
		else
		{
			m_pSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}

	if (FAILED(m_pSurface[0]->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)))		//ǥ�鿡 �����Ͽ� �аų� ���� �۾��� �ϱ� ���ؼ� ǥ���� ��ٴ�
		return false;						//���� ǥ���� ����ϴµ� �����Ұ�� false�� ��ȯ���ش�

	unsigned char *pDest, *pSrc;			//ǥ�����ּҸ� ������ �ִ� ����, ������ �ּҸ� ������ �ִ� ����

	pDest = (unsigned char*)ddsd.lpSurface;		//ǥ���� �ּҰ��� �ҷ���
	pSrc = m_pBuffer;						//������ �ּҸ� �ҷ���

	for (int y = 0; y < m_nHeight; y++)			//���̸�ŭ for���� ������
	{
		CopyMemory(pDest, pSrc, m_nWidth << 2);	//ǥ�鿡�ٰ� pSrc���� ���α��� * 4�� ũ�⸸ŭ �޸𸮸� �����Ѵ�
		pDest += ddsd.lPitch;					//pDest �����ּҸ� �������ν����ּҷ� �ٲ۴�
		pSrc += (m_nWidth << 2);				//pSrc �����ּҸ� �������ν����ּҷ� �ٲ۴�
	}

	m_pSurface[0]->Unlock(NULL);			//����� ��ü���ش�

	return true;
}

bool CWorldMap::CopyBufferToSurface2(LPDIRECTDRAW7 pDirectDraw)		//���ۿ��� ǥ������ �����ϴ� �Լ�
{
	char *filename = "image\\map\\bossmap.bmp";
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	BITMAPFILEHEADER bmpfilehead;		//��Ʈ������ ��ü�� ������ ������ ���� ����
	if (!ReadFile(hfile, &bmpfilehead, sizeof(bmpfilehead), &actualRead, NULL))		//��Ʈ������ ��ü�� ������ ������ ���ٸ�
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}
	if (bmpfilehead.bfType != 0x4D42)		//��Ʈ�������� �ƴҰ��
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}

	BITMAPINFOHEADER bmpinfohead;		//��Ʈ�ʿ��� ��ü�� ������ ������ ���� ����
	if (!ReadFile(hfile, &bmpinfohead, sizeof(bmpinfohead), &actualRead, NULL))		//��Ʈ�ʿ��� ��ü�� ������ ������ ���ٸ�
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}

	if (bmpinfohead.biBitCount != 24)		//�ȼ��� ��Ʈ���� 24���� �ƴϸ� 
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}

	BITMAPFILEHEADER bmpfilehead2 = bmpfilehead;
	BITMAPINFOHEADER bmpinfohead2 = bmpinfohead;

	CloseHandle(hfile);

	DWORD dwWritten;
	HANDLE hfile2;
	hfile2 = CreateFile(TEXT(filename), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);    //���� ����

	WriteFile(hfile2, &bmpfilehead2, sizeof(bmpfilehead2), &dwWritten, NULL);  //���Ͽ� �����͸� ���
	WriteFile(hfile2, &bmpinfohead2, sizeof(bmpinfohead2), &dwWritten, NULL);
	WriteFile(hfile2, m_pBuffer2, WORLDMAP_DATA_SIZE, &dwWritten, NULL);  //7680000
	CloseHandle(hfile2);

	if (!m_pBuffer)			//���۰� ������
		return false;		//false�� ��ȯ

	DDSURFACEDESC2 ddsd;					//ǥ���� ������ Ȯ���Ҽ� �ִ� ������ ����
	ZeroMemory(&ddsd, sizeof(ddsd));		//ddsd�ּҿ��� ddsdũ�⸸ŭ �޸𸮿����� 0���� ä����
	ddsd.dwSize = sizeof(ddsd);				//dwSize�� ddsdũ�⸸ŭ �ʱ�ȭ ���ش�

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	ddsd.dwHeight = m_nHeight;

	ddsd.dwWidth = m_nWidth;



	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = 0;

	for (int i = 0; i < 1; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_pSurface[i], NULL)))
		{
			m_pSurface[i] = NULL;
			return false;
		}
		else
		{
			m_pSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}

	if (FAILED(m_pSurface[0]->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)))		//ǥ�鿡 �����Ͽ� �аų� ���� �۾��� �ϱ� ���ؼ� ǥ���� ��ٴ�
		return false;						//���� ǥ���� ����ϴµ� �����Ұ�� false�� ��ȯ���ش�

	int nDestY, nDeltaY, nDestY2, nDeltaY2;		//���� ����,�̹����� ������ ���¿����� ������������ �ٲܶ� ���̿� �����ִ°�
	nDestY = m_nHeight - 1;
	nDeltaY = -1;
	//nDestY = 0;
	//nDeltaY = 1;

	//nDestY2 = m_nHeight - 1;
	//nDeltaY2 = -1;
	nDestY2 = 0;
	nDeltaY2 = 1;

	for (int y = 0; y < m_nHeight; y++)				//�̹����� �����ȼ�����ŭ
	{
		for (int x = 0; x < m_nWidth; x++)			//�̹����� �����ȼ�����ŭ
		{
			CopyMemory(m_pBuffer + (x << 2) + nDestY*(m_nWidth << 2), m_pBuffer2 + (x << 2) + nDestY2*(m_nWidth << 2) - (x + nDestY2*m_nWidth), 3);
		}

		nDestY += nDeltaY;
		nDestY2 += nDeltaY2;
	}

	unsigned char *pDest, *pSrc;			//ǥ�����ּҸ� ������ �ִ� ����, ������ �ּҸ� ������ �ִ� ����

	pDest = (unsigned char*)ddsd.lpSurface;		//ǥ���� �ּҰ��� �ҷ���
	pSrc = m_pBuffer;						//������ �ּҸ� �ҷ���

	for (int y = 0; y < m_nHeight; y++)			//���̸�ŭ for���� ������
	{
		CopyMemory(pDest, pSrc, m_nWidth << 2);	//ǥ�鿡�ٰ� pSrc���� ���α��� * 4�� ũ�⸸ŭ �޸𸮸� �����Ѵ�
		pDest += ddsd.lPitch;					//pDest �����ּҸ� �������ν����ּҷ� �ٲ۴�
		pSrc += (m_nWidth << 2);				//pSrc �����ּҸ� �������ν����ּҷ� �ٲ۴�
	}

	m_pSurface[0]->Unlock(NULL);			//����� ��ü���ش�

	return true;
}

bool CWorldMap::CopyBufferToSurface3(LPDIRECTDRAW7 pDirectDraw)		//���ۿ��� ǥ������ �����ϴ� �Լ�
{
	char *filename = "image\\map\\bossmapRoof.bmp";
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	BITMAPFILEHEADER bmpfilehead;		//��Ʈ������ ��ü�� ������ ������ ���� ����
	if (!ReadFile(hfile, &bmpfilehead, sizeof(bmpfilehead), &actualRead, NULL))		//��Ʈ������ ��ü�� ������ ������ ���ٸ�
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}
	if (bmpfilehead.bfType != 0x4D42)		//��Ʈ�������� �ƴҰ��
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}

	BITMAPINFOHEADER bmpinfohead;		//��Ʈ�ʿ��� ��ü�� ������ ������ ���� ����
	if (!ReadFile(hfile, &bmpinfohead, sizeof(bmpinfohead), &actualRead, NULL))		//��Ʈ�ʿ��� ��ü�� ������ ������ ���ٸ�
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}

	if (bmpinfohead.biBitCount != 24)		//�ȼ��� ��Ʈ���� 24���� �ƴϸ� 
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}

	CloseHandle(hfile);

	if (!m_pBuffer)			//���۰� ������
		return false;		//false�� ��ȯ

	DDSURFACEDESC2 ddsd;					//ǥ���� ������ Ȯ���Ҽ� �ִ� ������ ����
	ZeroMemory(&ddsd, sizeof(ddsd));		//ddsd�ּҿ��� ddsdũ�⸸ŭ �޸𸮿����� 0���� ä����
	ddsd.dwSize = sizeof(ddsd);				//dwSize�� ddsdũ�⸸ŭ �ʱ�ȭ ���ش�

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	ddsd.dwHeight = m_nHeight;

	ddsd.dwWidth = m_nWidth;



	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = 0;

	for (int i = 0; i < 1; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_pSurface[i], NULL)))
		{
			m_pSurface[i] = NULL;
			return false;
		}
		else
		{
			m_pSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}

	if (FAILED(m_pSurface[0]->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)))		//ǥ�鿡 �����Ͽ� �аų� ���� �۾��� �ϱ� ���ؼ� ǥ���� ��ٴ�
		return false;						//���� ǥ���� ����ϴµ� �����Ұ�� false�� ��ȯ���ش�

	int nDestY, nDeltaY, nDestY2, nDeltaY2;		//���� ����,�̹����� ������ ���¿����� ������������ �ٲܶ� ���̿� �����ִ°�
	nDestY = m_nHeight - 1;
	nDeltaY = -1;
	//nDestY = 0;
	//nDeltaY = 1;

	//nDestY2 = m_nHeight - 1;
	//nDeltaY2 = -1;
	nDestY2 = 0;
	nDeltaY2 = 1;

	for (int y = 0; y < m_nHeight; y++)				//�̹����� �����ȼ�����ŭ
	{
		for (int x = 0; x < m_nWidth; x++)			//�̹����� �����ȼ�����ŭ
		{
			CopyMemory(m_pBuffer + (x << 2) + nDestY*(m_nWidth << 2), m_pBuffer2 + (x << 2) + nDestY2*(m_nWidth << 2) - (x + nDestY2*m_nWidth), 3);
		}

		nDestY += nDeltaY;
		nDestY2 += nDeltaY2;
	}

	unsigned char *pDest, *pSrc;			//ǥ�����ּҸ� ������ �ִ� ����, ������ �ּҸ� ������ �ִ� ����

	pDest = (unsigned char*)ddsd.lpSurface;		//ǥ���� �ּҰ��� �ҷ���
	pSrc = m_pBuffer;						//������ �ּҸ� �ҷ���

	for (int y = 0; y < m_nHeight; y++)			//���̸�ŭ for���� ������
	{
		CopyMemory(pDest, pSrc, m_nWidth << 2);	//ǥ�鿡�ٰ� pSrc���� ���α��� * 4�� ũ�⸸ŭ �޸𸮸� �����Ѵ�
		pDest += ddsd.lPitch;					//pDest �����ּҸ� �������ν����ּҷ� �ٲ۴�
		pSrc += (m_nWidth << 2);				//pSrc �����ּҸ� �������ν����ּҷ� �ٲ۴�
	}

	m_pSurface[0]->Unlock(NULL);			//����� ��ü���ش�

	return true;
}

bool CWorldMap::CopyBufferToSurface4(LPDIRECTDRAW7 pDirectDraw)
{
	char* filename = "image\\map\\basemap.bmp";
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	BITMAPFILEHEADER bmpfilehead;		//��Ʈ������ ��ü�� ������ ������ ���� ����
	if (!ReadFile(hfile, &bmpfilehead, sizeof(bmpfilehead), &actualRead, NULL))		//��Ʈ������ ��ü�� ������ ������ ���ٸ�
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}
	if (bmpfilehead.bfType != 0x4D42)		//��Ʈ�������� �ƴҰ��
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}

	BITMAPINFOHEADER bmpinfohead;		//��Ʈ�ʿ��� ��ü�� ������ ������ ���� ����
	if (!ReadFile(hfile, &bmpinfohead, sizeof(bmpinfohead), &actualRead, NULL))		//��Ʈ�ʿ��� ��ü�� ������ ������ ���ٸ�
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}

	if (bmpinfohead.biBitCount != 24)		//�ȼ��� ��Ʈ���� 24���� �ƴϸ� 
	{
		CloseHandle(hfile);		//�ڵ��� �ݳ����ְ�
		return false;			//false�� ��ȯ���ش�
	}

	BITMAPFILEHEADER bmpfilehead2 = bmpfilehead;
	BITMAPINFOHEADER bmpinfohead2 = bmpinfohead;

	CloseHandle(hfile);

	DWORD dwWritten;
	HANDLE hfile2;
	hfile2 = CreateFile(TEXT(filename), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);    //���� ����

	WriteFile(hfile2, &bmpfilehead2, sizeof(bmpfilehead2), &dwWritten, NULL);  //���Ͽ� �����͸� ���
	WriteFile(hfile2, &bmpinfohead2, sizeof(bmpinfohead2), &dwWritten, NULL);
	WriteFile(hfile2, m_pBuffer2, WORLDMAP_DATA_SIZE, &dwWritten, NULL);  //7680000
	CloseHandle(hfile2);

	if (!m_pBuffer)			//���۰� ������
		return false;		//false�� ��ȯ

	DDSURFACEDESC2 ddsd;					//ǥ���� ������ Ȯ���Ҽ� �ִ� ������ ����
	ZeroMemory(&ddsd, sizeof(ddsd));		//ddsd�ּҿ��� ddsdũ�⸸ŭ �޸𸮿����� 0���� ä����
	ddsd.dwSize = sizeof(ddsd);				//dwSize�� ddsdũ�⸸ŭ �ʱ�ȭ ���ش�

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	ddsd.dwHeight = m_nHeight;

	ddsd.dwWidth = m_nWidth;



	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = 0;

	for (int i = 0; i < 1; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_pSurface[i], NULL)))
		{
			m_pSurface[i] = NULL;
			return false;
		}
		else
		{
			m_pSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}

	if (FAILED(m_pSurface[0]->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)))		//ǥ�鿡 �����Ͽ� �аų� ���� �۾��� �ϱ� ���ؼ� ǥ���� ��ٴ�
		return false;						//���� ǥ���� ����ϴµ� �����Ұ�� false�� ��ȯ���ش�

	int nDestY, nDeltaY, nDestY2, nDeltaY2;		//���� ����,�̹����� ������ ���¿����� ������������ �ٲܶ� ���̿� �����ִ°�
	nDestY = m_nHeight - 1;
	nDeltaY = -1;
	//nDestY = 0;
	//nDeltaY = 1;

	//nDestY2 = m_nHeight - 1;
	//nDeltaY2 = -1;
	nDestY2 = 0;
	nDeltaY2 = 1;

	for (int y = 0; y < m_nHeight; y++)				//�̹����� �����ȼ�����ŭ
	{
		for (int x = 0; x < m_nWidth; x++)			//�̹����� �����ȼ�����ŭ
		{
			CopyMemory(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2), m_pBuffer2 + (x << 2) + nDestY2 * (m_nWidth << 2) - (x + nDestY2 * m_nWidth), 3);
		}

		nDestY += nDeltaY;
		nDestY2 += nDeltaY2;
	}

	unsigned char* pDest, * pSrc;			//ǥ�����ּҸ� ������ �ִ� ����, ������ �ּҸ� ������ �ִ� ����

	pDest = (unsigned char*)ddsd.lpSurface;		//ǥ���� �ּҰ��� �ҷ���
	pSrc = m_pBuffer;						//������ �ּҸ� �ҷ���

	for (int y = 0; y < m_nHeight; y++)			//���̸�ŭ for���� ������
	{
		CopyMemory(pDest, pSrc, m_nWidth << 2);	//ǥ�鿡�ٰ� pSrc���� ���α��� * 4�� ũ�⸸ŭ �޸𸮸� �����Ѵ�
		pDest += ddsd.lPitch;					//pDest �����ּҸ� �������ν����ּҷ� �ٲ۴�
		pSrc += (m_nWidth << 2);				//pSrc �����ּҸ� �������ν����ּҷ� �ٲ۴�
	}

	m_pSurface[0]->Unlock(NULL);			//����� ��ü���ش�

	return true;
}

void CWorldMap::CopyMapData()
{
	DWORD dwWritten;
	HANDLE hfile;
	hfile = CreateFile("data\\blockData.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);    //���� ����

	WriteFile(hfile, g_editor.GetBlockData(), BLOCK_X*BLOCK_Y * 2, &dwWritten, NULL);  //���Ͽ� �����͸� ���

	CloseHandle(hfile);

}

void CWorldMap::InitMapData()
{
	DWORD dwWritten;
	HANDLE hfile;

	char *zero = "10";

	for (int i = 0; i < BLOCK_X*BLOCK_Y; i++)
	{
		memcpy(g_editor.GetBlockData()+i*2, zero, 2);
	}
	
	hfile = CreateFile("data\\blockData.txt", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);    //���� ����

	WriteFile(hfile, g_editor.GetBlockData(), BLOCK_X*BLOCK_Y * 2, &dwWritten, NULL);  //���Ͽ� �����͸� ���

	CloseHandle(hfile);
}