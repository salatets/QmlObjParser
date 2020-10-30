#ifndef MESH_H
#define MESH_H

#include <MeshNode.h>

class MeshRoot
{
public:
    MeshRoot() : m_center(0,0,0), m_size(0,0,0) {};

    template<typename Iterator>
    MeshRoot(Vec3 center, Vec3 size,
         Iterator p, Iterator q) : meshes(p,q), m_center(center),m_size(size) {};

    const Vec3& getCenter() const {return m_center;}
    const Vec3& getSize() const {return m_size;}

    size_t size() const {return meshes.size();}
    auto cbegin() const {return meshes.cbegin();}
    auto cend() const {return meshes.cend();}
    auto front() const {return meshes.front();}
    bool empty() const {return meshes.empty();}

    template<typename Iterator>
    void setNodes(Vec3 model_center, Vec3 model_size, Iterator p, Iterator q, size_t meshes_size){
        this->m_center = model_center;
        this->m_size = model_size;
        this->meshes.reserve(meshes_size);
        this->meshes(p,q);
    }

private:
    std::vector<MeshNode> meshes;
    Vec3 m_center;
    Vec3 m_size;
};

#endif // MESH_H
