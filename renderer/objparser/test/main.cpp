#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <ObjParser.h>
#include <ImageLoader.h>
#include "doctest.h"

bool eqArray(const float* array1,const float* array2, size_t size){
    bool eq = true;

    for(size_t i = 0; i < size; ++i){
        eq &= (array1[i] == array2[i]);
        if(!eq){
            std::cerr << "orig: " << array1[i] << " new: " << array2[i]  << " " << i << "\n";
            return eq;
        }
    }

    return eq;
}

TEST_CASE("load cube.obj"){
    Mesh test = parseOBJ("cube.obj");
    REQUIRE(test.getNodesSize() == 1);

    MeshNode mesh = *test.getNodesBegin();

    SUBCASE("test mesh"){
        CHECK(mesh.getType() == 'a');
        float arr[24] = {-1,1,-1, 0.875,0.5, 0,1,0,
                         1,1,1, 0.625,0.75, 0,1,0,
                         1,1,-1, 0.625,0.5, 0,1,0};
        CHECK(eqArray(mesh.getData(), arr, 24) == true);
    }


    SUBCASE("test material"){
        Mtl material = mesh.getMaterial();

        CHECK(material.name == "Material.001");
        CHECK(material.diffuse_map_path == "cube.bmp");
        CHECK(material.ambient == QVector3D(1,1,1));
        CHECK(material.diffuse == QVector3D(0.8,0.8,0.8));
        CHECK(material.diffuse == QVector3D(0.8,0.8,0.8));
        CHECK(material.specular == QVector3D(0.5,0.5,0.5));
        CHECK(material.illum_mode == illum::HIGHTLIGHT_ON);
    }
}

TEST_CASE("load bmp"){
    Texture *texture = parseBMP("bgr.bmp");
    REQUIRE(texture != nullptr);
    CHECK(texture->bitsPerPixel == 24);
    CHECK(texture->height == 16);
    CHECK(texture->width == 16);
    CHECK(texture->pixels.size() == texture->width*texture->height*3);
}

TEST_CASE("load bmp alpha"){
    Texture *texture = parseBMP("bgra.bmp");
    REQUIRE(texture != nullptr);
    CHECK(texture->bitsPerPixel == 32);
    CHECK(texture->height == 16);
    CHECK(texture->width == 16);
    CHECK(texture->pixels.size() == texture->width*texture->height*4);
}
