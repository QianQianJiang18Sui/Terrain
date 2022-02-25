#include "shader.h"

Shader::Shader(const QString& vertexShaderSourcePath, const QString& fragmentShaderSourcePath)
{
    QOpenGLShader vertexShader(QOpenGLShader::Vertex);
    bool success = vertexShader.compileSourceFile(vertexShaderSourcePath);
    if (!success)
    {
        qDebug() << vertexShader.log();
    }

    QOpenGLShader fragmentShader(QOpenGLShader::Fragment);
    success = fragmentShader.compileSourceFile(fragmentShaderSourcePath);
    if (!success)
    {
        qDebug() << fragmentShader.log();
    }

    shaderProgram.addShader(&vertexShader);
    shaderProgram.addShader(&fragmentShader);
    success = shaderProgram.link();
    if (!success)
    {
        qDebug() << shaderProgram.log();
    }
}

Shader::~Shader()
{
    shaderProgram.release();
}

void Shader::use()
{
    shaderProgram.bind();
}
