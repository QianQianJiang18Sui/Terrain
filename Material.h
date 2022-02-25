#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector3D>

enum Element_Type
{
    OBJ_NULL,
    OBJ_LINE,
    OBJ_GRID_LINE,
    OBJ_MODEL,
    OBJ_TERRAIN,
    OBJ_CUBLE,
    OBJ_CIRCLE,
    OBJ_SKY,
    OBJ_TERRAIN_FACE,
};

enum ElementDrawMode
{
    MT2D  = 1,   //���  ����Ļ���궨λ���Ⱥ����˳���������Ļ���ϲ㡣
    MT3DM = 2,   //3Dģ��  ģ������ϵ����ά�ռ䡣
    MT3DW = 3,   //3D��Ļ  ��Ļ����ϵ����ά�ռ䡣
};

enum ElementSelectState
{
    STATE_Null		=	0,
    STATE_NORMAL	=	1,
    STATE_SELECTED	=	2,
    STATE_SENSITIVE	=	3,
    STATE_FOCUS		=	4,
};

struct Material
{
    QVector3D ambient;       ///������
    QVector3D diffuse;		 ///������
    QVector3D specular;      ///�����
    float     shininess;
};


struct DefaultMaterial : Material
{
    DefaultMaterial()
    {
        ambient   = QVector3D(0.45f,		0.45f,		0.45f);
        diffuse   = QVector3D(0.7f,			0.7f,		0.7f);
        specular  = QVector3D(0.296648, 0.296648f, 0.296648f);
        shininess = 0.6f;
    }
};

/*������ʱ� http://devernay.free.fr/cours/opengl/materials.html */

///�̱�ʯ
struct Emerald : Material
{
    Emerald()
    {
        ambient   = QVector3D(0.0215f,		0.1745f,	0.0215f);
        diffuse   = QVector3D(0.07568f,		0.61424f,	0.07568f);
        specular  = QVector3D(0.633f,		0.727811f,	0.633f);
        shininess = 0.6f;
    }
};

///��
struct Jade : Material
{
    Jade()
    {
        ambient   = QVector3D(0.135f,		0.2225f,	0.1575f);
        diffuse   = QVector3D(0.54f,		0.89f,		0.63f);
        specular  = QVector3D(0.316228f,	0.316228f,	0.316228f);
        shininess = 0.1f;
    }
};

///����ʯ
struct Obsidian : Material
{
    Obsidian()
    {
        ambient   = QVector3D(0.05375f,		0.05f,		0.06625f);
        diffuse   = QVector3D(0.18275f,		0.17f,		0.22525f);
        specular  = QVector3D(0.332741f,	0.328634f,	0.346435f);
        shininess = 0.3f;
    }
};

///����
struct Pearl : Material
{
    Pearl()
    {
        ambient   = QVector3D(0.25f,		0.20725f,	0.20725f);
        diffuse   = QVector3D(1.f,			0.829f,		0.829f);
        specular  = QVector3D(0.296648f,	0.296648f,	0.296648f);
        shininess = 0.088f;
    }
};

///�챦ʯ
struct Ruby : Material
{
    Ruby()
    {
        ambient   = QVector3D(0.1745f,		0.01175f,	0.01175f);
        diffuse   = QVector3D(0.61424f,		0.04136f,	0.04136f);
        specular  = QVector3D(0.727811f,	0.626959f,	0.626959f);
        shininess = 0.6f;
    }
};

///��ɫ��
struct GreenRubber : Material
{
    GreenRubber()
    {
        ambient   = QVector3D(0.0f,			0.05f,		0.0f);
        diffuse   = QVector3D(0.4f,			0.5f,		0.4f);
        specular  = QVector3D(0.04f,		0.7f,		0.04f);
        shininess = 0.078125f;
    }
};



#endif //MATERIAL_H



