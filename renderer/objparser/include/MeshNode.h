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

// TODO unify material
class MeshNode
{
public:
    MeshNode() : type('u'){};

    const char& getType(){return type;}
    const float* const getData();
    const Mtl& getMaterial();

protected:
    const char type;
};

struct DataVNT{
    QVector3D vertex;
    QVector3D normal;
    QVector2D uv;
};

class MeshNodeVNT : public MeshNode {
public:
    MeshNodeVNT(const Mtl& material,
                const std::vector<QVector3D>& vertexs,
                const std::vector<QVector3D>& normals,
                const std::vector<QVector2D>& uvs);

    const float* const getData();
    const Mtl& getMaterial();

private:
    Mtl material;
    std::vector<DataVNT> vertex_data;

};

#endif // MESHNODE_H
