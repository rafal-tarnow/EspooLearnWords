import QtQuick
import QtQuick.Controls
import "../common"

Item {
    id: mainPage
    property bool details

    Column{
        id:smallView

        anchors.verticalCenter: parent.verticalCenter
        width: parent.width

        SwitchLine{
            id: data
            width: parent.width

            icon: "qrc:/images/temperature.svg"
            label: qsTr("Switch")
            switchState: brickController.switchState
        }
    }


    Column{
         id: bigView
         anchors.fill: parent
         opacity: 0
         enabled: false

        BigDataLine{
            id: bigTemp
            anchors.verticalCenter: bigView.verticalCenter
            anchors.horizontalCenter: bigView.horizontalCenter

            icon: "qrc:/images/temperature.svg"
            value: brickController.temperature.toFixed(1)
            unit: qsTr("°C")
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
