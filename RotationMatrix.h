#pragma once
class Quaternion;
class EulerAngles;
class VECTOR3D;
class RotationMatrix {
public:
	float m11, m12, m13, m21, m22, m23, m31, m32, m33;
	inline void identity();
	void setup(const EulerAngles&);
	void fromInterialToObjectQuaternion(const Quaternion &);
	void fromObjectToInterrialQuaternion(const Quaternion&);
	VECTOR3D intertialToObject(const VECTOR3D&v)const;
	VECTOR3D objectToIntertial(const VECTOR3D&v)const;
};