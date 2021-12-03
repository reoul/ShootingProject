#ifndef __vector2_h__
#define __vector2_h__

#include <math.h>
#include <algorithm>

class Vector2
{
public:
	float x;
	float y;

public:
	Vector2();
	Vector2(float _x, float _y);
	~Vector2();
	void SetRect(float _x, float _y);
	Vector2 operator- (Vector2 vec);
	float Length();
	float Distance(Vector2 v1, Vector2 v2);
	double norm();			//벡터의 길이 반환
	Vector2 normalize();
	void Rotate(float degrees);
	int VectorToAngle();
};

#endif