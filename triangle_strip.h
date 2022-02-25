#ifndef TRIANGLE_STRIP_H
#define TRIANGLE_STRIP_H

#include "GraphicObject.h"

/*
    三角形条带
    0       2       4       6       8       10
        1       3       5       7       9
*/


class CTriAnlgeStrip : public CGraphicObject
{
public:
    explicit CTriAnlgeStrip(CGraphicObject *parent = nullptr);
    virtual ~CTriAnlgeStrip();
public:

    virtual void        drawElement(camera*  camera);
    void                setTriangleStrip(std::vector<float> pts, QVector3D normal);

protected:

    virtual void        updateModelData();
};

#endif // TRIANGLE_STRIP_H
