#ifndef WIDGET_TERRAIN_H
#define WIDGET_TERRAIN_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLTexture>
#include <QTime>
#include <QTimer>
#include <QMatrix4x4>
#include <QVector3D>
#include <vector>
#include "GraphicObject.h"

class Shader;
class Cube;
class Axis;
class CGridline;
class CLine;
class Terrain;
class CText2D;

class WidgetTerrain : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit WidgetTerrain(QWidget *parent = 0);
    ~WidgetTerrain();



protected:

    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    virtual void paintEvent(QPaintEvent *e);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    //void keyPressEvent(QKeyEvent* event);
    //void keyReleaseEvent(QKeyEvent* event);

    void updateCamera();
    void getAABB(double AABB[]);
private:

    /*鼠标*/
    QPoint          m_firstPlace;
    QPoint          m_secondPlace;

    GLboolean       m_bleftMousePressed = GL_FALSE;
    GLboolean       m_brightMousePressed = GL_FALSE;
    GLboolean       m_bwheelButtonPressed = GL_FALSE;


    std::vector<CGraphicObject*>    m_graphicsList;

    camera*         m_camera = nullptr;
    Cube*           m_pCube = nullptr;
    Axis*           m_pAxis = nullptr;
    Terrain*        m_pTerrain = nullptr;
    CGridline*      m_pGridline = nullptr;

    CLine*          m_pXAxis = nullptr;
    CLine*          m_pZAxis = nullptr;
    CText2D*        m_pText = nullptr;



    double          m_AABB[6];

    void            drawVersion1();
    void            drawVersion2();
    void            drawVersion3();
    void            addModel();

    QColor          m_backGroundColor = QColor(255, 255, 255);
};

#endif // WIDGET_TERRAIN_H
