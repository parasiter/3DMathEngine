#include"RotationMatrix.h"
#include"VECTOR3D.h"
#include"EulerAngles.h"
#include"Quaternion.h"

inline void RotationMatrix::identity()
{
	m11 = 1.0f, m12 = 0.0f, m13 = 0.0f;
	m21 = 0.0f, m22 = 1.0f, m23 = 0.0f;
	m31 = 0.0f, m32 = 0.0f, m33 = 1.0f;
}
//得到的是M（惯性->物体）
void RotationMatrix::setup(const EulerAngles&e)
{
	float sh, sp, sb;
	float ch, cp, cb;
	sinCos(&sh, &ch, e.heading);
	sinCos(&sp, &cp, e.pitch);
	sinCos(&sb, &cb, e.bank);

	m11 = ch*cb + sh*sp*sb;
	m12 = ch*sb + sh*sp*cb;
	m13 = -sh*cp;

	m21 = -sb*cp;
	m22 = cb*cp;
	m23 = sp;

	m31 = sh*cb + ch*sp*sb;
	m32 = sb*sh - ch*sp*cb;
	m33 = ch*cp;
}
void RotationMatrix::fromInterialToObjectQuaternion(const Quaternion &q)
{
	m11 = 1.0f - 2.0f*(q.y*q.y + q.z*q.z);
	m12 = 2.0f*(q.x*q.y + q.w*q.z);
	m13 = 2.0f*(q.x*q.z - q.w*q.y);

	m21 = 2.0f*(q.x*q.y - q.w*q.z);
	m22 = 1.0f - 2.0f*(q.x*q.x + q.z*q.z);
	m23 = 2.0f*(q.y*q.z + q.w*q.x);

	m31 = 2.0f*(q.x*q.z + q.w*q.y);
	m32 = 2.0f*(q.y*q.z - q.w*q.x);
	m33 = 1.0f - 2.0f*(q.x*q.x + q.y*q.y);
}
void RotationMatrix::fromObjectToInterrialQuaternion(const Quaternion&q)
{
	m11 = 1.0f - 2.0f*(q.y*q.y + q.z*q.z);
	m12 = 2.0f*(q.x*q.y - q.w*q.z);
	m13 = 2.0f*(q.x*q.z + q.w*q.y);

	m21 = 2.0f*(q.x*q.y + q.w*q.z);
	m22 = 1.0f - 2.0f*(q.x*q.x + q.z*q.z);
	m23 = 2.0f*(q.y*q.z - q.w*q.x);

	m31 = 2.0f*(q.x*q.z - q.w*q.y);
	m32 = 2.0f*(q.y*q.z + q.w*q.x);
	m33 = 1.0f - 2.0f*(q.x*q.x + q.y*q.y);
}
VECTOR3D RotationMatrix::intertialToObject(const VECTOR3D&v)const
{
	//矩阵默认为惯性->物体矩阵
	return VECTOR3D(
		m11*v.x + m21*v.y + m31*v.z,
		m12*v.x + m22*v.y + m32*v.z,
		m13*v.x + m23*v.y + m33*v.z );
}
VECTOR3D RotationMatrix::objectToIntertial(const VECTOR3D&v)const {
	//V乘以转置矩阵
	return VECTOR3D(
		m11*v.x + m12*v.y + m13*v.z,
		m21*v.x + m22*v.y + m23*v.z,
		m31*v.x + m32*v.y + m33*v.z);
}

VECTOR3D operator*(const VECTOR3D &p, const RotationMatrix&m)
{
	return VECTOR3D(
		p.x*m.m11 + p.y*m.m21 + p.z*m.m31,
		p.x*m.m12 + p.y*m.m22 + p.z*m.m32,
		p.x*m.m13 + p.y*m.m23 + p.z*m.m33
	);
}
RotationMatrix operator*(const RotationMatrix&a, const RotationMatrix&b)
{
	RotationMatrix r;
	r.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31;
	r.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32;
	r.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33;
	r.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31;
	r.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32;
	r.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33;
	r.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31;
	r.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32;
	r.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33;

	return r;
}
VECTOR3D &operator*=(VECTOR3D&p, const RotationMatrix&m)
{
	p = p*m;
	return p;
}
RotationMatrix &operator*=(RotationMatrix&a, const RotationMatrix&b)
{
	a = a*b;
	return a;
}

ostream& operator<<(ostream&out, const RotationMatrix&r)
{
	out << r.m11 << ' ' << r.m12 << ' ' << r.m13 << endl;
	out << r.m21 << ' ' << r.m22 << ' ' << r.m23 << endl;
	out << r.m31 << ' ' << r.m32 << ' ' << r.m33 << endl;
	return out;
}