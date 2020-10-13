#include <MeshNode.h>
#include <cstring>

MeshNode::MeshNode() : type(UNDEFINED), material(), size(0), vertex_data(nullptr){}

MeshNode::MeshNode(const Mtl& material,
         const std::vector<QVector3D>& vertexs,
         const std::vector<QVector3D>& normals,
         const std::vector<QVector2D>& uvs) : type(VNT), material(material), size(vertexs.size()){

    vertex_data = new float[size * getTypeSize()];

    std::memcpy(vertex_data, vertexs.data(), vertexs.size() * 3 * sizeof(float));
    std::memcpy(vertex_data + this->size * 3, normals.data(), normals.size() * 3 * sizeof(float));
    std::memcpy(vertex_data + this->size * 6, uvs.data(), uvs.size() * 2 * sizeof(float));
};

MeshNode::MeshNode(const Mtl& material,
         const std::vector<QVector3D>& vertexs,
         const std::vector<QVector3D>& normals) : type(VN), material(material), size(vertexs.size()){

    vertex_data = new float[size * getTypeSize()];

    std::memcpy(vertex_data, vertexs.data(), vertexs.size() * 3 * sizeof(float));
    std::memcpy(vertex_data + this->size * 3, normals.data(), normals.size() * 3 * sizeof(float));
};

MeshNode::MeshNode(const Mtl& material,
         const std::vector<QVector3D>& vertexs,
         const std::vector<QVector2D>& uvs) : type(VT), material(material), size(vertexs.size()){

    vertex_data = new float[size * getTypeSize()];

    std::memcpy(vertex_data, vertexs.data(), vertexs.size() * 3 * sizeof(float));
    std::memcpy(vertex_data + this->size * 3, uvs.data(), uvs.size() * 2 * sizeof(float));
};

MeshNode::MeshNode(const Mtl& material,
         const std::vector<QVector3D>& vertexs) : type(V), material(material), size(vertexs.size()){

    vertex_data = new float[size * getTypeSize()];

    std::memcpy(vertex_data, vertexs.data(), vertexs.size() * 3 * sizeof(float));
};


size_t MeshNode::getTypeSize() const{
    switch (this->type) {
        case meshType::VNT:
            return 8;
        case meshType::VN:
            return 6;
        case meshType::VT:
            return 5;
        case meshType::V:
            return 3;
        case meshType::UNDEFINED:
            return 0;
    }

    return 0;
}
