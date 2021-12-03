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
	CWorldMap* m_pbassWorldMap;			//�⺻ ���� �� ��� �̹���
	CWorldMap* m_pBossWorldMap;			//�⺻ ���� �� ��� �̹���
	CWorldMap* m_pBossWorldMapRoof;		//�⺻ ��� ���� ���� �̹���
public:
	Map();
	~Map();
	void SetWorldMap(CWorldMap* baseMap, CWorldMap* bossWorldMap, CWorldMap* bossWorldMapRoof);
	void SetPos(int x, int y);
	void SetStage(int _stage);
	int GetStage();
	void Drawing(LPDIRECTDRAWSURFACE7 lpsurface, LPDIRECTDRAW7 lpDirectDrawObject);			//�⺻ ���
	void DrawingRoof(LPDIRECTDRAWSURFACE7 lpsurface, LPDIRECTDRAW7 lpDirectDrawObject);		//��� ���� ���� �̹��� �׷��ٶ�
	void NextStage();
};

#endif