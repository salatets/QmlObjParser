#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
//#include <QQuickWindow>

#include "glscene.h"

int main(int argc, char *argv[])
{
#if QT_VERSION_MAJOR == 5
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#endif

    QGuiApplication app(argc, argv);

    QGuiApplication::setOrganizationName("ORG");
    QGuiApplication::setOrganizationDomain("DOM");

    QTranslator appTranslator;
    appTranslator.load(QLocale(), QLatin1String("RGZ"), QLatin1String("_"), QLatin1String("translations"));
    app.installTranslator(&appTranslator);

    qmlRegisterType<GLScene>("GLScene", 1, 0, "GLScene");

    // TODO 6 set opengl backend
    //QQuickWindow::setSceneGraphBackend("opengl");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
