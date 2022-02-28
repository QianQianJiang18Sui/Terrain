#include "terrain_widget.h"
#include <QOpenGLVertexArrayObject>
#include <QDebug>
#include <QtMath>
#include <QMouseEvent>
#include <QtMath>
#include <QSurfaceFormat>

#include <QPainter>
#include <QLinearGradient>


#include "shader.h"
#include "camera.h"
#include "cube.h"
#include "axis.h"
#include "terrain.h"
#include "gridline.h"
#include "line.h"
#include "Text2D.h"

#include <QImage>


WidgetTerrain::WidgetTerrain(QWidget *parent) :
    QOpenGLWidget(parent)
{
    //抗锯齿
    QSurfaceFormat surface;
    surface.setSamples(4); //多重采样
    setFormat(surface);

    m_camera = new camera(QVector3D(0.0, 0.0, 3.0));

    for (auto i = 0; i < 6; ++i)
    {
        if (i % 2 == 0)
        {
            m_AABB[i] = DBL_MAX;
        }
        else
        {
            m_AABB[i] = DBL_MIN;
        }
    }
}


WidgetTerrain::~WidgetTerrain()
{

}

void WidgetTerrain::initializeGL()
{
    this->initializeOpenGLFunctions();

    //vec4(88/255.0f, 195/255.0f, 0.95, 0.2);
    //创建绘制对象
    m_pCube = new Cube();
    m_pCube->SetElementColor(QColor(30, 144, 255, 50));
    m_pAxis = new Axis();

    m_pTerrain = new Terrain();
    addModel();

    m_pGridline = new CGridline();
    m_pGridline->setGridType(GRID_XOZ);

    m_pXAxis = new CLine();
    m_pZAxis = new CLine();

    //m_pText = new CText2D();
    //m_pText->setText(QString("AAAAAAAAAA"), 100, QColor(255,255,255,255));

    m_graphicsList.push_back(m_pCube);
    m_graphicsList.push_back(m_pXAxis);
    m_graphicsList.push_back(m_pTerrain);
    m_graphicsList.push_back(m_pGridline);
    m_graphicsList.push_back(m_pZAxis);

    updateCamera();

//    m_pModel = new CGraphicsModel();
//    addModel();
//    m_pModel->getAABB(m_AABB);
//    updateCamera();

    //opengl 开启深度测试， 背景颜色
    m_backGroundColor = QColor(0, 0, 0);
    glClearColor(m_backGroundColor.redF(), m_backGroundColor.greenF(), m_backGroundColor.blueF(), 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); //多重采样

    m_bleftMousePressed = GL_FALSE;

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WidgetTerrain::paintGL()
{
    //更新相机

    //清空状态
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    m_pAxis->drawElement(m_camera);
    //drawVersion1();
    //m_pText->drawElement(m_camera);
    //drawVersion2();
    update();
}



void WidgetTerrain::resizeGL(int w, int h)
{
    m_camera->m_windowWidth = (float)w;
    m_camera->m_windowHeight = (float)h;
    this->glViewport(0, 0, w, h);
}

void WidgetTerrain::paintEvent(QPaintEvent *e)
{
    if (this->width() > 1 && this->height() > 1)
    {
//        QPainter painter(this);
//        //反走样
//        painter.setRenderHint(QPainter::Antialiasing, true);

//        QLinearGradient linear(100, 100, 200, 100);
//        linear.setColorAt(0, QColor(100, 0, 0));
//        linear.setColorAt(0.15, QColor(255, 30, 0));
//        linear.setColorAt(0.35, QColor(255, 255, 0));
//        linear.setColorAt(0.65, QColor(0, 255, 255));
//        linear.setColorAt(0.85, QColor(0, 50, 255));
//        linear.setColorAt(1.0, QColor(0, 0, 100));

//        linear.setSpread(QGradient::PadSpread);

//        painter.setPen(QPen(QColor(255,255,255,0), 1));
//        painter.setBrush(QBrush(linear));

//        painter.drawRect(QRect(100, 100, 200, 100));
    }
    return QOpenGLWidget::paintEvent(e);
}

void WidgetTerrain::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bleftMousePressed = GL_TRUE;
    }
    else if (event->button() == Qt::RightButton)
    {
        m_brightMousePressed = GL_TRUE;
        m_firstPlace = event->pos();
    }
    else if (event->button() == Qt::MiddleButton)
    {
        m_bwheelButtonPressed = GL_TRUE;
        m_firstPlace = event->pos();
    }
}

void WidgetTerrain::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bleftMousePressed = GL_FALSE;
    }

    if (event->button() == Qt::RightButton)
    {
        m_brightMousePressed = GL_FALSE;
    }

    if (event->button() == Qt::MiddleButton)
    {
        m_bwheelButtonPressed = GL_FALSE;
    }
}

