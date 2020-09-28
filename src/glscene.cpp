#include <QtQuick/qquickwindow.h>
#include <QtCore/QRunnable>
#include <QtMath>

#include "glscene.h"
#include "shaders.h"

float max(const QVector3D& vec){
    float max = vec.x();

    if(vec.y() > max)
        max = vec.y();
    if(vec.z() > max)
        max = vec.z();

    return max;
}

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

void GLScene::setPos(qreal pos)
{
    if (pos == m_pos)
        return;
    m_pos= pos;
    emit posChanged();
    if (window())
        window()->update();
}

void GLScene::setPath(QUrl path)
{
    if (path == m_path)
        return;
    m_path= path;
    emit pathChanged();
}

GLuint texture = 0;

void GLSceneRenderer::setPath(QUrl path)
{
    if(path == old_url)
        return;

    if(path.isRelative()){
        QString temp = path.toString();
        m_path = temp.toUtf8().constData();
    }else{
        int trunc_len = path.scheme().length() + 1;
        QString temp = path.toString();
        int i = 0;

        for(; i < 3; i++)
            if(temp[trunc_len] != '/')
                break;

        if(i > 1)
            trunc_len +=2;


        m_path = temp.toUtf8().constData();
        m_path = m_path.substr(trunc_len);
    }
    if(m_model.parseOBJ(m_path)){
        init_buffers();


        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D,texture);
        Texture text = m_model.getTexture();

        glTexImage2D(GL_TEXTURE_2D, 0, text.bitsPerPixel == 32 ? GL_RGBA : GL_RGB,
                     text.width, text.height, 0, text.bitsPerPixel == 32 ? GL_BGRA : GL_BGR,
                     GL_UNSIGNED_BYTE, text.pixels.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        old_url = path;
        m_window->update();
    }
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
    m_renderer->setPos(m_pos);
    m_renderer->setPath(m_path);
    m_renderer->setWindow(window());
}

// TODO add different format buffers

void GLSceneRenderer::init_program(){
    m_program = new QOpenGLShaderProgram();
    bool success = m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertex);
    success &= m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragment);
    Q_ASSERT(success);

    success = m_program->link();
    Q_ASSERT(success);
}

void init_buffer(QOpenGLBuffer &bo, QOpenGLShaderProgram* program, const char * attribute , const void *data, int count,int stride){
    if (!bo.isCreated()){
        bo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        bool success = bo.create();
        Q_ASSERT(success);
        bo.setUsagePattern(QOpenGLBuffer::StaticDraw);


    }

    bool success = bo.bind();
    Q_ASSERT(success);
    bo.allocate(data, count);
    program->setAttributeBuffer(attribute, GL_FLOAT, 0, stride);
    program->enableAttributeArray(attribute);

    bo.release();
}

void GLSceneRenderer::init_buffers(){
    vao.bind();

    init_buffer(vbo, m_program,"position", &m_model.getVertices().data()[0], m_model.getVertices().size() * sizeof(QVector3D),3);

    init_buffer(nbo, m_program,"normal", &m_model.getNormals().data()[0], m_model.getNormals().size() * sizeof(QVector3D),3);

    init_buffer(tbo, m_program,"texCoords", &m_model.getTextures().data()[0], m_model.getTextures().size() * sizeof(QVector2D),2);

    vao.release();
}

void GLSceneRenderer::init()
{
    if (!m_program) {
        QSGRendererInterface *rif = m_window->rendererInterface();
        Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL || rif->graphicsApi() == QSGRendererInterface::OpenGLRhi);

        initializeOpenGLFunctions();

        init_program();

        bool success = vao.create();
        Q_ASSERT(success);

        if (m_model.getFormat() == 'u')
            return;

        init_buffers();
    }
}
// FIXME light shader render after light pos move
void GLSceneRenderer::paint()
{
    if(m_model.getFormat() == 'u')
        return;

    m_window->beginExternalCommands();

    m_program->bind();
    vao.bind();

    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

    glEnable(GL_DEPTH_TEST);

    QMatrix4x4 mat;
    mat.scale(1/(max(m_model.getSize()))); // TODO fix clip bug
    mat.translate(- m_model.getCenter());
    // TODO yaw bug
    mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), m_pitch));
    mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), m_yaw));
    mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,0,1), m_roll));
    m_program->setUniformValue("model",mat);

    m_program->setUniformValue("lightColor", QVector3D(1.0f, 0.0f, 1.0f));
    m_program->setUniformValue("objectColor", QVector3D(1.0f, 0.5f, 0.31f));
    m_program->setUniformValue("lightPos", QVector3D(sin(m_pos), 0.3f, cos(m_pos)));
    m_program->setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 0.0f));

    m_program->setUniformValue("material.shininess", 64.0f);

    m_program->setUniformValue("material.specular",m_model.getSpecular());


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);


    glDrawArrays(GL_TRIANGLES, 0, m_model.getVertices().size());

    vao.release();
    m_program->release();

    m_window->resetOpenGLState();
    m_window->endExternalCommands();
}


