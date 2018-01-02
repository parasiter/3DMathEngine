#pragma once
#include"VECTOR3D.h"

// ���ڲ�����ֱ�ߺ������䷵��ֵ��
#define PARM_LINE_NO_INTERSECT_PARALLEL  1
#define PARM_LINE_INTERSECT_EVERYWHERE 2
#define PARM_LINE_INTERSECT_POINT 3
#define PARM_LINE_NO_INTERSECT_NO_PARALLEL 4
class PARMLINE3D {
public:
	VECTOR3D v0, v1;
	VECTOR3D n;//��������
	PARMLINE3D(const VECTOR3D &a, const VECTOR3D &b) :v0(a), v1(b)
	{
		n = b - a;
		if (n == ZEROVECTOR3D)
			//�׳��쳣
			n.normalized();
	}
	//�ཻ��⣬����ཻ���򽫽��㱣����intersect��
	int Intersect(const PARMLINE3D &a, VECTOR3D &intersect) const;
};