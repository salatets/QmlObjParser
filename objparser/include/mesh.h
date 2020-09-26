#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <QVector2D>
#include <QVector3D>


class Mesh
{
public:
    Mesh() = default;

    static char checkFaceFormat(std::basic_istream<char>& strm);

    bool parseOBJ(const std::string& path);

    char getFormat() const {return format;}
    std::vector<QVector3D> getVertices() const {return vertices;}
    std::vector<QVector2D> getTextures() const {return textures;}
    std::vector<QVector3D> getNormals() const {return normals;}

private:
    std::vector<QVector3D> vertices;
    std::vector<QVector2D> textures;
    std::vector<QVector3D> normals;
    char format;
};

#endif // MESH_H
