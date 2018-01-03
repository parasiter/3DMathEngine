#pragma once
#include"VECTOR3D.h"
#include"Parmline.h"

#define PARMLINE_PLANE_NO_INTERSECT 0x0001
#define PARMLINE_PLANE_INTERSECT_IN_SEGMENT 0x0002
#define PARMLINE_PLANE_INTERSECT_OUT_SEGMENT 0x0003
#define PARMLINE_PLANE_INTERSECT_EVERYWHERE 0x0004
class Plane {
public:
	VECTOR3D point;
	VECTOR3D normal;
	Plane() {}
	Plane(VECTOR3D p, VECTOR3D normal, int normalize = 1);
};

float ComputePointInPlane(const VECTOR3D&v, const Plane& p);//计算点在面的何处
int IntersectParmLineToPlane(const PARMLINE3D& line, const Plane&p,VECTOR3D& intersect);//计算直线和平面的交点


