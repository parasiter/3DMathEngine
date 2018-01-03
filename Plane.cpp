#include"Plane.h"

Plane::Plane(VECTOR3D p, VECTOR3D n, int normalize) {
	point = p;
	normal = n;
	if (normalize)
		n = n.normalized();
}

float ComputePointInPlane(const VECTOR3D&v, const Plane& p)
{
	return v.dot(p.normal);
}
int IntersectParmLineToPlane(const PARMLINE3D& line, const Plane& p,VECTOR3D& intersect)
{
	float d = p.point.dot(p.normal);
	float a = line.v0.dot(p.normal);
	float b = line.n.dot(p.normal);
	if (b < EPSILON_E4)//Æ½ÐÐ
	{
		if (fabsf(d - a) < EPSILON_E4)//ÖØµþ
			return PARMLINE_PLANE_INTERSECT_EVERYWHERE;
		else
		{
			return PARMLINE_PLANE_NO_INTERSECT;
		}
	}
	float t = (d - a) / b;
	intersect = line.v1 + t*line.n;
	if (t >= 0.0f&&t <= 1.0f)
		return PARMLINE_PLANE_INTERSECT_IN_SEGMENT;
	else
		return PARMLINE_PLANE_INTERSECT_OUT_SEGMENT;
}