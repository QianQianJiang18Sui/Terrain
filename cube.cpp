#include "cube.h"
#include "shader.h"
#include "camera.h"

Cube::Cube(CGraphicObject *parent)
{
    m_pShader = new Shader(":/cube.vert", ":/cube.frag");
    initData();
    m_transform_Ma.setToIdentity();
}

void Cube::setTransform(QMatrix4x4 matrix)
{
    m_transform_Ma.inverted();
    m_modelMa = m_modelMa * m_transform_Ma;
    m_modelMa *= matrix;
    m_transform_Ma = matrix;
}

void Cube::drawElement(camera *camera)
{
//    if (m_Blend)
//    {
//        m_pCore->glEnable(GL_BLEND);
//    }
    m_pShader->use();

    //m_modelMa.translate();
    m_pShader->setVec4("modelColor", QVector4D(m_ElementColor.redF(), m_ElementColor.greenF(), m_ElementColor.blueF(), m_ElementColor.alphaF()));
    m_pShader->setMat4("model", m_modelMa);

    m_viewMa = camera->getView();
    m_pShader->setMat4("view", m_viewMa);

    m_proMa = camera->getPerMatrix();
    m_pShader->setMat4("projection", m_proMa);

    m_pCore->glBindVertexArray(m_VAO);
    m_pCore->glDrawArrays(GL_TRIANGLES, 0, 36);

    //m_pCore->glDisable(GL_BLEND);
}

void Cube::initData()
{
    m_modelMa.setToIdentity();

    m_viewMa.setToIdentity();
    m_proMa.setToIdentity();

    m_pCore->glGenVertexArrays(1, &m_VAO);
    m_pCore->glGenBuffers(1, &m_VBO);

    m_pCore->glBindVertexArray(m_VAO);
    m_pCore->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    if (m_pVertex)
    {
        delete[] m_pVertex; m_pVertex = nullptr;
    }

    m_pVertex = new GLfloat[36 * 3];

    GLfloat vertex[36 * 3] = {
       -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
       -1.0f,  1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,

       -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f,

       -1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,

        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,

       -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
       -1.0f, -1.0f,  1.0f,
       -1.0f, -1.0f, -1.0f,

       -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f,  1.0f,
       -1.0f,  1.0f, -1.0f
    };
    memcpy(m_pVertex, vertex, sizeof(GL_FLOAT)*36*3);

    m_pCore->glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), m_pVertex, GL_STATIC_DRAW);
    m_pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3,  0);
    m_pCore->glEnableVertexAttribArray(0);

}

void Cube::updateModelData()
{

}
