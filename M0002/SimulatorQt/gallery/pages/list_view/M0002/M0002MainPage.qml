import QtQuick
import QtQuick.Controls
import "../common"

Item {
    id: mainPage
    property bool details: recipe.detailsEnabled

    Column{
        id:smallView

        anchors.verticalCenter: parent.verticalCenter
        width: parent.width

        DataLine{
            id: data
            width: parent.width
            icon: "qrc:/images/temperature.svg"
            label: qsTr("Temperature")
            value: brickController.temperature.toFixed(1)
            unit: qsTr("°C")


        }

    }
    Rectangle{
        anchors.fill: data
        color: "red"
    }

    Item {
        id: bigView
        anchors.fill: parent
        opacity: 0
        enabled: false

        Image {
            id: bigTempImage
            anchors.right: bigTemp.left
            anchors.verticalCenter: bigTemp.verticalCenter

            width: 50
            height: 50

            fillMode: Image.PreserveAspectFit
            sourceSize: Qt.size(width,height)
            source: "qrc:/images/temperature.svg"
        }

        Text {
            id: bigTemp
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            text: brickController.temperature.toFixed(1)
            font.pointSize: 56
        }

        Text{
            anchors.left: bigTemp.right
            anchors.bottom: bigTemp.verticalCenter
            text: qsTr("°C")
            font.pointSize: 20
        }
    }

    states:  State {
        name: "Details"
        when: mainPage.details


        PropertyChanges {
            smallView{
                opacity: 0
                enabled: false
            }
            bigView{
                opacity: 1
                enabled: true
            }


        }
    }


    transitions: Transition {
        // Make the state changes smooth
        ParallelAnimation {
            NumberAnimation { duration:250; properties: "detailsOpacity,x,z,y,contentY,height,width,opacity, font.pointSize" }
        }
    }
}