void WidgetTerrain::mouseMoveEvent(QMouseEvent *event)
{
    if (m_brightMousePressed)
    {
        m_secondPlace = event->pos();
        QPoint DisPoint = m_secondPlace - this->m_firstPlace;
        if(DisPoint.manhattanLength() > 50) //当鼠标移动了一定的距离后
        {
            this->m_firstPlace = event->pos(); //更新起点位置
            int MatDis = this->height();
            if(this->width() > this->height())
                MatDis = this->width();
            float xOffset = -(float) DisPoint.x() / MatDis;
            float yOffset = (float) DisPoint.y() / MatDis;
            m_camera->rotateCamera(xOffset,yOffset);
        }
    }
    else if(m_bwheelButtonPressed) //平移模型
    {
        //return QOpenGLWidget::mouseMoveEvent(event);
        m_secondPlace = event->pos();
        QPoint DisPoint = this->m_secondPlace - this->m_firstPlace;
        if(DisPoint.manhattanLength() > 30) //当鼠标移动了一定的距离后
        {
            this->m_firstPlace = event->pos(); //更新起点位置
            float xOffset = (float) DisPoint.x() / this->width();
            float yOffset = -(float) DisPoint.y() / this->height();
            m_camera->translateWorldM(xOffset,yOffset);

            for (auto i = 0; i < m_graphicsList.size(); ++i)
            {
                m_graphicsList[i]->setModelMatrix(m_camera->getWorldM());
            }
        }
     }
}

void WidgetTerrain::wheelEvent(QWheelEvent *event)
{
    float rotatef = event->angleDelta().y() / 40.0f;
    m_camera->scale(rotatef);
}

/*
    AABB
    minX, maxX,
    minY, maxY,
    minZ, maxZ 组成的包围框

*/
void WidgetTerrain::getAABB(double AABB[6])
{
    if (m_pCube == nullptr)
    {
        return;
    }
    m_pCube->getAABB(AABB);
}

void WidgetTerrain::drawVersion1()
{
    if (m_pTerrain && m_pAxis && m_pGridline && m_pCube)
    {
        //绘制 先绘制地形，再绘制cube，包围，否则blend混合错误
        m_pTerrain->drawElement(m_camera);

        m_pXAxis->drawElement(m_camera);
        m_pZAxis->drawElement(m_camera);
        m_pGridline->drawElement(m_camera);

        m_pCube->drawElement(m_camera);
    }
}

void WidgetTerrain::drawVersion2()
{
    m_pXAxis->drawElement(m_camera);
    m_pZAxis->drawElement(m_camera);
    m_pGridline->drawElement(m_camera);
}

void WidgetTerrain::addModel()
{
    QImage heightImage(":/img/layer_1.png");
    if (m_pTerrain)
    {
        m_pTerrain->loadFromImage(heightImage);
    }
}

void WidgetTerrain::updateCamera()
{
//    for (auto i = 0; i < m_graphicsList.size(); ++i)
//    {
//        m_graphicsList[i]->getAABB(m_AABB);
//    }

    m_pTerrain->getAABB(m_AABB);

    QVector2D leftBottom(m_AABB[0] - 2, m_AABB[4] - 2);
    QVector2D rightTop(m_AABB[1] + 2, m_AABB[5] + 2);
    m_pGridline->setParam(leftBottom, rightTop, m_AABB[2] - 1, 1);

    m_camera->m_windowHeight = this->height();
    m_camera->m_windowWidth  = this->width();

    float camDisNow = m_camera->getDiatance();
    //几何体中提取包裹立方体最大的边
    auto geoLength = .0;
    double maxVal = DBL_MIN;
    for (auto i = 0;i < 6; ++i)
    {
        maxVal = maxVal > qFabs(m_AABB[i]) ? maxVal : qFabs(m_AABB[i]);
        geoLength = qFabs(m_AABB[i]) >  geoLength ? qFabs(m_AABB[i]) : geoLength;
    }
    float scalef = (geoLength / camDisNow) * 5; //相机距离是几何体最大边的5倍
    m_camera->initialCamera(m_camera->getCameraOriPositon() * scalef);

    QMatrix4x4 trans;
    trans.translate(QVector3D((m_AABB[0] + m_AABB[1])/2/1.3, (m_AABB[2] + m_AABB[3])/2/1.3, (m_AABB[4] + m_AABB[5])/2)/1.3);
    trans.scale(qFabs(m_AABB[1] - m_AABB[0])/2, qFabs(m_AABB[3] - m_AABB[2])/2, qFabs(m_AABB[5] - m_AABB[4])/2);
    m_pCube->setTransform(trans);

    QVector3D XstartPos(m_AABB[0], m_AABB[2], 0);
    QVector3D XendPos(m_AABB[1], m_AABB[2], 0);
    m_pXAxis->setLine(XstartPos, XendPos);

    QVector3D ZstartPos(0, m_AABB[2], m_AABB[4]);
    QVector3D ZendPos(0, m_AABB[2], m_AABB[5]);
    m_pZAxis->setLine(ZstartPos, ZendPos);
}


