#include <MeshNode.h>

MeshNode::MeshNode() : type(UNDEFINED), material(), size(0), vertex_data(nullptr){}

MeshNode::MeshNode(const Mtl& material,
         const std::vector<QVector3D>& vertexs,
         const std::vector<QVector3D>& normals,
         const std::vector<QVector2D>& uvs) : type(VNT), material(material), size(vertexs.size()){

    vertex_data = new float[size * getTypeSize()];

    // TODO memcpy
    //std::cerr << "size " << this->size * getTypeSize() << "\n";
    for(size_t i = 0; i < this->size; ++i){
        //std::cerr << "vector " << i*3 << " " << i*3 +1 << " " << i*3+2 << "\n";
        vertex_data[i * 3] = vertexs[i].x();
        vertex_data[i* 3 + 1] = vertexs[i].y();
        vertex_data[i* 3 + 2] = vertexs[i].z();

        //std::cerr << "normal " << (this->size + i) * 3 << " " << (this->size + i) * 3 +1 << " " << (this->size + i) * 3+2 << "\n";
        vertex_data[(this->size + i) * 3] = normals[i].x();
        vertex_data[(this->size + i) * 3 + 1] = normals[i].y();
        vertex_data[(this->size + i) * 3 + 2] = normals[i].z();

        //std::cerr << "uv " << (this->size * 6) + i * 2 << " " << (this->size * 6) + i * 2 + 1  << "\n";
        vertex_data[(this->size * 6) + i * 2] = uvs[i].x();
        vertex_data[(this->size * 6) + i * 2 + 1] = uvs[i].y();
    }
};

size_t MeshNode::getTypeSize() const{
    switch (this->type) {
        case meshType::VNT:
            return 8;
        case meshType::UNDEFINED:
            return 0;
    }
}
