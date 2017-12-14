#pragma once

class VECTOR3D;
class EulerAngles;
/*��Ԫ�� <n,theta> = <w,x,y,z> = <cos��/2,x*sin��/2,y*sin��/2,z*sin��/2>
һ����Ԫ�������˸���w+x*i+y*j+z*k;
������Ԫ���������ֲ�ͬ�ı�ʾ�������罫theta = theta+2PI, 
q = <cos(��/2+PI),x*sin(��/2+PI),y*sin(��/2+PI),z*sin(��/2+PI)> = < -w, -v> = -q;
��Ԫ���Ĺ��q(*) = <w,-x,-y,-z>;
��Ԫ�����棺q-1 = q(*)/||q||
��||n|| =1����||q|| = 1,q-1 = q(*).ʵ�ʱ任�У���ʹ�õ�λ���������q-1 = q*;
��Ԫ���ĳ˻���ȥq1*q2 = ||q1||*||q2||;
3D���λ�ñ任��p1 = q*p*q-1;
*/
class Quaternion {
public:
	float w, x, y, z;
	//��λ��Ԫ����q = <1,0>;theta = 0,��δ������ת��
	inline void identity() { w = 1.0f; x = y = z = 0.0f; }
	//���찴ָ������ת����Ԫ��
	void setToRotateAboutX(float theta);
	void setToRotateAboutY(float theta);
	void setToRotateAboutZ(float theta);
	void setToRotateAboutAxis(const VECTOR3D& axis,float theta);
	//��ŷ����ת��Ϊ��Ԫ��
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
//��Ԫ������
extern Quaternion conjugate(const Quaternion&);
//��Ԫ������
extern Quaternion pow(const Quaternion &q, float exponent);