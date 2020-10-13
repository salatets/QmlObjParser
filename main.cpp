#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "glscene.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    app.setOrganizationName("ORG");
    app.setOrganizationDomain("DOM");

    qmlRegisterType<GLScene>("GLScene", 1, 0, "GLScene");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
