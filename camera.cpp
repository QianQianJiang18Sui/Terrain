#include "camera.h"
#include <QDebug>
#include <QVector3D>

camera::camera(QVector3D camPosition)
{
    initialCamera(camPosition);
}

QMatrix4x4 camera::getView()
{
    QMatrix4x4 Identiy;
    Identiy.lookAt(this->position,this->target,this->worldUp); //产生了一个视图变化矩阵
    view = Identiy;
    return view;
}

QMatrix4x4 camera::getPerMatrix()
{
    QMatrix4x4 projection;
    if (fabs(m_windowHeight) < 0.00000001 )
    {
        return projection;
    }
    projection.perspective(m_zoom, m_windowWidth / m_windowHeight, 0.1f, 100000.0f); //相机投影矩阵
    //projection.ortho(-10,10,-10,10,-10,10);
    return projection;
}

void camera::rotateCamera(float xOffset, float yOffset)
{
    xOffset = xOffset * sensitivity;
    yOffset = yOffset * sensitivity;

    QVector3D BallCenter(0,0,-1); //arcball球心坐标

    //获取球上两个坐标点 以及形成的向量
    QVector3D pointOne(0,0,0);
    float zOffset = sqrt((1 - xOffset * xOffset - yOffset * yOffset)) - 1;
    QVector3D pointTwo(xOffset,yOffset,zOffset);
    QVector3D vecOne(pointOne - BallCenter);
    QVector3D vecTwo(pointTwo - BallCenter);

    //获取旋转轴
    QVector3D axisView = QVector3D::crossProduct(vecOne,vecTwo); //相机视野中的旋转轴
    axisView.normalize();
    QVector4D axisWorld4D = this->view.inverted() * QVector4D(axisView[0],axisView[1],axisView[2],1); //世界坐标系中的旋转轴
    QVector3D axisWorld = QVector3D(axisWorld4D[0] - this->position[0],
            axisWorld4D[1] - this->position[1],axisWorld4D[2] - this->position[2]);
    axisWorld.normalize();

    //获取旋转角
    float theta = acos(QVector3D::dotProduct(vecOne,vecTwo));//vecOne和vecTwo是单位向量

    //获取世界变换矩阵
    worldRotateM = this->getWorldRotateM(axisWorld,theta);

    //更新相机的位置
    this->position = worldRotateM * this->position;

    //更新worldUp, 这样做是为了lookat函数中目标方向与世界坐标系的上方方向向量的叉乘方向避免突变
    QVector4D worldUpTemp = QVector4D(0,1,0,1);//当前坐标系的Y轴 单位向量 （y轴上一点）
    worldUpTemp = this->view.inverted() * worldUpTemp;  //世界坐标系下 单位向量的坐标
    worldUpTemp = worldRotateM * worldUpTemp; //世界坐标系发生了旋转，所以这个点也要旋转
    QVector3D worldUpPos = QVector3D(worldUpTemp[0],worldUpTemp[1],worldUpTemp[2]);
    this->worldUp = worldUpPos - this->position; // 新的世界坐标系的worldUp
}

QMatrix4x4 camera::getWorldRotateM(QVector3D axis, float theta)
{
    if(axis.length() == 0) //输入的轴有问题
    {
        //这里应该添加异常处理程序,方便代码调试
        QMatrix4x4 Identiy;
        return Identiy;
    }
    //构建平移矩阵
    QMatrix4x4 Trans;
    Trans(0,3) = -this->target[0];
    Trans(1,3) = -this->target[1];
    Trans(2,3) = -this->target[2];


    //绕X轴旋转 使轴落入xoz平面内
    float cosA;
    float sinA;
    float u = sqrt(axis[1]*axis[1]+axis[2]*axis[2]);
    if( u < 0.001) //表示已经在X轴上
    {
        cosA = 1;
        sinA = 0;
    }
    else
    {
        cosA = axis[2] / u;
        sinA = axis[1] / u;
    }
    QMatrix4x4 rotateToXZ;
    rotateToXZ(1,1) = cosA;
    rotateToXZ(2,1) = sinA;
    rotateToXZ(1,2) = -sinA;
    rotateToXZ(2,2) = cosA;

    //绕y轴旋转 与Z轴重合
    float v = sqrt(u*u + axis[0]*axis[0]);
    float cosB = u / v;
    float sinB = axis[0] / v;
    QMatrix4x4 rotateToZ;
    rotateToZ(0,0) = cosB;
    rotateToZ(0,2) = -sinB;
    rotateToZ(2,0) = sinB;
    rotateToZ(2,2) = cosB;

    //旋转theta角
    QMatrix4x4 rotateTheta;
    rotateTheta(0,0) = cos(theta);
    rotateTheta(0,1) = -sin(theta);
    rotateTheta(1,0) = sin(theta);
    rotateTheta(1,1) = cos(theta);

    //世界坐标系变换矩阵
    QMatrix4x4 worldRotateM = Trans.inverted() * rotateToXZ.inverted() * rotateToZ.inverted() * rotateTheta *
            rotateToZ * rotateToXZ * Trans;

    return  worldRotateM;
}

