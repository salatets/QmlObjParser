#include <ImageLoader.h>
#include <fstream>
#include <iostream>

std::int32_t readInt32(const std::vector<std::uint8_t>& buffer, int index)
{
    return (std::int32_t )(
                ((std::uint8_t)buffer[index + 3] << 24) |
            ((std::uint8_t)buffer[index + 2] << 16) |
            ((std::uint8_t)buffer[index + 1] << 8) |
            ((std::uint8_t)buffer[index]));
}

std::uint32_t readUint32(const std::vector<std::uint8_t>& buffer, int index)
{
    return (std::uint32_t )(
                ((std::uint8_t)buffer[index + 3] << 24) |
            ((std::uint8_t)buffer[index + 2] << 16) |
            ((std::uint8_t)buffer[index + 1] << 8) |
            ((std::uint8_t)buffer[index]));
}

std::uint16_t readUint16(const std::vector<std::uint8_t>& buffer, int index)
{
    return (std::uint16_t )(
                ((std::uint8_t)buffer[index + 1] << 8) |
            ((std::uint8_t)buffer[index]));
}

Texture parseBMP(const std::string& path){
    std::ifstream hFile(path, std::ios::binary);

    Texture texture;
    texture.type = ImageType::Undefined;

    if (!hFile.is_open()){
        std::cerr << "image " << path << " could not be opened\n";
        return texture;
    }

    std::vector<std::uint8_t> FileInfo(BMP_HEADER_SIZE);
    hFile.read(reinterpret_cast<char*>(FileInfo.data()), BMP_HEADER_SIZE);

    if(FileInfo[0] != 'B' && FileInfo[1] != 'M')
    {
        hFile.close();
        std::cerr << path << "Invalid File Format. Bitmap Required\n";
        return texture;
    }

    if (FileInfo[28] != 24 && FileInfo[28] != 32)
    {
        hFile.close();
        std::cerr << path << "Invalid File Format. 24 or 32 bit Image Required\n";
        return texture;
    }

    texture.type = ImageType::BMP;
    texture.bitsPerPixel = FileInfo[28];
    texture.width = readInt32(FileInfo,18);
    texture.height = readInt32(FileInfo,22);
    std::uint32_t PixelsOffset = readInt32(FileInfo,10);
    std::uint32_t size = ((texture.width * texture.bitsPerPixel + 31) / 32) * 4 * texture.height;
    texture.pixels.resize(size);

    hFile.seekg (PixelsOffset, std::ios::beg);
    hFile.read(texture.pixels.data(), size);
    hFile.close();
    return texture;
}
