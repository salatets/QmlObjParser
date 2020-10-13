#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <vector>
#include <cstdint>
#include <string>

enum ImageType{
    BMP,
    Undefined
};

struct Texture{
    std::uint16_t bitsPerPixel;
    std::int32_t width;
    std::int32_t height;
    std::vector<std::uint8_t> pixels;
    ImageType type;
};

constexpr std::size_t BMP_HEADER_SIZE = 54;

std::int32_t readInt32(const std::vector<std::uint8_t>& buffer, int index);
std::uint32_t readUint32(const std::vector<std::uint8_t>& buffer, int index);
std::uint16_t readUint16(const std::vector<std::uint8_t>& buffer, int index);

Texture parseBMP(const std::string& path);

#endif // IMAGELOADER_H
