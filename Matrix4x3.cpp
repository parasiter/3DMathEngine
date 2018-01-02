#include"Quaternion.h"
#include"VECTOR3D.h"
#include"EulerAngles.h"
#include"Matrix4x3.h"
#include"RotationMatrix.h"
#include<cassert>

Matrix4x3::Matrix4x3(float* f)
{
	memcpy(data, f, 48);
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
//����Ӹ�����ռ�<->������ռ��ת��
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
	//RotationMatrix Ĭ��ΪM����->���弴��M��->�ӣ���M��->��Ϊת�þ���
	m11 = m.m11, m12 = m.m21, m13 = m.m31;
	m21 = m.m12, m22 = m.m22, m23 = m.m32;
	m31 = m.m13, m32 = m.m23, m33 = m.m33;
	tx = pos.x, ty = pos.y, tz = pos.z;
}

//��ת����ƽ�Ʒ�����ƽ���������ڸ��ռ��������ġ�
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
	//��Ҫ��ƽ������������ת����ȡ��ֵ��
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
//������������k��������Ϊ��λ����
void Matrix4x3::setupScaleAlongAxis(const VECTOR3D&axis, float k)
{
	assert(fabs(axis.dot(axis) - 1.0f) < EPSILON_E4);
	//V.dot(N)*N*k+V-V.dot(N)*N;
	float a = k = 1.0f;
	float ax = a*axis.x;
	float ay = a*axis.y;
	float az = a*axis.z;

	m11 = ax*axis.x + 1.0f;
	m22 = ay*axis.y + 1.0f;
	m33 = az*axis.z + 1.0f;

	m12 = m21 = ax*axis.y;
	m13 = m31 = ax*axis.z;
	m23 = m32 = ay*axis.z;

	tx = ty = tz = 0.0f;
}
void Matrix4x3::setupShear(int axis, float s, float t)
{
	//�б���ȡ��һ�����꣬���Բ�ͬ�������ټӵ��������������ϡ�
	//�б�������axis������ָʾ��
	switch(axis) {
	case 1 :
		m11 = 1.0f, m12 = s, m13 = t;
		m21 = 0.0f, m22 = 1.0f, m23 = 0.0f;
		m31 = 0.0f, m32 = 0.0f, m33 = 1.0f;
		break;
	case 2:
		m11 = 1.0f, m13 = 0.0f, m13 = 0.0f;
		m21 = s, m22 = 1.0f, m23 = t;
		m31 = 0.0f, m32 = 0.0f, m33 = 1.0f;
		break;
	case 3:
		m11 = 1.0f, m13 = 0.0f, m13 = 0.0f;
		m21 = 0.0f, m22 = 1.0f, m23 = 0.0f;
		m31 = s, m32 = t, m33 = 1.0f;
		break;
	default:
		assert(false);
	}
	tx = ty = tz = 0.0f;
	
}
//ͶӰ����
void Matrix4x3::setupProject(const VECTOR3D&n)
{
	assert(fabs(n.dot(n) - 1.0f < EPSILON_E4));
	
	m11 = 1.0f - n.x*n.x;
	m22 = 1.0f - n.y*n.y;
	m33 = 1.0f - n.z*n.z;

	m12 = m21 = -n.x*n.y;
	m13 = m31 = -n.x*n.z;
	m23 = m32 = -n.y*n.z;

	tx = ty = tz = 0.0f;
}
//�������ᷴ��
void Matrix4x3::setupReflect(int axis, float k)
{
	switch (axis)
	{
	case 1:
		m11 = -1.0f, m12 = 0.0f, m13 = 0.0f;
		m21 = 0.0f, m22 = 1.0f, m23 = 0.0f;
		m31 = 0.0f, m32 = 0.0f, m33 = 1.0f;

		tx = 2.0f*k;
		ty = 0.0f;
		tz = 0.0f;
		break;
	case 2:
		m11 = 1.0f, m12 = 0.0f, m13 = 0.0f;
		m21 = 0.0f, m22 = -1.0f, m23 = 0.0f;
		m31 = 0.0f, m32 = 0.0f, m33 = 1.0f;

		tx = 0.0f;
		ty = 2.0f*k;
		tz = 0.0f;
		break;
	case 3:
		m11 = 1.0f, m12 = 0.0f, m13 = 0.0f;
		m21 = 0.0f, m22 = 1.0f, m23 = 0.0f;
		m31 = 0.0f, m32 = 0.0f, m33 = -1.0f;

		tx = 0.0f;
		ty = 0.0f;
		tz = 2.0f*k;
		break;
	default : 
		assert(false);
	}
}
void Matrix4x3::setupReflect(const VECTOR3D&n)
{
	assert(fabs(n.dot(n) - 1.0f) < EPSILON_E4);
	float ax = -2.0f*n.x;
	float ay = -2.0f*n.y;
	float az = -2.0f*n.z;

	m11 = 1.0f + ax*n.x;
	m22 = 1.0f + ay*n.y;
	m33 = 1.0f + az*n.z;

	m12 = m21 = ax*n.y;
	m13 = m31 = ax*n.z;
	m23 = m32 = ay*n.z;

	tx = ty = tz = 0.0f;
}

