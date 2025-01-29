import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout{
    property string icon
    property string label
    property alias relayState : relaySwitch.checked
    property int leftMarginValue: 20

    Image {
        width: 20
        height: 20
        Layout.leftMargin: leftMarginValue
        Layout.alignment: Qt.AlignLeft
        fillMode: Image.PreserveAspectFit
        sourceSize: Qt.size(width,height)
        source: parent.icon
    }

    Text {
        Layout.leftMargin: 20
        Layout.fillWidth: true
        text: label
        font.pointSize: 16;
        clip: true
        //color: "#7a7b7a"
    }

    Switch {
        id: relaySwitch
        Layout.alignment: Qt.AlignRight
        Layout.rightMargin: 10
    }
}
