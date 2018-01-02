#pragma once
class VECTOR3D;
class Quaternion;
class EulerAngles;
class RotationMatrix;
//4*3矩阵为4*4齐次矩阵的优化版，最后一列始终默认为0，0，0，1
class Matrix4x3 {
public:
	union {
		struct 
		{
			float m11, m12, m13, m21, m22, m23, m31, m32, m33;
			//平移
			float tx, ty, tz;
		};
		float data[12];
	};
	Matrix4x3() {};
	Matrix4x3(float* data);
	inline void identity()
	{
		m11 = 1.0f, m12 = 0.0f, m13 = 0.0f;
		m21 = 0.0f, m22 = 1.0f, m23 = 0.0f;
		m31 = 0.0f, m32 = 0.0f, m33 = 1.0f;
		tx = ty = tz = 0.0f;
	}
	inline void zeroTranslation()
	{
		tx = ty = tz = 0.0f;
	}
	void setTranslation(const VECTOR3D&v);
	//旋转部分设置为单位矩阵
	void setupTranslation(const VECTOR3D&);
	//构造从父坐标空间<->子坐标空间的转换
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
ostream& operator<<(ostream& out, const Matrix4x3& m);