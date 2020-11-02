#include "MeshLoader.h"

MeshLoader::~MeshLoader(){
    for(size_t i = 0; i < loaders_size; ++i){
        delete loaders[i];
    }

    delete[] loaders;
}

void MeshLoader::init_buffers(){
    for(size_t i = 0; i < loaders_size; ++i){
        loaders[i]->init_buffers();
    }
}

void MeshLoader::paint(){
    for(size_t i = 0; i < loaders_size; ++i){
        loaders[i]->paint();
    }
}

void MeshLoader::setMesh(MeshRoot mesh, std::string path){
    if(loaders_size != 0 && loaders != nullptr){
        for(size_t i = 0; i < loaders_size; ++i)
            delete loaders[i];
        delete[] loaders;
    }

    loaders_size = mesh.size();
    loaders = new MeshNodeLoader*[mesh.size()]; // TODO placement new

    auto p = mesh.cbegin();
    for(size_t i = 0; i < loaders_size; ++i){
        loaders[i] = new MeshNodeLoader(*p,path, getShader(p->getType()));
        loaders[i]->init_buffers();
        connect(this,&MeshLoader::shaderChanged,loaders[i],&MeshNodeLoader::setShader, Qt::DirectConnection); // MAYBE direct
        ++p;
    }
}

void MeshLoader::setShader(meshType type, char *frag, char *vert){
    auto shader = getShader(type);

    if(shader->isLinked())
        shader->removeAllShaders();

    bool success = shader->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vert);
    success &= shader->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, frag);
    Q_ASSERT(success);

    success = shader->link();
    Q_ASSERT(success);

    emit shaderChanged(type);


}

QOpenGLShaderProgram* MeshLoader::getShader(meshType type){ // if meshType undefined?
    QOpenGLShaderProgram* shader;

    switch(type){
        case VNT:{
            shader = &VNT_shader;
            break;
        }
        case VN:{
            shader = &VN_shader;
            break;
        }
        case VT:{
            shader = &VT_shader;
            break;
        }
        case V:{
            shader = &V_shader;
            break;
        }
    }

    return shader;
}


