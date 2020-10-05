#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include <MeshNode.h>
#include <iostream>

class MeshFactory{
public:
    static MeshNode MakeMesh(QOpenGLFunctions_4_3_Core* funcs, const Mtl& material,
                       const unsigned int& textureIds,
                       const char& format,
                       const std::vector<QVector3D>& vertexs,
                       const std::vector<QVector3D>& normals,
                       const std::vector<QVector2D>& uvs){

         switch(format){
         case 'a':{
             return MeshNodeVNT(funcs, textureIds, vertexs, normals, uvs);
             break;
         }
         case 'b':
             break;
         case 'c':
             break;
         case 'd':
             break;
         }

         std::cerr << "return pure MeshNode";

        return MeshNode();
     }
};

#endif // MESHFACTORY_H
