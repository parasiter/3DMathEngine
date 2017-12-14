#pragma once
#include"MathUtil.h"

#define ZEROVECTOR3D VECTOR3D(0.0f,0.0,0.0f)

class VECTOR3D {
public:
	float x, y, z;
	VECTOR3D() :x(1), y(1), z(1) {}
	VECTOR3D(float n1, float n2, float n3) :x(n1), y(n2), z(n3) {}
	VECTOR3D(const VECTOR3D &a) :x(a.x), y(a.y), z(a.z) {}
	inline VECTOR3D &operator=(const VECTOR3D &a) {
		x = a.x;
		y = a.y;
		z = a.y;
		return *this;
	}
	inline bool operator==(const VECTOR3D &a) const
	{
		return (fabs(x - a.x)<EPSILON_E6) && (abs(y - a.y)<EPSILON_E6) && (abs(z - a.z)<EPSILON_E6);
	}
	inline bool operator!=(const VECTOR3D &a) const
	{
		return (fabs(x - a.x)>EPSILON_E6) || (abs(y - a.y)>EPSILON_E6) || (abs(z - a.z)>EPSILON_E6);
	}
	inline VECTOR3D operator+(const VECTOR3D &a) const
	{
		return VECTOR3D(x + a.x, y + a.y, z + a.z);
	}
	inline VECTOR3D operator-(const VECTOR3D &a) const
	{
		return VECTOR3D(x - a.x, y - a.y, z - a.z);
	}
	inline VECTOR3D operator*(float a) const
	{
		return VECTOR3D(a*x, a*y, a*z);
	}
	inline VECTOR3D operator/(float a) const
	{
		return VECTOR3D(x / a, y / a, z / a);
	}

	inline VECTOR3D& operator+=(const VECTOR3D &a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	inline VECTOR3D& operator-=(const VECTOR3D &a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}

	inline VECTOR3D& operator*= (float a)
	{
		x *= a;
		y *= a;
		z *= z;
		return *this;
	}
	inline VECTOR3D& operator/= (float a)
	{
		x /= a;
		y /= a;
		z /= a;
		return *this;
	}

	//点乘                            
	inline float dot(const VECTOR3D &v3d) const
	{
		return x*v3d.x + y*v3d.y + z*v3d.z;
	}
	//叉乘
	inline VECTOR3D operator*(const VECTOR3D &v) const
	{
		return VECTOR3D(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}

	VECTOR3D& normalized();
	inline void SetZero() { x = 0.0f, y = 0.0f, z = 0.0f; }
};
//非成员函数
//标量左乘
inline VECTOR3D operator*(float k, const VECTOR3D &a)
{
	return VECTOR3D(k*a.x, k*a.y, k*a.z);
}
//两点距离
extern float Distance(const VECTOR3D &a, const VECTOR3D &b);


inline float VECTOR3DMSG(const VECTOR3D &a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}
