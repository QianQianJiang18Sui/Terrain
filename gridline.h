#ifndef GRID_LINE_H
#define GRID_LINE_H

#include "GraphicObject.h"

enum GRID_TYPE
{
    GRID_XOY,
    GRID_XOZ,
    GRID_YOZ,
};

class CGridline : public CGraphicObject
{
public:
    explicit CGridline(CGraphicObject *parent = nullptr);

public:

    virtual void        drawElement(camera*  camera);
    void                setParam(QVector2D& leftBottom, QVector2D& rightTop, float height, int step, GLfloat lineWidth = 1.0f);
    void                setGridType(GRID_TYPE type);

    void                setGridColor(const QColor& color);
    QColor              getGridColor() { return m_grid_Color; }

protected:

    GRID_TYPE   m_grid_type = GRID_XOY;
    QColor      m_grid_Color = QColor(255,255,255);

    float       m_height; //高度
    float       m_step;   //

    float       m_lineWidth = 1.0f; //线宽，目前暂时不用
    int         m_lineNum = 0; //线条数

    virtual void updateModelData();
};

#endif // GRID_LINE_H
