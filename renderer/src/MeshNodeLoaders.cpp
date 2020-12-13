#include "MeshNodeLoaders.h"

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

static const Texture undefined_texture = {24, 1, 1, {static_cast<char>(255),0,static_cast<char>(139)}, BMP}; // TODO constexpr

//------------------------------------------

void MeshNodeLoaderVNT::LoadTexture(Texture texture){ // TODO texture loader
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

MeshNodeLoaderVNT::MeshNodeLoaderVNT(const MeshNode& mesh,
        const std::string& path,
        std::shared_ptr<QOpenGLShaderProgram> &shad
        ): MeshNodeLoader(mesh, path, shad){

    Texture diffuse = parseBMP(m_path + m_mesh.getMaterial().diffuse_map_path);

    if(diffuse.type == ImageType::Undefined){
        diffuse = undefined_texture;
    }

    LoadTexture(diffuse);
}

void MeshNodeLoaderVNT::template_init_buffer(){
    vbo.allocate(m_mesh.getData(), m_mesh.getSize() * sizeof(float) * MeshNode::getTypeSize(m_mesh));
    program->setAttributeBuffer("position", GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
    program->enableAttributeArray("position");
    program->setAttributeBuffer("normal", GL_FLOAT, m_mesh.getSize() * 3  * sizeof(GLfloat), 3, 3 * sizeof(GLfloat));
    program->enableAttributeArray("normal");
    program->setAttributeBuffer("texCoords", GL_FLOAT, m_mesh.getSize() * 6  * sizeof(GLfloat), 2, 2 * sizeof(GLfloat));
    program->enableAttributeArray("texCoords");
}

void MeshNodeLoaderVNT::template_paint(){
    program->setUniformValue("material.specular", QVector3D(0.5,0.5,0.5)); // TODO replace

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glDrawArrays(GL_TRIANGLES, 0, m_mesh.getSize());

    glActiveTexture(GL_TEXTURE0);
}

MeshNodeLoaderVNT::~MeshNodeLoaderVNT(){
    glDeleteTextures(1, &textureId);
}

//------------------------------------------

MeshNodeLoaderVN::MeshNodeLoaderVN(const MeshNode& mesh,
        const std::string& path,
        std::shared_ptr<QOpenGLShaderProgram> &shad
        ): MeshNodeLoader(mesh, path,shad){}

void MeshNodeLoaderVN::template_init_buffer(){
    vbo.allocate(m_mesh.getData(), m_mesh.getSize() * sizeof(float) * MeshNode::getTypeSize(m_mesh));
    program->setAttributeBuffer("position", GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
    program->enableAttributeArray("position");
    program->setAttributeBuffer("normal", GL_FLOAT, m_mesh.getSize() * 3  * sizeof(GLfloat), 3, 3 * sizeof(GLfloat));
    program->enableAttributeArray("normal");
}

void MeshNodeLoaderVN::template_paint(){
    program->setUniformValue("material.specular", QVector3D(0.5,0.5,0.5)); // TODO replace

    glDrawArrays(GL_TRIANGLES, 0, m_mesh.getSize());
}

//------------------------------------------

MeshNodeLoaderV::MeshNodeLoaderV(
        const MeshNode& mesh,
        const std::string& path,
        std::shared_ptr<QOpenGLShaderProgram>& shad
        ): MeshNodeLoader(mesh, path,shad){}

void MeshNodeLoaderV::template_init_buffer(){
    vbo.allocate(m_mesh.getData(), m_mesh.getSize() * sizeof(float) * MeshNode::getTypeSize(m_mesh));
    program->setAttributeBuffer("position", GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
    program->enableAttributeArray("position");
}

void MeshNodeLoaderV::template_paint(){
    glDrawArrays(GL_TRIANGLES, 0, m_mesh.getSize());
}

//------------------------------------------

MeshNodeLoaderVT::MeshNodeLoaderVT(const MeshNode& mesh,
        const std::string& path,
        std::shared_ptr<QOpenGLShaderProgram> &shad
        ): MeshNodeLoader(mesh, path,shad){}

void MeshNodeLoaderVT::template_init_buffer(){
    vbo.allocate(m_mesh.getData(), m_mesh.getSize() * sizeof(float) * MeshNode::getTypeSize(m_mesh));
    program->setAttributeBuffer("position", GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
    program->enableAttributeArray("position");
    program->setAttributeBuffer("texCoords", GL_FLOAT, m_mesh.getSize() * 3  * sizeof(GLfloat), 2, 2 * sizeof(GLfloat));
    program->enableAttributeArray("texCoords");
}

void MeshNodeLoaderVT::template_paint(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glDrawArrays(GL_TRIANGLES, 0, m_mesh.getSize());
    glActiveTexture(GL_TEXTURE0);
}

void MeshNodeLoaderVT::LoadTexture(Texture texture){ // TODO texture loader
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

MeshNodeLoaderVT::~MeshNodeLoaderVT(){
    glDeleteTextures(1, &textureId);
}
