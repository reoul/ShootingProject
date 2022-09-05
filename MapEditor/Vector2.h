#pragma once

class Vector2
{
public:
	Vector2();
	Vector2(float x_, float y_);
	Vector2(const Vector2&) = default;
	~Vector2() = default;
	void SetXY(float x_, float y_);
	Vector2 operator-(Vector2 vec);
	float Length() const;
	static float Distance(Vector2 v1, Vector2 v2);
	float norm() const; //벡터의 길이 반환
	Vector2 normalize() const;
	void Rotate(float degrees);
	int VectorToAngle();
public:
	float x;
	float y;
};
