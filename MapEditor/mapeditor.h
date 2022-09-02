#pragma once
#include <string>
#include "SettingData.h"

class ChoiceWindow;
class Timer;
class Sprite;
class Block;

using namespace std;

class MAPEDITOR //맵에디터 관련 클래스
{
public:
	MAPEDITOR();
	~MAPEDITOR();
	void Initialize();
	void CheckChoiceWindow(int x, int y);
	bool IsChoiceWindow();
	void SetStartXY(int x, int y); //마우스 시작 x,y좌표 설정
	void SetEndXY(int x, int y); //마우스 종료 x,y좌표 설정
	void SetBlock(Block block[][BLOCK_X], Sprite* curBlock, Timer* timer);
	ChoiceWindow* GetChoiceWindow();
	char* GetBlockData();
	char* GetBlockData2();
	void SaveWallData();
private:
	int m_startX; //마우스 시작 x좌표
	int m_startY; //마우스 시작 y좌표
	int m_endX; //마우스 종료 x좌표
	int m_endY; //마우스 종료 y좌표
	bool isChoiceWindow;
	ChoiceWindow* window;
	char m_pBlockData[BLOCK_X * BLOCK_Y * 2];
	char m_pBlockData2[BLOCK_X * BLOCK_Y * 2];
	string wallData;
};
