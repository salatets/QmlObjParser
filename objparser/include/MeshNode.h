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

enum meshType{
    VNT,
    UNDEFINED,
};

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

    const meshType& getType() const {return type;}
    const float* getData() const { return vertex_data;};
    const Mtl& getMaterial() const { return material;};
    size_t getSize() const {return size;}

    MeshNode ( const MeshNode & other){
        material = other.material;
        size = other.size;
        type = other.type;

        if(other.vertex_data == nullptr){
            vertex_data = nullptr;
            return;
        }

        size_t vertex_size = size * getTypeSize();
        vertex_data = new float[vertex_size];

        for(size_t i = 0; i < vertex_size; ++i){
            vertex_data[i] = other.vertex_data[i];
        }
    }

    MeshNode ( MeshNode && other){
        material = other.material;
        size = other.size;
        type = other.type;
        vertex_data = other.vertex_data;
        other.vertex_data = nullptr;
    }

    ~MeshNode(){
        delete[] vertex_data;
    }

private:
    size_t getTypeSize() const;

    meshType type;
    Mtl material;
    size_t size;
    float* vertex_data;
};

#endif // MESHNODE_H
