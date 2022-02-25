#ifndef LINE_H
#define LINE_H

#include "GraphicObject.h"

class CLine : public CGraphicObject
{
public:
    explicit CLine(CGraphicObject *parent = nullptr);

public:

    virtual void        drawElement(camera*  camera);

    void                setLine(QVector3D start, QVector3D end, QColor lineColor = QColor(255, 0, 0), GLfloat lineWidth = 1.0f);

    void                setColor(const QColor& color);
    QColor              getColor() { return m_modelColor; }

    void                setLineWidth(float width) {m_lineWidth = width;}
    float               getLineWidth() { return m_lineWidth;}

protected:

    QColor              m_modelColor = QColor(255,0,0);
    float               m_lineWidth = 1.0f; //线宽，目前暂时不用

    QVector3D           m_startPos;
    QVector3D           m_endPos;

    virtual void        updateModelData();
};

#endif // LINE_H
