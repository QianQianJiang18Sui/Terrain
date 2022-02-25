#include "GraphicObject.h"


CGraphicObject::CGraphicObject(CGraphicObject *parent)
{
    m_pCore = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    m_modelMa.setToIdentity();
    m_viewMa.setToIdentity();
    m_proMa.setToIdentity();
}

CGraphicObject::~CGraphicObject()
{
    CGraphicObject::resetOpenGLBuffer();
    if (m_pVertex)
    {
        delete[] m_pVertex;
        m_pVertex = nullptr;
    }

    if (m_pIndex)
    {
        delete[] m_pIndex;
        m_pIndex = nullptr;
    }

    if (m_pNormal)
    {
        delete[] m_pNormal;
        m_pNormal = nullptr;
    }

    if (m_pTexture)
        delete m_pTexture, m_pTexture = nullptr;

    if (m_pShader)
        delete m_pShader, m_pShader = nullptr;

}

QMatrix4x4 CGraphicObject::getTransformMatrix()
{
    QMatrix4x4 ma;
    ma.setToIdentity();
    ma = m_proMa * m_viewMa * m_modelMa;
    return ma;
}

void CGraphicObject::drawElement(camera *camera)
{

}

void CGraphicObject::getAABB(double AABB[])
{
    if (m_pVertex == nullptr || m_ptCount == 0)
    {
        return;
    }

    for (auto i = 0; i < m_ptCount; ++i)
    {
        auto x = m_pVertex[i * 3 + 0];
        auto y = m_pVertex[i * 3 + 1];
        auto z = m_pVertex[i * 3 + 2];

        AABB[0] = AABB[0] < x ? AABB[0] : x;
        AABB[2] = AABB[2] < y ? AABB[2] : y;
        AABB[4] = AABB[4] < z ? AABB[4] : z;

        AABB[1] = AABB[1] > x ? AABB[1] : x;
        AABB[3] = AABB[3] > y ? AABB[3] : y;
        AABB[5] = AABB[5] > z ? AABB[5] : z;
    }

    //转为世界坐标系坐标
    if (m_modelMa.isIdentity() == false)
    {
        QVector4D min(AABB[0], AABB[2], AABB[4], 1.0);
        QVector4D max(AABB[1], AABB[3], AABB[5], 1.0);
        min = m_modelMa * min;
        max = m_modelMa * max;

        //缩放1.5倍，留边
//        QMatrix4x4 ma;
//        ma.scale(1.5);
//        min = ma * min;
//        max = ma * max;

        //赋值
        AABB[0] = min.x();
        AABB[1] = max.x();
        AABB[2] = min.y();
        AABB[3] = max.y();
        AABB[4] = min.z();
        AABB[5] = max.z();
    }
}

void CGraphicObject::setScale(double val)
{
    m_scale = val / m_scale;
    m_modelMa.scale(m_scale);
}

void CGraphicObject::UpdateModel()
{

}

void CGraphicObject::resetOpenGLBuffer()
{
    if (m_VAO)
        m_pCore->glDeleteVertexArrays(1, &m_VAO);

    if (m_VBO)
        m_pCore->glDeleteBuffers(1, &m_VBO);

    if (m_EBO)
        m_pCore->glDeleteBuffers(1, &m_VBO);

    if (m_NBO)
        m_pCore->glDeleteBuffers(1, &m_NBO);

    m_VAO = 0;
    m_VBO = 0;
    m_EBO = 0;
    m_NBO = 0;

    if (m_Color)
        m_pCore->glDeleteBuffers(1, &m_Color);

    if (m_Texture)
        m_pCore->glDeleteBuffers(1, &m_Texture);

    m_Color = 0; m_Texture = 0;
}

void CGraphicObject::updateModelData()
{

}

QMatrix4x4 CGraphicObject::getModelViewMatrix()
{
    QMatrix4x4 ma;
    ma = m_viewMa * m_modelMa;
    return ma;
}

QMatrix4x4 CGraphicObject::getViewProMatrix()
{
    QMatrix4x4 ma;
    ma = m_proMa * m_viewMa;
    return ma;
}

QMatrix4x4 CGraphicObject::getModelMatrix()
{
    return m_modelMa;
}

/* 累乘 变换顺序 从右向左*/
void CGraphicObject::setModelMatrix(QMatrix4x4 ma)
{
    m_modelMa = ma * m_modelMa;
}
