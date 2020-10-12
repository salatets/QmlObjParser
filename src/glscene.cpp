#include <QtQuick/qquickwindow.h>
#include <QtCore/QRunnable>
#include <QtMath>

#include <glscene.h>
#include<ObjParser.h>

#include "shaders.h"

float max(const QVector3D& vec){
    float max = vec.x();

    if(vec.y() > max)
        max = vec.y();
    if(vec.z() > max)
        max = vec.z();

    return max;
}

GLScene::GLScene() : m_renderer(nullptr){
    connect(this, &QQuickItem::windowChanged, this, &GLScene::handleWindowChanged);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);
}

void roundTo(QPoint& val, int to){
    if(val.x() > to)
        val.rx() -= to;
    if(val.x() < 0)
        val.rx() += to;

    if(val.y() > to)
        val.ry() -= to;
    if(val.y() < 0)
        val.ry() += to;
}

// TODO add not integer angles
QPoint GLScene::mouseToAngle(){
     QPoint pos{m_prev + m_start - m_current};
     roundTo(pos, 360);

     return pos;
}

void GLScene::mousePressEvent(QMouseEvent* event)
    {
        m_start = event->pos(); // MAYBE {{}};

        event->accept();
    }

void GLScene::mouseMoveEvent(QMouseEvent* event)
    {
        m_current = event->pos();

        event->accept();

        if (window())
            window()->update();
    }

void GLScene::mouseReleaseEvent(QMouseEvent* event)
    {
       m_current = event->pos();

       m_prev += m_start - m_current;

       m_start = {0,0};
       m_current = {0,0};

       roundTo(m_prev, 360);

       event->accept();

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

    m_model = parseOBJ(m_path);

    if(m_model.size() != 0){

        fh.setMesh(m_model);
        fh.init_buffers(m_program);

        old_url = path;
        m_window->update();
    }
}

void GLScene::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &GLScene::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &GLScene::cleanup, Qt::DirectConnection);

        win->setColor(Qt::gray);
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

GLSceneRenderer::~GLSceneRenderer(){
    delete m_program;
}

void GLScene::sync(){
    if (!m_renderer) {
        m_renderer = new GLSceneRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &GLSceneRenderer::init, Qt::DirectConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &GLSceneRenderer::paint, Qt::DirectConnection);
    }

    QPoint angles = mouseToAngle();

    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setPitch(angles.y());
    m_renderer->setYaw(angles.x());
    m_renderer->setPos(m_pos);
    m_renderer->setPath(m_path);
    m_renderer->setWindow(window());
}

void GLSceneRenderer::init_program(){
    m_program = new QOpenGLShaderProgram();
    bool success = m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, hor_vertex);
    success &= m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, hor_fragment);
    Q_ASSERT(success);

    success = m_program->link();
    Q_ASSERT(success);
}

void GLSceneRenderer::init_buffers(){
    fh.init_buffers(m_program);
}

void GLSceneRenderer::init(){
    if (!m_program) {
        QSGRendererInterface *rif = m_window->rendererInterface();
        Q_ASSERT(rif->graphicsApi() == QSGRendererInterface::OpenGL || rif->graphicsApi() == QSGRendererInterface::OpenGLRhi);

        initializeOpenGLFunctions();

        init_program();

        if (m_model.empty())
            return;

        init_buffers();
    }
}

// FIXME light shader render after light pos move
void GLSceneRenderer::paint(){
    if(m_model.empty())
        return;

    m_window->beginExternalCommands();

    QMatrix4x4 mat;
    mat.scale(1/(max(m_model.getSize())));
    mat.translate(- m_model.getCenter());
    mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), m_pitch));
    mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), m_yaw));

    // TODO size of points
    fh.paint(m_program,mat,m_viewportSize.width(),m_viewportSize.height());

    m_window->resetOpenGLState();
    m_window->endExternalCommands();
}
