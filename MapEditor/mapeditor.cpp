#include <stdio.h>
#include "mapeditor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "camera.h"
#include "worldmapBmp.h"
#include "gameEnum.h"

extern CCamera camera;
extern CWorldMap bossMap;
extern CWorldMap bossMapRoof;
//extern CWorldMap baseMap;
extern CWorldMap* curEditMap;


extern EDIT_STATE curEditState;
extern EDIT_WINDOW curEditWindow;
extern vector<int> wallBlockData3;
extern vector<Vector2> wallBlock;

vector<int> split(string input, char delimiter);

MAPEDITOR::MAPEDITOR()
{
	ifstream wallDataFile;
	wallDataFile.open("data\\wallData3.txt");
	if (wallDataFile.is_open())
	{
		while (!wallDataFile.eof())
		{
			std::getline(wallDataFile, wallData);
		}
		wallDataFile.close();    //파일 닫아줍니다.
	}
	if (wallData.size() > 0)
	{
		wallBlockData3 = split(wallData, ' ');
		for (int i = 0; i < wallBlockData3.size(); i += 2)
		{
			wallBlock.push_back(Vector2(wallBlockData3[i], wallBlockData3[i + 1]));
		}
	}
}

vector<int> split(string input, char delimiter) {
	vector<int> answer;
	stringstream ss(input);
	string temp;
	int tmp = 0;

	while (getline(ss, temp, delimiter)) {
		stringstream ssInt(temp);
		ssInt >> tmp;
		answer.push_back(tmp);
	}

	return answer;
}

MAPEDITOR::~MAPEDITOR()
{
}

void MAPEDITOR::Initialize()
{
	window = new CChoiceWindow();
	window->Initialize();
}

void MAPEDITOR::CheckChoiceWindow(int x, int y)
{
	if (window->GetRect().left < x && window->GetRect().top < y && window->GetRect().right > x && window->GetRect().bottom > y)
		isChoiceWindow = true;
	else
		isChoiceWindow = false;
}

bool MAPEDITOR::IsChoiceWindow()
{
	return isChoiceWindow;
}

void MAPEDITOR::SetStartXY(int x,int y)
{
	m_startX = x / DEFAULT_BLOCK_SIZE;
	m_startY = y / DEFAULT_BLOCK_SIZE;
}

void MAPEDITOR::SetEndXY(int x,int y)
{
	int _x = x / DEFAULT_BLOCK_SIZE;
	int _y = y / DEFAULT_BLOCK_SIZE;

	if (m_startX > _x)
	{
		m_endX = m_startX;
		m_startX = _x;
	}
	else
		m_endX = _x;

	if (m_startY > _y)
	{
		m_endY = m_startY;
		m_startY = _y;
	}
	else
		m_endY = _y;
}

void MAPEDITOR::SetBlock(CBLOCK block[][BLOCK_X], CSprite* curBlock, CTimer *timer)
{
	int tmp = curBlock->GetNumber();
	char tmp2[10];
	sprintf_s(tmp2, "%d", tmp);
	for (int i = m_startX; i < m_endX + 1; i++)
	{
		for (int j = m_startY; j < m_endY + 1; j++)
		{
			block[j][i].Initialize(curBlock, (i * DEFAULT_BLOCK_SIZE), (j * DEFAULT_BLOCK_SIZE), timer, 0, 150);
			block[j][i].SetSprite(curBlock);
			block[j][i].SetState(OBJECT_TYPE::EDITOR_BLOCK);
			memcpy(m_pBlockData + (j*BLOCK_X) + i * 2, &tmp2, 2);
			string aa = "";
			aa += to_string(i);
			aa += " ";
			aa += to_string(j);
			aa += " ";
			wallData += aa;
			//block[j][i].GetSprite()->GetBMP()->SaveWorldBMP((i * DEFAULT_BLOCK_SIZE), (j * DEFAULT_BLOCK_SIZE), curEditMap->GetBMPBuffer(), curEditMap->GetBMPBuffer2());
		}
	}
}

CChoiceWindow* MAPEDITOR::GetChoiceWindow()
{
	return window;
}

char* MAPEDITOR::GetBlockData()
{
	return m_pBlockData;
}

char* MAPEDITOR::GetBlockData2()
{
	return m_pBlockData2;
}

void MAPEDITOR::SaveWallData()
{
	string filePath = "data\\wallData3.txt";

	// write File
	ofstream writeFile(filePath.data());
	if (writeFile.is_open()) {
		writeFile << wallData;
		writeFile.close();
	}
}
