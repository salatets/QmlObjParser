#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <Mesh.h>
#include "MeshNodeLoader.h"

class MeshLoader : public QObject, protected QOpenGLFunctions{
    Q_OBJECT
public:
    MeshLoader() :
        loaders(nullptr),
        loaders_size(0),
        isInit(false),
        VNT_shader(nullptr),
        VN_shader(nullptr),
        VT_shader(nullptr),
        V_shader(nullptr){};

    ~MeshLoader();

    bool isInited(){return isInit;}

    void setMesh(const MeshRoot &mesh, const std::string &path);

    void setShader(meshType type, char* frag,char * vert);

    void init_buffers();
    void paint(std::function<void(QOpenGLShaderProgram*, MeshRoot)>);

Q_SIGNALS:
    void shaderChanged(meshType type);

private:
    QOpenGLShaderProgram* getShader(meshType type);
    QOpenGLShaderProgram* assignShader(meshType type, QOpenGLShaderProgram* new_point);

    MeshNodeLoader** loaders;
    size_t loaders_size;
    bool isInit;
    MeshRoot m_model;
    QOpenGLShaderProgram* VNT_shader;
    QOpenGLShaderProgram* VN_shader;
    QOpenGLShaderProgram* VT_shader;
    QOpenGLShaderProgram* V_shader;
};

#endif // MESHLOADER_H
