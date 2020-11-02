#ifndef MESHNODELOADER_H
#define MESHNODELOADER_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <functional>

#include <ImageLoader.h>
#include <MeshNode.h>

// only realization for VNT
class MeshNodeLoader: public QObject, protected QOpenGLFunctions{
    Q_OBJECT
public:
    // TODO needed?
    MeshNodeLoader(const MeshNodeLoader&) = delete;
    MeshNodeLoader(MeshNodeLoader&&) = delete;

    virtual ~MeshNodeLoader();

    void init_buffers();
    void paint(std::function<void(QOpenGLShaderProgram*)> f);

public slots:
    void setShader(meshType type);

protected:
    MeshNodeLoader(MeshNode mesh, std::string path, QOpenGLShaderProgram* shad)
        : m_mesh(mesh), m_path(path), program(shad){};

    virtual void template_init_buffer() = 0;
    virtual void post_init_buffer() = 0;
    virtual void template_paint() = 0;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    MeshNode m_mesh;
    std::string m_path;
    QOpenGLShaderProgram* program;
};

class MeshNodeLoaderVNT : public MeshNodeLoader{
    Q_OBJECT
public:
    MeshNodeLoaderVNT(
            MeshNode mesh,
            std::string path,
            QOpenGLShaderProgram* shad
            ): MeshNodeLoader(mesh, path,shad){}
protected:
    void template_init_buffer() override;
    void post_init_buffer() override;
    void template_paint() override;

    ~MeshNodeLoaderVNT();

private:
    void LoadTexture(Texture texture);
    GLuint textureId;
};


#endif // MESHNODELOADER_H
