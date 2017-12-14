#pragma once
#include<math.h>
#define PI 3.141592654f
#define TwoPI 6.283185308f
#define HalfPI 1.570796327f
#define ReciprocalPI 0.318309886f
#define ReciprocalTwoPI 0.159154943f

//����ǳ�С�����ĳ��������ڸ������Ƚϡ�
#define EPSILON_E4 (float)(1E-4)
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

// ���ڲ�����ֱ�ߺ������䷵��ֵ��
#define PARM_LINE_NO_INTERSECT_PARALLEL  1
#define PARM_LINE_INTERSECT_EVERYWHERE 2
#define PARM_LINE_INTERSECT_POINT 3
#define PARM_LINE_NO_INTERSECT_NO_PARALLEL 4

#define MAX(a,b) ((a)<(b) ? (b):(a))
#define MIN(a,b) ((a)<(b) ? (a):(b))

#define ANG_TO_RAD(ang)((ang)*PI/180.0f)
#define RAD_TO_ANG(rads)((rads)*180.0f/PI)
//������Ļ���������-PI��PI
extern float wrapPi(float);
//��ȫ�ķ����Ǻ���,���ܻ��ȵ�λ
extern float safeAcos(float);

inline void sinCos(float *returnSin, float *returnCos, float theta)
{
	*returnSin = sin(theta);
	*returnCos = cos(theta);
}

