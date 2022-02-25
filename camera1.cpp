#include "camera.h"
#include <QtMath>

Camera::Camera(QVector3D postion, QVector3D up, float yaw, float pitch) :m_camera_front(QVector3D(.0, .0, -1.0)), m_moveSpeed(SPEED), m_mouseSensitivity(SENSTIVITY), m_zoom(ZOOM)
{
    this->m_camera_pos = postion;
    this->m_world_up = up;

    this->m_yaw = yaw;
    this->m_pitch = pitch;

    this->updateCameraVector();

    for (int i = 0; i < 1024; ++i)
    {
        m_keys[i] = false;
    }
}

QMatrix4x4 Camera::getViewMatrix()
{
    QMatrix4x4 view;
    view.setToIdentity();
    view.lookAt(m_camera_pos, m_camera_pos + m_camera_front, m_world_up);
    return view;
}

QMatrix4x4 Camera::getPerMatrix()
{
    QMatrix4x4 perspectiveMa;
    perspectiveMa.setToIdentity();
    perspectiveMa.perspective(m_zoom, m_width / m_height, 0.1f, 100.0f);
    return perspectiveMa;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constraintPitch)
{
    xoffset *= this->m_mouseSensitivity;
    yoffset *= this->m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constraintPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0;
        if (m_pitch < -89.0)
            m_pitch = -89.0;
    }

    this->updateCameraVector();
}

void Camera::processMouseWheel(float yoffset)
{
    if (m_zoom >= 1.0 && m_zoom <= 45.0)
    {
        this->m_zoom -= yoffset;
    }

    if (m_zoom > 45.0)
        m_zoom = 45.0;

    if (m_zoom < 1.0)
        m_zoom = 1.0;
}

void Camera::processInput(float dt)
{
    if (m_keys[Qt::Key_W])
        processKeyBoard(FORWARD, dt);
    if (m_keys[Qt::Key_S])
        processKeyBoard(BACKWARD, dt);
    if (m_keys[Qt::Key_A])
        processKeyBoard(LEFT, dt);
    if (m_keys[Qt::Key_D])
        processKeyBoard(RIGHT, dt);
    if (m_keys[Qt::Key_E])
        processKeyBoard(UP, dt);
    if (m_keys[Qt::Key_Q])
        processKeyBoard(DOWN, dt);
}

void Camera::move(int beginx, int beginy, int endx, int endy)
{
    auto vecScreenX = endx - beginx;
    auto vecScrrenY = endy - beginy;

    auto rotateCenter = m_rotateCenter;
    auto distance = .0;
    auto move = .0;

    distance = (rotateCenter - m_camera_pos).length();
    if (qFabs(distance) < 0.00001)
        move = 0.00001;
    else
        move = qFabs(distance * 0.001);

    //moveRelative()
    //QQuaternion
}

void Camera::updateCameraVector()
{
    auto x = qCos(qDegreesToRadians(this->m_yaw)) * qCos(qDegreesToRadians(this->m_pitch));
    auto y = qSin(qDegreesToRadians(this->m_pitch));
    auto z = qSin(qDegreesToRadians(this->m_yaw)) * qCos(qDegreesToRadians(this->m_pitch));

//    auto x = qCos((this->m_yaw)) * qCos((this->m_pitch));
//    auto y = qSin((this->m_pitch));
//    auto z = qSin((this->m_yaw)) * qCos((this->m_pitch));

    QVector3D front(x, y, z);

    this->m_camera_front = front.normalized();
    this->m_view_right = QVector3D::crossProduct(this->m_camera_front, this->m_world_up).normalized();
    this->m_view_up = QVector3D::crossProduct(this->m_view_right, this->m_camera_front).normalized();
}

void Camera::processKeyBoard(Camera_Movement direction, float deltaTime)
{
    float velocity = this->m_moveSpeed * deltaTime;
    switch (direction) {
    case FORWARD:
        m_camera_pos += this->m_camera_front * velocity;
        break;
    case BACKWARD:
        m_camera_pos -= this->m_camera_front * velocity;
        break;
    case LEFT:
        m_camera_pos -= this->m_view_right * velocity;
        break;
    case RIGHT:
        m_camera_pos += this->m_view_right * velocity;
        break;
    case UP:
        m_camera_pos += this->m_view_up * velocity;
        break;
    case DOWN:
        m_camera_pos -= this->m_view_up * velocity;
        break;
    default:
        break;
    }

    this->updateCameraVector();
}
