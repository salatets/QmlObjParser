#include "MeshLoader.h"
#include "MeshNodeLoaders.h"

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
    isInit = true;
}

void MeshLoader::setMesh(const MeshRoot& mesh, const std::string& path){
    if(loaders_size != 0 && loaders != nullptr){
        for(size_t i = 0; i < loaders_size; ++i)
            delete loaders[i];
        delete[] loaders;
        loaders = nullptr;
    }

    loaders_size = mesh.size();
    loaders = new MeshNodeLoader*[mesh.size()]; // TODO placement new

    {
        auto p = mesh.cbegin();
        size_t i = 0;
        for(; i < loaders_size; ++i, ++p){
            switch(p->getType()){
            case meshType::VNT:{
                loaders[i] = new MeshNodeLoaderVNT(*p, path, VNT_shader);
                break;
            }
            case meshType::VN:{
                loaders[i] = new MeshNodeLoaderVN(*p, path, VN_shader);
                break;
            }
            case meshType::VT:{
                loaders[i] = new MeshNodeLoaderVT(*p, path, VT_shader); // TODO NOT TESTED
                break;
            }
            case meshType::V:{
                loaders[i] = new MeshNodeLoaderVN(*p, path, V_shader); // TODO NOT TESTED
                break;
            }
            default:
                Q_ASSERT(false);
            }
        }
    }

    m_model = mesh;
    isInit = false;
}

void MeshLoader::paint(const std::function<void(QOpenGLShaderProgram*, MeshRoot)>& f){
    for(size_t i = 0; i < loaders_size; ++i){
        loaders[i]->paint(
                    [f, model = this->m_model](QOpenGLShaderProgram* arg1)
        {f(arg1,model);}
        );
    }
}

void MeshLoader::setShader(meshType type, char *frag, char *vert){
    auto *shader = getShader(type);

    if(shader == nullptr){
        shader = assignShader(type, new QOpenGLShaderProgram()); // TODO refactor;
    }
    if(shader->isLinked())
        shader->removeAllShaders();

    bool success = shader->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vert);
    success &= shader->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, frag);
    Q_ASSERT(success);

    success = shader->link();
    Q_ASSERT(success);

    isInit = false;
}

QOpenGLShaderProgram* MeshLoader::getShader(meshType type){
    switch(type){
    case meshType::VNT:
        return VNT_shader;
    case meshType::VN:
        return VN_shader;
    case meshType::VT:
        return VT_shader;
    case meshType::V:
        return V_shader;
    }
    return nullptr;
}

QOpenGLShaderProgram* MeshLoader::assignShader(meshType type, QOpenGLShaderProgram* new_point){
    switch(type){
    case meshType::VNT:
        VNT_shader = new_point;
        return new_point;
    case meshType::VN:
        VN_shader = new_point;
        return new_point;
    case meshType::VT:
        VT_shader = new_point;
        return new_point;
    case meshType::V:
        V_shader = new_point;
        return new_point;
    }
    return nullptr;
}


