#include "map.h"

Map::Map()
{
	m_x = 0;
	m_y = 0;
	stage = 0;
}

Map::~Map()
{
}

void Map::SetWorldMap(CWorldMap* baseMap, CWorldMap* bossWorldMap, CWorldMap* bossWorldMapRoof)
{
	m_x = 0;
	m_y = 0;
	m_pbassWorldMap = baseMap;
	m_pBossWorldMap = bossWorldMap;
	m_pBossWorldMapRoof = bossWorldMapRoof;
}

void Map::SetPos(int x, int y)
{
	m_x = x;
	m_y = y;
}

void Map::SetStage(int _stage)
{
	stage = _stage;
}

int Map::GetStageNum()
{
	return stage;
}

void Map::Drawing(LPDIRECTDRAWSURFACE7 lpsurface, LPDIRECTDRAW7 lpDirectDrawObject)
{
	switch (stage)
	{
	case 0:
		m_pbassWorldMap->Drawing2(m_x, m_y, lpsurface, false, lpDirectDrawObject);
		break;
	case 1:
		m_pBossWorldMap->Drawing2(m_x, m_y, lpsurface, false, lpDirectDrawObject);
		break;
	default:
		break;
	}
}

void Map::DrawingRoof(LPDIRECTDRAWSURFACE7 lpsurface, LPDIRECTDRAW7 lpDirectDrawObject)
{
	switch (stage)
	{
	case 0:
		break;
	case 1:
		m_pBossWorldMapRoof->Drawing2(m_x, m_y, lpsurface, true, lpDirectDrawObject);
		break;
	default:
		break;
	}
}

void Map::NextStage()
{
	stage++;
}
