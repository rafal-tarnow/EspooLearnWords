import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../../"

//ScrollablePage {
//    id: secondPage
//    Rectangle{
//        anchors.fill: secondPage
//        color: "#185894"

//        border.color: "orange"
//        border.width: 1
//    }
//}


ScrollablePage {
    id: page

    Component.onCompleted: {
        console.log("Button Page created")
    }
    Component.onDestruction: {
        console.log("Button Page destroyed")
    }


    Column {
        spacing: 40
        width: parent.width

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("Button presents a push-button that can be pushed or clicked by the user. "
                + "Buttons are normally used to perform an action, or to answer a question.")
        }

        ColumnLayout {
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                text: qsTr("First")
                Layout.fillWidth: true
            }
            Button {
                id: button
                text: qsTr("Second")
                highlighted: true
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Third")
                enabled: false
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Push")
                Layout.fillWidth: true
                onClicked: {

                }
            }
            Button {
                text: qsTr("First")
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Second")
                highlighted: true
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Third")
                enabled: false
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Push")
                Layout.fillWidth: true
                onClicked: {

                }
            }
            Button {
                text: qsTr("First")
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Second")
                highlighted: true
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Third")
                enabled: false
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Push")
                Layout.fillWidth: true
                onClicked: {

                }
            }
        }
    }
}
