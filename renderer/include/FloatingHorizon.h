#ifndef FLOATINGHORIZON_H
#define FLOATINGHORIZON_H
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include <Mesh.h>

class FloatingHorizon{
public:
    FloatingHorizon(Mesh m, QOpenGLContext* m_context);

    void setMesh(Mesh m);

    void setShader(QOpenGLShaderProgram* shader);

    void initBuffers();

    void draw(QMatrix4x4 proj, QMatrix4x4 view, int width, int height);
private:
    void clearHorizons(int width);
    auto getPointsToDraw(QMatrix4x4 proj, QMatrix4x4 view, int width, int height);

    std::vector<float> vertices;
    std::vector<int> lower_horizon;
    std::vector<int> higher_horizon;
    size_t size;
    QOpenGLShaderProgram* shader;
    bool dirty;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QOpenGLFunctions_4_3_Core* m_funcs; // TODO not fix version
};

#endif // FLOATINGHORIZON_H
