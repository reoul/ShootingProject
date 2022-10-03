#include "Vector2.h"

#define PI 3.141592f

Vector2::Vector2()
	: x(0)
	, y(0)
{
}

Vector2::Vector2(float x, float y)
	: x(x)
	, y(y)
{
}

Vector2 Vector2::Normalize() const
{
	Vector2 vec = Vector2(x / Norm(), y / Norm());
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

	float tX = x;
	float tY = y;
	x = (cos * tX) - (sin * tY);
	y = (sin * tX) + (cos * tY);
}

//참고: https://3dmpengines.tistory.com/174 [3DMP]
/**
 * 방향벡터를 각도로 변환
 * \return 각도
 */
int Vector2::ToAngle() const
{
	Vector2 dir = Vector2(x, -y);
	if (dir.x > 1)
		dir.x = 1;
	else if (dir.x < -1)
		dir.x = -1;
	if (dir.y > 1)
		dir.y = 1;
	else if (dir.y < -1)
		dir.y = -1;

	float angle = acos(dir.x) * 180 / PI;

	const Vector2 toDir = (Vector2(1, 0) - Vector2(x, y)).Normalize();
	const float cross = (toDir.x * 0) - (toDir.y * 1); //어느방향으로 돌지 계산
	
	if (cross > 0) //외적 값이 음수면 시계방향으로 회전
		angle = 360 - angle;

	return angle;
}
