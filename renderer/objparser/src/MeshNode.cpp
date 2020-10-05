#include <MeshNode.h>

MeshNode::MeshNode() : type('u'), material(){}

MeshNode::MeshNode(const Mtl& material,
         const std::vector<QVector3D>& vertexs,
         const std::vector<QVector3D>& normals,
         const std::vector<QVector2D>& uvs) : type('a'), material(material){

    DataVNT* data = static_cast<DataVNT *>(malloc(vertexs.size() * sizeof(DataVNT)));

    for(size_t i = 0; i < vertexs.size(); ++i){
        data[i].vertex = vertexs[i];
        data[i].normal = normals[i];
        data[i].uv = uvs[i];
    }
    vertex_data = reinterpret_cast<float*>(data);
};
