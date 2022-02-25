#ifndef SHADER_H
#define SHADER_H

#include <QDebug>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QString>
#include <QColor>

class Shader
{
public:
    Shader(const QString& vertexShaderSourcePath, const QString& fragmentShaderSourcePath);
    ~Shader();

    QOpenGLShaderProgram    shaderProgram;

    void use();

    inline void setMat4(const QString& name, const QMatrix4x4& matrix)
    {
        auto loc = shaderProgram.uniformLocation(name);
        shaderProgram.setUniformValue(loc, matrix);
    }

    inline void setInt(const QString& name, const GLint& value)
    {
        auto loc = shaderProgram.uniformLocation(name);
        shaderProgram.setUniformValue(loc, value);
    }

    inline void setVec3(const QString& name, const QVector3D& val)
    {
        auto loc = shaderProgram.uniformLocation(name);
        shaderProgram.setUniformValue(loc, val);
    }

    inline void setBool(const QString& name, const bool& value)
    {
        auto loc = shaderProgram.uniformLocation(name);
        if (value)
            shaderProgram.setUniformValue(loc, 1);
        else
            shaderProgram.setUniformValue(loc, 0);
    }

    inline void setModelColor(const QString& name, const QColor& color)
    {
        auto loc = shaderProgram.uniformLocation(name);
        QVector3D uniColor(color.redF(), color.greenF(), color.blueF());
        shaderProgram.setUniformValue(loc, uniColor);
    }

    inline void setVec4(const QString& name, const QVector4D color)
    {
        auto loc = shaderProgram.uniformLocation(name);
        shaderProgram.setUniformValue(loc, color);
    }
};

#endif // SHADER_H
