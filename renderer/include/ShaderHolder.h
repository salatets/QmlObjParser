#ifndef SHADERHOLDER_H
#define SHADERHOLDER_H
#include <memory>
#include <QOpenGLShaderProgram>

#include <Mesh.h>
#include <unordered_map>

template<class T>
class ShaderHolder
{
public:

    ShaderHolder(std::initializer_list<T> names){
        for(T name: names){
            shaders.insert(std::make_pair(name, std::make_shared<QOpenGLShaderProgram>()));
        }

    }

    void setShader(T name, std::shared_ptr<QOpenGLShaderProgram>& program){
        if(program == nullptr)
            return;

        auto& shader = getShader(name);
        shader = program;

        afterSetShader(name);
    }
    void setShader(T name, char *frag, char *vert){
        auto& shader = getShader(name);

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

        afterSetShader(name);
    }

protected:
    virtual void afterSetShader(T) = 0;

    std::shared_ptr<QOpenGLShaderProgram>& getShader(T name){
        auto search = shaders.find(name);
        if (search != shaders.end())
            return search->second;

        Q_ASSERT(false);
    }

private: // TODO add compile-time storage http://stevehanov.ca/blog/index.php?id=119
    // QOpenGLShaderProgram  no has compiletime constuctor?
    std::unordered_map<T, std::shared_ptr<QOpenGLShaderProgram>> shaders;
};

#endif // SHADERHOLDER_H

