#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <QtQuick/QQuickItem>
#include <QOpenGLFunctions>

#include <Mesh.h>
#include "MeshNodeLoader.h"

class MeshLoader : public QObject, protected QOpenGLFunctions{
    Q_OBJECT
public:
    MeshLoader() : loaders(nullptr) {};

    ~MeshLoader();

    // TODO needed?
    MeshLoader(const MeshLoader&) = delete;
    MeshLoader(MeshLoader&&) = delete;

    void setMesh(MeshRoot mesh, std::string path);

    void setShader(meshType type, char* frag,char * vert);

    void init_buffers();
    void paint();

signals:
    void shaderChanged(meshType type);

private:
    QOpenGLShaderProgram* getShader(meshType type);

    MeshNodeLoader** loaders;
    size_t loaders_size;
    MeshRoot m_model;
    QOpenGLShaderProgram VNT_shader;
    QOpenGLShaderProgram VN_shader;
    QOpenGLShaderProgram VT_shader;
    QOpenGLShaderProgram V_shader;
};

#endif // MESHLOADER_H
