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

class MeshNodeLoaderVT : public MeshNodeLoader{
    Q_OBJECT
public:
    MeshNodeLoaderVT(
            const MeshNode& mesh,
            const std::string& path,
            QOpenGLShaderProgram* shad
            );
protected:
    void template_init_buffer() override;
    void template_paint() override;

    ~MeshNodeLoaderVT();
private:
    void LoadTexture(Texture texture);
    GLuint textureId;
};

class MeshNodeLoaderV : public MeshNodeLoader{
    Q_OBJECT
public:
    MeshNodeLoaderV(
            const MeshNode& mesh,
            const std::string& path,
            QOpenGLShaderProgram* shad
            );
protected:
    void template_init_buffer() override;
    void template_paint() override;

    ~MeshNodeLoaderV() = default;
};

#endif // MESHNODELOADERS_H
