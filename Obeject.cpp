#include"Obeject.h"
#include"string"
#include"Matrix4x3.h"

using namespace std;


//加载PLX文件。
/*
PLX文件格式
#注释。。。
#首行为物体名称，顶点数和多边形数。
obeject_name num_vertices num_polygons
#顶点列表，每个顶点的格式都为x,y,z
x0 y0 z0
x1 y1 z1 
....
xn yn zn

#多边形列表
surface_descriptor num_vertices v0 v1 ..vn
surface_descriptor num_vertices v0 v1 ..vn
....

#surface_descriptor 为16位，格式为 CSSD|RRRR|GGGG|BBBB
#C为RGB/索引颜色标记，SS为着色模式，D为单面标记，RRRR GGGG BBBB为RGB模式下红色绿色和蓝色分量；
#GGGGBBBB为8位模式下的颜色索引
*/

//用于抽取面属性的掩码
#define PLX_RGB_MASK 0x8000//抽取颜色模式
#define PLX_SHADE_MODE_MASK 0x6000//抽取着色模式
#define PLX_2SIDED_MASK 0x1000//抽取面标记
#define PLX_COLOR_MASK 0x0fff//抽取颜色值或颜色索引

//颜色模式标记
#define PLX_COLOR_MODE_RGB_FLAG 0x8000//RGB颜色模式
#define PLX_COLOR_MODE_INDEXED_FLAG 0x0000//调色板模式
//双面或单面模式
#define PLX_2SIDED_FLAG 0x1000//双面
#define PLX_1SIDED_FLAG 0x0000//单面
//着色模式标记
#define PLX_SHADE_MODE_PURE_FLAG 0x0000//固定着色
#define PLX_SHADE_MODE_FLAT_FLAG 0x2000//恒定着色
#define PLX_SHADE_MODE_GOURAUD_FLAG 0x4000//gouraud 着色
#define PLX_SHADE_MODE_PHONG_FLAG 0x6000//phong 着色


//PLX文件加载器，一次读取一行数据。
char *GetLinePLX(char *buffer, int maxlength, FILE *fp)
{
	int index = 0;
	int length = 0;
	while (1)
	{
		if (!fgets(buffer, maxlength, fp))
			return NULL;
		//index返回第一个不是空格的位置
		for (length = strlen(buffer), index = 0; isspace(buffer[index]); index++);
		if (index >= length||buffer[index] == '#')
			continue;
		return (&buffer[index]);
	}
}

//装载
int Load_Object4Dv1_PLX(Object4Dv1* object, char *filename, VECTOR3D scale, VECTOR3D pos, VECTOR3D rotate)
{
	FILE *fp;
	char buffer[256];
	char *token_string;//指向要分析的物体数据文本的指针。

	memset(object, 0, sizeof(Object4Dv1));
	object->state = OBJECT4Dv1_STATE_ACTIVE | OBJECT4Dv1_STATE_VISIBLE;
	object->world_pos = pos;
	if (!(fp = fopen(filename, "r")))
	{
		printf("Coundn't open PLX file %s", filename);
		return 0;
	}
	if (!(token_string = GetLinePLX(buffer, 255, fp))) 
	{
		printf("PLX file error with file %s (object descriptor invalid).", filename);
		return 0;
	}
	sscanf(token_string, "%s %d %d", object->name, &object->num_vertices, &object->num_polys);
	object->SetupList();

	for (int index = 0; index < object->num_vertices; index++)
	{
		if (!(token_string = GetLinePLX(buffer, 255, fp)))
		{
			printf("PLX file error with file %s (object descriptor invalid).", filename);
			return 0;
		}
		VECTOR3D vert;
		sscanf(token_string, "%f %f %f", &vert.x, &vert.y, &vert.z);
		vert.x *= scale.x;
		vert.y *= scale.y;
		vert.z *= scale.z;
		object->vlist_local[index] = vert;
		//ComputeObjectRadius(object);
	}
	//多面体描述符
	int poly_description = 0;
	int poly_nums_verts = 0;
	char temp_string[8];//存储面描述符的字符串，等待下一步分析

	for (int index = 0; index < object->num_polys; index++)
	{
		if (!(token_string = GetLinePLX(buffer, 255, fp)))
		{
			printf("PLX file error with file %s (object descriptor invalid).", filename);
			return 0;
		}
		sscanf(token_string, "%s %d %d %d %d", temp_string, &poly_nums_verts, &object->plist[index].vert[0],
			&object->plist[index].vert[1], &object->plist[index].vert[2]);

		if (temp_string[0] == '0' && toupper(temp_string[1])=='X')
			sscanf(temp_string, "%X", &poly_description);
		else
		{
			poly_description = atoi(temp_string);
		}
		if (poly_description&PLX_2SIDED_FLAG)
		{
			object->plist[index].attr |= POLY4Dv1_ATTR_2SIDED;
		}
		if (poly_description&PLX_COLOR_MODE_RGB_FLAG)
		{
			object->plist[index].attr |= POLY4Dv1_ATTR_RGB16;

			int red = (poly_description & 0xf00) >> 8;
			int green = (poly_description & 0x0f0) >> 4;
			int blue = (poly_description & 0x00f);
			//文件中,RGB为4，4，4,但是图形卡为5，5，5或5，6，5，而RGB16BIT接受8，8，8的格式，并将其转换为5，6，5.
			object->plist[index].color = RGB16BIT555(red << 4, green << 4, blue << 4);//假设为555格式。
		}
		else {
			object->plist[index].attr |= POLY4Dv1_ATTR_8BITCOLOR;
			object->plist[index].color = poly_description & 0x00ff;
		}
		//处理着色模式
		int shade_mode = poly_description & PLX_SHADE_MODE_MASK;
		switch (shade_mode)
		{
			case PLX_SHADE_MODE_FLAT_FLAG:
			{
				object->plist[index].attr |= POLY4Dv1_ATTR_SHADE_MODE_FLAT;
			}break;
			case PLX_SHADE_MODE_GOURAUD_FLAG:
			{
				object->plist[index].attr |= POLY4Dv1_ATTR_SHADE_MODE_GOURAUD;
			}break;
			case PLX_SHADE_MODE_PHONG_FLAG:
			{
				object->plist[index].attr |= POLY4Dv1_ATTR_SHADE_MODE_PHONG;
			}
			case PLX_SHADE_MODE_PURE_FLAG:
			{
				object->plist[index].attr |= POLY4Dv1_ATTR_SHADE_MODE_PURE;
			}
			default:
				break;
		}
		object->plist[index].state = POLY4Dv1_STATE_ACTIVE;
	}
	fclose(fp);
	return 1;
}

