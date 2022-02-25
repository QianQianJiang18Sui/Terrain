#include "terrain.h"
#include "gridline.h"
#include "rgradient.h"
#include "triangle_strip.h"
#include "MathTool.h"


Terrain::Terrain(CGraphicObject *parent)
{
    m_pShader = new Shader(":/terrain.vert", ":/terrain.frag");

    m_pGradient = new RGradient();
    m_pGradient->setColorStopAt(0, QColor(100, 0, 0));
    m_pGradient->setColorStopAt(0.15, QColor(255, 30, 0));
    m_pGradient->setColorStopAt(0.35, QColor(255, 255, 0));
    m_pGradient->setColorStopAt(0.65, QColor(0, 255, 255));
    m_pGradient->setColorStopAt(0.85, QColor(0, 50, 255));
    m_pGradient->setColorStopAt(1.0, QColor(0, 0, 100));

//    m_pGradient->setColorStopAt(0, QColor(0, 0, 255));
//    m_pGradient->setColorStopAt(1, QColor(255, 0, 0));

    m_pGradient->createColorBuffer();
}

Terrain::~Terrain()
{

}

void Terrain::drawElement(camera *camera)
{
    if (m_bNeedUpdate)
    {
        updateModelData();
        m_bNeedUpdate = false;
    }
    m_pShader->use();

    m_pShader->setBool("bEnableColorArray", m_bEnableColorArray);
    m_pShader->setModelColor("modelColor", m_modelColor);
    m_pShader->setVec3("cameraPos", camera->getCameraPos());

    m_pShader->setMat4("model", m_modelMa);

    m_viewMa = camera->getView();
    m_pShader->setMat4("view", m_viewMa);

    m_proMa = camera->getPerMatrix();
    m_pShader->setMat4("projection", m_proMa);

    m_pCore->glBindVertexArray(m_VAO);
    //glDrawArrays(GL_POINTS, 0, m_ptCount);
    //m_pCore->glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    m_pCore->glDrawArrays(GL_TRIANGLES, 0, m_ptCount);
    m_pShader->shaderProgram.release();

    for (auto i = 0; i < m_faces.size(); ++i)
    {
        m_faces[i]->drawElement(camera);
    }
}

void Terrain::setModelMatrix(QMatrix4x4 ma)
{
    this->m_modelMa *= ma;
    for (auto i = 0; i < m_faces.size(); ++i)
    {
        m_faces[i]->setModelMatrix(ma);
    }
}


