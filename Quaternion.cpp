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

void Quaternion::setToRotateObjectToIntertial(const EulerAngles& e)
{
	float sp, sb, sh;
	float cp, cb, ch;
	sinCos(&sp, &cp, e.pitch*0.5f);
	sinCos(&sb, &cb, e.bank*0.5f);
	sinCos(&sh, &ch, e.heading*0.5f);
	w = ch*cp*cb + sh*sp*sb;
	x = ch*sp*cb + sh*cp*sb;
	y = -ch*sp*sb + sh*cp*cb;
	z = -sh*sp*cb + ch*cp*sb;

}
void Quaternion::setToRotateIntertialToObject(const EulerAngles&e)
{
	float sp, sb, sh;
	float cp, cb, ch;
	sinCos(&sp, &cp, e.pitch*0.5f);
	sinCos(&sb, &cb, e.bank*0.5f);
	sinCos(&sh, &ch, e.heading*0.5f);
	w = ch*cp*cb + sh*sp*sb;
	x = -ch*sp*cb - sh*cp*sb;
	y = ch*sp*sb - sh*cp*cb;
	z = sh*sp*cb - ch*cp*sb;
}


Quaternion Quaternion::operator*(const Quaternion& q) const
{
	Quaternion result;
	result.w = w*q.w - x*q.x - y*q.y - z*q.z;
	result.x = w*q.x + x*q.w + z*q.y - y*q.z;
	result.y = w*q.y + y*q.w + x*q.z - z*q.x;
	result.z = w*q.z + z*q.w + y*q.x - x*q.y;
	return result;
}
Quaternion Quaternion::operator*=(const Quaternion&q)
{
	*this = *this *q;
	return *this;
}

void Quaternion::normalize()
{
	float mag = sqrtf(w*w + x*x + y*y + z*z);
	if (mag > 0.0f)
	{
		float divide = 1.0f / mag;
		w *= divide;
		x *= divide;
		y *= divide;
		z *= divide;
	}
	else
	{
		//出错，返回单位四元数
		identity();
	}
}

float Quaternion::getRotationAngle() const
{
	float halfTheta = safeAcos(w);
	return halfTheta*2.0f;
}
VECTOR3D Quaternion::getRotationAxis() const
{
	float sinThetaSq = 1.0f - w*w;
	if (sinThetaSq <= EPSILON_E6)
	{
		//单位四元数，只需返回有效的向量
		return VECTOR3D(1.0f, 0.0f, 0.0f);
	}
	else {
		float oneOverSinThetaOver2 = 1.0f / sqrtf(sinThetaSq);
		return VECTOR3D(x*oneOverSinThetaOver2, y*oneOverSinThetaOver2, z*oneOverSinThetaOver2);
	}
}

float dotProduct(const Quaternion&q1, const Quaternion&q2)
{
	return q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
}
Quaternion slerp(const Quaternion&q0, const Quaternion&q1, float t)
{
	//理论上，qt = q0*(q0-1 * q1)t;

}
//四元数共轭
Quaternion conjugate(const Quaternion&);
//四元数的幂
Quaternion pow(const Quaternion &q, float exponent);