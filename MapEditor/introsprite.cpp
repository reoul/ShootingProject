#include "introsprite.h"

CIntroSprite::CIntroSprite()
{
}

CIntroSprite::~CIntroSprite()
{
	free(m_pBuffer);
}

bool CIntroSprite::InitSprite(int nFrame, int nWidth, int nHeight, int nColorKey, LPDIRECTDRAW7 pDirectDraw)
{
	if (m_pSurface)
		delete[] m_pSurface;

	if (m_pBmp)
		delete[] m_pBmp;
	
	m_pSurface = new LPDIRECTDRAWSURFACE7[nFrame];

	if (!m_pSurface)
		return false;


	m_pBmp = new CBMP[nFrame];

	if (!m_pBmp)
		return false;

	DDSURFACEDESC2 ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	ddsd.dwHeight = nHeight;

	ddsd.dwWidth = nWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = nColorKey;

	for (int i = 0; i < nFrame; i++)
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

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nFrame = nFrame;

	return true;

}

bool CIntroSprite::LoadFrame(int nFrame, char* filename)
{
	if (!m_pBmp[nFrame].LoadBMPFile(filename))
		return false;
	if (!m_pBmp[nFrame].CopyBufferToSurface(m_pSurface[nFrame]))
		return false;

	return true;
}

bool CIntroSprite::ReleaseAll()
{
	if (!m_pSurface)
		return false;

	for (int i = 0; i < m_nFrame; i++)
	{
		if (m_pSurface[i])
			m_pSurface[i]->Release();
	}

	return true;
}

void CIntroSprite::RestoreAll(LPDIRECTDRAW7 pDirectDraw)
{
	delete[] m_pSurface;
	m_pSurface = new LPDIRECTDRAWSURFACE7[m_nFrame];

	DDSURFACEDESC2 ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	ddsd.dwHeight = m_nHeight;

	ddsd.dwWidth = m_nWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = 0;

	for (int i = 0; i < m_nFrame; i++)
	{
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_pSurface[i], NULL)))
		{
			m_pSurface[i] = NULL;
		}
		else
		{
			m_pSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	}
	for (int i = 0; i < m_nFrame; i++)
	{
		m_pBmp[i].CopyBufferToSurface(m_pSurface[i]);
	}
}

bool CIntroSprite::LoadBMPFile(char* filename)		//��Ʈ���� ������ �ҷ��� ���ۿ� ���������� ����ְ� 24bit�̹����� 32bit�̹����� �ٲ���
{
	m_nFrame = 1;
	HANDLE hfile;
	DWORD actualRead;
	hfile = CreateFile(TEXT(filename), GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		DWORD ab = GetLastError();
		return false;
	}

	m_pSurface = new LPDIRECTDRAWSURFACE7[m_nFrame];

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

	m_pBuffer = new unsigned char[m_nHeight * m_nWidth * 4];		//���ο� ���۸� 32��Ʈ�� ǥ�����ֱ����ؼ� ���ȼ�����*4�� ���ش�

	struct RBGstruct			//��Ʈ���� RGB�� ������ �ƴ� BGR������ �����
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	struct RBGstruct *rgb24 = new RBGstruct[m_nWidth];		//�ȼ����� ���� ���� ���� ����

	int nDestY, nDeltaY;		//���� ����,�̹����� ������ ���¿����� ������������ �ٲܶ� ���̿� �����ִ°�

	if (bBottomUp)				//�̹����� ��������������
	{
		nDestY = nHeight - 1;
		nDeltaY = -1;
	}
	else
	{
		nDestY = 0;
		nDeltaY = 1;
	}

	for (int y = 0; y < m_nHeight; y++)				//�̹����� �����ȼ�����ŭ
	{
		if (!ReadFile(hfile, rgb24, m_nWidth * sizeof(RBGstruct), &actualRead, NULL))		//���Ͽ� 3����Ʈ ũ�⸸ŭ �о�ͼ� rgb24�� ���������
		{														//������ �� �о�Դٸ�
			CloseHandle(hfile);									//�ڵ��� �ݳ����ְ�
			delete[] m_pBuffer;									//���۸� �������ְ�
			return false;										//false�� ��ȯ���ش�
		}

		for (int x = 0; x < m_nWidth; x++)			//�̹����� �����ȼ�����ŭ
		{
			CopyMemory(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2), &rgb24[x], 3);		//�� �ȼ����� 4����Ʈ�� �������ְ� 3����Ʈ�� �ȼ��� rgb��
			*(m_pBuffer + (x << 2) + nDestY * (m_nWidth << 2) + 3) = 0;					//������ 1����Ʈ�� ���İ��� �־ ������ ����
			if ((x == 0) || (x == (m_nWidth - 1)))
				bBottomUp = true;
		}

		nDestY += nDeltaY;
		
	}

	CloseHandle(hfile);

	return true;
}
bool CIntroSprite::Drawing(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey)
{
	RECT destRect;
	RECT destRect2;

	destRect.left = x;
	destRect.top = y;
	destRect.right = m_nWidth;
	destRect.bottom = m_nHeight;

	destRect2.left = 0;
	destRect2.top = 0;
	destRect2.right = m_nWidth;
	destRect2.bottom = m_nHeight;

	/*if (destRect.top < 0)
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
	}*/

	HRESULT	hResult;
	if (bUsingColorKey)
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[frame], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return false;
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[frame], &destRect2, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return false;
			else
				return false;
	}

	return true;

}

bool CIntroSprite::Drawing2(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey)
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

		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[frame], &destRect2, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return false;
			else
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_pSurface[frame], &destRect2, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return false;
			else
				return false;
	}

	return true;

}

bool CIntroSprite::CopyBufferToSurface(LPDIRECTDRAW7 pDirectDraw)		//���ۿ��� ǥ������ �����ϴ� �Լ�
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


//void CIntroSprite::KeyDown()
//{
//	buttondown.Down();
//}
//
//void CIntroSprite::KeyDownInit()
//{
//	buttondown.InitDown();
//}
//
//void CIntroSprite::KeyDownCheck()
//{
//	buttondown.CheckUp();
//}
//
//bool CIntroSprite::CheckDown()
//{
//	return buttondown.CheckClick();
//}

