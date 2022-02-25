#include "triangle_strip.h"
#include "MathTool.h"

CTriAnlgeStrip::CTriAnlgeStrip(CGraphicObject *parent)
{
    m_object_type = OBJ_TERRAIN_FACE;
    m_pShader = new Shader(":/triangle_strip.vert", ":/triangle_strip.frag");
}

CTriAnlgeStrip::~CTriAnlgeStrip()
{
}

void CTriAnlgeStrip::drawElement(camera *camera)
{
    if (m_bNeedUpdate)
    {
        updateModelData();
        m_bNeedUpdate = false;
    }

    m_pShader->use();
    m_pShader->setModelColor("modelColor", m_ElementColor);
    m_pShader->setVec3("cameraPos", camera->getCameraPos());

    m_pShader->setMat4("model", m_modelMa);

    m_viewMa = camera->getView();
    m_pShader->setMat4("view", m_viewMa);

    m_proMa = camera->getPerMatrix();
    m_pShader->setMat4("projection", m_proMa);

    m_pCore->glBindVertexArray(m_VAO);
    m_pCore->glDrawArrays(GL_TRIANGLE_STRIP, 0, m_ptCount);
}

void CTriAnlgeStrip::setTriangleStrip(std::vector<float> pts, QVector3D normal)
{
    if (m_pVertex)
    {
        delete[] m_pVertex; m_pVertex = nullptr;
    }

    if (m_pNormal)
    {
        delete[] m_pNormal; m_pNormal = nullptr;
    }

    m_pVertex = new GLfloat[pts.size()];
    memcpy(m_pVertex, pts.data(), sizeof(float) * pts.size());
    m_ptCount = pts.size() / 3;

    m_pNormal = new GLfloat[pts.size()];
    for(auto i = 0;i < m_ptCount; ++i)
    {
        m_pNormal[i * 3 + 0] = normal.x();
        m_pNormal[i * 3 + 1] = normal.y();
        m_pNormal[i * 3 + 2] = normal.z();
    }

    m_bNeedUpdate = true;
}

void CTriAnlgeStrip::updateModelData()
{
    resetOpenGLBuffer();

    m_pCore->glGenVertexArrays(1, &m_VAO);
    m_pCore->glBindVertexArray(m_VAO);

    m_pCore->glGenBuffers(1, &m_VBO);
    m_pCore->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_pCore->glBufferData(GL_ARRAY_BUFFER, m_ptCount * 3 * sizeof(GL_FLAT), m_pVertex, GL_STATIC_DRAW);
    m_pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3,  0);
    m_pCore->glEnableVertexAttribArray(0);

    m_pCore->glGenBuffers(1, &m_NBO);
    m_pCore->glBindBuffer(GL_ARRAY_BUFFER, m_NBO);
    m_pCore->glBufferData(GL_ARRAY_BUFFER, m_ptCount * 3 * sizeof(GL_FLAT), m_pNormal, GL_STATIC_DRAW);
    m_pCore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3,  0);
    m_pCore->glEnableVertexAttribArray(1);
}
