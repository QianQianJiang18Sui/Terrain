#ifndef TEXT_2D_H
#define TEXT_2D_H

#include "GraphicObject.h"

class CText2D : public CGraphicObject
{
public:
    explicit CText2D(CGraphicObject *parent = nullptr);
    virtual ~CText2D();
public:

    virtual void        drawElement(camera*  camera);
    void                setText(const QString &text, int textPixelSize = 10, QColor textColor = QColor(255,255,0,255), QFont font = QFont());
    void                setRotate(float angle, QVector3D axis);
    void                setPersType();
protected:

    GLfloat*            m_pTextureCoord = nullptr;

    virtual void        updateModelData();

    float               m_rotate_angle = 0;
    QVector3D           m_rotate_axis = QVector3D(0, 1, 0);
};

#endif // TEXT_2D_H
