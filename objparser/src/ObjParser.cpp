#include <sstream>
#include <fstream>
#include <iostream>

#include <ObjParser.h>
#include <MeshFactory.h>

/* Structure of parse wavefront .obj
 * mtllib headers
 * verticles data (v,vn,vt)
 * next parse mesh node
 * usemtl declaration
 * face data
 */

bool parseMTL(const std::string& path, std::list<Mtl>& materials){
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

QVector3D fillQVector3D(float&& val){
    return QVector3D(val,val,val);
}

// check format of faces
// possible formats
// a: v/t/n
// b: v//n
// c: v/t
// d: v
// u: undefined
char checkFaceFormat(std::basic_istream<char>& strm){
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

// TODO add move semantic
inline std::string getPWD(const std::string& path){
    return path.substr(0, path.rfind('/') + 1);
}

struct Indeces{
    std::vector<unsigned int> vertices;
    std::vector<unsigned int> normals;
    std::vector<unsigned int> UVs;
};

struct Vertices{
    std::vector<QVector3D> vertices;
    std::vector<QVector3D> normals;
    std::vector<QVector2D> UVs;
};

struct rawMesh{
    Mtl material;
    Indeces index;
    Vertices vertex;
    char format;
};

// v parse only 3 val
// todo multithread??
MeshRoot parseOBJ(const std::string& path){
    std::ifstream fstrm(path);
    if (!fstrm){
        std::cerr << "obj " << path << " could not be opened\n";
        return MeshRoot();
    }

    Vertices raw_data;
    // TODO forward list
    std::list<Mtl> mesh_materials;
    std::list<rawMesh> raw_mesh;
    QVector3D min = fillQVector3D(std::numeric_limits<float>::max());
    QVector3D max = fillQVector3D(std::numeric_limits<float>::lowest());

    std::string pwd = getPWD(path);
    std::string token;
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

            raw_data.vertices.emplace_back(x,y,z);
        }else if(token == "vn"){
            float x = 0,y = 0,z = 0;
            fstrm>>x>>y>>z;
            raw_data.normals.emplace_back(x,y,z);
        }else if (token == "vt"){
            float x = 0,y = 0;
            fstrm>>x>>y;
            raw_data.UVs.emplace_back(x,y);
        }else if (token == "f"){
            if(raw_mesh.back().format == 'u'){
                raw_mesh.back().format = checkFaceFormat(fstrm);
                if (raw_mesh.back().format == 'u'){
                    fstrm.close();
                    std::cerr << "could't parse faces\n";
                    return MeshRoot();
                }
            }
            int index = 0;
            switch(raw_mesh.back().format){ // get() skip slash
            case 'a':
                for(int i =0; i< 3; ++i){
                    fstrm >> index;
                    raw_mesh.back().index.vertices.push_back(index);
                    //std::cerr << "v " << index << ' ';
                    fstrm.get();
                    fstrm >> index;
                    raw_mesh.back().index.UVs.push_back(index);
                    //std::cerr << "t " << index << ' ';
                    fstrm.get();
                    fstrm >> index;
                    raw_mesh.back().index.normals.push_back(index);
                    //std::cerr << "n " << index << '\n';
                }
                break;
            case 'b':
                for(int i =0; i< 3; ++i){
                    fstrm >> index;
                    raw_mesh.back().index.vertices.push_back(index);
                    fstrm.get();
                    fstrm.get();
                    fstrm >> index;
                    raw_mesh.back().index.normals.push_back(index);
                }
                break;
            case 'c':
                for(int i =0; i< 3; ++i){
                    fstrm >> index;
                    raw_mesh.back().index.vertices.push_back(index);
                    fstrm.get();
                    fstrm >> index;
                    raw_mesh.back().index.UVs.push_back(index);
                }
                break;
            case 'd':
                for(int i =0; i< 3; ++i){
                    fstrm >> index;
                    raw_mesh.back().index.vertices.push_back(index);
                }
                break;
            }
        }else if(token == "usemtl"){
            std::string material_name;
            fstrm >> material_name;

            auto result = std::find_if(mesh_materials.begin(), mesh_materials.end(),
                                    [&material_name](const Mtl& material) {return material.name == material_name;});

            if(result == mesh_materials.end()){
                std::cerr << "material not found\n";
                return MeshRoot();
            }

            raw_mesh.emplace_back(rawMesh());
            raw_mesh.back().material = *result;
            raw_mesh.back().format = 'u';
            //raw_mesh.back().index = Indeces();

        }else if(token == "mtllib"){
            std::string filename;
            fstrm >> filename;

             if(!parseMTL(pwd + filename, mesh_materials)){
                fstrm.close();
                std::cerr << "could't parse mtl\n";
                return MeshRoot();
             }
        }


        fstrm.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    fstrm.close();

    std::list<MeshNode> meshes;
    auto meshIter = raw_mesh.begin();

    while(meshIter != raw_mesh.end()){

        if((*meshIter).format == 'u' || (*meshIter).index.vertices.empty())
            return MeshRoot();

        //vertices.emplace_back(Vertices());

        //transform data
        for(std::vector<int>::size_type j = 0; j < (*meshIter).index.vertices.size(); ++j){
            // Get the indices of its attributes
            unsigned int vertexIndex = (*meshIter).index.vertices[j];
            // Put the attributes in buffers
            (*meshIter).vertex.vertices.push_back(raw_data.vertices[ vertexIndex-1 ]);

            switch((*meshIter).format){
                case 'a':
                {
                    unsigned int normalIndex = (*meshIter).index.normals[j];
                    (*meshIter).vertex.normals.push_back(raw_data.normals[ normalIndex-1 ]);

                    unsigned int uvIndex = (*meshIter).index.UVs[j];
                    (*meshIter).vertex.UVs.push_back(raw_data.UVs[ uvIndex-1 ]);
                    break;
                }
                case 'b':
                {
                    unsigned int normalIndex = (*meshIter).index.normals[j];
                    (*meshIter).vertex.normals.push_back(raw_data.normals[ normalIndex-1 ]);
                    break;
                }
                case 'c':{
                    unsigned int uvIndex = (*meshIter).index.UVs[j];
                    (*meshIter).vertex.UVs.push_back(raw_data.UVs[ uvIndex-1 ]);
                    break;
                }
            }

        }

        meshes.emplace_back(MeshFactory::MakeMesh(
                               (*meshIter).material,
                               (*meshIter).format,
                               (*meshIter).vertex.vertices,
                               (*meshIter).vertex.normals,
                               (*meshIter).vertex.UVs));
        ++meshIter;
    }

    QVector3D size = max-min;

    return MeshRoot(min + size/2, size,meshes.begin(),meshes.end());
}
