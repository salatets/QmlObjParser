#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "mesh.h"

char Mesh::checkFaceFormat(std::basic_istream<char>& strm){
    int initPos = strm.tellg();
    int index = 0;
    char format = 'u';
    std::string expr;
    strm >> expr;
    std::istringstream stream(expr);
    stream >> index;
    if(index == 0){
        format = 'u';
    }
    else{

        if(stream.peek() == '/'){
            stream.get();
            if(stream.peek() == '/'){
                stream.get();
                if(stream.peek() != -1){
                stream >> index;
                format = 'b';
                }
            }else{
                if(stream.peek() != -1){
                    stream >> index;
                    if(stream.peek() == '/'){
                        stream.get();
                        if(stream.peek() != -1){
                            stream >> index;
                            format = 'a';
                        }
                    }
                    else{
                        format = 'c';
                    }
                }
            }
        }else{
            format = 'd';
        }

        if(stream.peek() != -1){
            format = 'u';
        }
    }

    strm.seekg(initPos);
    return format;
}




// v parse only 3 val
// todo multithread??
bool Mesh::parseOBJ(const std::string& path){
    std::ifstream fstrm(path);

    if (!fstrm){
        std::cerr << "obj " << path << " could not be opened\n";
        return false;
    }

    // format of faces
    // possible formats
    // a: v/t/n
    // b: v//n
    // c: v/t
    // d: v
    // u: undefined
    char format ='u';
    std::string token;
    std::vector<QVector3D> raw_vertexs;
    std::vector<QVector3D> raw_normals;
    std::vector<QVector2D> raw_textures;
    // indeces is not negative
    std::vector<unsigned int> vertexIndeces;
    std::vector<unsigned int> normalIndeces;
    std::vector<unsigned int> textureIndeces;
    // for center and size
    QVector3D min(std::numeric_limits<float>::max(),
                  std::numeric_limits<float>::max(),
                  std::numeric_limits<float>::max());
    QVector3D max(std::numeric_limits<float>::lowest(),
                  std::numeric_limits<float>::lowest(),
                  std::numeric_limits<float>::lowest());

    std::list<Mtl> mesh_materials;


    //parse file
    while (fstrm.peek() != -1){
        fstrm >> token;

        if(token == "v"){
            float x = 0,y= 0,z = 0;
            fstrm>>x>>y>>z;

            if(x > max.x())
                max.setX(x);
            if(y > max.y())
                max.setY(y);
            if(z > max.z())
                max.setZ(z);

            if(x < min.x())
                min.setX(x);
            if(y < min.y())
                min.setY(y);
            if(z < min.z())
                min.setZ(z);

            raw_vertexs.emplace_back(x,y,z);
        }else if(token == "vn"){
            float x = 0,y = 0,z = 0;
            fstrm>>x>>y>>z;
            raw_normals.emplace_back(x,y,z);
        }else if (token == "vt"){
            float x = 0,y = 0;
            fstrm>>x>>y;
            raw_textures.emplace_back(x,y);
        }else if (token == "f"){
            if(format == 'u'){
                format = checkFaceFormat(fstrm);
                if (format == 'u'){
                    fstrm.close();
                    std::cerr << "could't parse faces\n";
                    return false;
                }
            }
            int index = 0;
            switch(format){
            case 'a':
                for(int i =0; i< 3; ++i){
                    fstrm >> index;
                    vertexIndeces.push_back(index);
                    fstrm.get();
                    fstrm >> index;
                    textureIndeces.push_back(index);
                    fstrm.get();
                    fstrm >> index;
                    normalIndeces.push_back(index);
                }
                break;
            case 'b':
                for(int i =0; i< 3; ++i){
                    fstrm >> index;
                    vertexIndeces.push_back(index);
                    fstrm.get();
                    fstrm.get();
                    fstrm >> index;
                    normalIndeces.push_back(index);
                }
                break;
            case 'c':
                for(int i =0; i< 3; ++i){
                    fstrm >> index;
                    vertexIndeces.push_back(index);
                    fstrm.get();
                    fstrm >> index;
                    textureIndeces.push_back(index);
                }
                break;
            case 'd':
                for(int i =0; i< 3; ++i){
                    fstrm >> index;
                    vertexIndeces.push_back(index);
                }
                break;
            }
        }else if(token == "usemtl"){
            std::string material_name;
            fstrm >> material_name;

            auto result = std::find_if(mesh_materials.begin(), mesh_materials.end(),
                                    [&material_name](const Mtl& mat) {return mat.name == material_name;}
            );
            // TODO add pwd var or?
            // TODO add false check
            parseBMP(path.substr(0, path.rfind('/') + 1) + (*result).diffuse_map_path);

        }else if(token == "mtllib"){
            std::string filename;
            fstrm >> filename;

             if(!parseMTL(path.substr(0, path.rfind('/') + 1) + filename, mesh_materials)){
                fstrm.close();
                std::cerr << "could't parse mtl\n";
                return false;
             }
        }


        fstrm.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    fstrm.close();

    if(format == 'u' || vertexIndeces.size() == 0)
        return false;

    vertices.clear();
    normals.clear();
    textures.clear();

    this->format = format;

    ambient = mesh_materials.back().ambient;
    diffuse = mesh_materials.back().diffuse;
    specular = mesh_materials.back().specular;

    vertices.reserve(vertexIndeces.size());
    switch(format){
        case 'a':
        {
            normals.reserve(normalIndeces.size());
            textures.reserve(textureIndeces.size());
            break;
        }
        case 'b':
        {
            normals.reserve(normalIndeces.size());
            break;
        }
        case 'c':{
            textures.reserve(textureIndeces.size());
            break;
        }
    }

    //transform data
    for(std::vector<int>::size_type i = 0; i < vertexIndeces.size(); ++i){
        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndeces[i];
        // Put the attributes in buffers
        vertices.emplace_back(raw_vertexs[ vertexIndex-1 ]);

        switch(format){
            case 'a':
            {
                unsigned int normalIndex = normalIndeces[i];
                normals.emplace_back(raw_normals[ normalIndex-1 ]);

                unsigned int uvIndex = textureIndeces[i];
                textures.emplace_back(raw_textures[ uvIndex-1 ]);
                break;
            }
            case 'b':
            {
                unsigned int normalIndex = normalIndeces[i];
                normals.emplace_back(raw_normals[ normalIndex-1 ]);
                break;
            }
            case 'c':{
                unsigned int uvIndex = textureIndeces[i];
                textures.emplace_back(raw_textures[ uvIndex-1 ]);
                break;
            }
        }

    }

    this->size = max-min;
    this->center = min + this->size/2;

    return true;
}

bool Mesh::parseMTL(const std::string& path, std::list<Mtl>& materials){
    std::ifstream fstrm(path);

    if (!fstrm){
        std::cerr << "mtl " << path << " could not be opened\n";
        return false;
    }

    std::string token;
    bool added_new = false;

    while (fstrm.peek() != -1){
        fstrm >> token;

        if(token == "newmtl"){
            materials.push_back(Mtl());
            added_new = true;
            fstrm >> token;
            materials.back().name = token;
        }else if(added_new){
            if(token == "Ka"){
                float x,y,z;
                fstrm >> x >> y >>z;
                materials.back().ambient = QVector3D(x,y,z);
            }else if(token == "Kd"){
                float x,y,z;
                fstrm >> x >> y >>z;
                materials.back().diffuse = QVector3D(x,y,z);
            }else if(token == "Ks"){
                float x,y,z;
                fstrm >> x >> y >>z;
                materials.back().specular = QVector3D(x,y,z);
            }else if(token == "illum"){
                int x;
                fstrm >> x;
                if(x > 0 && x < 3)
                    materials.back().illum_mode = illum(x);
                else
                    materials.back().illum_mode = illum();
            }else if(token == "map_Kd"){
                fstrm >> token;
                materials.back().diffuse_map_path = token;
            }
        }

        fstrm.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return true;
}


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

constexpr std::size_t HEADER_SIZE = 54;

bool Mesh::parseBMP(const std::string& path){
    std::ifstream hFile(path, std::ios::binary);
    if (!hFile.is_open()){
        std::cerr << "image " << path << " could not be opened\n";
        return false;
    }

    std::vector<std::uint8_t> FileInfo(HEADER_SIZE);
    hFile.read(reinterpret_cast<char*>(FileInfo.data()), HEADER_SIZE);

    if(FileInfo[0] != 'B' && FileInfo[1] != 'M')
    {
        hFile.close();
        std::cerr << path << "Invalid File Format. Bitmap Required\n";
        return false;
    }

    if (FileInfo[28] != 24 && FileInfo[28] != 32)
    {
        hFile.close();
        std::cerr << path << "Invalid File Format. 24 or 32 bit Image Required\n";
        return false;
    }

    texture.bitsPerPixel = FileInfo[28];
    texture.width = readInt32(FileInfo,18);
    texture.height = readInt32(FileInfo,22);
    std::uint32_t PixelsOffset = readInt32(FileInfo,10);
    std::uint32_t size = ((texture.width * texture.bitsPerPixel + 31) / 32) * 4 * texture.height;
    texture.pixels.resize(size);

    hFile.seekg (PixelsOffset, std::ios::beg);
    hFile.read(reinterpret_cast<char*>(texture.pixels.data()), size);
    hFile.close();
    return true;
}
