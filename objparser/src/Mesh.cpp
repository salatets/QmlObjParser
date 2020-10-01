#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include <MeshFactory.h>
#include <ImageLoader.h>
#include <Mesh.h>


/* Structure of parse wavefront .obj
 * mtllib headers
 * verticles data (v,vn,vt)
 * next parse mesh node
 * usemtl declaration
 * face data
 */

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


struct Indeces{
    std::vector<unsigned int> vertex;
    std::vector<unsigned int> normal;
    std::vector<unsigned int> uv;
};

struct Vertices{
    std::vector<QVector3D> vertices;
    std::vector<QVector3D> normals;
    std::vector<QVector2D> uvs;
};

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
    std::string token;
    std::vector<QVector3D> raw_vertexs;
    std::vector<QVector3D> raw_normals;
    std::vector<QVector2D> raw_uvs;

    // TODO forward list
    // TODO one struct list
    // meshes
    std::list<unsigned int> textureIds;
    std::list<Mtl> mesh_materials;
    std::list<Indeces> indeces;
    std::list<Vertices> vertices;
    std::list<char> formats;

    std::list<MeshNode> meshes;

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
            raw_uvs.emplace_back(x,y);
        }else if (token == "f"){
            if(formats.back() == 'u'){
                formats.back() = checkFaceFormat(fstrm);
                if (formats.back() == 'u'){
                    fstrm.close();
                    std::cerr << "could't parse faces\n";
                    return false;
                }
            }
            int index = 0;
            switch(formats.back()){
            case 'a':
                for(int i =0; i< 3; ++i){
                    fstrm >> index;
                    indeces.back().vertex.push_back(index);
                    fstrm.get();
                    fstrm >> index;
                    indeces.back().uv.push_back(index);
                    fstrm.get();
                    fstrm >> index;
                    indeces.back().normal.push_back(index);
                }
                break;
            case 'b':
                for(int i =0; i< 3; ++i){
                    fstrm >> index;
                    indeces.back().vertex.push_back(index);
                    fstrm.get();
                    fstrm.get();
                    fstrm >> index;
                    indeces.back().normal.push_back(index);
                }
                break;
            case 'c':
                for(int i =0; i< 3; ++i){
                    fstrm >> index;
                    indeces.back().vertex.push_back(index);
                    fstrm.get();
                    fstrm >> index;
                    indeces.back().uv.push_back(index);
                }
                break;
            case 'd':
                for(int i =0; i< 3; ++i){
                    fstrm >> index;
                    indeces.back().vertex.push_back(index);
                }
                break;
            }
        }else if(token == "usemtl"){
            std::string material_name;
            fstrm >> material_name;

            auto result = std::find_if(mesh_materials.begin(), mesh_materials.end(),
                                    [&material_name](const Mtl& mat) {return mat.name == material_name;});

            if(result == mesh_materials.end()){
                std::cerr << "material not found\n";
                return false;
            }

            // TODO add pwd var or?
            Texture* diffuse = parseBMP(path.substr(0, path.rfind('/') + 1) + (*result).diffuse_map_path);
            if(diffuse == nullptr)
                throw std::invalid_argument("not implement default texture");
            else{
                textureIds.push_back(LoadTexture(diffuse));
            }

            formats.emplace_back('u');
            indeces.emplace_back(Indeces());

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

    if(formats.size() != indeces.size()){
        std::cerr << "why is not correct size\n";
        return false;
    }

    // TODO refactor
    auto formatsIter = formats.begin();
    auto indecesIter = indeces.begin();
    auto verticesIter = vertices.begin();
    auto textureIter = textureIds.begin();
    auto materialIter = mesh_materials.begin();

    while(formatsIter != formats.end()){

        if(*formatsIter == 'u' || (*indecesIter).vertex.empty())
            return false;

        vertices.emplace_back(Vertices());

        //transform data
        for(std::vector<int>::size_type j = 0; j < (*indecesIter).vertex.size(); ++j){
            // Get the indices of its attributes
            unsigned int vertexIndex =(*indecesIter).vertex[j];
            // Put the attributes in buffers
            (*verticesIter).vertices.push_back(raw_vertexs[ vertexIndex-1 ]);

            switch(*formatsIter){
                case 'a':
                {
                    unsigned int normalIndex = (*indecesIter).normal[j];
                    (*verticesIter).normals.push_back(raw_normals[ normalIndex-1 ]);

                    unsigned int uvIndex = (*indecesIter).uv[j];
                    (*verticesIter).uvs.push_back(raw_uvs[ uvIndex-1 ]);
                    break;
                }
                case 'b':
                {
                    unsigned int normalIndex = (*indecesIter).normal[j];
                    (*verticesIter).normals.push_back(raw_normals[ normalIndex-1 ]);
                    break;
                }
                case 'c':{
                    unsigned int uvIndex = (*indecesIter).uv[j];
                    (*verticesIter).uvs.push_back(raw_uvs[ uvIndex-1 ]);
                    break;
                }
            }

        }

        meshes.push_back(MeshFactory::MakeMesh(m_funcs,
                               *materialIter,
                               *textureIter,
                               *formatsIter,
                               (*verticesIter).vertices,
                               (*verticesIter).normals,
                               (*verticesIter).uvs));

        ++formatsIter;
        ++indecesIter;
        ++verticesIter;
        ++textureIter;
        ++materialIter;

    }

    this->size = max-min;
    this->center = min + this->size/2;
    this->meshes = meshes;

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
            materials.emplace_back(Mtl());
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

void  Mesh::init_buffers(QOpenGLShaderProgram& shader){
    for(auto&& mesh : meshes){
        mesh.initBuffers(shader);
    }
}

void Mesh::draw(QOpenGLShaderProgram &shader){
    for(auto&& mesh : meshes){
        mesh.draw(shader);
    }
}


