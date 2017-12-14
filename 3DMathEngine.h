#pragma once

#include<math.h>

//����
#define PI 3.141592654f

//����ǳ�С�����ĳ��������ڸ������Ƚϡ�
#define EPSILON_E4 (float)(1E-4)
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

// ���ڲ�����ֱ�ߺ������䷵��ֵ��
#define PARM_LINE_NO_INTERSECT_PARALLEL  1
#define PARM_LINE_INTERSECT_EVERYWHERE 2
#define PARM_LINE_INTERSECT_POINT 3
#define PARM_LINE_NO_INTERSECT_NO_PARALLEL 4

//
#define MAX(a,b) ((a)<(b) ? (b):(a))
#define MIN(a,b) ((a)<(b) ? (a):(b))

#define ANG_TO_RAD(ang)((ang)*PI/180.0f)
#define RAD_TO_ANG(rads)((rads)*180.0f/PI)

class VECTOR2D {
public:
	float x, y;
	VECTOR2D() :x(1), y(1) {}
	VECTOR2D(float n1, float n2) :x(n1), y(n2) {}
	//��ֵ��������ʼ����������Ĭ�����ɡ�

	inline void SetZero() { x = 0.0f, y = 0.0f; }
};
///////////////////////////////////////////////////////////////////
//                 3D������                                    ////
///////////////////////////////////////////////////////////////////
#define ZEROVECTOR3D VECTOR3D(0,0,0)
class VECTOR3D {
public:
	float x, y, z;
	VECTOR3D():x(1),y(1),z(1){}
	VECTOR3D(float n1,float n2,float n3):x(n1),y(n2),z(n3){}
	VECTOR3D(const VECTOR3D &a) :x(a.x), y(a.y), z(a.z){}
	inline VECTOR3D &operator=(const VECTOR3D &a){
		x = a.x;
		y = a.y;
		z = a.y;
		return *this;
	}
	inline bool operator==(const VECTOR3D &a) const
	{
		return (abs(x - a.x)<EPSILON_E6) && (abs(y - a.y)<EPSILON_E6) && (abs(z - a.z)<EPSILON_E6);
	}
	inline bool operator!=(const VECTOR3D &a) const 
	{
		return (abs(x - a.x)>EPSILON_E6) || (abs(y - a.y)>EPSILON_E6) || (abs(z - a.z)>EPSILON_E6);
	}
	inline VECTOR3D operator+(const VECTOR3D &a) const 
	{
		return VECTOR3D(x + a.x, y + a.y, z + a.z);
	}
	inline VECTOR3D operator-(const VECTOR3D &a) const 
	{
		return VECTOR3D(x - a.x, y - a.y, z - a.z);
	}
	inline VECTOR3D operator*(float a) const
	{
		return VECTOR3D(a*x, a*y, a*z);
	}
	inline VECTOR3D operator/(float a) const
	{
		return VECTOR3D(x / a, y / a, z / a);
	}

