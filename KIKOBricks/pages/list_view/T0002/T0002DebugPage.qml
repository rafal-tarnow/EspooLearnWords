import QtQuick
import QtQuick.Controls
import "../../"
import "../common"

ScrollablePage {
    id: page
    anchors.fill: parent

    function updateFrequency(){
        var  freq = (flowRate.value/60.0)/(dropletVolume.value)
        textFrequencyCalculated.text = "Frequency calc: " + freq + " Hz"
        brickController.frequency = freq
    }

    contentItem: Column {
        anchors.fill: parent


        Text{
            id: textFrequencyCalculated
            text: "Frequency calc:  Hz"
            font.pointSize: 20
        }

        Text{
            id: textFrequency
            text: "Frequency brick: " + brickController.frequency + " Hz"
            font.pointSize: 20
        }

        Row{
            enabled: false
            Slider {
                id: frequencySlider
                from: 0.0
                value: brickController.frequency
                to: 150.0
                onValueChanged: {
                    textFrequency.text = "Frequency brick: " + brickController.frequency + " Hz"
                }
            }
            Button{
                text: "Save"
                onClicked: {
                }
            }
        }

        Text{
            text: "Droplet volume: " + dropletVolume.value.toFixed(3) + " ml"
            font.pointSize: 20
        }


        Slider{
            id: dropletVolume
            from: 0.02
            to: 0.1
            onValueChanged: {
                updateFrequency()
            }
            anchors.left: parent.left
            anchors.right: parent.right
        }


        Text{
            text: "Flow rate: " + flowRate.value.toFixed(3) + " ml/min"
            font.pointSize: 20
        }

        Slider{
            id: flowRate
            from: 0.0
            to: 150

            onValueChanged: {
                updateFrequency()
            }
            anchors.left: parent.left
            anchors.right: parent.right
        }


        // Row{
        //     Slider {
        //         id: flowRateSlider
        //         from: 0.0
        //         //value: brickController.dropletVolume
        //         to: 150.0
        //         onValueChanged: {
        //             //brickController.dropletVolume = dropletVolumeSlider.value
        //         }
        //     }
        //     Button{
        //         text: "Save"
        //         onClicked: {
        //             brickController.dropletVolume = dropletVolumeSlider.value
        //         }
        //     }
        // }
        // Row{
        //     Slider {
        //         id: flowRateSlider
        //         from: 0.0
        //         //value: brickController.dropletVolume
        //         to: 150.0
        //         onValueChanged: {
        //             //brickController.dropletVolume = dropletVolumeSlider.value
        //         }
        //     }
        //     // Button{
        //     //     text: "Save"
        //     //     onClicked: {
        //     //         brickController.dropletVolume = dropletVolumeSlider.value
        //     //     }
        //     // }
        // }
    }

}
