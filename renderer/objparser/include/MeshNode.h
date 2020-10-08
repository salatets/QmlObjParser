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
// vertex / normal / texture
class MeshNode
{
public:
    MeshNode(); // u

    MeshNode(const Mtl& material,
             const std::vector<QVector3D>& vertexs,
             const std::vector<QVector3D>& normals,
             const std::vector<QVector2D>& uvs); // a

    const char& getType() const {return type;}
    const float* const getData() const { return vertex_data;};
    const Mtl& getMaterial() const { return material;};
    size_t getSize() const {return size;}

    ~MeshNode(){
        free(vertex_data);
    }

protected:
    char type;
    Mtl material;
    float* vertex_data;
    size_t size;
};

#endif // MESHNODE_H
