#include "MeshLoader.h"

MeshLoader::~MeshLoader(){
    for(size_t i = 0; i < loaders_size; ++i){
        delete loaders[i];
    }

    delete[] loaders;
}

void MeshLoader::init_buffers(QOpenGLShaderProgram* program){
    for(size_t i = 0; i < loaders_size; ++i){
        loaders[i]->init_buffers(program);
    }
}

void MeshLoader::paint(QOpenGLShaderProgram* program){
    for(size_t i = 0; i < loaders_size; ++i){
        loaders[i]->paint(program);
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
        loaders[i] = new MeshNodeLoader(*p,path);
        ++p;
    }
}


