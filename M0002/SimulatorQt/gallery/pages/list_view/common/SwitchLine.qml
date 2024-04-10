import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout{
    id: switchLine
    property string icon
    property string label
    property bool switchState
    property int leftMarginValue: 20

    CheckBox{
        id: switchCheckBox
        width: 20
        height: 20
        Layout.leftMargin: leftMarginValue
        Layout.alignment: Qt.AlignLeft
        checked: switchLine.switchState
        MouseArea{
            anchors.fill: parent
        }
    }

    // Image {
    //     width: 20
    //     height: 20
    //     Layout.leftMargin: leftMarginValue
    //     Layout.alignment: Qt.AlignLeft
    //     fillMode: Image.PreserveAspectFit
    //     sourceSize: Qt.size(width,height)
    //     source: parent.icon
    // }

    Text {
        Layout.leftMargin: 20
        Layout.fillWidth: true
        text: label
        font.pointSize: 16;
        clip: true
        //color: "#7a7b7a"
    }

    Text{
        id: stateText
        Layout.alignment: Qt.AlignRight
        Layout.rightMargin: 10
        font.pointSize: 16;
        text: switchLine.switchState === true ? "true" : "false"
    }

    // Switch {
    //     id: inputSwitch
    //     Layout.alignment: Qt.AlignRight
    //     Layout.rightMargin: 10
    // }
}
