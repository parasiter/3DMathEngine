#pragma once

class VECTOR3D;
class EulerAngles;
/*四元数 <n,theta> = <w,x,y,z> = <cosθ/2,x*sinθ/2,y*sinθ/2,z*sinθ/2>
一个四元数定义了复数w+x*i+y*j+z*k;
任意四元数都有两种不同的表示方法，如将theta = theta+2PI, 
q = <cos(θ/2+PI),x*sin(θ/2+PI),y*sin(θ/2+PI),z*sin(θ/2+PI)> = < -w, -v> = -q;
四元数的共轭：q(*) = <w,-x,-y,-z>;
四元数的逆：q-1 = q(*)/||q||
若||n|| =1，则||q|| = 1,q-1 = q(*).实际变换中，仅使用单位向量，因此q-1 = q*;
四元数的乘积：去q1*q2 = ||q1||*||q2||;
3D点的位置变换：p1 = q*p*q-1;
*/
class Quaternion {
public:
	float w, x, y, z;
	//单位四元数，q = <1,0>;theta = 0,即未发生旋转。
	inline void identity() { w = 1.0f; x = y = z = 0.0f; }
	//构造按指定轴旋转的四元数
	void setToRotateAboutX(float theta);
	void setToRotateAboutY(float theta);
	void setToRotateAboutZ(float theta);
	void setToRotateAboutAxis(const VECTOR3D& axis,float theta);
	//将欧拉角转换为四元数
	void setToRotateObjectToIntertial(const EulerAngles&);
	void setToRotateIntertialToObject(const EulerAngles&);

	Quaternion operator*(const Quaternion&) const;
	Quaternion operator*=(const Quaternion&);

	void normalize();

	float getRotationAngle() const;
	VECTOR3D getRotationAxis() const;
};

extern const Quaternion QuaternionIdentity;
extern float dotProduct(const Quaternion&, const Quaternion&);
extern Quaternion slerp(const Quaternion&, const Quaternion&, float t);
//四元数共轭
extern Quaternion conjugate(const Quaternion&);
//四元数的幂
extern Quaternion pow(const Quaternion &q, float exponent);