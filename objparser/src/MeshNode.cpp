#include <MeshNode.h>

MeshNodeVNT::MeshNodeVNT(unsigned int textureId, QOpenGLShaderProgram* shader,
                        const std::vector<QVector3D>& vertexs,
                        const std::vector<QVector3D>& normals,
                        const std::vector<QVector2D>& uvs)
    : textureId(textureId), shader(shader) {

    size_t size = vertexs.size();
    vertex_data = static_cast<DataVNT*>(malloc(sizeof(DataVNT) * size));

    for(size_t i = 0; i < size; ++i){
        vertex_data[i].vertex = vertexs[i];
        vertex_data[i].normal = normals[i];
        vertex_data[i].uv = uvs[i];
    }

    vao.bind();

    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    bool success = vbo.create();
    Q_ASSERT(success);
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    success = vao.create();
    Q_ASSERT(success);

    success = vbo.bind();
    Q_ASSERT(success);
    vbo.allocate(vertex_data, sizeof(vertex_data) / sizeof(float));
    shader->setAttributeBuffer("position", GL_FLOAT, 0, offsetof(DataVNT, vertex));
    shader->enableAttributeArray("position");
    shader->setAttributeBuffer("normal", GL_FLOAT, 0, offsetof(DataVNT, normal));
    shader->enableAttributeArray("normal");
    shader->setAttributeBuffer("texCoords", GL_FLOAT, 0, offsetof(DataVNT, uv));
    shader->enableAttributeArray("texCoords");

    vbo.release();
    vao.release();
}

MeshNodeVNT::~MeshNodeVNT(){
    delete diffuse;
    free(vertex_data);
}

 // run with binded shader
void MeshNodeVNT::Draw(){
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, textureId);

     vao.bind();
     glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex_data)/sizeof(float));
     vao.release();
}


