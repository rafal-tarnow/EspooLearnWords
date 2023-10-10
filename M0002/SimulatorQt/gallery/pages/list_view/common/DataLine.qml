import QtQuick
import QtQuick.Layouts

RowLayout{
    property string icon
    property string label
    property string value
    property string unit

    Image {
        width: 20
        height: 20
        Layout.leftMargin: 20
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
        //color: "#7a7b7a"
    }

    Text {
        id: valueLabel
        Layout.alignment: Qt.AlignRight
        Layout.rightMargin: 10
        text: value + unit
        font.pointSize: 16
        //color: "#7a7b7a"
    }
}
