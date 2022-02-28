#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QOpenGLContext>
#include <QVector2D>
#include <QVector3D>
#include <QtMath>
#include <QColor>

#include <QList>

#include "shader.h"
#include "camera.h"
#include "Material.h"




class CGraphicObject
{
public:
    explicit CGraphicObject(CGraphicObject *parent = nullptr);
    virtual ~CGraphicObject();

public:

    virtual void        getAABB(double AABB[]);


    virtual QMatrix4x4  getTransformMatrix();
    virtual QMatrix4x4  getModelViewMatrix();
    virtual QMatrix4x4  getViewProMatrix();

    virtual QMatrix4x4  getModelMatrix();
    virtual void        setModelMatrix(QMatrix4x4 ma);

    virtual void        drawElement(camera*  camera);


    virtual void        setScale(double val);

    virtual int         getDrawOrder() { return m_drawOrder;}
    virtual void        setDrawOrder(int val) { m_drawOrder = val; }

    virtual QString     getClassName() { return m_class_name;}

    virtual void        CalBoundBox();

public:

    void        SetDrawMode(ElementDrawMode mode){ m_DrawMode = mode; }

    void		SetElementSelectState(ElementSelectState state){m_ElementSelectState = state;}

    void        SetElementColor(QColor color) { m_ElementColor = color;}
    void		SetSelectedColor(QColor color){m_SelectedColor = color;}
    void        SetSensitiveColor(QColor color){m_SensitiveColor = color;}

    void        SetBlendOn() { m_Blend = true; }
    void        SetBlendOff() { m_Blend = false; }

    void        SetElementPos(const QVector3D& pos);
    QVector3D   GetElementPos() { return m_ElementPos;}

    //bool        getVisible() { return m_bVisible;}
    //void        setVisible(bool bVsisible) { m_bVisible = bVsisible ;}

    //对象可控制图元是否参与选择
    void	SetSelected(bool selected){m_selected = selected;}
    bool	GetSelected(){return m_selected;}

    void    UpdateModel();

    //设置材质
    void	SetMaterial(Material material);
    void    Win2ObjCurMT(camera* pCamera, QVector3D win, QVector3D &obj);
    void    Obj2WinCurMT(camera* pCamera, QVector3D obj, QVector3D &win);

    void    setProMa(QMatrix4x4 mat) { m_proMa = mat;}
protected:

    //bool        m_bVisible = true;


    double      m_scale = 1.0;
    int         m_drawOrder = 0;

    QOpenGLFunctions_3_3_Core*  m_pCore = nullptr;
    Shader*     m_pShader = nullptr;

    QMatrix4x4  m_modelMa;
    QMatrix4x4  m_viewMa;
    QMatrix4x4  m_proMa;

    GLfloat*    m_pVertex = nullptr;
    GLfloat*    m_pNormal = nullptr;
    unsigned int* m_pIndex = nullptr;

    long long   m_ptCount = 0;
    long long   m_indexCount = 0;

    unsigned int    m_VAO = 0;      //顶点缓冲对象
    unsigned int    m_VBO = 0;      //顶点坐标
    unsigned int    m_EBO = 0;      //顶点索引
    unsigned int    m_NBO = 0;      //顶点法线

    unsigned int    m_Color = 0;   //顶点颜色   缓冲
    unsigned int    m_Texture = 0; //顶点纹理   坐标缓冲

    QOpenGLTexture* m_pTexture = nullptr;

    virtual void resetOpenGLBuffer();
    virtual void updateModelData();

protected:
    //这里都是临时变量，绘制之前设置
    ElementDrawMode     m_DrawMode = MT3DM;

    //图元是选中状态还是敏感状态，不同与上一个状态
    ElementSelectState  m_ElementSelectState = STATE_NORMAL;

    bool                m_boundingComputed = false; //包围框重置
    bool                m_bBoundBox = false;        //存在包围框，主要是处理没有绘制元素的图元，比如该图元内部的图形被删除干净。
    bool                m_bBoundBoxSwitch = true;   //外部设置是否启用，包围框开关
    bool                m_bSensitive = true;        //图元能不能被敏感，模式为TRUE，可以敏感
    bool                m_Blend = false;            //图元是否开启混合


    QColor              m_SensitiveColor;           //敏感色
    QColor              m_SelectedColor;            //选中色
    QColor              m_ElementColor = QColor(0, 0, 255);//图元颜色

    QVector3D           m_ElementPos;               //图元位置
    QVector3D           m_boundboxpts[8];           //包围框


protected:

    Element_Type        m_object_type = OBJ_NULL;
    QString             m_class_name = QString("CGraphicObject");

    Material            m_material = DefaultMaterial(); //材质
    bool                m_selected = true; //是否可以被选中
    bool                m_bNeedUpdate = false; //刷新VBO

};

#endif // GRAPHIC_OBJECT_H
