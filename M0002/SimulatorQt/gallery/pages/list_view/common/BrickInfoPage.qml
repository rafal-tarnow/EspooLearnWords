import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../../"
import "../../controls"


ScrollablePage {
    id: page

    Column{
        id:smallView

        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        spacing: 20

        DataLine{
            width: parent.width
            icon: "qrc:/images/device_type.svg"
            label: qsTr("Type")
            value: brickController.type
        }

        DataLine{
            width: parent.width
            icon: "qrc:/images/wifi.svg"
            label: qsTr("WiFi")
            value: brickController.wifiSSID
        }

        DataLine{
            width: parent.width
            icon: "qrc:/images/ip_address.svg"
            label: qsTr("IP")
            value: brickController.ip
        }
    }
}
