#ifndef FLOATINGHORIZON_H
#define FLOATINGHORIZON_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>

#include <Mesh.h>

class FloatingHorizon : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    FloatingHorizon(){};

    void setMesh(Mesh mesh);

    void init_buffers(QOpenGLShaderProgram* program);
    void paint(QOpenGLShaderProgram* program, QMatrix4x4 mat, GLsizei width, GLsizei height);

private:
    float max(const QVector3D& vec);

    void clearHorizons(int width, int height);
    auto getPointsToDraw(QMatrix4x4 proj, int width, int height,int point_size);
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    std::vector<int> lower_horizon;
    std::vector<int> higher_horizon;
    std::vector<QVector3D> vertices;
};

#endif // FLOATINGHORIZON_H
