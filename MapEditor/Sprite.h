#pragma once
#include <ddraw.h>
#include <memory>

class Bmp;

class Sprite
{
public:
	Sprite();
	~Sprite() = default;
	int GetNumberOfFrame() const;
	Bmp* GetBMP() const;
	bool InitSprite(int frame, int width, int height, int colorKey, LPDIRECTDRAW7 pDirectDraw);
	bool LoadFrame(int frame, const char* filePath) const;
	bool Drawing(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true) const;
	bool Drawing(RECT rect, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true) const;
	bool Drawing2(int frame, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true) const;
	bool DrawingSkillCoolTime(float coolTimePercent, int x, int y, LPDIRECTDRAWSURFACE7 pSurface,
		bool bUsingColorKey = true) const;
	bool BlockDrawing(int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true) const;
	bool DrawingBossHp(int frame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true) const;
	bool Restore() const;
	bool ReleaseAll() const;
	void SetSize(float size);
	void SetNumber(int number);
	float GetSize() const;
	int GetNumber() const;
	int GetWidth() const;
	int GetHeight() const;
	void Rotate(double degree, int curFrame) const; //Degree는 회전값, 시계 반대방향으로 돈다
private:
	int mFrame;
	int mWidth;
	int mHeight;
	int mDrawWidth;
	int mDrawHeight;
	float mSize;
	int mNumber;

	std::unique_ptr<LPDIRECTDRAWSURFACE7[]> mSurfacePtr; //원본 이미지 surface
	std::unique_ptr<LPDIRECTDRAWSURFACE7[]> mDrawSurfacePtr; //출력 이미지 surface
	std::unique_ptr<Bmp[]> mBMPArray;
};

inline Bmp* Sprite::GetBMP() const
{
	return mBMPArray.get();
}

inline int Sprite::GetNumberOfFrame() const
{
	return mFrame;
}

inline void Sprite::SetSize(float size)
{
	mSize = size;
	mDrawHeight = mHeight * mSize;
	mDrawWidth = mWidth * mSize;
}

inline void Sprite::SetNumber(int number)
{
	mNumber = number;
}

inline float Sprite::GetSize() const
{
	return mSize;
}

inline int Sprite::GetNumber() const
{
	return mNumber;
}

inline int Sprite::GetWidth() const
{
	return mWidth;
}

inline int Sprite::GetHeight() const
{
	return mHeight;
}
