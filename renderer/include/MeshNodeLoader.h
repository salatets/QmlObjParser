#ifndef MESHNODELOADER_H
#define MESHNODELOADER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>
#include <functional>
#include <variant>
#include <forward_list>

#include <ImageLoader.h>
#include <MeshNode.h>

using program_param =
std::forward_list<
std::pair<
const char*, std::variant<QMatrix4x4,QVector3D,GLfloat>
>
>;

class MeshNodeLoader: public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:

    void init_buffers();
    void paint(const program_param&);

    virtual ~MeshNodeLoader();

    void setShader(meshType, std::shared_ptr<QOpenGLShaderProgram>&);

protected:
    MeshNodeLoader(const MeshNode& mesh, const std::string& path, std::shared_ptr<QOpenGLShaderProgram>& program)
        : m_mesh(mesh), m_path(path), program(program){
        initializeOpenGLFunctions();
    }

    virtual void template_init_buffer() = 0;
    virtual void template_paint() = 0;

    MeshNode m_mesh;
    std::string m_path;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    std::shared_ptr<QOpenGLShaderProgram> program;
};




#endif // MESHNODELOADER_H
