#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include <MeshNode.h>
#include <iostream>

class MeshFactory{
public:
    static MeshNode MakeMesh(const Mtl& material,
                       const char& format,
                       const std::vector<QVector3D>& vertexs,
                       const std::vector<QVector3D>& normals,
                       const std::vector<QVector2D>& uvs){

         switch(format){
         case 'a':{
             return MeshNodeVNT(material, vertexs, normals, uvs);
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
