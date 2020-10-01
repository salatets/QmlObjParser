#ifndef COMMON_H
#define COMMON_H
#include <vector>
#include <cstdint>
#include <QVector2D>
#include <QVector3D>

enum ImageType{
    BMP,
};

struct Texture{
    std::uint16_t bitsPerPixel;
    std::int32_t width;
    std::int32_t height;
    std::vector<std::uint8_t> pixels;
    ImageType type;
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


#endif // COMMON_H
