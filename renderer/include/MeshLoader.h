#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>

#include <Mesh.h>
#include <ImageLoader.h>

// only realization for VNT
class MeshNodeLoader: public QObject, protected QOpenGLFunctions{
    Q_OBJECT
public:
    MeshNodeLoader(){};
    MeshNodeLoader(MeshNode mesh, std::string path){
        setMeshNode(mesh, path);
    };

    ~MeshNodeLoader();

    void setMeshNode(MeshNode mesh, std::string path){ m_mesh = mesh; m_path = path;}

    void init_buffers(QOpenGLShaderProgram* program);
    void paint(QOpenGLShaderProgram* program);

private:
    void LoadTexture(Texture texture);

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    GLuint textureId;
    MeshNode m_mesh;
    std::string m_path;
};

class MeshLoader : public QObject, protected QOpenGLFunctions{
    Q_OBJECT
public:
    MeshLoader() : loaders(nullptr) {};

    ~MeshLoader();

    void setMesh(MeshRoot mesh, std::string path);

    void init_buffers(QOpenGLShaderProgram* program);
    void paint(QOpenGLShaderProgram* program);

private:
    MeshNodeLoader** loaders;
    size_t loaders_size;
    MeshRoot m_model;
};


#endif // MESHLOADER_H
