#include"Quaternion.h"
#include"VECTOR3D.h"
#include"EulerAngles.h"
#include"Matrix4x3.h"
#include"RotationMatrix.h"

inline void Matrix4x3::identity()
{
	m11 = 1.0f, m12 = 0.0f, m13 = 0.0f;
	m21 = 0.0f, m22 = 1.0f, m23 = 0.0f;
	m11 = 0.0f, m12 = 0.0f, m13 = 1.0f;
	tx = ty = tz = 0.0f;
}
void Matrix4x3::zeroTranslation()
{
	tx = ty = tz = 0.0f;
}
void Matrix4x3::setTranslation(const VECTOR3D&v)
{
	tx = v.x;
	ty = v.y;
	tz = v.z;
}
void Matrix4x3::setupTranslation(const VECTOR3D&v)
{
	m11 = 1.0f, m12 = 0.0f, m13 = 0.0f;
	m21 = 0.0f, m22 = 1.0f, m23 = 0.0f;
	m11 = 0.0f, m12 = 0.0f, m13 = 1.0f;
	tx = v.x;
	ty = v.y;
	tz = v.z;
}
//构造从父坐标空间<->子坐标空间的转换
void Matrix4x3::setupLocalToParent(const VECTOR3D&pos, const EulerAngles &e)
{
	float sh, sp, sb;
	float ch, cp, cb;
	sinCos(&sh, &ch, e.heading);
	sinCos(&sp, &cp, e.pitch);
	sinCos(&sb, &cb, e.bank);

	m11 = ch*cb + sh*sp*sb;
	m12 = sb*cp;
	m13 = -sh*cb + ch*sp*sb;
	m21 = -ch*sb + sh*sp*cb;
	m22 = cb*cp;
	m23 = sb*sh + ch*sp*cb;
	m31 = sh*cp;
	m32 = -sp;
	m33 = ch*cp;

	tx = pos.x;
	ty = pos.y;
	tz = pos.z;
}
void Matrix4x3::setupLocalToParent(const VECTOR3D&pos, const RotationMatrix &m)
{
	//RotationMatrix 默认为M惯性->物体即，M父->子，则M子->父为转置矩阵。
	m11 = m.m11, m12 = m.m21, m13 = m.m31;
	m21 = m.m12, m22 = m.m22, m23 = m.m32;
	m31 = m.m13, m32 = m.m23, m33 = m.m33;
	tx = pos.x, ty = pos.y, tz = pos.z;
}

//旋转先于平移发生，平移向量是在父空间中描述的。
void Matrix4x3::setupParentToLocal(const VECTOR3D&pos, const EulerAngles &e)
{
	float sh, sp, sb;
	float ch, cp, cb;
	sinCos(&sh, &ch, e.heading);
	sinCos(&sp, &cp, e.pitch);
	sinCos(&sb, &cb, e.bank);

	m11 = ch*cb + sh*sp*sb;
	m12 = -ch*sb + sh*sp*cb;
	m13 = sh*cp;
	m21 = sb*cp;
	m22 = cb*cp;
	m23 = -sp;
	m31 = -sh*cb + ch*sp*sb;
	m32 = sb*sh + ch*sp*cb;
	m33 = ch*cp;
	//需要将平移向量进行旋转，并取负值。
	tx = -(pos.x*m11 + pos.y*m21 + pos.z*m31);
	ty = -(pos.x*m12 + pos.y*m22 + pos.z*m32);
	tz = -(pos.x*m13 + pos.y*m23 + pos.z*m33);
}
void Matrix4x3::setupParentToLocal(const VECTOR3D&pos, const RotationMatrix&m) 
{
	m11 = m.m11, m12 = m.m12, m13 = m.m13;
	m21 = m.m21, m22 = m.m22, m23 = m.m23;
	m31 = m.m31, m32 = m.m32, m33 = m.m33;

	tx = -(pos.x*m11 + pos.y*m21 + pos.z*m31);
	ty = -(pos.x*m12 + pos.y*m22 + pos.z*m32);
	tz = -(pos.x*m13 + pos.y*m23 + pos.z*m33);
}

