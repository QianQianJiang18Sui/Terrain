#ifndef MATERIAL_H
#define MATERIAL_H

#include <QVector3D>
#include <QOpenGLTexture>
#include <QPainter>
#include <QFont>
#include <QTextOption>
#include <QPen>
#include <QFontMetricsF>
#include <QRect>
#include <QSize>

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
    MT2D  = 1,   //表层  以屏幕坐标定位按先后绘制顺序叠放于屏幕最上层。
    MT3DM = 2,   //3D模型  模型坐标系的三维空间。
    MT3DW = 3,   //3D屏幕  屏幕坐标系的三维空间。
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
    QVector3D ambient;       ///环境光
    QVector3D diffuse;		 ///漫反射
    QVector3D specular;      ///镜面光
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

/*物体材质表 http://devernay.free.fr/cours/opengl/materials.html */

///绿宝石
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

///玉
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

///黑曜石
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

///珍珠
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

///红宝石
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

///绿色橡胶
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

QSize           calImageSize(const QString& text, int textPixelSize, QFont font = QFont());
QOpenGLTexture* genTexture(const QString &text, int textPixelSize = 10, QColor textColor = QColor(255,255,0,255), QFont font = QFont());

#endif //MATERIAL_H



