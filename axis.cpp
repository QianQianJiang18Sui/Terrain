#include "axis.h"
#include "shader.h"
#include "camera.h"

Axis::Axis(CGraphicObject *parent)
{
    m_pShader = new Shader(":/axis.vert", ":/axis.frag");
    initData();
    setModelData();
}



void Axis::drawElement(camera *camera)
{
    m_pCore->glViewport(0, 0, 100,100);

    m_pShader->use();

    m_pShader->setBool("bEnableColorArray", true);

    m_modelMa.scale(m_scale);
    m_pShader->setMat4("model", m_modelMa);

    auto viewMa = camera->getView();
    m_pShader->setMat4("view", viewMa);

    m_pShader->setMat4("projection", m_proMa);

    m_pCore->glBindVertexArray(m_VAO);
    m_pCore->glDrawArrays(GL_LINES, 0, 6);

    m_pCore->glViewport(0, 0, camera->m_windowWidth, camera->m_windowHeight);
}

void Axis::setModelData()
{
    m_modelMa.setToIdentity();
    //设置坐标轴位置， 移动到Cube的角上
//    QVector4D pos(-1.0, -1.0, -1.0, 1.0);
//    QMatrix4x4 trans;
//    trans.scale(10);
//    QVector4D newPos = trans * pos;
//    QVector3D aPos(newPos.toVector3D());

//    //变换，从下往上进行，先缩放，后平移
//    m_modelMa.translate(aPos);

    m_modelMa.scale(150);
    m_viewMa.setToIdentity();
    //m_viewMa.lookAt(QVector3D(0, 0, 1), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    m_proMa.setToIdentity();
    m_proMa.ortho(-200, 200, -200, 200, -200, 200);

    if (m_pVertex)
    {
        delete[] m_pVertex; m_pVertex = nullptr;
    }

    m_pVertex = new GLfloat[6 * 3]
    {
         .0f, .0f, .0f,
        1.0f, .0f, .0f,
         .0f, .0f, .0f,
         .0f,1.0f, .0f,
         .0f, .0f, .0f,
         .0f, .0f,1.0f,
    };

    m_pColor  = new GLfloat[6 * 3]
    {
        1.0f, .0f, .0f,
        1.0f, .0f, .0f,
         .0f,1.0f, .0f,
         .0f,1.0f, .0f,
         .0f, .0f,1.0f,
         .0f, .0f,1.0f,         
    };
    m_lineNum = 3;

    //创建顶点缓冲
    m_pCore->glGenVertexArrays(1, &m_VAO);
    m_pCore->glBindVertexArray(m_VAO);

    m_pCore->glGenBuffers(1, &m_VBO);
    m_pCore->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_pCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*m_lineNum*2*3, m_pVertex, GL_STATIC_DRAW);
    m_pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_pCore->glEnableVertexAttribArray(0);

    m_pCore->glGenBuffers(1, &m_ColorBuffer);
    m_pCore->glBindBuffer(GL_ARRAY_BUFFER, m_ColorBuffer);
    m_pCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*m_lineNum*2*3, m_pColor, GL_STATIC_DRAW);
    m_pCore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_pCore->glEnableVertexAttribArray(1);
}
