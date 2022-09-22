#pragma once
#include <ddraw.h>

class WorldMap;

class Map
{
public:
	Map();
	~Map() = default;
	void SetWorldMap(WorldMap* baseMapPtr, WorldMap* bossWorldMapPtr, WorldMap* bossWorldMapRoofPtr);
	void SetPos(int x, int y);
	void SetStage(int _stage);
	void Init();
	int GetStageNum() const;
	void Drawing(LPDIRECTDRAWSURFACE7 lpSurface, LPDIRECTDRAW7 lpDirectDrawObject) const; //기본 배경
	void DrawingRoof(LPDIRECTDRAWSURFACE7 lpSurface, LPDIRECTDRAW7 lpDirectDrawObject) const; //배경 위에 덮는 이미지 그려줄때
	void NextStage();
private:
	int mX;
	int mY;
	int mStage;
	WorldMap* mBaseWorldMapPtr; //기본 맵 배경 이미지
	WorldMap* mBossWorldMapPtr; //기본 보스 맵 배경 이미지
	WorldMap* mBossWorldMapRoofPtr; //기본 배경 위에 덮는 이미지
};

inline void Map::SetPos(int x, int y)
{
	mX = x;
	mY = y;
}

inline void Map::SetStage(int _stage)
{
	mStage = _stage;
}

inline int Map::GetStageNum() const
{
	return mStage;
}

inline void Map::NextStage()
{
	mStage++;
}