void Terrain::loadFromImage(QImage heightImage)
{
    m_Image = heightImage;
    std::vector<float> poslist;
    auto m_minXValue = -10.0f, m_maxXValue = 10.0f, m_minZValue = -10.0f, m_maxZValue = 10.0f;
    // Convert to RGB32 to be sure we're reading the right bytes
    if (heightImage.format() != QImage::Format_RGB32)
        heightImage = heightImage.convertToFormat(QImage::Format_RGB32);

    uchar *bits = heightImage.bits();

    int imageHeight = heightImage.height();
    int imageWidth = heightImage.width();
    int bitCount = imageWidth * 4 * (imageHeight - 1);
    int widthBits = imageWidth * 4;
    float height = 0;

    float xMul = (m_maxXValue - m_minXValue) / float(imageWidth - 1);
    float zMul = (m_maxZValue - m_minZValue) / float(imageHeight - 1);

    // Last row and column are explicitly set to max values, as relying
    // on multiplier can cause rounding errors, resulting in the value being
    // slightly over the specified maximum, which in turn can lead to it not
    // getting rendered.
    int lastRow = imageHeight - 1;
    int lastCol = imageWidth - 1;

    auto minHeight = DBL_MAX; //, minX = DBL_MAX, minZ = DBL_MAX;
    auto maxHeight = DBL_MIN; //, maxX = DBL_MIN, maxZ = DBL_MIN;

    if (heightImage.isGrayscale())
    {
        // Grayscale, it's enough to read Red byte
        for (int i = 0; i < imageHeight; i++, bitCount -= widthBits)
        {
            float zVal;
            if (i == lastRow)
                zVal = m_maxZValue;
            else
                zVal = (float(i) * zMul) + m_minZValue;
            int j = 0;
            for (; j < lastCol; j++)
            {
                QVector3D pos((float(j) * xMul) + m_minXValue, float(bits[bitCount + (j * 4)]), zVal);
                poslist.push_back(pos.x());
                poslist.push_back(pos.y()/10);
                poslist.push_back(pos.z());

                minHeight = minHeight < pos.y()/10 ? minHeight : pos.y()/10;
                maxHeight = maxHeight > pos.y()/10 ? maxHeight : pos.y()/10;

//                minX = minX < pos.x() ? minX : pos.x();
//                maxX = maxX > pos.x() ? maxX : pos.x();

//                minZ = minZ < pos.z() ? minZ : pos.z();
//                maxZ = maxZ > pos.z() ? maxZ : pos.z();
            }

            QVector3D pos(m_maxXValue, float(bits[bitCount + (j * 4)]), zVal);
            poslist.push_back(pos.x());
            poslist.push_back(pos.y()/10);
            poslist.push_back(pos.z());

            minHeight = minHeight < pos.y()/10 ? minHeight : pos.y()/10;
            maxHeight = maxHeight > pos.y()/10 ? maxHeight : pos.y()/10;

//            minX = minX < pos.x() ? minX : pos.x();
//            maxX = maxX > pos.x() ? maxX : pos.x();

//            minZ = minZ < pos.z() ? minZ : pos.z();
//            maxZ = maxZ > pos.z() ? maxZ : pos.z();
        }
    }
    else
    {
        // Not grayscale, we'll need to calculate height from RGB
        for (int i = 0; i < imageHeight; i++, bitCount -= widthBits)
        {
            float zVal;
            if (i == lastRow)
                zVal = m_maxZValue;
            else
                zVal = (float(i) * zMul) + m_minZValue;
            int j = 0;
            int nextpixel = 0;
            for (; j < lastCol; j++)
            {
                nextpixel = j * 4;
                height = (float(bits[bitCount + nextpixel])
                        + float(bits[1 + bitCount + nextpixel])
                        + float(bits[2 + bitCount + nextpixel]));

                QVector3D pos((float(j) * xMul) + m_minXValue,
                                                height / 3.0f,
                                                zVal);
                poslist.push_back(pos.x());
                poslist.push_back(pos.y()/10);
                poslist.push_back(pos.z());

                minHeight = minHeight < pos.y()/10 ? minHeight : pos.y()/10;
                maxHeight = maxHeight > pos.y()/10 ? maxHeight : pos.y()/10;

//                minX = minX < pos.x() ? minX : pos.x();
//                maxX = maxX > pos.x() ? maxX : pos.x();

//                minZ = minZ < pos.z() ? minZ : pos.z();
//                maxZ = maxZ > pos.z() ? maxZ : pos.z();
            }
            nextpixel = j * 4;
            height = (float(bits[bitCount + nextpixel])
                    + float(bits[1 + bitCount + nextpixel])
                    + float(bits[2 + bitCount + nextpixel]));

            QVector3D pos(m_maxXValue, height / 3.0f, zVal);
            poslist.push_back(pos.x());
            poslist.push_back(pos.y()/10);
            poslist.push_back(pos.z());

            minHeight = minHeight < pos.y()/10 ? minHeight : pos.y()/10;
            maxHeight = maxHeight > pos.y()/10 ? maxHeight : pos.y()/10;

//            minX = minX < pos.x() ? minX : pos.x();
//            maxX = maxX > pos.x() ? maxX : pos.x();

//            minZ = minZ < pos.z() ? minZ : pos.z();
//            maxZ = maxZ > pos.z() ? maxZ : pos.z();
        }
    }

    auto m_rowCount = imageHeight;
    auto m_colCount = imageWidth;
    //m_ptCount = poslist.size() / 3;

    std::vector<unsigned int> index;
    for (int i = 1; i < m_rowCount; ++i)
    {
        for (int j = 1; j < m_colCount; ++j)
        {
            index.push_back((i - 1) * m_colCount + j - 1);
            index.push_back((i - 1) * m_colCount + j);
            index.push_back( i * m_colCount + j - 1);

            index.push_back( i * m_colCount + j -1);
            index.push_back((i - 1) * m_colCount +j);
            index.push_back(i * m_colCount + j);
        }
    }
    m_indexCount = index.size();
    m_ptCount = index.size();

    //更新索引和顶点坐标, 计算法线
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

    qDebug() << poslist.size() << index.size();

    m_pVertex = new GLfloat[m_ptCount * 3];
    for (auto i = 0; i < m_ptCount; ++i)
    {
        m_pVertex[i * 3 + 0] = poslist[index[i] * 3 + 0];
        m_pVertex[i * 3 + 1] = poslist[index[i] * 3 + 1];
        m_pVertex[i * 3 + 2] = poslist[index[i] * 3 + 2];
    }

    //计算法线
    auto triAngleCount = m_ptCount / 3;
    qDebug() << triAngleCount;
    CMathTool::CaculateNormalsAccordToVertex(m_pNormal, m_pVertex, triAngleCount);
    //m_pIndex = new unsigned int[m_indexCount];
    //memcpy(m_pIndex, index.data(), sizeof(unsigned int)* index.size());

    //计算颜色数组
    auto range = qAbs(maxHeight - minHeight);
    m_pColor = new GLfloat[m_ptCount * 3];
    for (auto i = 0; i < m_ptCount; ++i)
    {
        auto height = m_pVertex[i * 3 + 1];
        auto factor = qAbs(height - minHeight) / range;
        auto RColor = m_pGradient->getColor(1 - factor);

        m_pColor[i * 3 + 0] = RColor.redF();
        m_pColor[i * 3 + 1] = RColor.greenF();
        m_pColor[i * 3 + 2] = RColor.blueF();
    }

    genGLVertexBuffers();
    genGLColorBuffer();

    m_minY = minHeight, m_maxY = maxHeight;
    CalFaces();
}

