#ifndef MESHNODELOADER_H
#define MESHNODELOADER_H

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

    void init_buffers();
    void paint(std::function<void(QOpenGLShaderProgram*)>);

    virtual ~MeshNodeLoader();

public slots:
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

class MeshNodeLoaderVNT : public MeshNodeLoader{
    //Q_OBJECT
public:
    MeshNodeLoaderVNT(
            const MeshNode& mesh,
            const std::string& path,
            QOpenGLShaderProgram* shad
            );
protected:
    void template_init_buffer() override;
    void template_paint() override;

    ~MeshNodeLoaderVNT();

private:
    void LoadTexture(Texture texture);
    GLuint textureId;
};


#endif // MESHNODELOADER_H
