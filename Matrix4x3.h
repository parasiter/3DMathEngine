#pragma once
class VECTOR3D;
class Quaternion;
class EulerAngles;
class RotationMatrix;
//4*3����Ϊ4*4��ξ�����Ż��棬���һ��ʼ��Ĭ��Ϊ0��0��0��1
class Matrix4x3 {
public:
	float m11, m12, m13, m21, m22, m23, m31, m32, m33;
	//ƽ��
	float tx, ty, tz;

	inline void identity();
	inline void zeroTranslation();
	inline void setTranslation(const VECTOR3D&);
	//��ת��������Ϊ��λ����
	void setupTranslation(const VECTOR3D&);
	//����Ӹ�����ռ�<->������ռ��ת��
	void setupLocalToParent(const VECTOR3D&pos, const EulerAngles &e);
	void setupLocalToParent(const VECTOR3D&pos, const RotationMatrix &m);
	void setupParentToLocal(const VECTOR3D&pos, const EulerAngles &e);
	void setupParentToLocal(const VECTOR3D&pos, const RotationMatrix&m);

	void setupRotate(int axis, float theta);
	void setupRotate(const VECTOR3D& axis, float theta);
	void fromQuaternion(const Quaternion &q);
	void setupScale(const VECTOR3D&);
	void setupScaleAlongAxis(const VECTOR3D&axis, float k);
	void setupShear(int axis, float s, float t);
	void setupProject(const VECTOR3D&n);
	void setupReflect(int axis, float k = 0.0f);
	void setupReflect(const VECTOR3D&);
};

VECTOR3D operator*(const VECTOR3D &, const Matrix4x3&);
Matrix4x3 operator*(const Matrix4x3&, const Matrix4x3&);
VECTOR3D &operator*=(VECTOR3D&p, const Matrix4x3&);
Matrix4x3 &operator*=(Matrix4x3&, const Matrix4x3&);
float determinant(const Matrix4x3&);
Matrix4x3 inverse(const Matrix4x3&);
VECTOR3D getTranslation(const Matrix4x3&);
VECTOR3D getPositionFromParentToLocalMatrix(const Matrix4x3&);
VECTOR3D getPositionFromLocalToParent(const Matrix4x3&);