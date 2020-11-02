#ifndef MESHNODELOADER_H
#define MESHNODELOADER_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

#include <ImageLoader.h>
#include <MeshNode.h>

// only realization for VNT
class MeshNodeLoader: public QObject, protected QOpenGLFunctions{
    Q_OBJECT
public:
    MeshNodeLoader(MeshNode mesh, std::string path, QOpenGLShaderProgram* shad)
        : m_mesh(mesh), m_path(path), program(shad){};

    // TODO needed?
    MeshNodeLoader(const MeshNodeLoader&) = delete;
    MeshNodeLoader(MeshNodeLoader&&) = delete;

    ~MeshNodeLoader();

    void init_buffers();
    void paint();

public slots:
    void setShader(meshType type);

private:
    void LoadTexture(Texture texture);

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    GLuint textureId;
    MeshNode m_mesh;
    std::string m_path;
    QOpenGLShaderProgram* program;
};


#endif // MESHNODELOADER_H
