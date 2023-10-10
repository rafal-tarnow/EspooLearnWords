import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../common"

Item {
    id: mainPage
    property bool details: recipe.detailsEnabled

    Column{
        id:smallView

        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        spacing: 20

        DataLine{
            width: parent.width
            icon: "qrc:/images/temperature.svg"
            label: qsTr("Temperature")
            value: brickController.temperature.toFixed(1)
            unit: qsTr("°C")
        }

        DataLine{
            width: parent.width
            icon: "qrc:/images/pressure.svg"
            label: qsTr("Pressure")
            value: brickController.pressure.toFixed(0)
            unit: qsTr("hPa")
        }

        DataLine{
            width: parent.width
            icon: "qrc:/images/humidity.svg"
            label: qsTr("Humidity")
            value: brickController.humidity.toFixed(0)
            unit: qsTr("%")
        }
    }



    ColumnLayout {
        id: bigView
        anchors.fill: parent
        opacity: 0
        enabled: false

        BigDataLine{
            id: bigTemp
            anchors.horizontalCenter: bigView.horizontalCenter

            icon: "qrc:/images/temperature.svg"
            value: brickController.temperature.toFixed(1)
            unit: qsTr("°C")
        }

        BigDataLine{
            id: bigPressure
            anchors.horizontalCenter: bigView.horizontalCenter

            icon: "qrc:/images/pressure.svg"
            value: brickController.pressure.toFixed(0)
            unit: qsTr("hPa")
        }

        BigDataLine{
            id: bigHumidity
            anchors.horizontalCenter: bigView.horizontalCenter

            icon: "qrc:/images/humidity.svg"
            value: brickController.humidity.toFixed(0)
            unit: qsTr("%")
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
        ParallelAnimation {
            NumberAnimation { duration:250; properties: "detailsOpacity,x,z,y,contentY,height,width,opacity, font.pointSize" }
        }
    }
}
