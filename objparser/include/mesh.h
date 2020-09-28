#ifndef MESH_H
#define MESH_H
#include <QVector2D>
#include <QVector3D>

struct Texture{
    std::uint16_t bitsPerPixel;
    std::int32_t width;
    std::int32_t height;
    std::vector<std::uint8_t> pixels;
};

enum illum{
    AMBIENT_ON = 1,
    HIGHTLIGHT_ON = 2
};

struct Mtl{
    std::string name;
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    illum illum_mode;
    std::string diffuse_map_path;
    Mtl() : name(""), ambient(0,0,0), diffuse(0,0,0), specular(0,0,0),
        illum_mode(AMBIENT_ON), diffuse_map_path("") {};
};

class Mesh
{
public:
    Mesh() : vertices(), textures(), normals(), format('u'), center(0,0,0),size(0,0,0) {};

    static char checkFaceFormat(std::basic_istream<char>& strm);

    bool parseOBJ(const std::string& path);
    char getFormat() const {return format;}
    const QVector3D& getCenter() const {return center;}
    const QVector3D& getSize() const {return size;}
    const std::vector<QVector3D>& getVertices() const {return vertices;}
    const std::vector<QVector2D>& getTextures() const {return textures;}
    const std::vector<QVector3D>& getNormals() const {return normals;}

    bool parseBMP(const std::string& path);
    const Texture& getTexture() const {return texture;}

    bool parseMTL(const std::string& path, std::list<Mtl>& materials);

    const QVector3D& getSpecular() const {return specular;}

private:
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;

    Texture texture;
    std::vector<QVector3D> vertices;
    std::vector<QVector2D> textures;
    std::vector<QVector3D> normals;

    char format;
    QVector3D center;
    QVector3D size;
};

#endif // MESH_H
