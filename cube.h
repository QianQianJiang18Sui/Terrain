#ifndef CUBE_H
#define CUBE_H

#include "GraphicObject.h"

class Cube : public CGraphicObject
{
public:
    explicit Cube(CGraphicObject *parent = nullptr);

public:

    void                setTransform(QMatrix4x4 matrix);
    virtual void        drawElement(camera*  camera);

protected:
    void initData();
    virtual void updateModelData();

private:
    QMatrix4x4          m_transform_Ma;
};

#endif // CUBE_H
