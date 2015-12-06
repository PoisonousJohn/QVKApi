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
            vk.init(appId)
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
