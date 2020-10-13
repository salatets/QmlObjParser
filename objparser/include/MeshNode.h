#ifndef MESHNODE_H
#define MESHNODE_H

#include <QVector2D>
#include <QVector3D>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_3_Core>

enum illum{
    AMBIENT_ON = 1,
    HIGHTLIGHT_ON = 2
};

// TODO unify material
struct Mtl{
    std::string name;
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    illum illum_mode;
    std::string diffuse_map_path;
    Mtl() : name(""), ambient(0,0,0), diffuse(0,0,0), specular(0,0,0),
        illum_mode(AMBIENT_ON), diffuse_map_path("") {};
};

enum meshType{
    VNT,
    VT,
    VN,
    V,
    UNDEFINED,
};


// vertex / normal / texture
class MeshNode
{
public:
    MeshNode(); // UNDEFINED

    MeshNode(const Mtl& material,
             const std::vector<QVector3D>& vertexs,
             const std::vector<QVector3D>& normals,
             const std::vector<QVector2D>& uvs); // VNT

    MeshNode(const Mtl& material,
             const std::vector<QVector3D>& vertexs,
             const std::vector<QVector3D>& normals); // VN

    MeshNode(const Mtl& material,
             const std::vector<QVector3D>& vertexs,
             const std::vector<QVector2D>& uvs); // VT

    MeshNode(const Mtl& material,
             const std::vector<QVector3D>& vertexs); // V

    const meshType& getType() const {return type;}
    const float* getData() const { return vertex_data;};
    const Mtl& getMaterial() const { return material;};
    size_t getSize() const {return size;}

    MeshNode(const MeshNode& other){
        material = other.material;
        size = other.size;
        type = other.type;

        if(other.vertex_data == nullptr){
            vertex_data = nullptr;
        }else{
            size_t vertex_size = size * getTypeSize();
            vertex_data = new float[vertex_size];

            for(size_t i = 0; i < vertex_size; ++i){
                vertex_data[i] = other.vertex_data[i];
            }
        }
    }

    const MeshNode& operator=(const MeshNode& other){
        if(this == &other)
            return *this;

        material = other.material;
        size = other.size;
        type = other.type;

        if(other.vertex_data == nullptr){
            delete[] vertex_data;
            vertex_data = nullptr;
        }else{
            delete[] vertex_data; // add check for same size
            size_t vertex_size = size * getTypeSize();
            vertex_data = new float[vertex_size];

            for(size_t i = 0; i < vertex_size; ++i){
                vertex_data[i] = other.vertex_data[i];
            }
        }
        return *this;
    }

    // TODO add move operator=()

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
