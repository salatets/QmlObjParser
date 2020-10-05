#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <Mesh.h>
#include <ImageLoader.h>
#include <QOpenGLContext>
#include "doctest.h"

TEST_CASE("load MTL"){
    std::list<Mtl> materials;
    REQUIRE(Mesh::parseMTL("cube.mtl",materials) == true);
    CHECK(materials.size() == 1);
    CHECK(materials.back().ambient == QVector3D(1,1,1));
    CHECK(materials.back().diffuse == QVector3D(0.8,0.8,0.8));
    CHECK(materials.back().specular == QVector3D(0.5,0.5,0.5));
    CHECK(materials.back().diffuse_map_path == "cube.bmp");
    CHECK(materials.back().illum_mode == illum::HIGHTLIGHT_ON);

}

TEST_CASE("load BMP"){
    Texture* text = parseBMP("cube.bmp");

    REQUIRE(text != nullptr);
    CHECK(text->bitsPerPixel == 24);
    CHECK(text->height == 128);
    CHECK(text->width == 128);
    //CHECK(text->type = ImageType::BMP);

}


TEST_CASE("load cube"){
//    QOpenGLContext* m_context;

//    // Специфицируем формат и создаем платфоро-зависимый сюрфейс
//    QSurfaceFormat format;
//    format.setDepthBufferSize(24);
//    format.setRenderableType(QSurfaceFormat::OpenGL);
//    format.setMajorVersion(4);
//    format.setMinorVersion(3);
//    format.setSamples(4);
//    format.setProfile(QSurfaceFormat::CoreProfile);

//    // Создаем OpenGL контекст
//    m_context = new QOpenGLContext;
//    m_context->setFormat(format);
//    m_context->create();

    // Сделаем контекст текущим для этого окна

    Mesh test(nullptr);
    REQUIRE(test.parseOBJ("cube.obj") == true);

}
