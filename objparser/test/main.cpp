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

    SUBCASE("detailedCheckCube1"){
        QVector3D fiveV(-1,1,-1);
        REQUIRE(test.getVertices()[0].x() == fiveV.x());
        REQUIRE(test.getVertices()[0].y() == fiveV.y());
        REQUIRE(test.getVertices()[0].z() == fiveV.z());

        QVector3D fiveN(0,1,0);
        CHECK(test.getNormals()[0].x() == fiveN.x());
        CHECK(test.getNormals()[0].y() == fiveN.y());
        CHECK(test.getNormals()[0].z() == fiveN.z());

        QVector2D fiveT(0.875000, 0.500000);
        CHECK(test.getTextures()[0].x() == fiveT.x());
        CHECK(test.getTextures()[0].y() == fiveT.y());
    }

    SUBCASE("detailedCheckCube2"){
        QVector3D fiveV(1,1,-1);
        CHECK(test.getVertices()[2].x() == fiveV.x());
        CHECK(test.getVertices()[2].y() == fiveV.y());
        CHECK(test.getVertices()[2].z() == fiveV.z());

        QVector3D fiveN(0,1,0);
        CHECK(test.getNormals()[2].x() == fiveN.x());
        CHECK(test.getNormals()[2].y() == fiveN.y());
        CHECK(test.getNormals()[2].z() == fiveN.z());

        QVector2D fiveT(0.625000, 0.500000);
        CHECK(test.getTextures()[2].x() == fiveT.x());
        CHECK(test.getTextures()[2].y() == fiveT.y());
    }

    SUBCASE("detailedCheckCube3"){
        QVector3D fiveV(1,1,1);
        CHECK(test.getVertices()[3].x() == fiveV.x());
        CHECK(test.getVertices()[3].y() == fiveV.y());
        CHECK(test.getVertices()[3].z() == fiveV.z());

        QVector3D fiveN(0,0,1);
        CHECK(test.getNormals()[3].x() == fiveN.x());
        CHECK(test.getNormals()[3].y() == fiveN.y());
        CHECK(test.getNormals()[3].z() == fiveN.z());

        QVector2D fiveT(0.625000, 0.750000);
        CHECK(test.getTextures()[3].x() == fiveT.x());
        CHECK(test.getTextures()[3].y() == fiveT.y());
    }

    SUBCASE("detailedCheckCube4"){
        QVector3D fiveV(-1,-1,1);
        CHECK(test.getVertices()[4].x() == fiveV.x());
        CHECK(test.getVertices()[4].y() == fiveV.y());
        CHECK(test.getVertices()[4].z() == fiveV.z());

        QVector3D fiveN(0,0,1);
        CHECK(test.getNormals()[4].x() == fiveN.x());
        CHECK(test.getNormals()[4].y() == fiveN.y());
        CHECK(test.getNormals()[4].z() == fiveN.z());

        QVector2D fiveT(0.375000, 1.000000);
        CHECK(test.getTextures()[4].x() == fiveT.x());
        CHECK(test.getTextures()[4].y() == fiveT.y());
    }
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
