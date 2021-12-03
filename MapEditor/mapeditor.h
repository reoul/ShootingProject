#ifndef __mapeditor_h__
#define __mapeditor_h__

#include "block.h"
#include "define.h"
#include "sprite.h"
#include "timer.h"
#include "choicewindow.h"
#include <string>

using namespace std;

class MAPEDITOR			//�ʿ����� ���� Ŭ����
{
private:
	int m_startX;			//���콺 ���� x��ǥ
	int m_startY;			//���콺 ���� y��ǥ
	int m_endX;			//���콺 ���� x��ǥ
	int m_endY;			//���콺 ���� y��ǥ
	bool isChoiceWindow;
	CChoiceWindow* window;
	char m_pBlockData[BLOCK_X*BLOCK_Y*2];
	char m_pBlockData2[BLOCK_X*BLOCK_Y*2];
	string wallData;

public:
	MAPEDITOR();
	~MAPEDITOR();
	void Initialize();
	void CheckChoiceWindow(int x, int y);
	bool IsChoiceWindow();
	void SetStartXY(int x,int y);		//���콺 ���� x,y��ǥ ����
	void SetEndXY(int x,int y);			//���콺 ���� x,y��ǥ ����
	void SetBlock(CBLOCK block[][BLOCK_X],CSprite* curBlock,CTimer *timer);
	CChoiceWindow* GetChoiceWindow();
	char* GetBlockData();
	char* GetBlockData2();
	void SaveWallData();
};
#endif
