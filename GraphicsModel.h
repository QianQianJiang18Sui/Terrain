#ifndef GRAPHICS_MODEL_H
#define GRAPHICS_MODEL_H

#include "GraphicObject.h"

class CGraphicsModel : public CGraphicObject
{
public:
    explicit CGraphicsModel(CGraphicObject *parent = nullptr);

public:

    virtual void        drawElement(camera*  camera);

    //void                setParam(QVector2D& leftBottom, QVector2D& rightTop, float height, int step, GLfloat lineWidth = 1.0f);

    void                setModelData(GLfloat* pVerts, int triNum, int lineNum, GLfloat* pNormal = nullptr, GLfloat* pColor = nullptr);

    void                setColor(QColor color);


protected:

    GLfloat*    m_pColor = nullptr;
    GLfloat*    m_pNormal = nullptr;

    bool        m_bNeedUpdate = false;
    bool        m_bEnableColorArray = false; //启用颜色数组 渐变还是单色

    int         m_triNum = 0;  //三角形个数
    int         m_lineNum = 0; //线条数
    float       m_scale = 1.0f;

    QColor      m_ModelColor = QColor(0, 255, 0, 255);

    unsigned int m_ColorBuffer = 0;
    unsigned int m_NormalBuffer = 0;

    virtual void updateModelData();
    virtual void initData();
};

#endif // GRAPHICS_MODEL_H
