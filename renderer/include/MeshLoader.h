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

    void init_buffers(QOpenGLShaderProgram* program);
    void paint(QOpenGLShaderProgram* program);

private:
    MeshNodeLoader** loaders;
    size_t loaders_size;
    MeshRoot m_model;
};

#endif // MESHLOADER_H
