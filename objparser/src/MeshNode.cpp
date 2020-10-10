#include <MeshNode.h>

MeshNode::MeshNode() : type('u'), material(){}

MeshNode::MeshNode(const Mtl& material,
         const std::vector<QVector3D>& vertexs,
         const std::vector<QVector3D>& normals,
         const std::vector<QVector2D>& uvs) : type('a'), material(material){

    float* data = static_cast<float *>(malloc(vertexs.size() * 8 * sizeof(float)));
    this->size = vertexs.size();

    // TODO memcpy
    for(size_t i = 0; i < this->size; ++i){
        data[i * 3] = vertexs[i].x();
        data[i* 3 + 1] = vertexs[i].y();
        data[i* 3 + 2] = vertexs[i].z();

        data[(this->size + i) * 3] = normals[i].x();
        data[(this->size + i) * 3 + 1] = normals[i].y();
        data[(this->size + i) * 3 + 2] = normals[i].z();

        data[(this->size * 6) + i * 2] = uvs[i].x();
        data[(this->size * 6) + i * 2 + 1] = uvs[i].y();
    }

//    for(size_t i = 0; i < this->size; ++i){
//        data[i].vertex = vertexs[i];
//        data[i].normal = normals[i];
//        data[i].uv = uvs[i];
//    }
    vertex_data = data;
};
