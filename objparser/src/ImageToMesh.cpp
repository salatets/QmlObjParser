#include <algorithm>
#include "ImageToMesh.h"

MeshRoot BmpToMesh(const Texture& texture){
    std::int32_t width = texture.width;
    //std::int32_t height = texture.height % 2 != 0 ? texture.height - 1 : texture.height;
    std::int32_t step;

    switch (texture.bitsPerPixel) {
    case 24:
        step = 3;
        break;
    case 32:
        step = 4;
        break;
    default:
        exit(-1);
        break;
    }
    std::uint8_t padding = texture.width % 4;
    std::uint8_t maxZ = 0;
    std::uint8_t minZ = std::numeric_limits<uint8_t>::max();

    // todo reserve size
    std::vector<Vec3> verticies;
    std::vector<Vec3> normals;

    // assume that texture is monochrome
    for(int y = 0; y < texture.height-1; y++){
        for(int x = 0; x < (width-1)*step; x+=step){
            Vec3 normal(0,0,0);
            std::int32_t x_elem = x / (step);
            std::int32_t line = texture.width * step +padding;
            // 3 4
            // 1 2
            Vec3 po1 = Vec3(x_elem,y, static_cast<std::uint8_t>(texture.pixels[y*line + x]));
            Vec3 po2 = Vec3(x_elem + 1,y, static_cast<std::uint8_t>(texture.pixels[y*line + x + step]));
            Vec3 po3 = Vec3(x_elem, y+1, static_cast<std::uint8_t>(texture.pixels[(y+1)*line+ x]));
            Vec3 po4 = Vec3(x_elem + 1, y+1, static_cast<std::uint8_t>(texture.pixels[(y+1)*line+ x + step]));

            std::uint8_t local_maxZ = (std::max({po1,po2,po3,po4}, [](auto& left, auto& right){return left.z > right.z;})).z;
            std::uint8_t local_minZ = (std::max({po1,po2,po3,po4}, [](auto& left, auto& right){return left.z < right.z;})).z;
            if(maxZ < local_maxZ)
                maxZ = local_maxZ;
            if(minZ > local_minZ)
                minZ = local_minZ;


            //
            if(po1.z == po2.z && po1.z == po3.z){
                Vec3 po3m = Vec3(po3.x,po3.y,po4.z);
                normal = (po3m - po1)^(po2-po1);
                verticies.push_back(po2);
                verticies.push_back(po1);
                verticies.push_back(po3m);

                verticies.push_back(po3m);
                verticies.push_back(po4);
                verticies.push_back(po2);

                normals.push_back(normal);
                normals.push_back(normal);
                normals.push_back(normal);
                normals.push_back(normal);
                normals.push_back(normal);
                normals.push_back(normal);

                po2.z = po4.z;
            }else if(po2.z == po3.z && po2.z == po4.z){
                Vec3 po3m = Vec3(po3.x,po3.y,po1.z);
                normal = (po3m - po1)^(po2-po1);
                verticies.push_back(po2);
                verticies.push_back(po1);
                verticies.push_back(po3m);

                verticies.push_back(po3m);
                verticies.push_back(po4);
                verticies.push_back(po2);

                normals.push_back(normal);
                normals.push_back(normal);
                normals.push_back(normal);
                normals.push_back(normal);
                normals.push_back(normal);
                normals.push_back(normal);

                po2.z = po1.z;
            }

            normal = (po3 - po1)^(po2-po1);
            // 3 *
            // 2 1
            verticies.push_back(po2);
            verticies.push_back(po1);
            verticies.push_back(po3);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
            // 1 2
            // * 3
            verticies.push_back(po3);
            verticies.push_back(po4);
            verticies.push_back(po2);
            normals.push_back(normal);
            normals.push_back(normal);
            normals.push_back(normal);
        }
    }

    Mtl mtl; // TODO fill
    return MeshRoot(Vec3(width/2,texture.height/2, maxZ-minZ / 2), Vec3(width,texture.height,maxZ-minZ), {MeshNode::make(mtl, meshType::VN, verticies,normals)});
}


