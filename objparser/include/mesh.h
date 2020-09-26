#ifndef MESH_H
#define MESH_H
#include <QVector2D>
#include <QVector3D>


class Mesh
{
public:
    Mesh() : vertices(), textures(), normals(), format('u'), center(0,0,0),size(0,0,0) {};

    static char checkFaceFormat(std::basic_istream<char>& strm);

    bool parseOBJ(const std::string& path);

    char getFormat() const {return format;}
    QVector3D getCenter() const {return center;}
    QVector3D getSize() const {return size;}
    std::vector<QVector3D> getVertices() const {return vertices;}
    std::vector<QVector2D> getTextures() const {return textures;}
    std::vector<QVector3D> getNormals() const {return normals;}

private:
    std::vector<QVector3D> vertices;
    std::vector<QVector2D> textures;
    std::vector<QVector3D> normals;
    char format;
    QVector3D center;
    QVector3D size;
};

#endif // MESH_H
