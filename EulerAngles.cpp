#include"Quaternion.h"
#include"Matrix4x3.h"
#include"RotationMatrix.h"
#include"MathUtil.h"
#include"EulerAngles.h"

void EulerAngles::canonize()
{
	pitch = wrapPi(pitch);
	if (pitch < -HalfPI)
	{
		pitch = -PI - pitch;
		heading += PI;
		bank += PI;
	}
	else if (pitch > HalfPI)
	{
		pitch = PI - pitch;
		heading += PI;
		bank += PI;
	}
	if (fabs(pitch) > HalfPI - EPSILON_E4) {
		heading += bank;
		bank = 0.0f;
	}
	else
	{
		bank = wrapPi(bank);
	}
	heading = wrapPi(heading);
}
//�����嵽��������ϵ��ת����Ԫ�����ó�ŷ���ǡ�ŷ���ǽ���ʾ������ӹ���������ת��Ŀ�����꣨Ŀ����������ϵ��
void EulerAngles::FromObjectToIntertialQuaternion(const Quaternion &q)
{
	//��Ԫ�� q = ��cos(��/2),x*sin(��/2),y*sin(��/2),z*sin(��/2)������ȡ����
	float sp = -2.0f*(q.y*q.z - q.w*q.x);
	if (fabs(sp) > 1.0f - EPSILON_E5)
	{
		pitch = HalfPI*sp;
		heading = atan2(-q.x*q.z + q.w*q.y, 0.5f - q.y*q.y - q.z*q.z);
		bank = 0.0f;
	}
	else
	{
		pitch = asin(sp);
		heading = atan2(q.x*q.z + q.w*q.y, 0.5 - q.x*q.x - q.y*q.y);
		bank = atan2(q.x*q.y + q.w*q.z, 0.5f - q.x*q.x - q.z*q.z);
	}
}
//
void EulerAngles::FromIntertialToObjectQuaternion(const Quaternion &q)
{
	float sp = -2.0f*(q.y*q.z + q.w*q.x);
	if (fabs(sp) > 1.0f - EPSILON_E6)
	{
		pitch = HalfPI*sp;
		heading = atan2(-q.x*q.z - q.w*q.y, 0.5f - q.y*q.y - q.z*q.z);
		bank = 0.0f;
	}
	else
	{
		pitch = asin(sp);
		heading = atan2(q.x*q.z - q.w*q.y, 0.5 - q.x*q.x - q.y*q.y);
		bank = atan2(q.x*q.y - q.w*q.z, 0.5f - q.x*q.x - q.z*q.z);
	}
}

void EulerAngles::FromObjectToIntertialMatix(const Matrix4x3& m)
{
	float sp = -m.m32;
	if (fabs(sp) > 1 - EPSILON_E6)
	{
		pitch = HalfPI*sp;
		heading = atan2(-m.m32, m.m11);
		bank = 0.0f;
	}
	else
	{
		heading = atan2(m.m31, m.m33);
		pitch = asin(sp);
		bank = atan2(m.m12, m.m22);
	}
}

void EulerAngles::FromIntertialToObjectMatrix(const Matrix4x3&m)
{
	float sp = -m.m23;
	if (fabs(sp) > 1 - EPSILON_E6)
	{
		pitch = HalfPI*sp;
		heading = atan2(-m.m31, m.m11);
		bank = 0.0f;
	}
	else
	{
		heading = atan2(m.m13, m.m33);
		pitch = asin(sp);
		bank = atan2(m.m21, m.m22);
	}
}

void EulerAngles::FromRotationMatrix(const RotationMatrix &m)
{
	float sp = -m.m23;
	if (fabs(sp) > 1 - EPSILON_E6)
	{
		pitch = HalfPI*sp;
		heading = atan2(-m.m31, m.m11);
		bank = 0.0f;
	}
	else
	{
		heading = atan2(m.m13, m.m33);
		pitch = asin(sp);
		bank = atan2(m.m21, m.m22);
	}
}