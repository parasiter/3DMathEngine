#pragma once
#include<cstring>
#include<iostream>
class Quaternion;
class EulerAngles;
class VECTOR3D;
using namespace std;

using namespace std;

class RotationMatrix {
public:
	union 
	{
		struct {
			float m11, m12, m13, m21, m22, m23, m31, m32, m33;
		};
		float data[9];
	};
	RotationMatrix(float *f) {
		memcpy(data, f, 36);
	}
	RotationMatrix() {}
	inline void identity();
	void setup(const EulerAngles&);
	void fromInterialToObjectQuaternion(const Quaternion &);
	void fromObjectToInterrialQuaternion(const Quaternion&);
	VECTOR3D intertialToObject(const VECTOR3D&v)const;
	VECTOR3D objectToIntertial(const VECTOR3D&v)const;
};

VECTOR3D operator*(const VECTOR3D &p, const RotationMatrix&m);
RotationMatrix operator*(const RotationMatrix&a, const RotationMatrix&b);
VECTOR3D &operator*=(VECTOR3D&p, const RotationMatrix&m);
RotationMatrix &operator*=(RotationMatrix&a, const RotationMatrix&b);
ostream& operator<<(ostream&out, const RotationMatrix&r);