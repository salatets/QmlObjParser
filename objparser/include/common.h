#ifndef COMMON_H
#define COMMON_H
#include <vector>
#include <cstdint>

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


#endif // COMMON_H
