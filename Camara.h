#pragma once
#include"VECTOR3D.h"
#include"Plane.h"
#include"Matrix4x3.h"
class Camera4Dv1 {
	int state;
	int attr;

	VECTOR3D world_pos;
	VECTOR3D dir;//欧拉角或UVN相机的注视方向

	VECTOR3D u, v, n;//UVN相机的朝向向量

	VECTOR3D target;//UVN相机的目标位置

	float view_dist;//垂直视距和水平视距
	float fov;//水平方向和垂直方向视角

	float near_clip_z, far_clip_z;//近裁剪面和远裁剪面

	Plane rt_clip_plane;
	Plane lt_clip_plane;
	Plane tp_clip_plane;
	Plane bt_clip_plane;//bottom

	float viewplane_width;//视平面的宽度和高度
	float viewplane_height;//对于归一化投影，为2x2，否则大小与视口或屏幕窗口相同

	int viewport_width;//屏幕的大小
	int viewport_height;
	int viewport_center_x;//屏幕中心
	int viewport_center_y;

	float aspect_ratio;//宽高比

	//是否需要这些矩阵取决于变换方法
	Matrix4x3 mcam;//存储世界坐标到相机左边的变换矩阵
	Matrix4x3 mper;//存储相机坐标到透视坐标变换矩阵
	Matrix4x3 mscr;//存储透视左边到屏幕坐标的变换矩阵

	Camera4Dv1() { memset(this, 0, sizeof(Camera4Dv1)); }
	void Init_Camera4Dv1(int cam_attr, const VECTOR3D& cam_pos, const VECTOR3D& cam_dir, const VECTOR3D* cam_target, float near_clip_z,
		float far_clop_z, float fov, int viewport_width, int viewport_height);

};