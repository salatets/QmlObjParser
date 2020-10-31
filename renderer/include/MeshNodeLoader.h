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
    MeshNodeLoader(){};
    MeshNodeLoader(MeshNode mesh, std::string path){
        setMeshNode(mesh, path);
    };

    // TODO needed?
    MeshNodeLoader(const MeshNodeLoader&) = delete;
    MeshNodeLoader(MeshNodeLoader&&) = delete;

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


#endif // MESHNODELOADER_H
