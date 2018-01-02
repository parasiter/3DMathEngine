#pragma once
#include"VECTOR3D.h"

// 用于参数化直线函数及其返回值。
#define PARM_LINE_NO_INTERSECT_PARALLEL  1
#define PARM_LINE_INTERSECT_EVERYWHERE 2
#define PARM_LINE_INTERSECT_POINT 3
#define PARM_LINE_NO_INTERSECT_NO_PARALLEL 4
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
};