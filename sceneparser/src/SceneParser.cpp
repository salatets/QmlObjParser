#include <SceneParser.h>
#include <fstream>
#include <iostream>
#include <string>
#include <limits>

// TODO add move semantic
inline std::string getPWD(const std::string& path){
    return path.substr(0, path.rfind('/') + 1);
}


/*
 * currently parse mesh scale(x y z) translate(x y z) name
 * mesh = objfile
 */
Scene ParseScene(const std::string& path){
    std::ifstream fstrm(path, std::ios::binary);
    if(!fstrm){
        std::cerr << "scene " << path << " could not be opened\n";
        return Scene();
    }

    Scene scene;
    std::string pwd = getPWD(path); // dubplicant from ObjParser
    std::string token;
    while (fstrm.peek() != -1){
       fstrm >> token;

       if(token == "mesh"){
            float x = 0,y= 0,z = 0;
            fstrm>>x>>y>>z;
            Vec3 scale(x,y,z);

            fstrm>>x>>y>>z;
            Vec3 translate(x,y,z);

            fstrm >> token;
            if(token.empty()){
                std::cerr << "mesh in scene " << " could not specified\n";
                return Scene();
            }

            if(token.back() == '\n')
                token = token.substr(0, path.rfind('\n'));

            MeshRoot mesh = parseOBJ(pwd + token);
            scene.meshes.emplace_back(scale,translate,mesh);
       }
       fstrm.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    fstrm.close();

    return scene;
}