void Terrain::setValueRanges(double minx, double maxx, double minz, double maxz)
{
    m_minXValue = minx;
    m_maxXValue = maxx;
    m_minZValue = minz;
    m_maxZValue = maxz;
    m_bNeedUpdate = true;
}

void Terrain::genGLVertexBuffers()
{
    //更新VAO， VBO， EBO
    resetOpenGLBuffer();

    m_modelMa.setToIdentity();

    m_viewMa.setToIdentity();
    m_proMa.setToIdentity();

    m_pCore->glGenVertexArrays(1, &m_VAO);
    m_pCore->glBindVertexArray(m_VAO);

    //顶点缓冲
    m_pCore->glGenBuffers(1, &m_VBO);
    m_pCore->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_pCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * m_ptCount * 3, m_pVertex, GL_STATIC_DRAW);
    m_pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3,  0);
    m_pCore->glEnableVertexAttribArray(0);
    //顶点索引
    //m_pCore->glGenBuffers(1, &m_EBO);
    //m_pCore->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    //m_pCore->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indexCount, m_pIndex, GL_STATIC_DRAW);
    //法线索引
    m_pCore->glGenBuffers(1, &m_NBO);
    m_pCore->glBindBuffer(GL_ARRAY_BUFFER, m_NBO);
    m_pCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * m_ptCount * 3, m_pNormal, GL_STATIC_DRAW);
    m_pCore->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)* 3,  0);
    m_pCore->glEnableVertexAttribArray(2);
}

void Terrain::genGLColorBuffer()
{
    if (m_CBO)
    {
        m_pCore->glDeleteBuffers(1, &m_CBO);
        m_CBO = 0;
    }

    if (m_VAO)
    {
        m_pCore->glBindVertexArray(m_VAO);
    }
    else
    {
        qDebug() << "no VAO object can be found;";
        return;
    }

    m_pCore->glGenBuffers(1, &m_CBO);
    m_pCore->glBindBuffer(GL_ARRAY_BUFFER, m_CBO);
    m_pCore->glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLAT) * m_ptCount * 3, m_pColor, GL_STATIC_DRAW);
    m_pCore->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*3, 0);
    m_pCore->glEnableVertexAttribArray(1);
    //QString::number()
}



void Terrain::updateModelData()
{
    loadFromImage(m_Image);
}

void Terrain::CalFaces()
{
    std::vector<float> minXPts;
    std::vector<float> maxXPts;
    std::vector<float> minZPts;
    std::vector<float> maxZPts;

    auto minY = m_minY - 1;
    qDebug() << m_maxY;

    for (int i = 0; i < m_ptCount; ++i)
    {
        auto x = m_pVertex[i * 3 + 0];
        auto y = m_pVertex[i * 3 + 1];
        auto z = m_pVertex[i * 3 + 2];

        if (x == m_minXValue)
        {
            minXPts.push_back(x);
            minXPts.push_back(y);
            minXPts.push_back(z);

            minXPts.push_back(x);
            minXPts.push_back(minY);
            minXPts.push_back(z);
        }

        if (x == m_maxXValue)
        {
            maxXPts.push_back(x);
            maxXPts.push_back(y);
            maxXPts.push_back(z);

            maxXPts.push_back(x);
            maxXPts.push_back(minY);
            maxXPts.push_back(z);
        }

        if (z == m_maxZValue)
        {
            maxZPts.push_back(x);
            maxZPts.push_back(y);
            maxZPts.push_back(z);

            maxZPts.push_back(x);
            maxZPts.push_back(minY);
            maxZPts.push_back(z);
        }

        if (z == m_minZValue)
        {
            minZPts.push_back(x);
            minZPts.push_back(y);
            minZPts.push_back(z);

            minZPts.push_back(x);
            minZPts.push_back(minY);
            minZPts.push_back(z);
        }
    }

    if (m_faces.size() != 0)
    {
        for (auto i = 0; i < m_faces.size(); ++i)
        {
            delete m_faces[i];
            m_faces[i] = nullptr;
        }
    }

    auto ptr = new CTriAnlgeStrip();
    ptr->setTriangleStrip(minXPts, QVector3D(-1, 0, 0));
    ptr->SetElementColor(QColor(114, 114, 114));
    m_faces.push_back(ptr);

    ptr = new CTriAnlgeStrip();
    ptr->setTriangleStrip(maxXPts, QVector3D(1, 0, 0));
    ptr->SetElementColor(QColor(114, 114, 114));
    m_faces.push_back(ptr);

    ptr = new CTriAnlgeStrip();
    ptr->setTriangleStrip(minZPts, QVector3D(0, 0, -1));
    ptr->SetElementColor(QColor(114, 114, 114));
    m_faces.push_back(ptr);

    ptr = new CTriAnlgeStrip();
    ptr->setTriangleStrip(maxZPts, QVector3D(0, 0, 1));
    ptr->SetElementColor(QColor(114, 114, 114));
    m_faces.push_back(ptr);
}


