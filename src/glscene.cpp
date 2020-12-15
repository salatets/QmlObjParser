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

GLScene::GLScene() : light_pos(3.14), wheel_current(0), m_renderer(nullptr), viewType(Helper::Renderer::Model){
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

void GLScene::setPerspective(bool perspective){
    is_perspective = perspective;
    if (window() != nullptr) // TODO check needed??
        window()->update();
}

void GLScene::setViewMode(Helper::Renderer type){
    if(type == viewType)
        return;
    viewType = type;
    emit viewModeChanged();
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

    m_renderer->setPath(object_path);
    m_renderer->setSceneType(viewType);

    m_renderer->setPitch(angles.y());
    m_renderer->setYaw(angles.x());
    m_renderer->setPos(light_pos);

    m_renderer->setZoom(wheel_current);
    m_renderer->setPerspective(is_perspective);

    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setWindow(window());
}

GLSceneRenderer::GLSceneRenderer() : scene_type(Helper::Model){
    ml.setShader(meshType::VNT, VNT_fragment, VNT_vertex);
    ml.setShader(meshType::VN, VN_fragment, VN_vertex);
    ml.setShader(meshType::VT, VT_fragment, VT_vertex);
    ml.setShader(meshType::V, V_fragment, V_vertex);
    sl.setShader(meshType::VNT, VNT_fragment, VNT_vertex);
    sl.setShader(meshType::VN, VN_fragment, VN_vertex);
    sl.setShader(meshType::VT, VT_fragment, VT_vertex);
    sl.setShader(meshType::V, V_fragment, V_vertex);
}

Scene FileLoader(const std::string& path){
    std::string ext = path.substr(path.rfind('.') + 1);

    if(ext == "obj"){
        Scene scene;
        scene.meshes.emplace_back(Vec3(0,0,0),Vec3(0,0,0),parseOBJ(path));
        return scene;
    }else if(ext == "scene"){
        return ParseScene(path);
    }

    qDebug() << "could not parse giving " << QString(ext.data()) << " object\n";
    return Scene();
}

void GLSceneRenderer::setPath(const QUrl& url)
{
    if(url == old_url)
        return;

    m_path = convertPath(url);

    scene = FileLoader(m_path);

    if(!scene.meshes.empty()){

        switch (scene_type) {
        case Helper::FH_Model:
            fh.setMesh(std::get<2>(scene.meshes[0]));
            //fh.init_buffers(m_program);
            break;
        case Helper::Scene:
            sl.setScene(scene,getPWD(m_path));
            break;
        case Helper::Model:
            ml.setMesh(std::get<2>(scene.meshes[0]), getPWD(m_path));
            break;
        }
    }
    old_url = url;
    m_window->update();
}

void GLSceneRenderer::init(){
    if(!ml.isInited()){
        ml.init_buffers();
    }
    sl.init_buffers();
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
    if(scene.meshes.empty())
        return;

    m_window->beginExternalCommands();

    switch (scene_type) {
    case Helper::FH_Model:
        // TODO size of points
        //fh.paint(mat, m_viewportSize.width(), m_viewportSize.height());
        break;
    case Helper::Scene:
        sl.paint([this](const MeshRoot& model){
            initializeOpenGLFunctions();
            glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

            QMatrix4x4 proj;
            QMatrix4x4 mat;
            proj.setToIdentity();
            mat.setToIdentity();

            mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), m_pitch));
            mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), m_yaw));

            program_param params {
                {"model",mat},
                {"projection",proj},
                {"objectColor", vec3ToQVector3D(model.front().getMaterial().diffuse)}, // todo add properties to meshes
            };
            return params;
        });
        break;
    case Helper::Model:
        ml.paint([this](const MeshRoot& model){
            initializeOpenGLFunctions();
            glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

            QMatrix4x4 proj;
            QMatrix4x4 mat;
            proj.setToIdentity();
            mat.setToIdentity();

            auto m_corr = m_zoom -1;

            // TODO remade this hack
            if(m_perspective){
                proj.perspective(45,(double)(m_viewportSize.width())/m_viewportSize.height(),0.1,10);
                mat.translate(QVector3D(0,0, m_corr - 2));
            }else{
                mat.scale(-1/(m_corr),-1/(m_corr),1/(m_corr));
            }

            mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0), m_pitch));
            mat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), m_yaw));
            float m = 1.0f / max((model.getSize()));

            program_param params {
                {"model",mat},
                {"projection",proj},
                {"lightColor", QVector3D(1.0f, 0.0f, 1.0f)},
                {"lightPos", QVector3D(static_cast<float>(sin(m_pos)), 0.3f, (m_perspective ? m_corr - 2 : 0) + static_cast<float>(cos(m_pos)))},
                {"viewPos", QVector3D(0.0f, 0.0f, 0.0f)},
                {"material.shininess", 64.0f}
            };

            return std::make_tuple(QVector3D(m,m,m),- vec3ToQVector3D(model.getCenter()),params);
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