void Transform_Renderlist4Dv1(Renderlist4Dv1* rend_list, Matrix4x3 matrix, int coord_select)
{
	switch (coord_select)
	{
	case TRANSFORM_LOCAL_ONLY://对vlist进行变换
	{
		for (int poly = 0; poly < rend_list->num_polys; poly++)
		{
			PolyF4Dv1 *cur_poly = rend_list->poly_ptrs[poly];
			if (cur_poly == nullptr||!(cur_poly->state & POLY4Dv1_STATE_ACTIVE)||(cur_poly->state&POLY4Dv1_STATE_CLIPPED)||(cur_poly->state&POLY4Dv1_STATE_BACKFACE))//指针为空，是否被激活，是否被裁切，是否是背面
			{
				continue;
			}
			for (int vertex = 0; vertex < 3; vertex++)
				cur_poly->vlist[vertex] *= matrix;
		}
	}break;
	case TRANSFORM_TRANS_ONLY:
	{
		for (int poly = 0; poly < rend_list->num_polys; poly++)
		{
			PolyF4Dv1 *cur_poly = rend_list->poly_ptrs[poly];
			if (cur_poly == nullptr || !(cur_poly->state & POLY4Dv1_STATE_ACTIVE) || (cur_poly->state&POLY4Dv1_STATE_CLIPPED) || (cur_poly->state&POLY4Dv1_STATE_BACKFACE))//指针为空，是否被激活，是否被裁切，是否是背面
			{
				continue;
			}
			for (int vertex = 0; vertex < 3; vertex++)
				cur_poly->tvlist[vertex] *= matrix;
		}
	}break;
	case TRANSFORM_LOCAL_TO_TRANS:
	{
		for (int poly = 0; poly < rend_list->num_polys; poly++)
		{
			PolyF4Dv1 *cur_poly = rend_list->poly_ptrs[poly];
			if (cur_poly == nullptr || !(cur_poly->state & POLY4Dv1_STATE_ACTIVE) || (cur_poly->state&POLY4Dv1_STATE_CLIPPED) || (cur_poly->state&POLY4Dv1_STATE_BACKFACE))//指针为空，是否被激活，是否被裁切，是否是背面
			{
				continue;
			}
			for (int vertex = 0; vertex < 3; vertex++)
				cur_poly->tvlist[vertex] = cur_poly->vlist[vertex]* matrix;
		}
	}
	default:
		break;
	}
}

void Transform_Object4Dv1(Object4Dv1 *object, Matrix4x3 matrix, int coord_select, int transform_basis)
{
	switch (coord_select)
	{
	case TRANSFORM_LOCAL_ONLY:
	{

	}
	default:
		break;
	}
}