	inline VECTOR3D& operator+=(const VECTOR3D &a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	inline VECTOR3D& operator-=(const VECTOR3D &a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}

	inline VECTOR3D& operator*= (float a)
	{
		x *= a;
		y *= a;
		z *= z;
		return *this;
	}
	inline VECTOR3D& operator/= (float a)
	{
		x /= a;
		y /= a;
		z /= a;
		return *this;
	}

	//���                            
	inline float dot(const VECTOR3D &v3d) const
	{
		return x*v3d.x + y*v3d.y + z*v3d.z;
	}
	//���
	inline VECTOR3D operator*(const VECTOR3D &v) const
	{
		return VECTOR3D(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); 
	}

	VECTOR3D& normalized() 
	{ 
		float value = x*x + y*y + z*z;
		value = sqrtf(value);
		if (value > EPSILON_E6)
		{
			x /= value;
			y /= value;
			z /= value;
		}
		return *this;
	}
	inline void SetZero() { x = 0.0f, y = 0.0f, z = 0.0f; }
};
//�ǳ�Ա����
//�������
inline VECTOR3D operator*(float k, const VECTOR3D &a)
{
	return VECTOR3D(k*a.x, k*a.y, k*a.z);
}
//�������
inline float Distance(const VECTOR3D &a, const VECTOR3D &b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return sqrt(dx*dx + dy*dy + dz*dz);
}

inline float VECTOR3DMSG(const VECTOR3D &a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}



///////////////////////////////////////////////////

class VECTOR4D {
public:
	float w, x, y, z;
	VECTOR4D() :w(1), x(1), y(1), z(1) {}
	VECTOR4D(float n1, float n2, float n3, float n4) :w(n1), x(n2), y(n3), z(n4) {}                                                                                                                                                    
	inline void SetZero() { x = 0, y = 0, z = 0, w = 1; }
	inline VECTOR3D GetVECTOR3D() { return VECTOR3D(x, y, z); }
};
/*û��Ҫ�ṩPOINT3D��


};
*/


class PARMLINE3D {
public:
	VECTOR3D v0, v1;
	VECTOR3D n;//��������
	PARMLINE3D(const VECTOR3D &a, const VECTOR3D &b) :v0(a), v1(b)
	{ 
		n = b-a;
		if (n == ZEROVECTOR3D)
			//�׳��쳣
		n.normalized();
	}
	//�ཻ��⣬����ཻ���򽫽��㱣����intersect��
	int Intersect(const PARMLINE3D &a, VECTOR3D &intersect) const
	{
		if (n == a.n)
		{
			VECTOR3D temp = v0 - a.v0;
			if (temp == ZEROVECTOR3D || temp.normalized() == n)
				return PARM_LINE_INTERSECT_EVERYWHERE;
			else
				return PARM_LINE_NO_INTERSECT_PARALLEL;
		}
		//�ཻ���㣬ʹ�ò�����ʽ��(vector)r = (vector)v0 + (float)t*(vector)n;
		float t1 = ((v0 - a.v0)*a.n).dot(n*a.n) / VECTOR3DMSG(n*a.n);
		float t2 = ((v0 - a.v0)*n).dot(n*a.n) / VECTOR3DMSG(n*a.n);
		VECTOR3D point1 = v0 + t1*n;
		VECTOR3D point2 = a.v0 + t2*a.n;
		if (point1 == point2)
		{
			intersect = point1;
			return PARM_LINE_INTERSECT_POINT;  
		}
		else
			return PARM_LINE_NO_INTERSECT_NO_PARALLEL;
	}

};
//////
//3��3����������ת�����ţ��任��
class MATRIX {
public:
	union{
		float data[3][3];
		struct 
		{
			VECTOR3D row1, row2, row3;
		};
	};
};


class  HOMOGENOURS_CONDINATE {
public:
	union 
	{
		float data[4][4];
		struct 
		{
			VECTOR3D row1, row2, row3, row4;
		};
	};
	//Ĭ�ϳ�ʼ��Ϊ��λ����
	HOMOGENOURS_CONDINATE() 
	{
		data[0][0] = 1;
		data[1][1] = 1;
		data[2][2] = 1;
		data[3][3] = 1;
	}
	//���ܶ�άָ�빹�����
	HOMOGENOURS_CONDINATE(float** pdata) 
	{
		
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				data[i][j] = pdata[i][j];
			}
		}
	}
	//����һάָ�빹�����
	HOMOGENOURS_CONDINATE(float *pdata)
	{
		for (int i = 0; i < 16; ++i)
		{
			data[i / 4][i % 4] = pdata[i];
		}
	}
	//�������캯��
	HOMOGENOURS_CONDINATE(const HOMOGENOURS_CONDINATE &ho)
	{
		for (int i = 0; i < 16; ++i)
		{
			data[i / 4][i % 4] = ho.data[i / 4][i % 4];
		}
	}
	//������ֵ�����
	inline HOMOGENOURS_CONDINATE operator=(const HOMOGENOURS_CONDINATE &ho)
	{
		for (int i = 0; i < 16; ++i)
		{
			data[i / 4][i % 4] = ho.data[i / 4][i % 4];
		}
	}
	//�Ծ�����ĳһԪ�ظ�ֵ
	inline void SetElement(int i, int j,float value)
	{
		data[i][j] = value;
	}
	//���س˷������
	HOMOGENOURS_CONDINATE operator*(const HOMOGENOURS_CONDINATE &h)
	{
		float tempdata[4][4];
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				for (int k = 0; k < 4; ++k)
				{
					tempdata[i][j] += data[i][k] * h.data[k][j];
				}
			}
		}
		return HOMOGENOURS_CONDINATE((float **) tempdata);
	}
	//ת�þ���
	HOMOGENOURS_CONDINATE Transposition()
	{
		float tempdata[4][4];
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (i == j)
					continue;
				tempdata[j][i] = data[i][j];
			}
		}
		return HOMOGENOURS_CONDINATE((float**)tempdata);
	}
	//�������
	HOMOGENOURS_CONDINATE Adjugate()
	{

	}
	HOMOGENOURS_CONDINATE Inverse()
	{

	}

	//�������棬ת�þ���ת����ŷ���ǣ���Ԫ����Ͷ��任����ת��λ��
};

class MATRIX1X3 {
public:
	union 
	{
		float data[3];
		struct 
		{
			float M00, M01, M02;
		};
	};
};


//��������ϵ
class CYLINDRICAL3D {
public:
	float r;//��x-yƽ��ľ���
	float theta;//��x-yƽ���ϵ�ͶӰ��Զ��ľ���
	float z;//��x-yƽ���ϵ�ͶӰ��X��ļн�
};

class SPHERICAL3D {
public:
	float r;//����ԭ�����
	float theta;//��Z��н�
	float phi;//��x-yƽ���ͶӰ��X��ļн�
};
