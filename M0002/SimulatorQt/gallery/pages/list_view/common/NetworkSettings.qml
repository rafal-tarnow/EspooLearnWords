import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../../"
import "../../controls"


ScrollablePage {
    id: page
    property bool active: false;

    Column {
        spacing: 10
        width: parent.width

        TextField {
            id: deviceName
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("Brick name")
            text: brickController.name
            enabled: page.active
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
                enabled: page.active
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
