#include <QtCore/QRunnable>
#include <QtMath>

#if QT_VERSION_MAJOR == 6
#include <QQuickOpenGLUtils>
#endif

#include <ObjParser.h>
#include <glscene.h>
#include <vecUtils.h>

#include "shaders.h"

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

#ifndef _WIN32
        --trunc_len;
#endif

        res = temp.toUtf8().constData();
        res = res.substr(trunc_len + i);
    }

    return res;
}

// TODO add move semantic
inline std::string getPWD(const std::string& path){
    return path.substr(0, path.rfind('/') + 1);
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

GLScene::GLScene() : light_pos(3.14), m_renderer(nullptr){
    connect(this, &QQuickItem::windowChanged, this, &GLScene::handleWindowChanged);
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);
}

QPoint GLScene::mouseToAngle(){
    QPoint pos{mouse_prev + mouse_start - mouse_current};
    roundTo(pos, 360);
    return pos;
}

void GLScene::mousePressEvent(QMouseEvent* event)
{
    mouse_start = event->pos();

    event->accept();
}

void GLScene::mouseMoveEvent(QMouseEvent* event)
{
    mouse_current = event->pos();

    event->accept();

    if (window() != nullptr)
        window()->update();
}

void GLScene::mouseReleaseEvent(QMouseEvent* event)
{
    mouse_current = event->pos();

    mouse_prev += mouse_start - mouse_current;

    mouse_start = {0,0};
    mouse_current = {0,0};

    roundTo(mouse_prev, 360);

    event->accept();

    if (window() != nullptr)
        window()->update();
}

void GLScene::wheelEvent(QWheelEvent *event)
{
    wheel_current += event->angleDelta().y() / 240.0;
    event->accept();

    if (window() != nullptr)
        window()->update();
}

void GLScene::setPos(qreal pos)
{
    if (pos == light_pos)
        return;
    light_pos= pos;
    if (window() != nullptr)
        window()->update();
}

void GLScene::setPath(const QUrl& path)
{
    if (path == object_path)
        return;
    object_path= path;
    if (window() != nullptr)
        window()->update();
}

void GLScene::handleWindowChanged(QQuickWindow *win)
{
    if (win != nullptr) {
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
    explicit CleanupJob(GLSceneRenderer *renderer) : m_renderer(renderer) { }
    void run() override { delete m_renderer; }
private:
    GLSceneRenderer *m_renderer;
};

void GLScene::releaseResources()
{
    window()->scheduleRenderJob(new CleanupJob(m_renderer), QQuickWindow::BeforeSynchronizingStage);
    m_renderer = nullptr;
}

void GLScene::sync(){
    if (m_renderer == nullptr) {
        m_renderer = new GLSceneRenderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &GLSceneRenderer::init, Qt::DirectConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &GLSceneRenderer::paint, Qt::DirectConnection);
    }

    QPoint angles = mouseToAngle();

    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setPitch(angles.y());
    m_renderer->setYaw(angles.x());
    m_renderer->setPos(light_pos);
    m_renderer->setPath(object_path);
    m_renderer->setZoom(wheel_current);
    m_renderer->setWindow(window());
}

GLSceneRenderer::GLSceneRenderer() : type(Model){
    ml.setShader(meshType::VNT, VNT_fragment, VNT_vertex);
    ml.setShader(meshType::VN, VN_fragment, VN_vertex);
    ml.setShader(meshType::VT, VT_fragment, VT_vertex);
    ml.setShader(meshType::V, V_fragment, V_vertex);
}

void GLSceneRenderer::setPath(const QUrl& path)
{
    if(path == old_url)
        return;

    m_path = convertPath(path);
    m_model = parseOBJ(m_path);

    if(!m_model.empty()){

        switch (type) {
        case FH_Model:
            fh.setMesh(m_model);
            //fh.init_buffers(m_program);
            break;
        case Scene:
            // SCENE
            break;
        case Model:
            ml.setMesh(m_model, getPWD(m_path));
            break;
        }
    }
    old_url = path;
    m_window->update();
}

void GLSceneRenderer::init(){
    if(!ml.isInited()){
        ml.init_buffers();
    }
}

float max(Vec3 vec){
    float max = vec.x;

    if(vec.y > max)
        max = vec.y;
    if(vec.z > max)
        max = vec.z;

    return max;
}

void GLSceneRenderer::paint(){
    if(m_model.empty())
        return;

    m_window->beginExternalCommands();

    switch (type) {
    case FH_Model:
        // TODO size of points
        //fh.paint(mat, m_viewportSize.width(), m_viewportSize.height());
        break;
    case Scene:
        // SCENE
        break;
    case Model:
        ml.paint([this](QOpenGLShaderProgram* program, MeshRoot model){
            initializeOpenGLFunctions();
            glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

            QMatrix4x4 mat;
            mat.setToIdentity();
            mat.translate(QVector3D(0,0, m_zoom ));
            mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), m_pitch));
            mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), m_yaw));
            mat.scale(1.0f/(max((model.getSize()))));
            mat.translate(- vec3ToQVector3D(model.getCenter()));

            program->setUniformValue("model",mat);
            program->setUniformValue("lightColor", QVector3D(1.0f, 0.0f, 1.0f));
            program->setUniformValue("objectColor", QVector3D(1.0f, 0.5f, 0.31f));
            program->setUniformValue("lightPos", QVector3D(static_cast<float>(sin(m_pos)), 0.3f,static_cast<float>(cos(m_pos))));
            program->setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 0.0f));

            program->setUniformValue("material.shininess", 64.0f);
        });
        break;
    }

    #if QT_VERSION_MAJOR == 6
    QQuickOpenGLUtils::resetOpenGLState();
    #else
    m_window->resetOpenGLState();
    #endif

    m_window->endExternalCommands();
}
