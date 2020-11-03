#include "MeshLoader.h"

MeshLoader::~MeshLoader(){
    for(size_t i = 0; i < loaders_size; ++i){
        delete loaders[i];
    }

    delete[] loaders;
}

void MeshLoader::init_buffers(){
    initializeOpenGLFunctions();
    for(size_t i = 0; i < loaders_size; ++i){
        loaders[i]->init_buffers();
    }
}

void MeshLoader::setMesh(const MeshRoot& mesh, const std::string& path){
    if(loaders_size != 0 && loaders != nullptr){
        for(size_t i = 0; i < loaders_size; ++i)
            delete loaders[i];
        delete[] loaders;
    }

    loaders_size = mesh.size();
    loaders = new MeshNodeLoader*[mesh.size()]; // TODO placement new

    auto p = mesh.cbegin();
    for(size_t i = 0; i < loaders_size; ++i){
        loaders[i] = new MeshNodeLoaderVNT(*p,path, VNT_shader); // todo change
        loaders[i]->init_buffers();
        connect(this,&MeshLoader::shaderChanged,loaders[i],&MeshNodeLoader::setShader, Qt::DirectConnection); // MAYBE direct
        ++p;
    }
}

void MeshLoader::paint(std::function<void(QOpenGLShaderProgram*, const MeshRoot&)> f){
    for(size_t i = 0; i < loaders_size; ++i){
        loaders[i]->paint(
                    [f,model = this->m_model](QOpenGLShaderProgram* arg1){return f(arg1, model);}
        );
    }
}

void MeshLoader::setShader(meshType type, char *frag, char *vert){
    initializeOpenGLFunctions();
    auto *shader = getShader(type);

    if(shader == nullptr){
        assignShader(type, new QOpenGLShaderProgram()); // TODO refactor;
        shader = getShader(type);
    }
    if(shader->isLinked())
        shader->removeAllShaders();

    bool success = shader->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vert);
    success &= shader->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, frag);
    Q_ASSERT(success);

    success = shader->link();
    Q_ASSERT(success);

    Q_EMIT shaderChanged(type);
}

QOpenGLShaderProgram* MeshLoader::getShader(meshType type){ // if meshType undefined?
    switch(type){
        case meshType::VNT:{
            return VNT_shader;
        }
        case meshType::VN:{
            return VN_shader;
        }
        case meshType::VT:{
            return VT_shader;
        }
        case meshType::V:{
            return V_shader;
        }
    }

    return nullptr;
}

void MeshLoader::assignShader(meshType type, QOpenGLShaderProgram* new_point){
    switch(type){
        case meshType::VNT:{
            VNT_shader = new_point;
            break;
        }
        case meshType::VN:{
            VN_shader = new_point;
            break;
        }
        case meshType::VT:{
            VT_shader = new_point;
            break;
        }
        case meshType::V:{
            V_shader = new_point;
            break;
        }
    }
}


