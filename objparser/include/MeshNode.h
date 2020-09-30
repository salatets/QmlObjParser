#ifndef MESHNODE_H
#define MESHNODE_H

#include <QVector2D>
#include <QVector3D>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>

#include <common.h>

enum illum{
    AMBIENT_ON = 1,
    HIGHTLIGHT_ON = 2
};

struct Mtl{
    std::string name;
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    illum illum_mode;
    std::string diffuse_map_path;
    Mtl() : name(""), ambient(0,0,0), diffuse(0,0,0), specular(0,0,0),
        illum_mode(AMBIENT_ON), diffuse_map_path("") {};
};

class MeshNode
{
    void Draw();
};

struct DataVNT{
    QVector3D vertex;
    QVector3D normal;
    QVector2D uv;
};

class MeshNodeVNT : public MeshNode {
public:
    MeshNodeVNT(unsigned int textureId, QOpenGLShaderProgram* shader,
                const std::vector<QVector3D>& vertexs,
                const std::vector<QVector3D>& normals,
                const std::vector<QVector2D>& uvs);

    ~MeshNodeVNT();

    void setShader(QOpenGLShaderProgram* shader) {this->shader = shader;}

    void Draw();

private:
    unsigned int textureId;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QOpenGLShaderProgram* shader;

    Texture* diffuse;
    DataVNT* vertex_data;

};

#endif // MESHNODE_H
