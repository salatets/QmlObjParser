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
                loaders[i] = new MeshNodeLoaderVNT(*p, path, getShader(meshType::VNT));
                break;
            }
            case meshType::VN:{
                loaders[i] = new MeshNodeLoaderVN(*p, path, getShader(meshType::VN));
                break;
            }
            case meshType::VT:{
                loaders[i] = new MeshNodeLoaderVT(*p, path, getShader(meshType::VT));
                break;
            }
            case meshType::V:{
                loaders[i] = new MeshNodeLoaderVN(*p, path, getShader(meshType::V));
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

inline void MeshLoader::internal_paint(const program_param& params){
    for(size_t i = 0; i < loaders_size; ++i){
        loaders[i]->paint(params);
    }
}

inline void apply_transform(const QVector3D& scale, const QVector3D& translate, program_param& params){
    const char* model_name = params.front().first;
    QMatrix4x4 model = std::get<QMatrix4x4>(params.front().second);
    params.pop_front();

    model.scale(scale);
    model.translate(translate);
    params.push_front(std::make_pair(model_name, model));
}

void MeshLoader::paint(const QVector3D& scale, const QVector3D& translate, const std::function<program_param(const MeshRoot&)>& get_params){
    auto params = get_params(m_model);
    apply_transform(scale,translate, params);
    internal_paint(params);
}

void MeshLoader::paint(const std::function<std::tuple<QVector3D,QVector3D,program_param>(const MeshRoot&)>& get_params){
    auto out = get_params(m_model);
    program_param params = std::get<2>(out);

    apply_transform(std::get<0>(out), std::get<1>(out),params);
    internal_paint(params);
}

void MeshLoader::afterSetShader(meshType type){
    for(size_t i = 0; i < loaders_size; ++i){
        loaders[i]->setShader(type, getShader(type));
    }

    isInit=false;
}
