#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include "VKApi.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("vk", new poison::VKApi());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
