#ifndef __map_h__
#define __map_h__

#include "sprite.h"
#include "worldmapBmp.h"

class Map
{
private:
	int m_x;
	int m_y;
	int stage;
	CWorldMap* m_pbassWorldMap;			//기본 보스 맵 배경 이미지
	CWorldMap* m_pBossWorldMap;			//기본 보스 맵 배경 이미지
	CWorldMap* m_pBossWorldMapRoof;		//기본 배경 위에 덮는 이미지
public:
	Map();
	~Map();
	void SetWorldMap(CWorldMap* baseMap, CWorldMap* bossWorldMap, CWorldMap* bossWorldMapRoof);
	void SetPos(int x, int y);
	void SetStage(int _stage);
	int GetStage();
	void Drawing(LPDIRECTDRAWSURFACE7 lpsurface, LPDIRECTDRAW7 lpDirectDrawObject);			//기본 배경
	void DrawingRoof(LPDIRECTDRAWSURFACE7 lpsurface, LPDIRECTDRAW7 lpDirectDrawObject);		//배경 위에 덮는 이미지 그려줄때
	void NextStage();
};

#endif