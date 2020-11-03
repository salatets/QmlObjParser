#include "MeshNodeLoader.h"

void MeshNodeLoader::init_buffers(){
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
    template_init_buffer();

    vbo.release();
    vao.release();
}

void MeshNodeLoader::setShader(meshType type){
    if(m_mesh.getType() != type)
        return;
}

void MeshNodeLoader::paint(const std::function<void(QOpenGLShaderProgram*)>& f){
    program->bind();

    f(program);

    vao.bind();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    template_paint();

    glDisable(GL_CULL_FACE);

    vao.release();
    program->release();
}

MeshNodeLoader::~MeshNodeLoader(){
    vbo.destroy();
    vao.destroy();
}
