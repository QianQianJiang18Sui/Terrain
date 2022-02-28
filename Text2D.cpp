#include "Text2D.h"

CText2D::CText2D(CGraphicObject *parent)
{
    m_object_type = OBJ_TERRAIN_FACE;
    m_pShader = new Shader(":/text_2D.vert", ":/text_2D.frag");
}

CText2D::~CText2D()
{
}

void CText2D::drawElement(camera *camera)
{
    if (m_bNeedUpdate)
    {
        updateModelData();
        m_bNeedUpdate = false;
    }

    //纹理绑定
    m_pCore->glActiveTexture(GL_TEXTURE0);
    m_pTexture->bind();

    m_pShader->use();

    m_pShader->setVec3("cameraPos", camera->getCameraPos());

    QMatrix4x4 model;
    model.translate(m_ElementPos);
    model.scale(m_scale);
    model.rotate(m_rotate_angle, m_rotate_axis);
    m_modelMa = model;
    m_pShader->setMat4("model", model);

    m_viewMa = camera->getView();
    m_pShader->setMat4("view", m_viewMa);

    if (m_DrawMode != MT2D)
    {
        m_proMa = camera->getPerMatrix();
        m_pShader->setMat4("projection", m_proMa);
    }
    else
    {
        m_pShader->setMat4("projection", m_proMa);
    }

    m_pCore->glBindVertexArray(m_VAO);
    m_pCore->glDrawArrays(GL_QUADS, 0, m_ptCount);
}

void CText2D::setText(const QString &text, int textPixelSize, QColor textColor, QFont font)
{
    auto size = calImageSize(text, textPixelSize, font);
    if (m_pVertex)
    {
        delete m_pVertex; m_pVertex = nullptr;
    }

    m_pVertex = new GLfloat[4 * 3]
    {
         0.5f,  0.5f, 0.0f,    // top right
         0.5f, -0.5f, 0.0f,    // bottom right
        -0.5f, -0.5f, 0.0f,    // bottom left
        -0.5f,  0.5f, 0.0f,    // top left
    };
    m_ptCount = 4;

    m_pTextureCoord = new GLfloat[4 * 2]
    {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    if (m_pTexture)
    {
        delete m_pTexture; m_pTexture = nullptr;
    }

    m_pTexture = genTexture(text, textPixelSize, textColor, font);
    m_bNeedUpdate = true;
}

void CText2D::setRotate(float angle, QVector3D axis)
{
    m_rotate_angle = angle;
    m_rotate_axis = axis;
}

void CText2D::setPersType()
{

}

void CText2D::updateModelData()
{
    resetOpenGLBuffer();

    m_pCore->glGenVertexArrays(1, &m_VAO);
    m_pCore->glBindVertexArray(m_VAO);

    m_pCore->glGenBuffers(1, &m_VBO);
    m_pCore->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_pCore->glBufferData(GL_ARRAY_BUFFER, m_ptCount * 3 * sizeof(GL_FLOAT), m_pVertex, GL_STATIC_DRAW);
    m_pCore->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3,  0);
    m_pCore->glEnableVertexAttribArray(0);

    //m_NBO 纹理坐标
    m_pCore->glGenBuffers(1, &m_NBO);
    m_pCore->glBindBuffer(GL_ARRAY_BUFFER, m_NBO);
    m_pCore->glBufferData(GL_ARRAY_BUFFER, m_ptCount * 2 * sizeof(GL_FLOAT), m_pTextureCoord, GL_STATIC_DRAW);
    m_pCore->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2,  0);
    m_pCore->glEnableVertexAttribArray(1);
}
