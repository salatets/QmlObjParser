#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "mesh.h"
#include "doctest.h"


TEST_CASE("checkFace"){
    std::string el2;
    char format;
    std::istringstream stream("1/1/1 2/2 3//3 4");

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    CHECK(format == 'a');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    CHECK(format == 'c');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    CHECK(format == 'b');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    CHECK(format == 'd');
}

TEST_CASE("checkWrongFace"){
    std::string el2;
    char format;
    std::istringstream stream("1/1/ 2/ 3//");

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    CHECK(format == 'n');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    CHECK(format == 'n');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    CHECK(format == 'n');


}

TEST_CASE("checkWrongFace2"){
    std::string el2;
    char format;
    std::istringstream stream("/ // /1");

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    CHECK(format == 'n');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    CHECK(format == 'n');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    CHECK(format == 'n');
}

TEST_CASE("checkCube"){
    Mesh test;
    REQUIRE(test.parseOBJ("cube.obj") ==  true);
    CHECK(test.getFormat() ==  'a');
    CHECK(test.getVertices().size() == test.getTextures().size());
    CHECK(test.getVertices().size() == test.getNormals().size());
}

TEST_CASE("checkCude"){
    Mesh test;
    REQUIRE(test.parseOBJ("cude.obj") ==  true);
    CHECK(test.getFormat() ==  'd');
    CHECK(test.getTextures().size() == 0);
    CHECK(test.getNormals().size() == 0);
}

TEST_CASE("checkMonkey"){
    Mesh test;
    REQUIRE(test.parseOBJ("monkey.obj") ==  true);
    CHECK(test.getFormat() ==  'd');
    CHECK(test.getTextures().size() == 0);
    CHECK(test.getNormals().size() == 0);
}

TEST_CASE("checkDahl"){
    Mesh test;
    REQUIRE(test.parseOBJ("dahl.obj") ==  true);
    CHECK(test.getFormat() ==  'b');
    CHECK(test.getTextures().size() == 0);
    CHECK(test.getNormals().size() == test.getVertices().size());
}
