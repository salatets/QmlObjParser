#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <ObjParser.h>
#include <vector>

struct Scene{
    std::vector<std::pair<Vec3,MeshRoot>> meshes;
    //std::vector<std::pair<Vec3,Light>>
};

Scene ParseScene(const std::string& path);

#endif // SCENEPARSER_H
