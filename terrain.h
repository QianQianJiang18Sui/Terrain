#ifndef TERRAIN_H
#define TERRAIN_H

#include "GraphicObject.h"

class RGradient;
class CTriAnlgeStrip;
class Terrain : public CGraphicObject
{
public:
    explicit Terrain(CGraphicObject *parent = nullptr);
    virtual ~Terrain();

public:


    virtual void    drawElement(camera*  camera);
    virtual void    setModelMatrix(QMatrix4x4 ma);

    void            loadFromImage(QImage img);
    void            setValueRanges(double minx, double maxx, double minz, double maxz);

protected:

    bool            m_bEnableColorArray = true;  /*默认启用顶点颜色数组*/

    GLfloat*        m_pColor = nullptr;
    unsigned int    m_CBO = 0;              //颜色缓冲
    QColor          m_modelColor = QColor(0, 255, 0);

    /*从灰度图加载地形*/
    QImage          m_Image;

    double          m_minXValue = -10.0f;
    double          m_maxXValue = 10.0f;
    double          m_minZValue = -10.0f;
    double          m_maxZValue = 10.0f;


    void            genGLVertexBuffers();
    void            genGLColorBuffer();
    virtual void    updateModelData();

    RGradient*      m_pGradient = nullptr; //渐变色计算

    double          m_minY;
    double          m_maxY;

    void            CalFaces();

    std::vector<CTriAnlgeStrip*> m_faces;
};

#endif // TERRAIN_H
