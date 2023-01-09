import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend 1.0

ScrollablePage {
    id: page
    property string pageName: stackView.deviceName

    Column {
        spacing: 40
        width: parent.width

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Switch is an option button that can be dragged or toggled on or off. "
                + "Switches are typically used to select between two states."
        }

        M0001Controller{
            Component.onCompleted: {
                initialize(stackView.deviceName)
            }
            id: device
        }

        Column {
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter

            Switch {
                id: firstSwitch
                text: "First"
                onToggled: {
                    device.turnOnLed = checked;
                }
            }
            Switch {
                text: "Second"
                checked: true
            }
            Switch {
                text: "Third"
                enabled: false
            }
        }
    }
}

