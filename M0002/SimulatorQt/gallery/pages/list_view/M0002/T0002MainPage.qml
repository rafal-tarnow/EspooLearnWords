import QtQuick
import QtQuick.Controls

Item {

    Label {
        id: tempLabel
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        //text aligment
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        text: qsTr("T0002 19°C") + (brickController.connected ? qsTr("+") : qsTr("-"))
        //font.bold: true;
        font.pointSize: 96

    }

    Rectangle{
        z: -1
        anchors.fill: parent
        color: "red"

        border.color: "blue"
        border.width: 1
    }
}
