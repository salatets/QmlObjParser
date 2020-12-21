#ifndef FLOATINGHORIZON_H
#define FLOATINGHORIZON_H

#include <QtQuick/QQuickItem>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

#include "ShaderHolder.h"
#include <Mesh.h>

class FloatingHorizon : public QObject, protected QOpenGLFunctions, public ShaderHolder<int>{ // TODO replace shaderHolder
    Q_OBJECT
public:
    FloatingHorizon() :
        ShaderHolder({1}),
        isInit(false)
    {
        initializeOpenGLFunctions();
    }

    void setMesh(MeshRoot mesh);

    bool isInited(){return isInit;}

    void init_buffers();
    // tuple(width,height,model matrix)
    void paint(const std::function<std::tuple<GLsizei, GLsizei, QMatrix4x4>(const MeshRoot &)> &);
    //QMatrix4x4 mat, GLsizei width, GLsizei height);

private:

    void clearHorizons(GLsizei width, GLsizei height);
    auto getPointsToDraw(QMatrix4x4 model, GLsizei width, GLsizei height, int point_size);

    void afterSetShader(int) override;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    std::vector<GLsizei> lower_horizon;
    std::vector<GLsizei> higher_horizon;
    bool isInit;
    MeshRoot m_model; // huge overhead
    std::vector<QVector3D> vertices;
};

#endif // FLOATINGHORIZON_H
