#ifndef SHADERHOLDER_H
#define SHADERHOLDER_H
#include <memory>
#include <QOpenGLShaderProgram>

#include <Mesh.h>

class ShaderHolder
{
public:
    void setShader(meshType type, std::shared_ptr<QOpenGLShaderProgram> &program);
    void setShader(meshType type, char* frag,char* vert);

protected:
    virtual void afterSetShader(meshType) = 0;

    std::shared_ptr<QOpenGLShaderProgram>& getShader(meshType type);

    std::shared_ptr<QOpenGLShaderProgram> VNT_shader;
    std::shared_ptr<QOpenGLShaderProgram> VN_shader;
    std::shared_ptr<QOpenGLShaderProgram> VT_shader;
    std::shared_ptr<QOpenGLShaderProgram> V_shader;
};

#endif // SHADERHOLDER_H
