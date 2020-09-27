#include <iostream>
#include <fstream>
#include <sstream>

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
    std::ifstream fstrm;
    fstrm.open(path);

    if (!fstrm){
        std::cerr << path << " could not be opened\n";
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
