#pragma once
#include"Vector.h"
class PARMLINE3D {
public:
	VECTOR3D v0, v1;
	VECTOR3D n;//方向向量
	PARMLINE3D(const VECTOR3D &a, const VECTOR3D &b) :v0(a), v1(b)
	{
		n = b - a;
		if (n == ZEROVECTOR3D)
			//抛出异常
			n.normalized();
	}
	//相交检测，如果相交，则将交点保存在intersect中
	int Intersect(const PARMLINE3D &a, VECTOR3D &intersect) const;
	{
		if (n == a.n)
		{
			VECTOR3D temp = v0 - a.v0;
			if (temp == ZEROVECTOR3D || temp.normalized() == n)
				return PARM_LINE_INTERSECT_EVERYWHERE;
			else
				return PARM_LINE_NO_INTERSECT_PARALLEL;
		}
		//相交计算，使用参数形式，(vector)r = (vector)v0 + (float)t*(vector)n;
		float t1 = ((v0 - a.v0)*a.n).dot(n*a.n) / VECTOR3DMSG(n*a.n);
		float t2 = ((v0 - a.v0)*n).dot(n*a.n) / VECTOR3DMSG(n*a.n);
		VECTOR3D point1 = v0 + t1*n;
		VECTOR3D point2 = a.v0 + t2*a.n;
		if (point1 == point2)
		{
			intersect = point1;
			return PARM_LINE_INTERSECT_POINT;
		}
		else
			return PARM_LINE_NO_INTERSECT_NO_PARALLEL;
	}
};