#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include "VKApi.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QSettings settings;
    engine.rootContext()->setContextProperty("vk", new poison::VKApi(&settings));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
