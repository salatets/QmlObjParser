#include "MeshLoader.h"

MeshLoader::MeshLoader()
{

}


Mesh(QOpenGLContext* m_context) : center(0,0,0),size(0,0,0) {
    if(m_context == nullptr)
        return;

    m_funcs = m_context->versionFunctions<QOpenGLFunctions_4_3_Core>();

      if (!m_funcs) {
        qWarning("Could not obtain OpenGL versions object");
        exit(1);
      }

      m_funcs->initializeOpenGLFunctions();
};

QOpenGLFunctions_4_3_Core* m_funcs;

int giveGLType(ImageType type, std::uint16_t bitsPerPixel){
    switch (type) {
    case BMP:{
        if(bitsPerPixel == 24)
            return GL_BGR;
        else if(bitsPerPixel == 32)
            return GL_BGRA;
        else
            return -1;
        break;
        }
    }
    return -1;
}

unsigned int LoadTexture(Texture* texture){
    GLuint textureID;
    m_funcs->glGenTextures(1, &textureID);

    m_funcs->glBindTexture(GL_TEXTURE_2D, textureID);  //MAYBE crash
    m_funcs->glTexImage2D(GL_TEXTURE_2D, 0, texture->bitsPerPixel == 32 ? GL_RGBA : GL_RGB, texture->width, texture->height, 0, giveGLType(texture->type,texture->bitsPerPixel), GL_UNSIGNED_BYTE, texture->pixels.data());
    m_funcs->glGenerateMipmap(GL_TEXTURE_2D);

    m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(texture);
    return textureID;
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

void  Mesh::init_buffers(QOpenGLShaderProgram& shader){
    for(auto&& mesh : meshes){
        mesh.initBuffers(shader);
    }
}

void Mesh::draw(QOpenGLShaderProgram &shader){
    for(auto&& mesh : meshes){
        mesh.draw(shader);
    }
}
