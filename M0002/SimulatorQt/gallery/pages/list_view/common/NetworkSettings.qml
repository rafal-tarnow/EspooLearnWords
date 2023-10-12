import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../../"
import "../../controls"


ScrollablePage {
    id: page

    Column {
        spacing: 10
        width: parent.width

        TextField {
            id: deviceName
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("Brick name")
            text: brickController.name
        }

        TextField {
            id: wifiNetworkName
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("WiFi Network Name")
            text: brickController.wifiSSID
        }

        TextField {
            id: wifiPassword
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("WiFi password")
            text: brickController.wifiPWD
        }

        RowLayout{
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width < 300 ? parent.width : 300
            visible: false
            RadioButton {
                id: dynamicIP
                text: qsTr("Dynamic IP")
                checked: true
                Layout.alignment: Qt.AlignLeft

            }
            RadioButton {
                id: staticIPButton
                text: qsTr("Static IP")
                Layout.alignment: Qt.AlignRight
            }
        }

        TextField {
            id: ip
            anchors.horizontalCenter: parent.horizontalCenter
            visible: staticIPButton.checked ? true : false
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("IP")
        }

        TextField {
            id: gateway
            anchors.horizontalCenter: parent.horizontalCenter
            visible: staticIPButton.checked ? true : false
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("Gateway")
        }

        TextField {
            id: netmast
            anchors.horizontalCenter: parent.horizontalCenter
            visible: staticIPButton.checked ? true : false
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("Netmask")
        }

        RowLayout{
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width < 300 ? parent.width : 300
            SButton {
                id: buttonSave
                text: qsTr("SAVE CONFIG")
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    brickController.cmdSaveBrickName(deviceName.text)
                    brickController.cmdSaveNetworkConfig(wifiNetworkName.text, wifiPassword.text)
                }
            }
        }

    }
}

//ScrollablePage {
//    id: secondPage
//    Rectangle{
//        anchors.fill: secondPage
//        color: "#185894"

//        border.color: "orange"
//        border.width: 1
//    }
//}


//ScrollablePage {
//    id: page

//    Component.onCompleted: {
//        console.log("Button Page created")
//    }
//    Component.onDestruction: {
//        console.log("Button Page destroyed")
//    }


//    Column {
//        spacing: 40
//        width: parent.width

//        Label {
//            width: parent.width
//            wrapMode: Label.Wrap
//            horizontalAlignment: Qt.AlignHCenter
//            text: qsTr("Button presents a push-button that can be pushed or clicked by the user. "
//                + "Buttons are normally used to perform an action, or to answer a question.")
//        }

//        ColumnLayout {
//            spacing: 20
//            anchors.horizontalCenter: parent.horizontalCenter

//            Button {
//                text: qsTr("First")
//                Layout.fillWidth: true
//            }
//            Button {
//                id: button
//                text: qsTr("Second")
//                highlighted: true
//                Layout.fillWidth: true
//            }
//            Button {
//                text: qsTr("Third")
//                enabled: false
//                Layout.fillWidth: true
//            }
//            Button {
//                text: qsTr("Push")
//                Layout.fillWidth: true
//                onClicked: {

//                }
//            }
//            Button {
//                text: qsTr("First")
//                Layout.fillWidth: true
//            }
//            Button {
//                text: qsTr("Second")
//                highlighted: true
//                Layout.fillWidth: true
//            }
//            Button {
//                text: qsTr("Third")
//                enabled: false
//                Layout.fillWidth: true
//            }
//            Button {
//                text: qsTr("Push")
//                Layout.fillWidth: true
//                onClicked: {

//                }
//            }
//            Button {
//                text: qsTr("First")
//                Layout.fillWidth: true
//            }
//            Button {
//                text: qsTr("Second")
//                highlighted: true
//                Layout.fillWidth: true
//            }
//            Button {
//                text: qsTr("Third")
//                enabled: false
//                Layout.fillWidth: true
//            }
//            Button {
//                text: qsTr("Push")
//                Layout.fillWidth: true
//                onClicked: {

//                }
//            }
//        }
//    }
//}
