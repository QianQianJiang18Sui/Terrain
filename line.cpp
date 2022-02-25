#include "line.h"

CLine::CLine(CGraphicObject *parent)
{
    m_pShader = new Shader(":/line.vert", ":/line.frag");
}

void CLine::drawElement(camera *camera)
{
    if (m_bNeedUpdate)
    {
        updateModelData();
        m_bNeedUpdate = false;
    }

    m_pShader->use();

    m_pShader->setModelColor("modelColor", m_modelColor);

    m_pShader->setMat4("model", m_modelMa);

    m_viewMa = camera->getView();
    m_pShader->setMat4("view", m_viewMa);

    m_proMa = camera->getPerMatrix();
    m_pShader->setMat4("projection", m_proMa);

    m_pCore->glBindVertexArray(m_VAO);
    m_pCore->glDrawArrays(GL_LINES, 0, 2);
}

void CLine::setLine(QVector3D start, QVector3D end, QColor lineColor, GLfloat lineWidth)
{
    m_lineWidth = lineWidth;
    m_startPos = start;
    m_endPos = end;
    m_modelColor = lineColor;

    if (m_pVertex)
    {
        delete[] m_pVertex; m_pVertex = nullptr;
    }

    m_pVertex = new GLfloat[2 * 3] {
        m_startPos.x(),
        m_startPos.y(),
        m_startPos.z(),
        m_endPos.x(),
        m_endPos.y(),
        m_endPos.z()
    };

    m_ptCount = 2;
    m_bNeedUpdate = true;
}

void CLine::setColor(const QColor &color)
{
    m_modelColor = color;
}

void CLine::updateModelData()
{
    resetOpenGLBuffer();

    m_pCore->glGenVertexArrays(1, &m_VAO);
    m_pCore->glBindVertexArray(m_VAO);

    m_pCore->glGenBuffers(1, &m_VBO);
    m_pCore->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_pCore->glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GL_FLAT), m_pVertex, GL_STATIC_DRAW);
    m_pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3,  0);
    m_pCore->glEnableVertexAttribArray(0);
}
