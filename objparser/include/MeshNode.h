#ifndef MESHNODE_H
#define MESHNODE_H

#include <string>
#include <vector>
#include "vec.h"

enum illum{
    AMBIENT_ON = 1,
    HIGHTLIGHT_ON = 2
};

// TODO unify material
struct Mtl{
    std::string name;
    Vec3 ambient;
    Vec3 diffuse;
    Vec3 specular;
    illum illum_mode;
    std::string diffuse_map_path;
    Mtl() : name(""), ambient(0,0,0), diffuse(0,0,0), specular(0,0,0),
        illum_mode(AMBIENT_ON), diffuse_map_path("") {};
};

enum meshType{
    VNT,
    VN,
    VT,
    V,
    UNDEFINED, // This type no constructable
};


// vertex / normal / texture
class MeshNode
{
public:
    static MeshNode make(const Mtl& material,
                             meshType format,
                             const std::vector<Vec3>& vertexs,
                             const std::vector<Vec3>& normals,
                             const std::vector<Vec2>& uvs);

    const meshType& getType() const {return type;}
    const float* getData() const { return vertex_data;}
    const Mtl& getMaterial() const { return material;}
    size_t getSize() const {return size;}
    size_t static getTypeSize(const MeshNode& mesh);

    MeshNode(const MeshNode& other){
        material = other.material;
        size = other.size;
        type = other.type;

        if(other.vertex_data == nullptr){
            vertex_data = nullptr;
        }else{
            size_t vertex_size = size * getTypeSize(*this);
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
            size_t vertex_size = size * getTypeSize(*this);
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
    meshType type;
    Mtl material;
    size_t size; // size of vertex, not float
    float* vertex_data; // coord data, then normals data, then uv data

    MeshNode(const Mtl& material,
             const std::vector<Vec3>& vertexs,
             const std::vector<Vec3>& normals,
             const std::vector<Vec2>& uvs); // VNT

    MeshNode(const Mtl& material,
             const std::vector<Vec3>& vertexs,
             const std::vector<Vec3>& normals); // VN

    MeshNode(const Mtl& material,
             const std::vector<Vec3>& vertexs,
             const std::vector<Vec2>& uvs); // VT

    MeshNode(const Mtl& material,
             const std::vector<Vec3>& vertexs); // V
};

#endif // MESHNODE_H
