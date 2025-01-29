import QtQuick
import QtQuick.Controls
import QtCore
import "../../"
import "../common"


ScrollablePage {
    id: page
    anchors.fill: parent

    contentItem: Column{
        anchors.fill: parent
        anchors.leftMargin: 7
        anchors.rightMargin: 7

        spacing: 20

        Text{
            text: "Objętość kropli: " + brickController.dropletVolume.toFixed(3) + "ml"
            font.pointSize: 20
        }


        Row{
            width: parent.width
            height: 30
            Slider{
                id: dropletSlider
                from: 0.02
                stepSize: 0.005
                to: 0.1
                value: brickController.dropletVolume
                anchors.left: parent.left
                anchors.right: saveDroplet.left
                anchors.verticalCenter: parent.verticalCenter

                Label {
                    text: dropletSlider.value.toFixed(3)
                    anchors.bottom: dropletSlider.handle.top
                    anchors.bottomMargin: 5
                    anchors.horizontalCenter: dropletSlider.handle.horizontalCenter
                }
            }
            Button{
                id: saveDroplet
                text: "Zapisz"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    brickController.dropletVolume = dropletSlider.value
                }
            }
        }

        Text{
            text: "Maksymalny przepływ: " + brickController.maxFlowDisplay + "ml/min"
            font.pointSize: 20
        }

        Row{
            width: parent.width
            height: 30
            Slider{
                id: maxFlowRateSlider
                from: 40.0
                stepSize: 5
                to: 160.0
                value: brickController.maxFlowDisplay
                anchors.left: parent.left
                anchors.right: saveMaxFlowRate.left
                anchors.verticalCenter: parent.verticalCenter

                Label {
                    text: maxFlowRateSlider.value.toFixed(3)
                    anchors.bottom: maxFlowRateSlider.handle.top
                    anchors.bottomMargin: 5
                    anchors.horizontalCenter: maxFlowRateSlider.handle.horizontalCenter
                }
            }
            Button{
                id: saveMaxFlowRate
                text: "Zapisz"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onClicked: {
                    brickController.maxFlowDisplay = maxFlowRateSlider.value
                }
            }
        }
    }
}
