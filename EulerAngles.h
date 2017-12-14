#pragma once
class Quaternion;
class Matrix4x3;
class RotationMatrix;
/*
使用heading-pitch-bank 约定，使用左手坐标系，
heading沿Y轴旋转，pitch沿x轴旋转，bank沿z轴旋转
定义：一个旋转序列，该序列将物体从惯性坐标系变换到物体坐标系。
VECTOR3D v1,v2;v1是在点在物体坐标系中的描述，v2是物体在惯性坐标系中的描述。
Matrix3x3 M;M：旋转序列,旋转物体，从A->B，依然是在惯性坐标系中描述。
当将物体进行旋转时，v1*M = v2; M表示将点从物体坐标系的坐标变换到到惯性坐标系。
v1 = v2*(M-1); M-1 : 表示将点从惯性坐标系的坐标变换到物体坐标系的坐标。
欧拉角定义的是坐标系的变换，<h,p,b>对应于M-1表示为<-h,-p,-b>





*/



class EulerAngles {
public:
	float heading, pitch, bank;
	EulerAngles(){}
	EulerAngles(float a,float b,float c):heading(a),pitch(b),bank(c){}
	//置零
	inline void identify() { heading = 0.0f, pitch = 0.0f, bank = 0.0f; }
	//变换为限制集欧拉角（万向锁问题）
	void canonize();
	//将四元数转换为欧拉角，将矩阵转换为欧拉角。
	//将当前惯性坐标系变换为物体坐标系的坐标，所有角度取负值。
	void FromIntertialToObjectQuaternion(const Quaternion &q);
	//将当前物体坐标系变换为惯性坐标系的坐标，即物体内各个点，从惯性坐标系出发，进行h,p,b旋转，得到的物体中点的坐标。
	void FromObjectToIntertialQuaternion(const Quaternion &Q);
	void FromIntertialToObjectMatrix(const Matrix4x3 &);
	void FromObjectToIntertialMatix(const Matrix4x3 &);
	void FromRotationMatrix(const RotationMatrix&);
};