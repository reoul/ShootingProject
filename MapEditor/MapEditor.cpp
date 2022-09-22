#include "MapEditor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Camera.h"
#include "ChoiceWindow.h"
#include "WorldMapBmp.h"
#include "GameEnum.h"
#include "Block.h"

extern Camera camera;
extern WorldMap bossMap;
extern WorldMap bossMapRoof;
//extern WorldMap baseMap;
extern WorldMap* curEditMap;


extern EEditState curEditState;
extern EEditWindow curEditWindow;
extern vector<int> wallBlockData3;
extern vector<Vector2> wallBlock;

vector<int> split(string input, char delimiter);

MapEditor::MapEditor()
	: mStartX(0)
	, mStartY(0)
	, mEndX(0)
	, mEndY(0)
	, mIsChoiceWindow(false)
	, mWindowPtr(nullptr)
	, mBlockDataArr{}
	, mBlockDataArr2{}
{
	ifstream wallDataFile;
	wallDataFile.open("data\\wallData3.txt");
	if (wallDataFile.is_open())
	{
		while (!wallDataFile.eof())
		{
			std::getline(wallDataFile, mWallData);
		}
		wallDataFile.close(); //파일 닫아줍니다.
	}
	if (!mWallData.empty())
	{
		wallBlockData3 = split(mWallData, ' ');
		for (int i = 0; i < wallBlockData3.size(); i += 2)
		{
			wallBlock.emplace_back(wallBlockData3[i], wallBlockData3[i + 1]);
		}
	}
}

vector<int> split(string input, char delimiter)
{
	vector<int> answer;
	stringstream ss(input);
	string temp;
	int tmp = 0;

	while (getline(ss, temp, delimiter))
	{
		stringstream ssInt(temp);
		ssInt >> tmp;
		answer.push_back(tmp);
	}

	return answer;
}

void MapEditor::Initialize()
{
	mWindowPtr = make_shared<ChoiceWindow>();
	mWindowPtr->Initialize();
}

void MapEditor::CheckChoiceWindow(int x, int y)
{
	if (mWindowPtr->GetRect().left < x && mWindowPtr->GetRect().top < y && mWindowPtr->GetRect().right > x && mWindowPtr->GetRect().
		bottom > y)
		mIsChoiceWindow = true;
	else
		mIsChoiceWindow = false;
}

void MapEditor::SetEndXY(int x, int y)
{
	int _x = x / DEFAULT_BLOCK_SIZE;
	int _y = y / DEFAULT_BLOCK_SIZE;

	if (mStartX > _x)
	{
		mEndX = mStartX;
		mStartX = _x;
	}
	else
		mEndX = _x;

	if (mStartY > _y)
	{
		mEndY = mStartY;
		mStartY = _y;
	}
	else
		mEndY = _y;
}

void MapEditor::SetBlock(Block blocks[][BLOCK_X], Sprite* curBlock)
{
	int tmp = curBlock->GetNumber();
	char tmp2[10];
	sprintf_s(tmp2, "%d", tmp);
	for (int i = mStartX; i < mEndX + 1; i++)
	{
		for (int j = mStartY; j < mEndY + 1; j++)
		{
			blocks[j][i].Initialize(curBlock, (i * DEFAULT_BLOCK_SIZE), (j * DEFAULT_BLOCK_SIZE), 0, 150);
			blocks[j][i].SetSprite(curBlock);
			blocks[j][i].SetState(EObjectType::EditorBlock);
			memcpy(mBlockDataArr + (j * BLOCK_X) + i * 2, &tmp2, 2);
			string aa = "";
			aa += to_string(i);
			aa += " ";
			aa += to_string(j);
			aa += " ";
			mWallData += aa;
		}
	}
}

void MapEditor::SaveWallData() const
{
	string filePath = "data\\wallData3.txt";

	// write File
	ofstream writeFile(filePath.data());
	if (writeFile.is_open())
	{
		writeFile << mWallData;
		writeFile.close();
	}
}
