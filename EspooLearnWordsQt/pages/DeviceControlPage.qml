import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

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

        Column {
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter

            Switch {
                text: "First"
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

