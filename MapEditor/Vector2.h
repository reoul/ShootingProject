#pragma once
#include <corecrt_math.h>
#include <cmath>

class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2&) = default;
	~Vector2() = default;
	void SetXY(float x_, float y_);
	Vector2 operator-(Vector2 vec) const;
	float Length() const;
	static float Distance(Vector2 v1, Vector2 v2);
	float Norm() const; //벡터의 길이 반환
	Vector2 Normalize() const;
	void Rotate(float degrees);
	int VectorToAngle() const;
public:
	float x;
	float y;
};

inline Vector2 Vector2::operator-(Vector2 vec) const
{
	return Vector2(this->x - vec.x, this->y - vec.y);
}

inline float Vector2::Length() const
{
	return sqrtf(x * x + y * y);
}

inline float Vector2::Distance(Vector2 v1, Vector2 v2)
{
	return (v2 - v1).Length();
}

inline float Vector2::Norm() const
{
	return std::hypot(x, y);
}
