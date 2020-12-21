#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <Mesh.h>
#include "MeshNodeLoader.h"
#include "ShaderHolder.h"

class MeshLoader : public QObject, protected QOpenGLFunctions, public ShaderHolder<meshType>{
    Q_OBJECT
public:
    MeshLoader() :
        ShaderHolder({meshType::VNT,meshType::VN,meshType::VT,meshType::V}),
        loaders(nullptr),
        loaders_size(0),
        isInit(false)
    {}

    ~MeshLoader();

    bool isInited(){return isInit;}

    void setMesh(const MeshRoot &mesh, const std::string &path);

    void init_buffers();
    // tuple(scale,transform,params) first param must be model matrix
    void paint(const std::function<std::tuple<QVector3D,QVector3D,program_param>(const MeshRoot &)> &);
    // first param must be model matrix
    void paint(const QVector3D& scale, const QVector3D& translate, const std::function<program_param(const MeshRoot&)> &);

private:
    void internal_paint(const program_param& params);

    void afterSetShader(meshType) override;

    MeshNodeLoader** loaders;
    size_t loaders_size;
    bool isInit;
    MeshRoot m_model;
};

#endif // MESHLOADER_H
