#include"VECTOR3D.h"

VECTOR3D& VECTOR3D::normalized()
{
	float value = x*x + y*y + z*z;
	value = sqrtf(value);
	if (value > EPSILON_E6)
	{
		x /= value;
		y /= value;
		z /= value;
	}
	return *this;
}

float Distance(const VECTOR3D &a, const VECTOR3D &b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return sqrt(dx*dx + dy*dy + dz*dz);
}