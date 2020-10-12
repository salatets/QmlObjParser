#ifndef MESH_H
#define MESH_H

#include <MeshNode.h>
#include <common.h>

// todo add iterator impl
class MeshRoot
{
public:
    MeshRoot() : m_center(0,0,0), m_size(0,0,0) {};

    template<typename Iterator>
    MeshRoot(QVector3D center, QVector3D size,
         Iterator p, Iterator q) : meshes(p,q), m_center(center),m_size(size) {};

    const QVector3D& getCenter() const {return m_center;}
    const QVector3D& getSize() const {return m_size;}

    size_t size() const {return meshes.size();}
    auto cbegin() const {return meshes.cbegin();}
    auto cend() const {return meshes.cend();}
    auto front() const {return meshes.front();}

    template<typename Iterator>
    void setNodes(QVector3D model_center, QVector3D model_size, Iterator p, Iterator q, size_t meshes_size){
        this->m_center = model_center;
        this->m_size = model_size;
        this->meshes.reserve(meshes_size);
        this->meshes(p,q);
    }

private:
    std::vector<MeshNode> meshes;
    QVector3D m_center;
    QVector3D m_size;
};

#endif // MESH_H
