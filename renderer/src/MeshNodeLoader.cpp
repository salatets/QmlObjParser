#include "MeshNodeLoader.h"
#include "vecUtils.h"

MeshNodeLoader::~MeshNodeLoader(){
    glDeleteTextures(1, &textureId);
    vbo.destroy();
    vao.destroy();
}

Texture undefined_texture(){ // TODO singleton
    Texture tex;
    tex.bitsPerPixel = 24;
    tex.height = 1;
    tex.width = 1;
    tex.type = BMP;
    tex.pixels = {255,0,139}; // violet
    return tex;
}

void MeshNodeLoader::init_buffers(QOpenGLShaderProgram *program){
    if(!vao.isCreated()){
        bool success = vao.create();
        Q_ASSERT(success);
    }

    vao.bind();

    if (!vbo.isCreated()){
        vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        bool success = vbo.create();
        Q_ASSERT(success);
        vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    }

    bool success = vbo.bind();
    Q_ASSERT(success);
    vbo.allocate(m_mesh.getData(), m_mesh.getSize() * sizeof(float) * 8);
    program->setAttributeBuffer("position", GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
    program->enableAttributeArray("position");
    program->setAttributeBuffer("normal", GL_FLOAT, m_mesh.getSize() * 3  * sizeof(GLfloat), 3, 3 * sizeof(GLfloat));
    program->enableAttributeArray("normal");
    program->setAttributeBuffer("texCoords", GL_FLOAT, m_mesh.getSize() * 6  * sizeof(GLfloat), 2, 2 * sizeof(GLfloat));
    program->enableAttributeArray("texCoords");

    vbo.release();
    vao.release();

    Texture diffuse = parseBMP(m_path + m_mesh.getMaterial().diffuse_map_path);

    if(diffuse.type == ImageType::Undefined){
        diffuse = undefined_texture();
    }

    LoadTexture(diffuse);
}

void MeshNodeLoader::paint(QOpenGLShaderProgram* program){
    initializeOpenGLFunctions();

    vao.bind();

    glEnable(GL_DEPTH_TEST);
    program->setUniformValue("material.specular", vec3ToQVector3D( m_mesh.getMaterial().specular));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glDrawArrays(GL_TRIANGLES, 0, m_mesh.getSize());

    vao.release();
    glActiveTexture(GL_TEXTURE0);

}

unsigned int giveGLType(ImageType type, std::uint16_t bitsPerPixel){
    switch (type) {
    case BMP:{
        if(bitsPerPixel == 24)
            return GL_BGR;
        else if(bitsPerPixel == 32)
            return GL_BGRA;
        else
            return -1;
        }
    case Undefined:
        return 0;
    }
    return -1;
}

void MeshNodeLoader::LoadTexture(Texture texture){
    initializeOpenGLFunctions();
    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0,
                 texture.bitsPerPixel == 32 ? GL_RGBA : GL_RGB,
                 texture.width, texture.height,
                 0, giveGLType(texture.type,texture.bitsPerPixel),
                 GL_UNSIGNED_BYTE, texture.pixels.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}