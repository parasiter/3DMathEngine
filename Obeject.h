#pragma once
#include<vector>
#include"VECTOR3D.h"
using namespace std;

class Matrix4x3;
//�����״̬
#define OBJECT4Dv1_STATE_ACTIVE 0x0001
#define OBJECT4Dv1_STATE_VISIBLE 0x0002
//����κͶ�����������
#define POLY4Dv1_ATTR_2SIDED 0X0001
#define POLY4Dv1_ATTR_TRANSPARENT 0X0002
#define POLY4Dv1_ATTR_8BITCOLOR 0X0004
#define POLY4Dv1_ATTR_RGB16 0X0008
#define POLY4Dv1_ATTR_RGB24 0X0010

#define POLY4Dv1_ATTR_SHADE_MODE_PURE 0X0020
#define POLY4Dv1_ATTR_SHADE_MODE_FLAT 0X0040
#define POLY4Dv1_ATTR_SHADE_MODE_GOURAUD 0X0080
#define POLY4Dv1_ATTR_SHADE_MODE_PHONG 0X0100
//����κ����״ֵ̬
#define POLY4Dv1_STATE_ACTIVE 0x0001
#define POLY4Dv1_STATE_CLIPPED 0X0002
#define POLY4Dv1_STATE_BACKFACE 0X0004

#define RGB16BIT555(r,g,b) ((b&31)+(g&31)>>5+(r&31)>>5); 
//ѡ��Զ���ε��ĸ�������б任
#define TRANSFORM_LOCAL_ONLY 0x0000
#define TRANSFORM_TRANS_ONLY 0x0001
#define TRANSFORM_LOCAL_TO_TRANS 0x0002



//���ڶ����б�ĵ��������
class Poly4Dv1 {
public:
	int state = 0;
	int attr = 0;
	int color = 0;
	VECTOR3D* vlist;//�����б�
	int vert[3];//�����б������
};

//�԰����Ķ��������
class PolyF4Dv1 {
public:
	int state = 0;
	int attr = 0;
	int color = 0;
	VECTOR3D vlist[3];//�任ǰ�ö���εĶ���
	VECTOR3D tvlist[3];//�任��ö���εĶ��㡣
	PolyF4Dv1 *prev;//ǰһ�������
	PolyF4Dv1 *next;//��һ�������
};
//���ڶ���Ͷ�����б������
class Object4Dv1 {
public:
	int id;
	char name[10];
	int state;
	int attr;
	float avg_radius;
	float max_radius;

	VECTOR3D world_pos;//����������ϵ�е�λ��
	VECTOR3D dir;//���򣬵����ܱ�ʾpitch�ǡ�
	VECTOR3D ux, uy, uz;//�ֲ�������,���ڴ洢����ĳ���

	int num_vertices;//����Ķ�����
	VECTOR3D *vlist_local;//�洢����ֲ����������
	VECTOR3D *vlist_trans;//�洢�任�󶥵����������

	int num_polys;
	Poly4Dv1 *plist;//���������
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
void Transform_Renderlist4Dv1(Renderlist4Dv1* rend_list, Matrix4x3 matrix, int coord_select);//����Ⱦ�б�����任�����һ������ָ��Ҫ�任������:vlist����tvlist
void Transform_Object4Dv1(Object4Dv1 *object, Matrix4x3 matrix, int coord_select, int transform_basis);//coord_selectͬ�ϣ�transform_basis�����Ƿ�Գ�����б任
void Translation_LocalToWorld_Object(Object4Dv1 *obj, int coord_select = TRANSFORM_LOCAL_TO_TRANS);//ִ��ƽ�Ʋ���
void Build_LocalToWorld_Translation_Matrix4x3(const VECTOR3D &pos, Matrix4x3 &m);//����ƽ�ƾ���
void Translation_LocalToWorld_RenderList(Renderlist4Dv1 rend, const VECTOR3D& world_pos, int coord_select = TRANSFORM_LOCAL_TO_TRANS);//����Ⱦ�б�ִ��ƽ��
