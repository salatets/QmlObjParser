#ifndef GLSCENE_H
#define GLSCENE_H

#include <QQuickItem>
#include <QQuickWindow>

#include <Mesh.h>
#include <FloatingHorizon.h>
#include <MeshLoader.h>
#include <SceneParser.h>

class Helper : public QObject  // TODO ugly name
{
    Q_OBJECT

public:
    enum Renderer{
        FH_Model,
        Scene,
        Model
    };
    Q_ENUM(Renderer)
};

class GLSceneRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLSceneRenderer();
    ~GLSceneRenderer(){}

    void setPath(const QUrl& url);
    void setSceneType(Helper::Renderer scene_type){this->scene_type = scene_type;}

    void setPitch(qreal pitch){m_pitch = pitch;}
    void setYaw(qreal yaw){m_yaw = yaw;}
    void setPos(qreal pos){m_pos = pos;}

    void setZoom(qreal zoom){m_zoom = zoom;}
    void setPerspective(bool is_perspective){m_perspective = is_perspective;}

    void setViewportSize(QSize size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }


public Q_SLOTS:
    void init();
    void paint();

private:

    Helper::Renderer scene_type;
    FloatingHorizon fh;
    MeshLoader ml;

    Scene scene;
    qreal m_pitch {0};
    qreal m_yaw {0};
    qreal m_pos {0};
    qreal m_zoom {0};
    std::string m_path;
    bool m_perspective {true};
    QUrl old_url;

    QSize m_viewportSize;
    QQuickWindow *m_window {nullptr};
};


class GLScene: public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal pos READ pos WRITE setPos)
    Q_PROPERTY(QUrl path READ path WRITE setPath)
    Q_PROPERTY(bool perspective READ perspective WRITE setPerspective)
    Q_PROPERTY(Helper::Renderer viewMode READ viewMode WRITE setViewMode NOTIFY viewModeChanged)
    QML_ELEMENT

public:
    GLScene();

    qreal pos() const { return light_pos; }
    QUrl path() const { return object_path; }
    bool perspective() const {return is_perspective;}
    Helper::Renderer viewMode() const {return viewType;}

    void setPos(qreal pos);
    void setPath(const QUrl &path);
    void setPerspective(bool perspective);
    void setViewMode(Helper::Renderer type);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

Q_SIGNALS:
    void viewModeChanged();

public Q_SLOTS:
    void sync();
    void cleanup();

private Q_SLOTS:
    void handleWindowChanged(QQuickWindow *win);

private:
    void releaseResources() override;
    QPoint mouseToAngle();

    qreal light_pos;
    QUrl object_path;
    bool is_perspective;

    qreal wheel_current;
    QPoint mouse_start;
    QPoint mouse_current;
    QPoint mouse_prev;

    Helper::Renderer viewType;
    GLSceneRenderer *m_renderer;
};
#endif // GLSCENE_H
