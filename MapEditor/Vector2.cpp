#include "Vector2.h"
#include <corecrt_math.h>

#define PI 3.141592

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(float _x, float _y)
{
	x = _x;
	y = _y;
}

Vector2::~Vector2()
{
}

void Vector2::SetRect(float _x, float _y)
{
	x = _x;
	y = _y;
}

Vector2 Vector2::operator-(Vector2 vec)
{
	return Vector2(this->x - vec.x, this->y - vec.y);
}

float Vector2::Length()
{
	return sqrtf(x * x + y * y);
}

float Vector2::Distance(Vector2 v1, Vector2 v2)
{
	return (v2 - v1).Length();
}

double Vector2::norm()
{
	return hypot(x, y);
}

Vector2 Vector2::normalize()
{
	Vector2 vec = Vector2(x / norm(), y / norm());
	if (vec.x > 1)
		vec.x = 1;
	else if (vec.x < -1)
		vec.x = -1;
	if (vec.y > 1)
		vec.y = 1;
	else if (vec.y < -1)
		vec.y = -1;
	return vec;
}

void Vector2::Rotate(float degrees)
{
	float sin = sinf(degrees * (PI * 2) / 360);
	float cos = cosf(degrees * (PI * 2) / 360);

	float tx = x;
	float ty = y;
	x = (cos * tx) - (sin * ty);
	y = (sin * tx) + (cos * ty);
}

int Vector2::VectorToAngle()
{
	Vector2 a = Vector2(x, -y);
	if (a.x > 1)
		a.x = 1;
	else if (a.x < -1)
		a.x = -1;
	if (a.y > 1)
		a.y = 1;
	else if (a.y < -1)
		a.y = -1;
	//Vector2 a = Vector2(-tmp.y,tmp.x);

	float angle = acos(a.x) * 180 / PI;

	Vector2 toDir = Vector2(1, 0) - Vector2(x, y);
	toDir = toDir.normalize();
	float cross = (toDir.x * 0) - (toDir.y * 1); //어느방향으로 돌지 계산

	//float angle = acosf((toDir.x * m_direction.x) + (toDir.y * m_direction.y));		//두 벡터의 내각(0~180 양수)
	//if (isnan(angle))
	//	angle = 0;
	if (cross > 0) //외적 값이 음수면 시계방향으로 회전
	{
		angle = 360 - angle;
	}

	//float fAng = acos((a.x * 0 + a.y * 0) / (sqrt(a.x * a.x + a.y * a.y) * sqrt(0))) * 360 / PI;

	//fAng의 크기에 따라 두점사이의 각도를 다른 방식으로 구함.
	//if (fAng > 90)
	//	return (int)(360 - fAng);

	//else
	//	return (int)fAng;


	//출처: https://3dmpengines.tistory.com/174 [3DMP]

	return (int)angle;
}
