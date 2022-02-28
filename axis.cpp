#include "axis.h"
#include "Text2D.h"

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

    QMatrix4x4 model;
    model.translate(m_ElementPos);
    model.scale(m_factor);
    m_modelMa = model;
    m_pShader->setMat4("model", model);

    auto viewMa = camera->getView();
    m_pShader->setMat4("view", viewMa);

    m_pShader->setMat4("projection", m_proMa);
    //m_pShader->setMat4("projection", camera->getPerMatrix());

    m_pCore->glBindVertexArray(m_VAO);
    m_pCore->glDrawArrays(GL_LINES, 0, 6);

    //m_pX->setScale(10);
    m_pX->drawElement(camera);
    m_pY->drawElement(camera);
    m_pZ->drawElement(camera);

    m_pCore->glViewport(0, 0, camera->m_windowWidth, camera->m_windowHeight);
}

void Axis::setModelData()
{
    m_modelMa.setToIdentity();
    //m_modelMa.scale(150);
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
        0.6f, .0f, .0f,
         .0f, .0f, .0f,
         .0f,0.6f, .0f,
         .0f, .0f, .0f,
         .0f, .0f,0.6f,
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

    drawXYZ();
}

void Axis::drawXYZ()
{
    QMatrix4x4 mat;
    mat.scale(m_factor);

    if (m_pX)
    {
        delete m_pX; m_pX = nullptr;
    }
    m_pX = new CText2D(this);
    m_pX->setText("X", 100, QColor(255,0,0));
    m_pX->SetElementPos((mat * m_XPos).toVector3D());
    m_pX->setScale(0.5 * m_factor);

    if (m_pY)
    {
        delete m_pY; m_pY = nullptr;
    }
    m_pY = new CText2D(this);
    m_pY->setText("Y",100, QColor(0, 255, 0));
    m_pY->SetElementPos((mat * m_YPos).toVector3D());
    m_pY->setScale(0.5 * m_factor);

    if (m_pZ)
    {
        delete m_pZ; m_pZ = nullptr;
    }
    m_pZ = new CText2D(this);
    m_pZ->setText("Z",100, QColor(0, 0, 255));
    m_pZ->SetElementPos((mat * m_ZPos).toVector3D());
    m_pZ->setScale(0.50 * m_factor);
    //m_pZ->setRotate(-90, QVector3D(0, 1, 0));

    m_pX->setProMa(m_proMa);
    m_pY->setProMa(m_proMa);
    m_pZ->setProMa(m_proMa);

    m_pX->SetDrawMode(MT2D);
    m_pY->SetDrawMode(MT2D);
    m_pZ->SetDrawMode(MT2D);
}

void Axis::drawNEH()
{

}
