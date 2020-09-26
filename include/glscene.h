#ifndef GLSCENE_H
#define GLSCENE_H
#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>
#include "mesh.h"

class GLSceneRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GLSceneRenderer() : m_pitch(0), m_yaw(0), m_roll(0), m_program(nullptr){};
    ~GLSceneRenderer();

    void setPitch(qreal pitch){m_pitch = pitch;}
    void setYaw(qreal yaw){m_yaw = yaw;}
    void setRoll(qreal roll){m_roll = roll;}
    void setPos(qreal pos){m_pos = pos;}
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }

public slots:
    void init();
    void paint();

private:
    Mesh m_model;
    qreal m_pitch;
    qreal m_yaw;
    qreal m_roll;
    qreal m_pos;
    QSize m_viewportSize;
    QOpenGLShaderProgram *m_program;
    QQuickWindow *m_window;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QOpenGLBuffer nbo;
};


class GLScene: public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal pitch READ pitch WRITE setPitch NOTIFY pitchChanged)
    Q_PROPERTY(qreal yaw READ yaw WRITE setYaw NOTIFY yawChanged)
    Q_PROPERTY(qreal roll READ roll WRITE setRoll NOTIFY rollChanged)
    Q_PROPERTY(qreal pos READ pos WRITE setPos NOTIFY posChanged)
    QML_ELEMENT

public:
    GLScene();

    qreal pitch() const { return m_pitch; }
    qreal yaw() const { return m_yaw; }
    qreal roll() const { return m_roll; }
    qreal pos() const { return m_pos; }
    void setPitch(qreal pitch);
    void setYaw(qreal yaw);
    void setRoll(qreal roll);
    void setPos(qreal pos);

signals:
    void pitchChanged();
    void yawChanged();
    void rollChanged();
    void posChanged();


public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    void releaseResources() override;

    qreal m_pitch;
    qreal m_yaw;
    qreal m_roll;
    qreal m_pos;
    GLSceneRenderer *m_renderer;
};

#endif // GLSCENE_H
