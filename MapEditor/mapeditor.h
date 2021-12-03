#ifndef __mapeditor_h__
#define __mapeditor_h__

#include "block.h"
#include "define.h"
#include "sprite.h"
#include "timer.h"
#include "choicewindow.h"
#include <string>

using namespace std;

class MAPEDITOR			//맵에디터 관련 클래스
{
private:
	int m_startX;			//마우스 시작 x좌표
	int m_startY;			//마우스 시작 y좌표
	int m_endX;			//마우스 종료 x좌표
	int m_endY;			//마우스 종료 y좌표
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
	void SetStartXY(int x,int y);		//마우스 시작 x,y좌표 설정
	void SetEndXY(int x,int y);			//마우스 종료 x,y좌표 설정
	void SetBlock(CBLOCK block[][BLOCK_X],CSprite* curBlock,CTimer *timer);
	CChoiceWindow* GetChoiceWindow();
	char* GetBlockData();
	char* GetBlockData2();
	void SaveWallData();
};
#endif
