#include"Camara.h"
#include"MathUtil.h"

void  Camera4Dv1::Init_Camera4Dv1(int cam_attr, const VECTOR3D& cam_pos, const VECTOR3D& cam_dir, const VECTOR3D* cam_target, float cam_near_clip_z,
	float cam_far_clop_z, float cam_fov, int cam_viewport_width, int cam_viewport_height)
{
	attr = cam_attr;
	world_pos = cam_pos;
	dir = cam_dir;
	//对于UVN相机，设置其为标准坐标系
	u = VECTOR3D(1, 0, 0);
	v = VECTOR3D(0, 1, 0);
	n = VECTOR3D(0, 0, 1);
	if (cam_target != nullptr)
		target = *cam_target;
	else
	{
		target = ZEROVECTOR3D;
	}
	near_clip_z = cam_near_clip_z;
	far_clip_z = cam_far_clop_z;

	fov = cam_fov;
	viewport_width = cam_viewport_width;
	viewport_height = cam_viewport_height;
	viewport_center_x = (viewport_width - 1) / 2;
	viewport_center_y = (viewport_height - 1) / 2;
	aspect_ratio = static_cast<float>(viewport_width) / static_cast<float>(viewport_height);

	mcam.identity();
	mper.identity();
	mscr.identity();

	viewplane_width = 2.0f;
	viewplane_height = 2.0f / aspect_ratio;

	float tan_fov_div2 = tan(ANG_TO_RAD(fov / 2));
	view_dist = 0.5*viewplane_width*tan_fov_div2;
	if (fov == 90.0f)
	{
		VECTOR3D pt_origin;

	}
}