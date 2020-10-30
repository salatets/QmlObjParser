#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include <MeshNode.h>

class MeshFactory{
public:
    static MeshNode MakeMesh(const Mtl& material,
                       const meshType& format,
                       const std::vector<QVector3D>& vertexs,
                       const std::vector<QVector3D>& normals,
                       const std::vector<QVector2D>& uvs){

         switch(format){
         case meshType::VNT:{
             return MeshNode(material, vertexs, normals, uvs);
             break;
         }
         case meshType::VN:
             break;
         case meshType::VT:
             break;
         case meshType::V:
             break;
         case meshType::UNDEFINED:
             return MeshNode();
         }


        return MeshNode();
     }
};

#endif // MESHFACTORY_H
