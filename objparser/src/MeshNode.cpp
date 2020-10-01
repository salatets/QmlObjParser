#include <MeshNode.h>

MeshNodeVNT::MeshNodeVNT(QOpenGLFunctions_4_3_Core* funcs, unsigned int textureId,
                        const std::vector<QVector3D>& vertexs,
                        const std::vector<QVector3D>& normals,
                        const std::vector<QVector2D>& uvs)
    :  textureId(textureId), m_funcs(funcs), diffuse(nullptr), vertex_data(vertexs.size()) {

    size_t size = vertexs.size();

    for(size_t i = 0; i < size; ++i){
        vertex_data[i].vertex = vertexs[i];
        vertex_data[i].normal = normals[i];
        vertex_data[i].uv = uvs[i];
    }
}

MeshNodeVNT::~MeshNodeVNT(){
    delete diffuse;

    vbo.destroy();
    vao.destroy();
}

void MeshNodeVNT::initBuffers(QOpenGLShaderProgram& shader){
    bool success = vao.create();
    Q_ASSERT(success);

    vao.bind();

    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    success = vbo.create();
    Q_ASSERT(success);
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);

    success = vbo.bind();
    Q_ASSERT(success);
    vbo.allocate(vertex_data.data(), vertex_data.size() * sizeof(DataVNT));
    shader.setAttributeBuffer("position", GL_FLOAT, 0, offsetof(DataVNT, vertex));
    shader.enableAttributeArray("position");
    shader.setAttributeBuffer("normal", GL_FLOAT, 0, offsetof(DataVNT, normal));
    shader.enableAttributeArray("normal");
    shader.setAttributeBuffer("texCoords", GL_FLOAT, 0, offsetof(DataVNT, uv));
    shader.enableAttributeArray("texCoords");

    vbo.release();
    vao.release();
}



 // run with binded shader
void MeshNodeVNT::draw(QOpenGLShaderProgram& shader){
     m_funcs->glActiveTexture(GL_TEXTURE0);
     m_funcs->glBindTexture(GL_TEXTURE_2D, textureId);

     shader.bind();
     vao.bind();
     m_funcs->glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex_data)/sizeof(float));
     vao.release();
     shader.release();
}


