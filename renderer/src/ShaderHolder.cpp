#include "ShaderHolder.h"

void ShaderHolder::setShader(meshType type, std::shared_ptr<QOpenGLShaderProgram>& program){
    if(program == nullptr)
        return;

    auto& shader = getShader(type);
    shader = program;

    afterSetShader(type);
}

void ShaderHolder::setShader(meshType type, char *frag, char *vert){
    auto& shader = getShader(type);

    if(shader == nullptr){
        shader = std::make_shared<QOpenGLShaderProgram>();
    }
    if(shader->isLinked())
        shader->removeAllShaders();

    bool success = shader->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vert);
    success &= shader->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, frag);
    Q_ASSERT(success);

    success = shader->link();
    Q_ASSERT(success);

    afterSetShader(type);
}

std::shared_ptr<QOpenGLShaderProgram>& ShaderHolder::getShader(meshType type){
    switch(type){
    case meshType::VNT:
        return VNT_shader;
    case meshType::VN:
        return VN_shader;
    case meshType::VT:
        return VT_shader;
    case meshType::V:
        return V_shader;
    }
    Q_ASSERT(false);
}
