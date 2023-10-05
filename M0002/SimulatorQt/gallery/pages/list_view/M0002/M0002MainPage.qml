import QtQuick
import QtQuick.Controls

Item {

    Column{
        id: data

        //opacity: !recipe.detailsOpacity
        //enabled: !recipe.detailsEnabled

        anchors.top: parent.top

        Row{
            Image {
                width: 20
                height: 20

                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(width,height)
                source: "qrc:/images/temperature.svg"
            }


            Text {
                text: qsTr("Temperature: ") + brickController.temperature + qsTr("'C")
                font.pointSize: 16;
                //font.bold: true
                color: "#7a7b7a"
            }
        }
    }



    Label {
        id: tempLabel
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        //text aligment
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter

        text: qsTr("19°C") + (brickController.connected ? qsTr("+") : qsTr("-"))
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
