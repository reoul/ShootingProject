#pragma once
#include <ddraw.h>

class WorldMap;

class Map
{
public:
	Map();
	~Map();
	void SetWorldMap(WorldMap* baseMap, WorldMap* bossWorldMap, WorldMap* bossWorldMapRoof);
	void SetPos(int x, int y);
	void SetStage(int _stage);
	void Init();
	int GetStageNum();
	void Drawing(LPDIRECTDRAWSURFACE7 lpsurface, LPDIRECTDRAW7 lpDirectDrawObject); //기본 배경
	void DrawingRoof(LPDIRECTDRAWSURFACE7 lpsurface, LPDIRECTDRAW7 lpDirectDrawObject); //배경 위에 덮는 이미지 그려줄때
	void NextStage();
private:
	int m_x;
	int m_y;
	int stage;
	WorldMap* m_pbassWorldMap; //기본 보스 맵 배경 이미지
	WorldMap* m_pBossWorldMap; //기본 보스 맵 배경 이미지
	WorldMap* m_pBossWorldMapRoof; //기본 배경 위에 덮는 이미지
};
