#pragma once
#include <memory>
#include <string>
#include "SettingData.h"

class ChoiceWindow;
class Timer;
class Sprite;
class Block;

using namespace std;

class MapEditor
{
public:
	MapEditor();
	~MapEditor() = default;
	void Initialize();
	void CheckChoiceWindow(int x, int y);
	bool IsChoiceWindow() const;
	void SetStartXY(int x, int y); //마우스 시작 x,y좌표 설정
	void SetEndXY(int x, int y); //마우스 종료 x,y좌표 설정
	void SetBlock(Block blocks[][BLOCK_Y], Sprite* curBlock);
	shared_ptr<ChoiceWindow> GetChoiceWindow() const;
	char* GetBlockData();
	char* GetBlockData2();
	void SaveWallData() const;
private:
	int mStartX; //마우스 시작 x좌표
	int mStartY; //마우스 시작 y좌표
	int mEndX; //마우스 종료 x좌표
	int mEndY; //마우스 종료 y좌표
	bool mIsChoiceWindow;
	shared_ptr<ChoiceWindow> mWindowPtr;
	char mBlockDataArr[BLOCK_X * BLOCK_Y * 2];
	char mBlockDataArr2[BLOCK_X * BLOCK_Y * 2];
	string mWallData;
};

inline bool MapEditor::IsChoiceWindow() const
{
	return mIsChoiceWindow;
}

inline void MapEditor::SetStartXY(int x, int y)
{
	mStartX = x / DEFAULT_BLOCK_SIZE;
	mStartY = y / DEFAULT_BLOCK_SIZE;
}

inline shared_ptr<ChoiceWindow> MapEditor::GetChoiceWindow() const
{
	return mWindowPtr;
}

inline char* MapEditor::GetBlockData()
{
	return mBlockDataArr;
}

inline char* MapEditor::GetBlockData2()
{
	return mBlockDataArr2;
}
