#ifndef MESH_H
#define MESH_H

#include <MeshNode.h>
#include <common.h>

class Mesh
{
public:
    Mesh() : center(0,0,0),size(0,0,0) {};
    template<typename Iterator>
    Mesh(QVector3D center, QVector3D size,
         Iterator p, Iterator q) : meshes(p,q), center(center),size(size) {};

    const QVector3D& getCenter() const {return center;}
    const QVector3D& getSize() const {return size;}
    size_t getNodesSize() const {return meshes.size();}
    auto getNodesBegin() const {return meshes.cbegin();}
    auto getNodesEnd() const {return meshes.cend();}

    template<typename Iterator>
    void setNodes(QVector3D model_center, QVector3D model_size, Iterator p, Iterator q, size_t meshes_size){
        this->center = model_center;
        this->size = model_size;
        this->meshes.reserve(meshes_size);
        this->meshes(p,q);
    }

private:
    std::vector<MeshNode> meshes;
    QVector3D center;
    QVector3D size;
};

#endif // MESH_H
