QVKApi
======

Qt based API client for VKontakte (vk.com)

##Usage example

```
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
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	return app.exec();
}
```

```
import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("QVKApi")
    width: 640
    height: 480
    visible: true

    MainForm {
        id: mainForm
        anchors.fill: parent
        button1.onClicked: {
            consoleStr += "Authorization started\n"
            vk.init(appId, "friends")
            vk.login()
        }
        Connections {
            target: vk
            onLoginStatusChanged: {
                if (vk.isLoggedIn())
                {
                    mainForm.consoleStr += "Authorization completed. Your data:\n Name: " + vk.getUser().name + "\nid: " + vk.getUser().uid
                }
                else
                {
                    mainForm.consoleStr += "Failed to authorize"
                }

            }
        }
    }

}
```
