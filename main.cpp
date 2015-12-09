#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include "VKApi.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    auto vk = new poison::VKApi();
    vk->init(QStringLiteral("3296609"), QStringLiteral("friends,offline"));
    engine.rootContext()->setContextProperty("vk", vk);
    engine.load(QUrl(QStringLiteral("qrc:/analyzer.qml")));

    return app.exec();
}
