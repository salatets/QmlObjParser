#ifndef MESHNODE_H
#define MESHNODE_H

#include <QVector2D>
#include <QVector3D>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_3_Core>

#include <common.h>

class MeshNode
{
    void Draw();
};

struct DataVNT{
    QVector3D vertex;
    QVector3D normal;
    QVector2D uv;
};

class MeshNodeVNT : public MeshNode {
public:
    MeshNodeVNT(QOpenGLFunctions_4_3_Core* funcs, unsigned int textureId,
                const std::vector<QVector3D>& vertexs,
                const std::vector<QVector3D>& normals,
                const std::vector<QVector2D>& uvs);

    ~MeshNodeVNT();

    void initBuffers(QOpenGLShaderProgram& shader);

    void Draw(QOpenGLShaderProgram& shader);

private:
    unsigned int textureId;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QOpenGLFunctions_4_3_Core* m_funcs;

    Texture* diffuse;
    std::vector<DataVNT> vertex_data;

};

#endif // MESHNODE_H
