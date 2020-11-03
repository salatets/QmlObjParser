#ifndef MESHNODELOADER_H
#define MESHNODELOADER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <functional>

#include <ImageLoader.h>
#include <MeshNode.h>

// only realization for VNT
class MeshNodeLoader: public QObject, protected QOpenGLFunctions{
    Q_OBJECT
public:

    void init_buffers();
    void paint(const std::function<void(QOpenGLShaderProgram *)> &);

    virtual ~MeshNodeLoader();

public Q_SLOTS:
    void setShader(meshType);

protected:
    MeshNodeLoader(const MeshNode& mesh, const std::string& path, QOpenGLShaderProgram* program)
        : m_mesh(mesh), m_path(path), program(program){
        initializeOpenGLFunctions();
    };

    virtual void template_init_buffer() = 0;
    virtual void template_paint() = 0;

    MeshNode m_mesh;
    std::string m_path;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QOpenGLShaderProgram* program;
};




#endif // MESHNODELOADER_H
