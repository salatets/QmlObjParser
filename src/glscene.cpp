#include <QtQuick/qquickwindow.h>
#include <QtCore/QRunnable>
#include <QtMath>

#include <glscene.h>
#include<ObjParser.h>
#include <vecUtils.h>

#include "shaders.h"

float max(const QVector3D& vec){
    float max = vec.x();

    if(vec.y() > max)
        max = vec.y();
    if(vec.z() > max)
        max = vec.z();

    return max;
}

GLScene::GLScene() : m_pos(3.14), m_renderer(nullptr){
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

std::string convertPath(const QUrl& path){
    std::string res;

    if(path.isRelative()){
        QString temp = path.toString();
        res = temp.toUtf8().constData();
    }else{ // format like file:/// or file:/
        int trunc_len = path.scheme().length() + 1;
        QString temp = path.toString();
        int i = 0;

        for(; i < 3; i++){
            if(temp[trunc_len + i] != '/')
                break;
        }

        res = temp.toUtf8().constData();
        res = res.substr(trunc_len + i);
    }

    return res;
}

// TODO add move semantic
inline std::string getPWD(const std::string& path){
    return path.substr(0, path.rfind('/') + 1);
}

void GLSceneRenderer::setPath(QUrl path)
{
    if(path == old_url)
        return;

    m_path = convertPath(path);
    m_model = parseOBJ(m_path);

    if(!m_model.empty()){

        switch (type) {
        case FH_Model:
            fh.setMesh(m_model);
            fh.init_buffers(m_program);
            break;
        case Scene:
            // SCENE
            break;
        case Model:
            ml.setMesh(m_model, getPWD(m_path));
            ml.setShader(meshType::VNT, fragment,vertex);
            break;
        }
    }
    old_url = path;
    m_window->update();
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
    if(m_program == nullptr)
        m_program = new QOpenGLShaderProgram();
    if(m_program->isLinked())
        m_program->removeAllShaders();

    char* ver;
    char* frag;

    switch (type) {
    case FH_Model:
        ver = hor_vertex;
        frag = hor_fragment;
        break;
    case Scene:
        // SCENE
        break;
    case Model:
        ver = vertex;
        frag = fragment;
        break;
    }

    bool success = m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, ver);
    success &= m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, frag);
    Q_ASSERT(success);

    success = m_program->link();
    Q_ASSERT(success);
}

void GLSceneRenderer::init_buffers(){
    switch (type) {
    case FH_Model:
        fh.init_buffers(m_program);
        break;
    case Scene:
        // SCENE
        break;
    case Model:
        ml.init_buffers();
        break;
    }


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

// TODO add center point view
void GLSceneRenderer::paint(){
    if(m_model.empty())
        return;

    m_window->beginExternalCommands();
    m_program->bind();

    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

    QMatrix4x4 mat;
    mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), m_pitch));
    mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), m_yaw));
    mat.scale(1/(max(vec3ToQVector3D(m_model.getSize()))));
    mat.translate(- vec3ToQVector3D(m_model.getCenter()));

    m_program->setUniformValue("model",mat);
    m_program->setUniformValue("lightColor", QVector3D(1.0f, 0.0f, 1.0f));
    m_program->setUniformValue("objectColor", QVector3D(1.0f, 0.5f, 0.31f));
    m_program->setUniformValue("lightPos", QVector3D(sin(m_pos), 0.3f, cos(m_pos)));
    m_program->setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 0.0f));

    m_program->setUniformValue("material.shininess", 64.0f);

    switch (type) {
    case FH_Model:
        // TODO size of points
        fh.paint(mat, m_viewportSize.width(), m_viewportSize.height());
        break;
    case Scene:
        // SCENE
        break;
    case Model:
        ml.paint();
        break;
    }

    m_program->release();
    m_window->resetOpenGLState();
    m_window->endExternalCommands();
}
