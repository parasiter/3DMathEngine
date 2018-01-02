#include"VECTOR3D.h"
#include<iostream>
#include"RotationMatrix.h"
#include"EulerAngles.h"
#include"Quaternion.h"
#include"MathUtil.h"
#include"Matrix4x3.h"
using namespace std;
int main()
{
	float data1[9] = { 1,0,0,0,0.6,0.8,0,0,1 };
	float data2[9] = { 0.6,0.8,0,0,1,0,0,0,1 };

	VECTOR3D v1(1, 0.5, 1);
	VECTOR3D v2(1, 1, 1);
	RotationMatrix matrix1(data1);
	Matrix4x3 matrix2;
	matrix2.identity();
	matrix2.setTranslation(VECTOR3D(1, 1, 1));
	cout << v1*matrix2 << endl;
	return 0;
}