#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <vector>
#include <common.h>

constexpr std::size_t BMP_HEADER_SIZE = 54;

std::int32_t readInt32(const std::vector<std::uint8_t>& buffer, int index);
std::uint32_t readUint32(const std::vector<std::uint8_t>& buffer, int index);
std::uint16_t readUint16(const std::vector<std::uint8_t>& buffer, int index);

Texture* parseBMP(const std::string& path);

#endif // IMAGELOADER_H
