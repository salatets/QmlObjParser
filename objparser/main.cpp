#include <iostream>
#include <fstream>
#include <sstream>
#include <QVector2D>
#include <QVector3D>
#include "mesh.h"

// v parse only 3 val


int main()
{
    std::vector<QVector3D> vertices;
    std::vector<QVector2D> textures;
    std::vector<QVector3D> normals;
    char format;
    std::cout << Mesh::parseOBJ("cube.obj", vertices,textures,normals,format) << " " << format << '\n';
    return 0;



    std::string filename;
    std::cin >> filename;

    std::ifstream fstrm;
    fstrm.open(filename);

    if (!fstrm)
        throw std::invalid_argument(filename + " could not be opened");

    std::vector<QVector3D> raw_vertexs;
    std::vector<QVector3D> raw_normals;
    std::vector<QVector2D> raw_textures;

    for (std::string line; std::getline(fstrm, line);)
    {
        std::string token;
        std::istringstream stream(line);
        stream >> token;

        if(token == "v"){
            double x,y,z;
            stream>>x>>y>>z;
            raw_vertexs.push_back(QVector3D(x,y,z));
        }else if(token == "vn"){
            double x,y,z;
            stream>>x>>y>>z;
            raw_normals.push_back(QVector3D(x,y,z));
        }else if (token == "vt"){
            double x,y;
            stream>>x>>y;
            raw_textures.push_back(QVector2D(x,y));
        }else if (token == "f"){

            int vertexIndexs[3];
            int textureIndexs[3];
            int normalIndexs[3];
            char format = 'n';
            std::string el;
            stream >> el;
            std::istringstream elem(el);

            stream >> vertexIndexs[0];

            if(elem.peek() == '/'){
                elem.get();
                if(elem.peek() == '/'){
                    elem.get();
                    elem >> normalIndexs[0];
                    format = 'b';
                }else{
                    elem >> textureIndexs[0];
                    if(elem.peek() == '/'){
                        elem.get();
                        elem >> normalIndexs[0];
                        format = 'a';
                    }
                    else{
                        format = 'c';
                    }
                }
            }else
                format = 'd';

            std::cout << format << "/n";




        }
    }

    return 0;
}
