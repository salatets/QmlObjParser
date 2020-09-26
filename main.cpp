#include <QGuiApplication>
#include <QtQuick/QQuickView>

#include "glscene.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setOrganizationName("ORG");
    app.setOrganizationDomain("DOM");

    qmlRegisterType<GLScene>("GLScene", 1, 0, "GLScene");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///main.qml"));
    view.show();

    return app.exec();
}
