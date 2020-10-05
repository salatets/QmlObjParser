#include <MeshNode.h>

MeshNodeVNT::MeshNodeVNT(const Mtl& material,
                         const std::vector<QVector3D>& vertexs,
                         const std::vector<QVector3D>& normals,
                         const std::vector<QVector2D>& uvs)
    : material(material), vertex_data(vertexs.size()) {

    for(size_t i = 0; i < vertexs.size(); ++i){
        vertex_data[i].vertex = vertexs[i];
        vertex_data[i].normal = normals[i];
        vertex_data[i].uv = uvs[i];
    }
}

const float* const MeshNodeVNT::getData(){return reinterpret_cast<const float* const>(vertex_data.data());}
const Mtl& MeshNodeVNT::getMaterial(){return material;}