VECTOR3D operator*(const VECTOR3D &p, const Matrix4x3&m)
{
	return VECTOR3D(
		p.x*m.m11 + p.y*m.m21 + p.z*m.m31 + m.tx,
		p.x*m.m12 + p.y*m.m22 + p.z*m.m32 + m.ty,
		p.x*m.m13 + p.y*m.m23 + p.z*m.m33 + m.tx
	);
}
Matrix4x3 operator*(const Matrix4x3&a, const Matrix4x3&b)
{
	Matrix4x3 r;
	r.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31;
	r.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32;
	r.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33;
	r.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31;
	r.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32;
	r.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33;
	r.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31;
	r.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32;
	r.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33;

	
	r.tx = a.tx*b.m11 + a.ty*b.m21 + a.tz*b.m31 + b.tx;
	r.ty = a.tx*b.m12 + a.ty*b.m22 + a.tz*b.m32 + b.ty;
	r.tz = a.tx*b.m13 + a.ty*b.m23 + a.tz*b.m33 + b.tz;
	return r;
}
VECTOR3D &operator*=(VECTOR3D&p, const Matrix4x3&m)
{
	p = p*m;
	return p;
}
Matrix4x3 &operator*=(Matrix4x3&a, const Matrix4x3&b)
{
	a = a*b;
	return a;
}
float determinant(const Matrix4x3&m)
{
	return m.m11*(m.m22*m.m33 - m.m23*m.m32) +
		m.m12*(m.m23*m.m31 - m.m21*m.m33) +
		m.m13*(m.m21*m.m32 - m.m22*m.m31);
}
Matrix4x3 inverse(const Matrix4x3&m)
{
	float det = determinant(m);
	assert(fabs(det) > EPSILON_E6);
	float oneOverDet = 1.0f / det;

	Matrix4x3 r;
	r.m11 = (m.m22*m.m33 - m.m23*m.m32)*oneOverDet;
	r.m12 = (m.m13*m.m32 - m.m12*m.m33)*oneOverDet;
	r.m13 = (m.m12*m.m23 - m.m13*m.m22)*oneOverDet;
	r.m21 = (m.m23*m.m31 - m.m21*m.m33)*oneOverDet;
	r.m22 = (m.m11*m.m33 - m.m13*m.m31)*oneOverDet;
	r.m23 = (m.m13*m.m21 - m.m11*m.m23)*oneOverDet;
	r.m31 = (m.m21*m.m32 - m.m22*m.m31)*oneOverDet;
	r.m32 = (m.m12*m.m31 - m.m11*m.m32)*oneOverDet;
	r.m33 = (m.m11*m.m22 - m.m12*m.m21)*oneOverDet;


	r.tx = -(m.tx*r.m11 + m.ty*r.m21 + m.tz*r.m31);
	r.ty = -(m.tx*r.m12 + m.ty*r.m22 + m.tz*r.m32);
	r.tz = -(m.tx*r.m13 + m.ty*r.m23 + m.tz*r.m33);
	return r;
}
VECTOR3D getTranslation(const Matrix4x3&m)
{
	return VECTOR3D(m.tx, m.ty, m.tz);
}
VECTOR3D getPositionFromParentToLocalMatrix(const Matrix4x3&m)
{
	return VECTOR3D(
		-(m.tx*m.m11 + m.ty*m.m12 + m.tz*m.m13),
		-(m.tx*m.m21 + m.ty*m.m22 + m.tz*m.m23),
		-(m.tx*m.m31 + m.ty*m.m32 + m.tz*m.m33)
	);
}
VECTOR3D getPositionFromLocalToParent(const Matrix4x3&m)
{
	return VECTOR3D(m.tx, m.ty, m.tz);
}

ostream& operator<<(ostream& out, const Matrix4x3& m)
{
	out << m.m11 << ' ' << m.m12 << ' ' << m.m13 << endl;
	out << m.m21 << ' ' << m.m22 << ' ' << m.m23 << endl;
	out << m.m31 << ' ' << m.m32 << ' ' << m.m33 << endl;
	out << m.tx << ' ' << m.ty << ' ' << m.tz << endl;
	return out;
}