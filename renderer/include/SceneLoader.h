#ifndef SCENELOADER_H
#define SCENELOADER_H
#include <forward_list>

#include <MeshLoader.h>
#include <SceneParser.h>
#include "ShaderHolder.h"

class SceneLoader : public QObject, protected QOpenGLFunctions, public ShaderHolder{
    Q_OBJECT
public:
    SceneLoader(){}

    void setScene(const Scene &scene, const std::string &path);

    // only not inited
    void init_buffers();

    // tuple(scale,transform,params) first param must be model matrix
    void paint(const std::function<std::tuple<QVector3D,QVector3D,program_param>(const MeshRoot &)> &);
    // first param must be model matrix
    void paint(const std::function<program_param(const MeshRoot&)> &);

private:
    void afterSetShader(meshType type) override;

    std::forward_list<MeshLoader> loaders;
    std::forward_list<std::pair<QVector3D,QVector3D>> transformations;
};

#endif // SCENELOADER_H
