#ifndef AXIS_H
#define AXIS_H

#include "GraphicsModel.h"

class Axis : public CGraphicsModel
{
public:
    explicit Axis(CGraphicObject *parent = nullptr);

public:

    virtual void    drawElement(camera*  camera);

protected:

    void setModelData();
};

#endif // AXIS_H
