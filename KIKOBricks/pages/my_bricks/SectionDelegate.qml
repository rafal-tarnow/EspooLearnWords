import QtQuick
import QtQuick.Controls

ToolBar {
    id: background

    Label {
        id: label
        text: section
        anchors.fill: parent
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
    }
}
