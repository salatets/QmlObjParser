#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include <MeshNode.h>

class MeshFactory{
public:
    static MeshNode MakeMesh(const Mtl& material,
                       const meshType& format,
                       const std::vector<Vec3>& vertexs,
                       const std::vector<Vec3>& normals,
                       const std::vector<Vec2>& uvs){

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
