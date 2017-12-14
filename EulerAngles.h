#pragma once
class Quaternion;
class Matrix4x3;
class RotationMatrix;
/*
ʹ��heading-pitch-bank Լ����ʹ����������ϵ��
heading��Y����ת��pitch��x����ת��bank��z����ת
���壺һ����ת���У������н�����ӹ�������ϵ�任����������ϵ��
VECTOR3D v1,v2;v1���ڵ�����������ϵ�е�������v2�������ڹ�������ϵ�е�������
Matrix3x3 M;M����ת����,��ת���壬��A->B����Ȼ���ڹ�������ϵ��������
�������������תʱ��v1*M = v2; M��ʾ�������������ϵ������任������������ϵ��
v1 = v2*(M-1); M-1 : ��ʾ����ӹ�������ϵ������任����������ϵ�����ꡣ
ŷ���Ƕ����������ϵ�ı任��<h,p,b>��Ӧ��M-1��ʾΪ<-h,-p,-b>





*/



class EulerAngles {
public:
	float heading, pitch, bank;
	EulerAngles(){}
	EulerAngles(float a,float b,float c):heading(a),pitch(b),bank(c){}
	//����
	inline void identify() { heading = 0.0f, pitch = 0.0f, bank = 0.0f; }
	//�任Ϊ���Ƽ�ŷ���ǣ����������⣩
	void canonize();
	//����Ԫ��ת��Ϊŷ���ǣ�������ת��Ϊŷ���ǡ�
	//����ǰ��������ϵ�任Ϊ��������ϵ�����꣬���нǶ�ȡ��ֵ��
	void FromIntertialToObjectQuaternion(const Quaternion &q);
	//����ǰ��������ϵ�任Ϊ��������ϵ�����꣬�������ڸ����㣬�ӹ�������ϵ����������h,p,b��ת���õ��������е�����ꡣ
	void FromObjectToIntertialQuaternion(const Quaternion &Q);
	void FromIntertialToObjectMatrix(const Matrix4x3 &);
	void FromObjectToIntertialMatix(const Matrix4x3 &);
	void FromRotationMatrix(const RotationMatrix&);
};