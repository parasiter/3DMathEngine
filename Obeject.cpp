#include"Obeject.h"
#include"string"
#include"Matrix4x3.h"

using namespace std;


//����PLX�ļ���
/*
PLX�ļ���ʽ
#ע�͡�����
#����Ϊ�������ƣ��������Ͷ��������
obeject_name num_vertices num_polygons
#�����б�ÿ������ĸ�ʽ��Ϊx,y,z
x0 y0 z0
x1 y1 z1 
....
xn yn zn

#������б�
surface_descriptor num_vertices v0 v1 ..vn
surface_descriptor num_vertices v0 v1 ..vn
....

#surface_descriptor Ϊ16λ����ʽΪ CSSD|RRRR|GGGG|BBBB
#CΪRGB/������ɫ��ǣ�SSΪ��ɫģʽ��DΪ�����ǣ�RRRR GGGG BBBBΪRGBģʽ�º�ɫ��ɫ����ɫ������
#GGGGBBBBΪ8λģʽ�µ���ɫ����
*/

//���ڳ�ȡ�����Ե�����
#define PLX_RGB_MASK 0x8000//��ȡ��ɫģʽ
#define PLX_SHADE_MODE_MASK 0x6000//��ȡ��ɫģʽ
#define PLX_2SIDED_MASK 0x1000//��ȡ����
#define PLX_COLOR_MASK 0x0fff//��ȡ��ɫֵ����ɫ����

//��ɫģʽ���
#define PLX_COLOR_MODE_RGB_FLAG 0x8000//RGB��ɫģʽ
#define PLX_COLOR_MODE_INDEXED_FLAG 0x0000//��ɫ��ģʽ
//˫�����ģʽ
#define PLX_2SIDED_FLAG 0x1000//˫��
#define PLX_1SIDED_FLAG 0x0000//����
//��ɫģʽ���
#define PLX_SHADE_MODE_PURE_FLAG 0x0000//�̶���ɫ
#define PLX_SHADE_MODE_FLAT_FLAG 0x2000//�㶨��ɫ
#define PLX_SHADE_MODE_GOURAUD_FLAG 0x4000//gouraud ��ɫ
#define PLX_SHADE_MODE_PHONG_FLAG 0x6000//phong ��ɫ


//PLX�ļ���������һ�ζ�ȡһ�����ݡ�
char *GetLinePLX(char *buffer, int maxlength, FILE *fp)
{
	int index = 0;
	int length = 0;
	while (1)
	{
		if (!fgets(buffer, maxlength, fp))
			return NULL;
		//index���ص�һ�����ǿո��λ��
		for (length = strlen(buffer), index = 0; isspace(buffer[index]); index++);
		if (index >= length||buffer[index] == '#')
			continue;
		return (&buffer[index]);
	}
}

//װ��
int Load_Object4Dv1_PLX(Object4Dv1* object, char *filename, VECTOR3D scale, VECTOR3D pos, VECTOR3D rotate)
{
	FILE *fp;
	char buffer[256];
	char *token_string;//ָ��Ҫ���������������ı���ָ�롣

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
	//������������
	int poly_description = 0;
	int poly_nums_verts = 0;
	char temp_string[8];//�洢�����������ַ������ȴ���һ������

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
			//�ļ���,RGBΪ4��4��4,����ͼ�ο�Ϊ5��5��5��5��6��5����RGB16BIT����8��8��8�ĸ�ʽ��������ת��Ϊ5��6��5.
			object->plist[index].color = RGB16BIT555(red << 4, green << 4, blue << 4);//����Ϊ555��ʽ��
		}
		else {
			object->plist[index].attr |= POLY4Dv1_ATTR_8BITCOLOR;
			object->plist[index].color = poly_description & 0x00ff;
		}
		//������ɫģʽ
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
	case TRANSFORM_LOCAL_ONLY://��vlist���б任
	{
		for (int poly = 0; poly < rend_list->num_polys; poly++)
		{
			PolyF4Dv1 *cur_poly = rend_list->poly_ptrs[poly];
			if (cur_poly == nullptr||!(cur_poly->state & POLY4Dv1_STATE_ACTIVE)||(cur_poly->state&POLY4Dv1_STATE_CLIPPED)||(cur_poly->state&POLY4Dv1_STATE_BACKFACE))//ָ��Ϊ�գ��Ƿ񱻼���Ƿ񱻲��У��Ƿ��Ǳ���
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
			if (cur_poly == nullptr || !(cur_poly->state & POLY4Dv1_STATE_ACTIVE) || (cur_poly->state&POLY4Dv1_STATE_CLIPPED) || (cur_poly->state&POLY4Dv1_STATE_BACKFACE))//ָ��Ϊ�գ��Ƿ񱻼���Ƿ񱻲��У��Ƿ��Ǳ���
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
			if (cur_poly == nullptr || !(cur_poly->state & POLY4Dv1_STATE_ACTIVE) || (cur_poly->state&POLY4Dv1_STATE_CLIPPED) || (cur_poly->state&POLY4Dv1_STATE_BACKFACE))//ָ��Ϊ�գ��Ƿ񱻼���Ƿ񱻲��У��Ƿ��Ǳ���
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