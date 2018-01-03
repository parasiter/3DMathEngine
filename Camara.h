#pragma once
#include"VECTOR3D.h"
#include"Plane.h"
#include"Matrix4x3.h"
class Camera4Dv1 {
	int state;
	int attr;

	VECTOR3D world_pos;
	VECTOR3D dir;//ŷ���ǻ�UVN�����ע�ӷ���

	VECTOR3D u, v, n;//UVN����ĳ�������

	VECTOR3D target;//UVN�����Ŀ��λ��

	float view_dist;//��ֱ�Ӿ��ˮƽ�Ӿ�
	float fov;//ˮƽ����ʹ�ֱ�����ӽ�

	float near_clip_z, far_clip_z;//���ü����Զ�ü���

	Plane rt_clip_plane;
	Plane lt_clip_plane;
	Plane tp_clip_plane;
	Plane bt_clip_plane;//bottom

	float viewplane_width;//��ƽ��Ŀ�Ⱥ͸߶�
	float viewplane_height;//���ڹ�һ��ͶӰ��Ϊ2x2�������С���ӿڻ���Ļ������ͬ

	int viewport_width;//��Ļ�Ĵ�С
	int viewport_height;
	int viewport_center_x;//��Ļ����
	int viewport_center_y;

	float aspect_ratio;//��߱�

	//�Ƿ���Ҫ��Щ����ȡ���ڱ任����
	Matrix4x3 mcam;//�洢�������굽�����ߵı任����
	Matrix4x3 mper;//�洢������굽͸������任����
	Matrix4x3 mscr;//�洢͸����ߵ���Ļ����ı任����

	Camera4Dv1() { memset(this, 0, sizeof(Camera4Dv1)); }
	void Init_Camera4Dv1(int cam_attr, const VECTOR3D& cam_pos, const VECTOR3D& cam_dir, const VECTOR3D* cam_target, float near_clip_z,
		float far_clop_z, float fov, int viewport_width, int viewport_height);

};