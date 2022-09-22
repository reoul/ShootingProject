#pragma once
#include <ddraw.h>
#include <memory>

class Bmp
{
public:
	Bmp();
	~Bmp() = default;
	int GetWidth() const;
	int GetHeight() const;
	unsigned char* GetBMPBuffer() const;
	bool LoadBMPFile(const char* filePath);
	bool CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface) const;
	bool CopyBufferToSurface(LPDIRECTDRAWSURFACE7 lpSurface, unsigned char* buffer) const;
	bool SaveWorldBMP(int x, int y, unsigned char* buffer, unsigned char* buffer2) const;
private:
	int mWidth;
	int mHeight;
	std::unique_ptr<unsigned char[]> mBufferPtr;
};

inline int Bmp::GetWidth() const
{
	return mWidth;
}


inline int Bmp::GetHeight() const
{
	return mHeight;
}

inline unsigned char* Bmp::GetBMPBuffer() const
{
	return mBufferPtr.get();
}
