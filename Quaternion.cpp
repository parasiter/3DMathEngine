#include"Quaternion.h"
#include"VECTOR3D.h"
#include"EulerAngles.h"
#include"MathUtil.h"

const Quaternion QuaternionIdentity = { 1.0f,0.0f, 0.0f,0.0f };

void Quaternion::setToRotateAboutX(float theta)
{
	float halftheta = theta *0.5f;
	w = cos(halftheta);
	x = sin(halftheta);
	y = 0.0f;
	z = 0.0f;
}

void Quaternion::setToRotateAboutY(float theta)
{
	float halftheta = theta *0.5f;
	w = cos(halftheta);
	x = 0.0f;
	y = sin(halftheta);
	z = 0.0f;
}

void Quaternion::setToRotateAboutZ(float theta)
{
	float halftheta = theta *0.5f;
	w = cos(halftheta);
	x = 0.0f;
	y = 0.0f;
	z = sin(halftheta);
}

void Quaternion::setToRotateAboutAxis(const VECTOR3D& axis,float theta)
{
	float halftheta = theta *0.5f;
	float sin_halftheta = sin(halftheta);
	w = cos(halftheta);
	x = axis.x*sin_halftheta;
	y = axis.y*sin_halftheta;
	z = axis.z*sin_halftheta;
}

void Quaternion::setToRotateObjectToIntertial(const EulerAngles&)
{
	////
}
void Quaternion::setToRotateIntertialToObject(const EulerAngles&)
{

}


Quaternion Quaternion::operator*(const Quaternion&) const
{

}
Quaternion Quaternion::operator*=(const Quaternion&)
{

}

void Quaternion::normalize()
{

}

float Quaternion::getRotationAngle() const
{

}
VECTOR3D Quaternion::getRotationAxis() const
{

}

const Quaternion QuaternionIdentity;
float dotProduct(const Quaternion&, const Quaternion&);
Quaternion slerp(const Quaternion&, const Quaternion&, float t);
//四元数共轭
Quaternion conjugate(const Quaternion&);
//四元数的幂
Quaternion pow(const Quaternion &q, float exponent);