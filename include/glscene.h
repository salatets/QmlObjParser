#ifndef GLSCENE_H
#define GLSCENE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>

#include <Mesh.h>
#include <FloatingHorizon.h>

class GLSceneRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLSceneRenderer() : m_program(nullptr){};
    ~GLSceneRenderer();

    void setPitch(qreal pitch){m_pitch = pitch;}
    void setYaw(qreal yaw){m_yaw = yaw;}
    void setPos(qreal pos){m_pos = pos;}
    void setPath(QUrl path);
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }

public slots:
    void init();
    void paint();

private:
    void init_program();
    void init_buffers();

    FloatingHorizon fh;

    Mesh m_model;
    qreal m_pitch;
    qreal m_yaw;
    qreal m_pos;
    std::string m_path;
    QUrl old_url;

    QSize m_viewportSize;
    QOpenGLShaderProgram *m_program;
    QQuickWindow *m_window;
};


class GLScene: public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal pos READ pos WRITE setPos NOTIFY posChanged)
    Q_PROPERTY(QUrl path READ path WRITE setPath NOTIFY pathChanged)
    QML_ELEMENT

public:
    GLScene();

    qreal pos() const { return m_pos; }
    QUrl path() const { return m_path; }

    void setPos(qreal pos);
    void setPath(QUrl path);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

signals:
    void posChanged();
    void pathChanged();

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    void releaseResources() override;
    QPoint mouseToAngle();

    qreal m_pos;
    QUrl m_path;

    QPoint m_start;
    QPoint m_current;
    QPoint m_prev;

    GLSceneRenderer *m_renderer;
};
#endif // GLSCENE_H
