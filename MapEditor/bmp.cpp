#include <Windows.h>
#include <windowsx.h>
#include "bmp.h"
#include "define.h"

extern bool g_bIsFirst;

CBMP::CBMP()			//��������� �ʱ�ȭ���ش�
{
	m_pBuffer = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
}
CBMP::~CBMP()
{
	if (m_pBuffer)				//���۰� �ִٸ�
		delete[] m_pBuffer;		//���۸� ���������ش�
}

int CBMP::GetHeight()			//���̸� �������ش�
{
	return (m_nHeight);
}

unsigned char* CBMP::GetBMPBuffer()
{
	return m_pBuffer;
}

int CBMP::GetWidth()			//������ ���̸� �������ش�
{
	return (m_nWidth);
}

bool CBMP::LoadBMPFile(char *filename)		//��Ʈ���� ������ �ҷ��� ���ۿ� ���������� ����ְ� 24bit�̹����� 32bit�̹����� �ٲ���
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

	m_pBuffer = new unsigned char[m_nHeight *m_nWidth * 4];		//���ο� ���۸� 32��Ʈ�� ǥ�����ֱ����ؼ� ���ȼ�����*4�� ���ش�

	struct RBGstruct			//��Ʈ���� RGB�� ������ �ƴ� BGR������ �����
	{
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	//struct RBGstruct rgb24;		//�ȼ����� ���� ���� ���� ����
	struct RBGstruct *rgb24 = new RBGstruct[m_nWidth];


	int nStoredLine = (m_nWidth * 3 + 3)&~3;
	unsigned char temp[4];
	int nRemainder = nStoredLine - (m_nWidth * 3);

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

	for (int y = 0; y < nHeight; y++)				//�̹����� �����ȼ�����ŭ
	{
		if (!ReadFile(hfile, rgb24, 3 * m_nWidth, &actualRead, NULL))		//���Ͽ� 3����Ʈ ũ�⸸ŭ �о�ͼ� rgb24�� ���������
		{														//������ �� �о�Դٸ�
			CloseHandle(hfile);									//�ڵ��� �ݳ����ְ�
			delete[] m_pBuffer;									//���۸� �������ְ�
			return false;										//false�� ��ȯ���ش�
		}
		for (int x = 0; x < m_nWidth; x++)			//�̹����� �����ȼ�����ŭ
		{
			CopyMemory(m_pBuffer + (x << 2) + nDestY*(m_nWidth << 2), rgb24+x, 3);		//�� �ȼ����� 4����Ʈ�� �������ְ� 3����Ʈ�� �ȼ��� rgb��
			*(m_pBuffer + (x << 2) + nDestY*(m_nWidth << 2) + 3) = 0;					//������ 1����Ʈ�� ���İ��� �־ ������ ����
		}

		nDestY += nDeltaY;			//��������� ������������ �ٲ���

		if (!ReadFile(hfile, temp, nRemainder, &actualRead, NULL))
		{
			CloseHandle(hfile);
			delete[] m_pBuffer;
			return false;
		}
	}

	delete[] rgb24;
	rgb24 = NULL;

	CloseHandle(hfile);

	return true;
}

bool CBMP::CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface)		//���ۿ��� ǥ������ �����ϴ� �Լ�
{
	if (!m_pBuffer)			//���۰� ������
		return false;		//false�� ��ȯ

	DDSURFACEDESC2 ddsd;					//ǥ���� ������ Ȯ���Ҽ� �ִ� ������ ����
	ZeroMemory(&ddsd, sizeof(ddsd));		//ddsd�ּҿ��� ddsdũ�⸸ŭ �޸𸮿����� 0���� ä����
	ddsd.dwSize = sizeof(ddsd);				//dwSize�� ddsdũ�⸸ŭ �ʱ�ȭ ���ش�

	if (FAILED(lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)))		//ǥ�鿡 �����Ͽ� �аų� ���� �۾��� �ϱ� ���ؼ� ǥ���� ��ٴ�
		return false;						//���� ǥ���� ����ϴµ� �����Ұ�� false�� ��ȯ���ش�

	unsigned char *pDest, *pSrc;			//ǥ�����ּҸ� ������ �ִ� ����, ������ �ּҸ� ������ �ִ� ����

	pDest = (unsigned char*)ddsd.lpSurface;		//ǥ���� �ּҰ��� �ҷ���
	pSrc = m_pBuffer;						//������ �ּҸ� �ҷ���

	for (int y = 0; y < m_nHeight; y++)			//���̸�ŭ for���� ������
	{
		CopyMemory(pDest, pSrc, m_nWidth<<2);	//ǥ�鿡�ٰ� pSrc���� ���α��� * 4�� ũ�⸸ŭ �޸𸮸� �����Ѵ�
		pDest += ddsd.lPitch;					//pDest �����ּҸ� �������ν����ּҷ� �ٲ۴�
		pSrc += (m_nWidth << 2);				//pSrc �����ּҸ� �������ν����ּҷ� �ٲ۴�
	}

	lpSurface->Unlock(NULL);			//����� ��ü���ش�

	return true;
}

