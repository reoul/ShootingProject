#pragma once
#include <ddraw.h>
#include "Sprite.h"

class WorldMap
{
public:
	WorldMap();
	~WorldMap() = default;
	unsigned char* GetBMPBuffer() const;
	unsigned char* GetBMPBuffer2() const;
	LPDIRECTDRAWSURFACE7* GetSurface() const;
	bool LoadBMPFile(const char* filePath);
	bool CopyBufferToSurface(LPDIRECTDRAW7 pDirectDraw);
	bool CopyBufferToSurface2(LPDIRECTDRAW7 pDirectDraw);
	bool CopyBufferToSurface3(LPDIRECTDRAW7 pDirectDraw);
	bool CopyBufferToSurface4(LPDIRECTDRAW7 pDirectDraw);
	void CopyMapData();
	void InitMapData();
	bool Drawing(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey, LPDIRECTDRAW7 pDirectDraw) const;
	bool Drawing2(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey, LPDIRECTDRAW7 pDirectDraw) const;
	bool ReleaseAll() const;
	void Restore() const;
private:
	int mWidth;
	int mHeight;
	std::unique_ptr<unsigned char[]> mBuffer;
	std::unique_ptr<unsigned char[]> mBuffer2;
	std::unique_ptr<LPDIRECTDRAWSURFACE7> mSurface;
};

inline unsigned char* WorldMap::GetBMPBuffer() const
{
	return mBuffer.get();
}

inline unsigned char* WorldMap::GetBMPBuffer2() const
{
	return mBuffer2.get();
}

inline LPDIRECTDRAWSURFACE7* WorldMap::GetSurface() const
{
	return mSurface.get();
}
