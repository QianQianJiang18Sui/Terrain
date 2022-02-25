#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QDebug>

#include "ArcBall.h"

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSTIVITY = 0.0f;
const float ZOOM = 45.0f;

class Camera
{
public:
    Camera(QVector3D postion = QVector3D(.0, .0, .0),
           QVector3D up = QVector3D(.0, 1.0, .0),
           float yaw = YAW,
           float pitch = PITCH);

    QMatrix4x4 getViewMatrix();
    QMatrix4x4 getPerMatrix();

    void processMouseMovement(float xoffset, float yoffset, bool constraintPitch = true);
    void processMouseWheel(float yoffset);
    void processInput(float dt);

    //arcBall
    void move(int beginx, int beginy, int endx, int endy);
    QVector3D   m_rotateCenter;


    ///////////

    QVector3D   m_camera_pos;
    QVector3D   m_world_up;
    QVector3D   m_camera_front;

    float     m_yaw;
    float     m_pitch;

    float     m_moveSpeed;
    float     m_mouseSensitivity;
    float     m_zoom;

    QVector3D   m_view_up;
    QVector3D   m_view_right;

    bool   m_keys[1024];

    float     m_width = .0f;
    float     m_height = .0f;

private:
    void updateCameraVector();
    void processKeyBoard(Camera_Movement direction, float deltaTime);

    ArcBall_t   m_arcball{100, 100};
};

#endif // CAMERA_H