bool CBMP::CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface, unsigned char* _buffer)		//���ۿ��� ǥ������ �����ϴ� �Լ�
{
	if (!_buffer)			//���۰� ������
		return false;		//false�� ��ȯ

	DDSURFACEDESC2 ddsd;					//ǥ���� ������ Ȯ���Ҽ� �ִ� ������ ����
	ZeroMemory(&ddsd, sizeof(ddsd));		//ddsd�ּҿ��� ddsdũ�⸸ŭ �޸𸮿����� 0���� ä����
	ddsd.dwSize = sizeof(ddsd);				//dwSize�� ddsdũ�⸸ŭ �ʱ�ȭ ���ش�

	if (FAILED(lpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL)))		//ǥ�鿡 �����Ͽ� �аų� ���� �۾��� �ϱ� ���ؼ� ǥ���� ��ٴ�
		return false;						//���� ǥ���� ����ϴµ� �����Ұ�� false�� ��ȯ���ش�

	unsigned char* pDest, * pSrc;			//ǥ�����ּҸ� ������ �ִ� ����, ������ �ּҸ� ������ �ִ� ����

	pDest = (unsigned char*)ddsd.lpSurface;		//ǥ���� �ּҰ��� �ҷ���
	pSrc = _buffer;						//������ �ּҸ� �ҷ���

	for (int y = 0; y < m_nHeight; y++)			//���̸�ŭ for���� ������
	{
		CopyMemory(pDest, pSrc, m_nWidth << 2);	//ǥ�鿡�ٰ� pSrc���� ���α��� * 4�� ũ�⸸ŭ �޸𸮸� �����Ѵ�
		pDest += ddsd.lPitch;					//pDest �����ּҸ� �������ν����ּҷ� �ٲ۴�
		pSrc += (m_nWidth << 2);				//pSrc �����ּҸ� �������ν����ּҷ� �ٲ۴�
	}

	lpSurface->Unlock(NULL);			//����� ��ü���ش�

	return true;
}

bool CBMP::SaveWorldBMP(int _x, int _y, unsigned char *buffer, unsigned char *buffer2)
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

	int nHeight = bmpinfohead.biHeight;		//��Ʈ���� ���̸� �ҷ��� nHeight�� �ʱ�ȭ
	int nWidth = bmpinfohead.biWidth;

	int nDestY, nDeltaY, nDestY2, nDeltaY2;		//���� ����,�̹����� ������ ���¿����� ������������ �ٲܶ� ���̿� �����ִ°�

	nDestY = nHeight - 1;
	nDeltaY = -1;
	nDestY2 = 0; //0
	nDeltaY2 = 1; // 1

	for (int y = 0; y < m_nHeight; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			if (*(m_pBuffer + (x << 2) + nDestY2*(m_nWidth << 2)) == 0)
			if (*(m_pBuffer + (x << 2) + nDestY2*(m_nWidth << 2) + 1) == 0)
			if (*(m_pBuffer + (x << 2) + nDestY2*(m_nWidth << 2) + 2) == 0)
				continue;
			CopyMemory(buffer2 + ((x + _x) << 2) + ((nDestY - _y)*(nWidth << 2)) - (x + _x + (nDestY - _y)*nWidth), m_pBuffer + (x << 2) + nDestY2*(m_nWidth << 2), 3);
		}

		nDestY += nDeltaY;
		nDestY2 += nDeltaY2;
	}
	CloseHandle(hfile);
	return true;
}