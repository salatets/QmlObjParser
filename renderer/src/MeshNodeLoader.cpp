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

void MeshNodeLoader::setShader(meshType type, std::shared_ptr<QOpenGLShaderProgram>& shader){
    if(m_mesh.getType() != type)
        return;

    program = shader;
}

void MeshNodeLoader::paint(const program_param &params){
    program->bind();

    for(auto&& p : params){
        std::visit([program = this->program, &p](auto&& arg){program->setUniformValue(p.first,arg);},p.second);
    }

    vao.bind();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    template_paint();

    glDisable(GL_CULL_FACE);

    vao.release();
    program->release();
}

MeshNodeLoader::~MeshNodeLoader(){
    vbo.destroy();
    vao.destroy();
}
