#pragma once
#include<math.h>
#define PI 3.141592654f
#define TwoPI 6.283185308f
#define HalfPI 1.570796327f
#define ReciprocalPI 0.318309886f
#define ReciprocalTwoPI 0.159154943f

//定义非常小的数的常量，用于浮点数比较。
#define EPSILON_E4 (float)(1E-4)
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

// 用于参数化直线函数及其返回值。
#define PARM_LINE_NO_INTERSECT_PARALLEL  1
#define PARM_LINE_INTERSECT_EVERYWHERE 2
#define PARM_LINE_INTERSECT_POINT 3
#define PARM_LINE_NO_INTERSECT_NO_PARALLEL 4

#define MAX(a,b) ((a)<(b) ? (b):(a))
#define MIN(a,b) ((a)<(b) ? (a):(b))

#define ANG_TO_RAD(ang)((ang)*PI/180.0f)
#define RAD_TO_ANG(rads)((rads)*180.0f/PI)
//将传入的弧度限制在-PI到PI
extern float wrapPi(float);
//安全的反三角函数,接受弧度单位
extern float safeAcos(float);

inline void sinCos(float *returnSin, float *returnCos, float theta)
{
	*returnSin = sin(theta);
	*returnCos = cos(theta);
}

