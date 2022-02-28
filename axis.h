#ifndef AXIS_H
#define AXIS_H

#include "GraphicsModel.h"

class CText2D;
class Axis : public CGraphicsModel
{
public:
    explicit Axis(CGraphicObject *parent = nullptr);

public:

    virtual void    drawElement(camera*  camera);

protected:

    void setModelData();
    void drawXYZ();
    void drawNEH();

    CText2D*    m_pX = nullptr;
    CText2D*    m_pY = nullptr;
    CText2D*    m_pZ = nullptr;

    QVector4D   m_XPos = QVector4D(.7, 0, 0, 1);
    QVector4D   m_YPos = QVector4D(0, .7, 0, 1);
    QVector4D   m_ZPos = QVector4D(0, 0, .7, 1);
    QVector4D   m_NPos = QVector4D(0, 0,-.7, 1);

    double      m_factor = 150;
};

#endif // AXIS_H
