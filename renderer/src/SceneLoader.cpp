#include "SceneLoader.h"
#include "vecUtils.h"

void SceneLoader::setScene(const Scene &scene, const std::string &path){
    loaders.resize(scene.meshes.size());
    transformations.resize(scene.meshes.size());

    auto p = loaders.begin();
    auto q = transformations.begin();
    for(auto& mesh : scene.meshes){
        (*q) = std::make_pair(vec3ToQVector3D( std::get<0>(mesh)),vec3ToQVector3D(std::get<1>(mesh)));
        p->setMesh(std::get<2>(mesh),path);
        p->setShader(meshType::VNT, getShader(meshType::VNT));
        p->setShader(meshType::VN, getShader(meshType::VN));
        p->setShader(meshType::VT, getShader(meshType::VT));
        p->setShader(meshType::V, getShader(meshType::V));
        p++;
        q++;
    }
}

void SceneLoader::init_buffers(){
    for(auto& loader : loaders){
        if(!loader.isInited())
            loader.init_buffers();
    }
}

void SceneLoader::paint(const std::function<program_param (const MeshRoot &)>& get_param){
    auto p = loaders.begin();
    auto q = transformations.begin();
    for(; p != loaders.end();p++,q++){
        p->paint(q->first,q->second,get_param);
    }
}

void SceneLoader::paint(const std::function<std::tuple<QVector3D,QVector3D,program_param>(const MeshRoot &)>& get_param){
    for(auto p = loaders.begin(); p != loaders.end();p++){
        p->paint(get_param);
    }
}

void SceneLoader::afterSetShader(meshType type){
    for(auto& loader : loaders){
            loader.setShader(type,getShader(type));
    }
}