void Matrix4x3::setupRotate(int axis, float theta)
{
	float s, c;
	sinCos(&s, &c, theta);
	switch (axis)
	{
	case 1:
		m11 = 1.0f, m12 = 0.0f, m13 = 0.0f;
		m21 = 0.0f, m22 = c, m23 = s;
		m31 = 0.0f, m32 = -s, m33 = c;
		break;
	case 2:
		m11 = c, m12 = 0.0f, m13 = -s;
		m21 = 0.0f, m22 = 1.0f, m23 = 0.0f;
		m31 = s, m32 = 0.0f, m33 = c;
		break;
	case 3:
		m11 = c, m12 = s, m13 = 0.0f;
		m21 = -s, m22 = c, m23 = 0.0f;
		m31 = 0.0f, m32 = 0.0f, m33 = 1.0f;
		break;
	default:
		break;
	}
	tx = ty = tz = 0.0f;
}
void Matrix4x3::setupRotate(const VECTOR3D& axis, float theta)
{
	float s, c;
	sinCos(&s, &c, theta);
	float a = 1.0f - c;
	float ax = a*axis.x;
	float ay = a*axis.y;
	float az = a*axis.z;

	m11 = ax*axis.x + c;
	m12 = ax*axis.y + axis.z*s;
	m13 = ax*axis.z - axis.y*s;

	m21 = ay*axis.x - axis.z*s;
	m22 = ay*axis.y + c;
	m23 = ay*axis.z + axis.x*s;

	m31 = az*axis.x + axis.y*s;
	m32 = az*axis.y - axis.x*s;
	m33 = az*axis.z + c;

	tx = ty = tz = 0.0f;
}
void Matrix4x3::fromQuaternion(const Quaternion &q)
{
	float ww = 2.0f*q.w;
	float xx = 2.0f*q.x;
	float yy = 2.0f*q.y;
	float zz = 2.0f*q.z;

	m11 = 1.0f - yy*q.y - zz*q.z;
	m12 = xx*q.y + ww*q.z;
	m13 = xx*q.z - ww*q.x;

	m21 = xx*q.y - ww*q.z;
	m22 = 1.0f - xx*q.x - zz*q.z;
	m23 = yy*q.z + ww*q.x;

	m31 = xx*q.z + ww*q.y;
	m32 = yy*q.z - ww*q.x;
	m33 = 1.0f - xx*q.x - yy*q.y;

	tx = ty = tz = 0.0f;
}
void Matrix4x3::setupScale(const VECTOR3D&s)
{
	m11 = s.x, m12 = 0.0f, m13 = 0.0f;
	m21 = 0.0f, m22 = s.y, m23 = 0.0f;
	m31 = 0.0f, m32 = 0.0f, m33 = s.z;
	tx = ty = tz = 0.0f;
}
void Matrix4x3::setupScaleAlongAxis(const VECTOR3D&axis, float k);
void Matrix4x3::setupShear(int axis, float s, float t);
void Matrix4x3::setupProject(const VECTOR3D&n);
void Matrix4x3::setupReflect(int axis, float k = 0.0f);
void Matrix4x3::setupReflect(const VECTOR3D&);

VECTOR3D operator*(const VECTOR3D &, const Matrix4x3&);
Matrix4x3 operator*(const Matrix4x3&, const Matrix4x3&);
VECTOR3D &operator*=(VECTOR3D&p, const Matrix4x3&);
Matrix4x3 &operator*=(Matrix4x3&, const Matrix4x3&);
float determinant(const Matrix4x3&);
Matrix4x3 inverse(const Matrix4x3&);
VECTOR3D getTranslation(const Matrix4x3&);
VECTOR3D getPositionFromParentToLocalMatrix(const Matrix4x3&);
VECTOR3D getPositionFromLocalToParent(const Matrix4x3&);