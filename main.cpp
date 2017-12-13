#include <QGuiApplication>
#include <QQuickWindow>
#include <QQuickView>
#include "cubeitem.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

//    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Direct3D12);

    qmlRegisterType<CubeItem>("CubeItem",1,0,"CubeItem");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();

    qDebug() << "Backend: " << QQuickWindow::sceneGraphBackend();

    return app.exec();
}
