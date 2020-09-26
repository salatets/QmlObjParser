#ifndef TST_PARSETEST_H
#define TST_PARSETEST_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "mesh.h"

using namespace testing;

TEST(ParseTest, checkFace){
    std::string el2;
    char format;
    std::istringstream stream("1/1/1 2/2 3//3 4");

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    EXPECT_EQ(format,'a');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    EXPECT_EQ(format,'c');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    EXPECT_EQ(format,'b');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    EXPECT_EQ(format,'d');
}

TEST(ParseTest, checkWrongFace){
    std::string el2;
    char format;
    std::istringstream stream("1/1/ 2/ 3// / //");

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    EXPECT_EQ(format,'n');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    EXPECT_EQ(format,'n') << el2;

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    EXPECT_EQ(format,'n');


}

TEST(ParseTest, checkWrongFace2){
    std::string el2;
    char format;
    std::istringstream stream("/ // /1");

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    EXPECT_EQ(format,'n');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    EXPECT_EQ(format,'n');

    format = Mesh::checkFaceFormat(stream);
    stream >> el2;
    EXPECT_EQ(format,'n');
}

TEST(ParseTest, checkCube){
    Mesh test;
    ASSERT_EQ(test.parseOBJ("cube.obj"), true);
    ASSERT_EQ(test.getFormat(), 'a');
    ASSERT_EQ(test.getVertices().size(),test.getTextures().size());
    ASSERT_EQ(test.getVertices().size(),test.getNormals().size());
}

TEST(ParseTest, checkCude){
    Mesh test;
    ASSERT_EQ(test.parseOBJ("cude.obj"), true);
    ASSERT_EQ(test.getFormat(), 'd');
    ASSERT_EQ(test.getTextures().size(),0);
    ASSERT_EQ(test.getNormals().size(),0);
}

TEST(ParseTest, checkMonkey){
    Mesh test;
    ASSERT_EQ(test.parseOBJ("monkey.obj"), true);
    ASSERT_EQ(test.getFormat(), 'd');
    ASSERT_EQ(test.getTextures().size(),0);
    ASSERT_EQ(test.getNormals().size(),0);
}

TEST(ParseTest, checkDahl){
    Mesh test;
    ASSERT_EQ(test.parseOBJ("dahl.obj"), true);
    ASSERT_EQ(test.getFormat(), 'b');
    ASSERT_EQ(test.getTextures().size(),0);
    ASSERT_EQ(test.getNormals().size(),test.getVertices().size());
}

#endif // TST_PARSETEST_H
