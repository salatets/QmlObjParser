#include "glscene.h"

#include <QtQuick/qquickwindow.h>
#include <QtCore/QRunnable>
#include "shaders.h"

GLScene::GLScene() : m_pitch(0)
  , m_yaw(0)
  , m_roll(0)
  , m_renderer(nullptr){
    connect(this, &QQuickItem::windowChanged, this, &GLScene::handleWindowChanged);
}

void GLScene::setPitch(qreal pitch)
{
    if (pitch == m_pitch)
        return;
    m_pitch= pitch;
    emit pitchChanged();
    if (window())
        window()->update();
}

void GLScene::setYaw(qreal yaw)
{
    if (yaw == m_yaw)
        return;
    m_yaw= yaw;
    emit yawChanged();
    if (window())
        window()->update();
}

void GLScene::setRoll(qreal roll)
{
    if (roll == m_roll)
        return;
    m_roll= roll;
    emit rollChanged();
    if (window())
        window()->update();
}

void GLScene::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &GLScene::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &GLScene::cleanup, Qt::DirectConnection);

        win->setColor(Qt::black);
    }
}

void GLScene::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}

class CleanupJob : public QRunnable
{
public:
    CleanupJob(GLSceneRenderer *renderer) : m_renderer(renderer) { }
    void run() override { delete m_renderer; }
private:
    GLSceneRenderer *m_renderer;
};

void GLScene::releaseResources()
{
    window()->scheduleRenderJob(new CleanupJob(m_renderer), QQuickWindow::BeforeSynchronizingStage);
    m_renderer = nullptr;
}

GLSceneRenderer::~GLSceneRenderer()
{
    delete m_program;
}

void GLScene::sync()
{
    if (!m_renderer) {
        m_renderer = new GLSceneRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &GLSceneRenderer::init, Qt::DirectConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &GLSceneRenderer::paint, Qt::DirectConnection);
    }

    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setPitch(m_pitch);
    m_renderer->setYaw(m_yaw);
    m_renderer->setRoll(m_roll);
    m_renderer->setWindow(window());
}
Mesh classC;
void GLSceneRenderer::init()
{
    if (!m_program) {
        QSGRendererInterface *rif = m_window->rendererInterface();
        Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL || rif->graphicsApi() == QSGRendererInterface::OpenGLRhi);


        classC.parseOBJ("monkey.obj");

        initializeOpenGLFunctions();

        bool success = vao.create();
        Q_ASSERT(success);
        vao.bind();

        m_program = new QOpenGLShaderProgram();
        success = m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertex);
        success &= m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragment);
        Q_ASSERT(success);

        success = m_program->link();
        Q_ASSERT(success);
        m_program->bind();

        vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        success = vbo.create();
        Q_ASSERT(success);
        vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
        success = vbo.bind();
        Q_ASSERT(success);
        vbo.allocate(&classC.getVertices().data()[0], classC.getVertices().size() * sizeof(QVector3D));
        m_program->setAttributeBuffer("position", GL_FLOAT, 0,3);
        m_program->enableAttributeArray("position");
        vbo.release();

        nbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        success = nbo.create();
        Q_ASSERT(success);
        nbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
        success = nbo.bind();
        Q_ASSERT(success);
        nbo.allocate(&classC.getNormals().data()[0], classC.getNormals().size() * sizeof(QVector3D));
        m_program->setAttributeBuffer("normal", GL_FLOAT, 0,3);
        m_program->enableAttributeArray("normal");
        nbo.release();

        vao.release();
        m_program->release();
    }
}

void GLSceneRenderer::paint()
{
    m_window->beginExternalCommands();

    m_program->bind();

    //m_program->enableAttributeArray(0);
    vao.bind();

    //m_program->setAttributeArray(0, GL_FLOAT, values, 2);
    //m_program->setUniformValue("t", (float) m_t);

    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

    glEnable(GL_DEPTH_TEST);

    QVector3D viewPos(0.0f, 0.0f, 0.0f);

    QMatrix4x4 mat;
    //mat.scale(2);
    //mat.translate(0,0,10);
    mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), m_pitch));
    mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), m_yaw));
    mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,0,1), m_roll));
    m_program->setUniformValue("model",mat);



    QMatrix4x4 view;
    view.translate(viewPos);
    m_program->setUniformValue("view",view);

    m_program->setUniformValue("lightColor", QVector3D(1.0f, 0.0f, 1.0f));
    m_program->setUniformValue("objectColor", QVector3D(1.0f, 0.5f, 0.31f));
    m_program->setUniformValue("lightPos", QVector3D(0.5f, .3f, -.3f));
    m_program->setUniformValue("viewPos", viewPos);

    glDrawArrays(GL_TRIANGLES, 0, classC.getVertices().size());

    vao.release();
    m_program->release();

    m_window->resetOpenGLState();
    m_window->endExternalCommands();
}


