#ifndef MESHNODELOADERS_H
#define MESHNODELOADERS_H

#include "MeshNodeLoader.h"

class MeshNodeLoaderVNT : public MeshNodeLoader{
    Q_OBJECT
public:
    MeshNodeLoaderVNT(
            const MeshNode& mesh,
            const std::string& path,
            QOpenGLShaderProgram* shad
            );
protected:
    void template_init_buffer() override;
    void template_paint() override;

    ~MeshNodeLoaderVNT();

private:
    void LoadTexture(Texture texture);
    GLuint textureId;
};

class MeshNodeLoaderVN : public MeshNodeLoader{
    Q_OBJECT
public:
    MeshNodeLoaderVN(
            const MeshNode& mesh,
            const std::string& path,
            QOpenGLShaderProgram* shad
            );
protected:
    void template_init_buffer() override;
    void template_paint() override;

    ~MeshNodeLoaderVN() = default;
};

#endif // MESHNODELOADERS_H