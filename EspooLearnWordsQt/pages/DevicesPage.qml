import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend 1.0

Page {
    id: page

    Column {
        spacing: 40
        width: parent.width

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("Searching for devices ...")
        }

        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
        }

        DevicesManager{
            id: devicesModel
        }

        ColumnLayout {
            spacing: 20
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter

            ListView {
                id: listview
                width: parent.width
                height: 320
                model: devicesModel
                ScrollBar.vertical: ScrollBar { }

                delegate: Rectangle {
                    width: listview.width;
                    height: 25

                    //                    required color
                    //                    required property string name

                    Text {
                        text: deviceName
                        font.bold: true
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }



}
