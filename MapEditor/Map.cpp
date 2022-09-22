#include "Map.h"

#include <cassert>

#include "WorldMapBmp.h"

Map::Map()
	: mX(0)
	, mY(0)
	, mStage(0)
	, mBaseWorldMapPtr(nullptr)
	, mBossWorldMapPtr(nullptr)
	, mBossWorldMapRoofPtr(nullptr)
{
}

void Map::SetWorldMap(WorldMap* baseMapPtr, WorldMap* bossWorldMapPtr, WorldMap* bossWorldMapRoofPtr)
{
	mBaseWorldMapPtr = baseMapPtr;
	mBossWorldMapPtr = bossWorldMapPtr;
	mBossWorldMapRoofPtr = bossWorldMapRoofPtr;
}

void Map::Init()
{
	mX = 0;
	mY = 0;
	mStage = 0;
}

void Map::Drawing(LPDIRECTDRAWSURFACE7 lpSurface, LPDIRECTDRAW7 lpDirectDrawObject) const
{
	switch (mStage)
	{
	case 0:
		mBaseWorldMapPtr->Drawing2(mX, mY, lpSurface, false, lpDirectDrawObject);
		break;
	case 1:
		mBossWorldMapPtr->Drawing2(mX, mY, lpSurface, false, lpDirectDrawObject);
		break;
	default:
		assert(false);
		break;
	}
}

void Map::DrawingRoof(LPDIRECTDRAWSURFACE7 lpSurface, LPDIRECTDRAW7 lpDirectDrawObject) const
{
	switch (mStage)
	{
	case 0:
		break;
	case 1:
		mBossWorldMapRoofPtr->Drawing2(mX, mY, lpSurface, true, lpDirectDrawObject);
		break;
	default:
		assert(false);
		break;
	}
}
