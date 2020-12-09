#ifndef GLSCENE_H
#define GLSCENE_H

#include <QQuickItem>
#include <QQuickWindow>

#include <Mesh.h>
#include <FloatingHorizon.h>
#include <MeshLoader.h>

enum Renderer{
    FH_Model,
    Scene,
    Model
};

class GLSceneRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLSceneRenderer();
    ~GLSceneRenderer(){};

    void setPitch(qreal pitch){m_pitch = pitch;}
    void setYaw(qreal yaw){m_yaw = yaw;}
    void setPos(qreal pos){m_pos = pos;}
    void setPath(const QUrl& path);
    void setZoom(qreal zoom){m_zoom = zoom;}

    void setViewportSize(QSize size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }

public Q_SLOTS:
    void init();
    void paint();

private:

    Renderer type;
    FloatingHorizon fh;
    MeshLoader ml;

    MeshRoot m_model;
    qreal m_pitch {0};
    qreal m_yaw {0};
    qreal m_pos {0};
    qreal m_zoom {0};
    std::string m_path;
    QUrl old_url;

    QSize m_viewportSize;
    QQuickWindow *m_window {nullptr};
};


class GLScene: public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal pos READ pos WRITE setPos NOTIFY posChanged)
    Q_PROPERTY(QUrl path READ path WRITE setPath NOTIFY pathChanged)
    QML_ELEMENT

public:
    GLScene();

    qreal pos() const { return light_pos; }
    QUrl path() const { return object_path; }

    void setPos(qreal pos);
    void setPath(const QUrl &path);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

Q_SIGNALS:
    void posChanged();
    void pathChanged();

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

    qreal wheel_current;
    QPoint mouse_start;
    QPoint mouse_current;
    QPoint mouse_prev;

    GLSceneRenderer *m_renderer;
};
#endif // GLSCENE_H
