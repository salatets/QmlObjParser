#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <Mesh.h>
#include <ImageLoader.h>
#include "doctest.h"

TEST_CASE("load bmp"){
    Texture *texture = parseBMP("bgr.bmp");
    REQUIRE(texture != nullptr);
    REQUIRE(texture->bitsPerPixel == 24);
    REQUIRE(texture->height == 16);
    REQUIRE(texture->width == 16);
    REQUIRE(texture->pixels.size() == texture->width*texture->height*3);
}
