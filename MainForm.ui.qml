import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: item1
    width: 640
    height: 480
    property alias button1: button1
    property alias appId: textField1.text
    property alias consoleStr: textArea1.text

    ColumnLayout {
        id: columnLayout1
        anchors.fill: parent

        RowLayout {
            id: rowLayout1
            scale: 1
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            clip: false
            spacing: 0

            TextField {
                id: textField1
                width: 200
                placeholderText: qsTr("ID приложения")
                Layout.fillWidth: true
            }

            Button {
                id: button1
                text: qsTr("Авторизоваться")
                isDefault: true
            }

        }

        TextArea {
            id: textArea1
            readOnly: true
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
