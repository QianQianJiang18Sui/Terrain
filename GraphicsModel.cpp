#include "GraphicsModel.h"

CGraphicsModel::CGraphicsModel(CGraphicObject *parent)
{

}

void CGraphicsModel::drawElement(camera *camera)
{
    if (m_bNeedUpdate)
    {
        updateModelData();
        m_bNeedUpdate = false;
    }

    if (m_pShader == nullptr)
    {
        return;
    }

    m_pShader->use();

    m_pShader->setMat4("model", m_modelMa);

    m_viewMa = camera->getView();
    m_pShader->setMat4("view", m_viewMa);

    m_proMa = camera->getPerMatrix();
    m_pShader->setMat4("projection", m_proMa);

    m_pCore->glBindVertexArray(m_VAO);
    m_pCore->glDrawArrays(GL_LINES, 0, m_lineNum * 2);
    m_pCore->glDrawArrays(GL_TRIANGLES, 0, m_triNum * 3);
}

//void CGraphicsModel::setParam(QVector2D &start, QVector2D &end, float height, int step, GLfloat lineWidth)
//{
//    if (m_pVertex)
//    {
//        delete[] m_pVertex; m_pVertex = nullptr;
//    }
//    m_lineNum = 0;
//    //m_lineWidth = lineWidth;

//    auto startX = (int)(start.x()) / step * step;
//    auto endX   = ((int) (end.x()) / step + 1) * step;

//    auto startY = (int)(start.y()) / step * step;
//    auto endY   = (int)(end.y())   / step * step;

//    auto numX = abs((endX - startX) / step - 1);
//    auto numY = abs((endY - startY) / step - 1);

//    m_lineNum += numX;
//    m_lineNum += numY;
//    m_lineNum += 4;

//    m_pVertex = new GLfloat[m_lineNum * 6];
//    unsigned int count = 0;
//    //内部网格
//    for (auto i = 0; i < numX; ++i)
//    {
//        m_pVertex[count++] = startX + (i + 1) * step;
//        m_pVertex[count++] = startY;
//        m_pVertex[count++] = height;

//        m_pVertex[count++] = startX + (i + 1) * step;
//        m_pVertex[count++] = endY;
//        m_pVertex[count++] = height;
//    }

//    for (auto i = 0; i < numY; ++i)
//    {
//        m_pVertex[count++] = startX;
//        m_pVertex[count++] = startY + (i + 1) * step;
//        m_pVertex[count++] = height;

//        m_pVertex[count++] = endX;
//        m_pVertex[count++] = startY + (i + 1) * step;
//        m_pVertex[count++] = height;
//    }

//    //外框 ----------
//    m_pVertex[count++] = startX;
//    m_pVertex[count++] = startY;
//    m_pVertex[count++] = height;

//    m_pVertex[count++] = endX;
//    m_pVertex[count++] = startY;
//    m_pVertex[count++] = height;

//    m_pVertex[count++] = startX;		   //  |
//    m_pVertex[count++] = startY;		   //  |
//    m_pVertex[count++] = height;           //  |
//                                           //  |
//    m_pVertex[count++] = startX;		   //  |
//    m_pVertex[count++] = endY;             //  |
//    m_pVertex[count++] = height;           //  |

//    m_pVertex[count++] = endX;		       //                                      |
//    m_pVertex[count++] = startY;		   //                                      |
//    m_pVertex[count++] = height;		   //                                      |
//                                           //                                      |
//    m_pVertex[count++] = endX;             //                                      |
//    m_pVertex[count++] = endY;             //                                      |
//    m_pVertex[count++] = height;		   //                                      |

//                                           //     -------------------------------
//    m_pVertex[count++] = startX;
//    m_pVertex[count++] = endY;
//    m_pVertex[count++] = height;

//    m_pVertex[count++] = endX;
//    m_pVertex[count++] = endY;
//    m_pVertex[count++] = height;

//    m_ptCount = m_lineNum * 2;
//    m_bNeedUpdate = true;
//}

void CGraphicsModel::setModelData(GLfloat *pVerts, int triNum, int lineNum, GLfloat *pNormal, GLfloat *pColor)
{
    if (pVerts)
    {
        if (m_pVertex)
        {
            delete[] m_pVertex, m_pVertex = nullptr;
        }


    }

    m_bNeedUpdate = true;
}

void CGraphicsModel::setColor(QColor color)
{
    m_ModelColor = color;
    if (m_pShader)
    {
        QVector3D uniColor(color.redF(), color.greenF(), color.blueF());
        m_pShader->setVec3(QString("modelColor"), uniColor);
    }
}

void CGraphicsModel::updateModelData()
{
    initData();

    m_pCore->glGenVertexArrays(1, &m_VAO);
    m_pCore->glBindVertexArray(m_VAO);

    m_pCore->glGenBuffers(1, &m_VBO);
    m_pCore->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_pCore->glBufferData(GL_ARRAY_BUFFER, (m_lineNum * 6 + m_triNum * 9) * sizeof(GL_FLAT), m_pVertex, GL_STATIC_DRAW);
    m_pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3,  0);
    m_pCore->glEnableVertexAttribArray(0);
}

void CGraphicsModel::initData()
{
    CGraphicObject::resetOpenGLBuffer();
    if (m_ColorBuffer)
    {
        m_pCore->glDeleteBuffers(1, &m_ColorBuffer);
        m_ColorBuffer = 0;
    }

    if (m_NormalBuffer)
    {
        m_pCore->glDeleteBuffers(1, &m_NormalBuffer);
        m_NormalBuffer = 0;
    }
}