void camera::scale(float scalef)
{
    if (m_zoom >= 1 && m_zoom <= 179.0)
    {
        this->m_zoom -= scalef;
    }

    if (m_zoom > 180.0)
        m_zoom = 179.0;

    if (m_zoom < 1)
        m_zoom = 1;
}

void camera::translateWorldM(float xOffset, float yOffset)
{
    //计算平移的方向
    QVector4D TransInView(xOffset,yOffset,0,1);
    //向量的变换 必须是向量的两个端点同时变换，简单起见可以假设向量的起始点都是原坐标系的原点
    QVector4D TransInWorld = this->view.inverted() * TransInView - QVector4D(position[0],position[1],position[2],1);

    // 计算平移的距离
    float TransDis = sqrt(this->position.length())/2;
    TransInWorld = TransInWorld * TransDis;

    //改变世界矩阵
    this->worldM(0,3) += TransInWorld.x() * TransDis;
    this->worldM(1,3) += TransInWorld.y() * TransDis;
    this->worldM(2,3) += TransInWorld.z() * TransDis;
}

QMatrix4x4 camera::getWorldM()
{
    return this->worldM;
}

float camera::getDiatance() const
{
    return this->position.length();
}

void camera::initialCamera(QVector3D cameraPositon)
{
    this->position = cameraPositon;
    this->OrignPosition = cameraPositon;
    this->target = QVector3D(0,0,0);

    //根据相机位置确定上向量，当相机处在世界坐标系的Y轴上时，用（0，1，0）做上向量不合适
    if(abs(this->OrignPosition[0]) < 0.01 && abs(this->OrignPosition[2]) < 0.01)
    {
            this->worldUp = QVector3D(0,1,1);
    }
    else
    {
            this->worldUp = QVector3D(0,1,0);
    }
    this->view = this->getView();
    this->sensitivity = 3.0;
    this->worldM = QMatrix4x4();
    this->worldRotateM = QMatrix4x4();
    this->m_zoom = 45.0f;
}

QVector3D camera::getCameraOriPositon()
{
    return this->OrignPosition;
}

void camera::reSetCamera()
{
    this->initialCamera(this->OrignPosition);
}

void camera::setCameraXPlus()
{
    float Dis = this->OrignPosition.length();
    this->initialCamera(QVector3D(Dis,0,0));
}

void camera::setCameraXMinus()
{
    float Dis = this->OrignPosition.length();
    this->initialCamera(QVector3D(-Dis,0,0));
}

void camera::setCameraYPlus()
{
    float Dis = this->OrignPosition.length();
    this->initialCamera(QVector3D(0,-Dis,0));
}

void camera::setCameraYMinus()
{
    float Dis = this->OrignPosition.length();
    this->initialCamera(QVector3D(0,Dis,0));
}

void camera::setCameraZPlus()
{
    float Dis = this->OrignPosition.length();
    this->initialCamera(QVector3D(0,0,Dis));
}

void camera::setCameraZMinus()
{
    float Dis = this->OrignPosition.length();
    this->initialCamera(QVector3D(0,0,-Dis));
}

void camera::setCameraIsometric()
{
    float Dis = this->OrignPosition.length();
    this->initialCamera(QVector3D(Dis / sqrt(2),Dis / 2,Dis / 2));
}
