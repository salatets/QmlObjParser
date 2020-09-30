#ifndef MESHFACTORY_H
#define MESHFACTORY_H
#include <MeshNode.h>
#include <ImageLoader.h>

class MeshFactory{
public:
    MeshNode* MakeMesh(const Mtl& material, QOpenGLShaderProgram* shader,
                       const std::vector<unsigned int>& textureIds,
                       const char& format,
                       const std::vector<QVector3D>& vertexs,
                       const std::vector<QVector3D>& normals,
                       const std::vector<QVector2D>& uvs){

         switch(format){
         case 'a':{
             return new MeshNodeVNT(textureIds[0], shader, vertexs, normals, uvs);
             break;
         }
         case 'b':
             break;
         case 'c':
             break;
         case 'd':
             break;
         }

        return nullptr;
     }
};

#endif // MESHFACTORY_H
