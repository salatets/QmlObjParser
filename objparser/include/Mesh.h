#ifndef MESH_H
#define MESH_H
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>
#include <ImageLoader.h>


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


class Mesh
{
public:
    Mesh(QOpenGLContext* m_context) : vertices(), uvs(), normals(), format('u'), center(0,0,0),size(0,0,0) {
        m_funcs = m_context->versionFunctions<QOpenGLFunctions_4_3_Core>();

          if (!m_funcs) {
            qWarning("Could not obtain OpenGL versions object");
            exit(1);
          }

          m_funcs->initializeOpenGLFunctions();
    };

    static char checkFaceFormat(std::basic_istream<char>& strm);

    bool parseOBJ(const std::string& path);
    char getFormat() const {return format;}
    const QVector3D& getCenter() const {return center;}
    const QVector3D& getSize() const {return size;}
    const std::vector<QVector3D>& getVertices() const {return vertices;}
    const std::vector<QVector2D>& getUvs() const {return uvs;}
    const std::vector<QVector3D>& getNormals() const {return normals;}

    //bool parseBMP(const std::string& path);

    bool parseMTL(const std::string& path, std::list<Mtl>& materials);

    const QVector3D& getSpecular() const {return specular;}

private:
    QOpenGLFunctions_4_3_Core* m_funcs;

    int giveGLType(ImageType type, std::uint16_t bitsPerPixel){
        switch (type) {
        case BMP:{
            if(bitsPerPixel == 24)
                return GL_BGR;
            else if(bitsPerPixel == 32)
                return GL_BGRA;
            else
                return -1;
            break;
        }
            return -1;

        }
    }

    unsigned int LoadTexture(Texture* texture){
        GLuint textureID;

        m_funcs->glBindTexture(GL_TEXTURE_2D, textureID);  //MAYBE crash
        m_funcs->glTexImage2D(GL_TEXTURE_2D, 0, texture->bitsPerPixel == 32 ? GL_RGBA : GL_RGB, texture->width, texture->height, 0, giveGLType(texture->type,texture->bitsPerPixel), GL_UNSIGNED_BYTE, texture->pixels.data());
        m_funcs->glGenerateMipmap(GL_TEXTURE_2D);

        m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        free(texture);
        return textureID;
    }

    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;

    std::vector<QVector3D> vertices;
    std::vector<QVector2D> uvs;
    std::vector<QVector3D> normals;

    char format;
    QVector3D center;
    QVector3D size;
};

#endif // MESH_H
