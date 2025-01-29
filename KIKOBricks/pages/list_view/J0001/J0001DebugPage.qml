import QtQuick
import QtQuick.Controls
import "../../"
import "../common"


ScrollablePage {
    id: page

    Column{
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        spacing: 20

        Text{
            text: "DEBUG"
            font.pointSize: 12
        }

        DataLine{
            width: parent.width
            icon: "qrc:/images/id.svg"
            label: qsTr("Window:")
            value: brickController.windowDuration
        }

        Row{
            Slider {
                id: windowDurationSlider
                from: 0.0
                stepSize: 1
                value: brickController.windowDuration
                to: 10

                Label {
                    text: windowDurationSlider.value
                    anchors.bottom: windowDurationSlider.handle.top
                    anchors.bottomMargin: 5
                    anchors.horizontalCenter: windowDurationSlider.handle.horizontalCenter
                }
            }
            Button{
                text: "Save"
                onClicked: {
                    brickController.windowDuration = windowDurationSlider.value
                }
            }
        }



        DataLine{
            width: parent.width
            icon: "qrc:/images/id.svg"
            label: qsTr("Run:")
            value: brickController.run
        }

        DataLine{
            width: parent.width
            icon: "qrc:/images/id.svg"
            label: qsTr("Count:")
            value: brickController.count
        }

        DataLine{
            width: parent.width
            icon: "qrc:/images/id.svg"
            label: qsTr("Milliliters: ")
            value: brickController.milliliters.toFixed(3)
        }
        DataLine{
            width: parent.width
            icon: "qrc:/images/id.svg"
            label: qsTr("Objętość kropli: ")
            value: brickController.dropletVolume.toFixed(3)
        }
        DataLine{
            width: parent.width
            icon: "qrc:/images/id.svg"
            label: qsTr("Szybkość przepływu: ")
            value: brickController.flowRate.toFixed(3)
        }
        DataLine{
            width: parent.width
            icon: "qrc:/images/id.svg"
            label: qsTr("Czas działania/max46dni/:")
            value: (brickController.runtime/60).toFixed(0)
            unit: qsTr("min")
        }
        DataLine{
            width: parent.width
            icon: "qrc:/images/id.svg"
            label: qsTr("Maksymalny przepływ:")
            value: brickController.maxFlowDisplay
        }

    }
}
