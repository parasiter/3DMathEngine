#pragma once
#include<vector>
#include"VECTOR3D.h"
using namespace std;

class Matrix4x3;
//物体的状态
#define OBJECT4Dv1_STATE_ACTIVE 0x0001
#define OBJECT4Dv1_STATE_VISIBLE 0x0002
//多边形和多边形面的属性
#define POLY4Dv1_ATTR_2SIDED 0X0001
#define POLY4Dv1_ATTR_TRANSPARENT 0X0002
#define POLY4Dv1_ATTR_8BITCOLOR 0X0004
#define POLY4Dv1_ATTR_RGB16 0X0008
#define POLY4Dv1_ATTR_RGB24 0X0010

#define POLY4Dv1_ATTR_SHADE_MODE_PURE 0X0020
#define POLY4Dv1_ATTR_SHADE_MODE_FLAT 0X0040
#define POLY4Dv1_ATTR_SHADE_MODE_GOURAUD 0X0080
#define POLY4Dv1_ATTR_SHADE_MODE_PHONG 0X0100
//多边形和面的状态值
#define POLY4Dv1_STATE_ACTIVE 0x0001
#define POLY4Dv1_STATE_CLIPPED 0X0002
#define POLY4Dv1_STATE_BACKFACE 0X0004

#define RGB16BIT555(r,g,b) ((b&31)+(g&31)>>5+(r&31)>>5); 
//选择对多边形的哪个数组进行变换
#define TRANSFORM_LOCAL_ONLY 0x0000
#define TRANSFORM_TRANS_ONLY 0x0001
#define TRANSFORM_LOCAL_TO_TRANS 0x0002



//基于顶点列表的单个多边形
class Poly4Dv1 {
public:
	int state = 0;
	int attr = 0;
	int color = 0;
	VECTOR3D* vlist;//顶点列表
	int vert[3];//顶点列表的索引
};

//自包含的多边形链表
class PolyF4Dv1 {
public:
	int state = 0;
	int attr = 0;
	int color = 0;
	VECTOR3D vlist[3];//变换前该多边形的顶点
	VECTOR3D tvlist[3];//变换后该多边形的顶点。
	PolyF4Dv1 *prev;//前一个多边形
	PolyF4Dv1 *next;//后一个多边形
};
//基于顶点和多边形列表的物体
class Object4Dv1 {
public:
	int id;
	char name[10];
	int state;
	int attr;
	float avg_radius;
	float max_radius;

	VECTOR3D world_pos;//在世界坐标系中的位置
	VECTOR3D dir;//朝向，但不能表示pitch角。
	VECTOR3D ux, uy, uz;//局部坐标轴,用于存储物体的朝向。

	int num_vertices;//物体的顶点数
	VECTOR3D *vlist_local;//存储顶点局部坐标的数组
	VECTOR3D *vlist_trans;//存储变换后顶点坐标的数组

	int num_polys;
	Poly4Dv1 *plist;//多边形数组
	void SetupList()
	{
		vlist_local = new VECTOR3D[num_vertices];
		vlist_trans = new VECTOR3D[num_vertices];
		plist = new Poly4Dv1[num_polys];
	}

	~Object4Dv1(){
		delete[] vlist_local;
		delete[] vlist_trans;
		delete[] plist;
	}
};
class Renderlist4Dv1 {
public:
	int state;
	int attr;
	vector<PolyF4Dv1*> poly_ptrs;
	vector<PolyF4Dv1> poly_data;
	int num_polys;
};



char *GetLinePLX(char *buffer, int maxlength, FILE *fp);
int Load_Object4Dv1_PLX(Object4Dv1* object, char *filename, VECTOR3D scale, VECTOR3D pos, VECTOR3D rotate);
void Transform_Renderlist4Dv1(Renderlist4Dv1* rend_list, Matrix4x3 matrix, int coord_select);//将渲染列表按矩阵变换，最后一个参数指定要变换的数组:vlist还是tvlist
void Transform_Object4Dv1(Object4Dv1 *object, Matrix4x3 matrix, int coord_select, int transform_basis);//coord_select同上，transform_basis决定是否对朝向进行变换
void Translation_LocalToWorld_Object(Object4Dv1 *obj, int coord_select = TRANSFORM_LOCAL_TO_TRANS);//执行平移操作
void Build_LocalToWorld_Translation_Matrix4x3(const VECTOR3D &pos, Matrix4x3 &m);//设置平移矩阵
void Translation_LocalToWorld_RenderList(Renderlist4Dv1 rend, const VECTOR3D& world_pos, int coord_select = TRANSFORM_LOCAL_TO_TRANS);//对渲染列表执行平移
