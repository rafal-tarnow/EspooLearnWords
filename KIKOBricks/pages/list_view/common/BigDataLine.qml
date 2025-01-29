import QtQuick
import QtQuick.Layouts

RowLayout {
    property string icon
    property string value
    property string unit

    Image {
        id: bigTempImage

        width: 50
        height: 50

        fillMode: Image.PreserveAspectFit
        sourceSize: Qt.size(width,height)
        source: icon
    }

    Text {
        id: bigTemp
        text: value
        font.pointSize: 56
    }

    Text{
        text: unit
        font.pointSize: 20
        Layout.alignment: Qt.AlignTop
        Layout.topMargin: 10
    }
}
