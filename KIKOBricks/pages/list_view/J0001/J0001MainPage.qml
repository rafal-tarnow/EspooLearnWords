import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import "../common"

Item {
    id: mainPage
    property bool details

    Settings{
        category: "dup"
        //category: brickController.identifier
        //property alias maxFlow: ellipticalArcc.max_value
    }

    Column{
        id:smallView

        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        spacing: 20

        DataLine{
            id: dataMilliliters
            width: parent.width

            icon: "qrc:/images/pressure.svg"
            label: qsTr("Całkowity przepływ: ")
            value: brickController.milliliters.toFixed(1)
            unit: qsTr("ml")
        }

        DataLine{
            id: dataFlowRate
            width: parent.width

            icon: "qrc:/images/humidity.svg"
            label: qsTr("Szybkość przepływu: ")
            value: brickController.flowRate.toFixed(1)
            unit: qsTr("ml/min")
        }
    }



    Rectangle{
        id: bigView
        color: "#ffffff"
        // border.width: 1
        // border.color: "papayawhip"
        anchors.centerIn: parent

        property real ratio: 1.5
        property real maxWidth: 300
        property real maxHeight: maxWidth*ratio


        property size parentDimensions: Qt.size(parent.width, parent.height)
        onParentDimensionsChanged: {
            var parentRatio = parent.height/parent.width;
            if(parentRatio > bigView.ratio){
                if(parent.width > bigView.maxWidth){
                    bigView.width = bigView.maxWidth
                }else{
                    bigView.width = parent.width
                }
                bigView.height = bigView.width * bigView.ratio
            }else{
                if(parent.height > bigView.maxHeight){
                    bigView.height = bigView.maxHeight
                }else{
                    bigView.height = parent.height
                }
                bigView.width = bigView.height/bigView.ratio
            }

        }


        opacity: 0
        enabled: false

        // DataLine{
        //     id: totalFlow
        //     anchors.top: parent.top
        //     anchors.horizontalCenter: parent.horizontalCenter


        //     icon: "qrc:/images/pressure.svg"
        //     label: qsTr("Całkowity przepływ ")
        //     value: brickController.milliliters.toFixed(1)
        //     unit: qsTr(" ml")

        // }

        Text{
            anchors.top: parent.top
            anchors.horizontalCenter: ellipticalArcc.horizontalCenter
            text: "Całkowity przepływ"
            color: "#727272"
            font.pointSize: 16
            Text{
                anchors.top: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: brickController.milliliters.toFixed(1) + "ml"
                //font.bold: true
                font.pointSize: 24
            }
        }



        Item {
            id: dropAnimationArea
            height: ellipticalArcc.height * 0.7
            width: 20

            anchors.left: parent.left
            anchors.top: ellipticalArcc.top


            Image {
                id: dropImage
                anchors.horizontalCenter: parent.horizontalCenter
                y: 0
                width: parent.width
                height: width

                fillMode: Image.PreserveAspectFit
                source: "qrc:/images/drop.svg"
                sourceSize: Qt.size(width,height)
                //source: "qrc:/images/humidity.svg"

                SequentialAnimation on y {
                    loops: Animation.Infinite
                    running: true
                    NumberAnimation { from: 0; to: dropAnimationArea.height; duration: 1000; easing.type: Easing.InQuad }
                    ScriptAction { script: dropImage.y = 0 }
                }
            }
        }


        EllipticalArc {
            id: ellipticalArcc
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: dropAnimationArea.right
            anchors.right: parent.right
            height: width
            value: brickController.flowRate.toFixed(1)
            max_value: brickController.maxFlowDisplay
            //value: brickController.milliliters
            unit: qsTr("ml/min")
        }

        // Rectangle{
        //     anchors.top: ellipticalArcc.bottom
        //     anchors.bottom: parent.bottom
        //     anchors.left: parent.left
        //     anchors.right: parent.right
        //     color: "#33004400"
            Row{
                anchors.horizontalCenter: ellipticalArcc.horizontalCenter
                anchors.top: ellipticalArcc.bottom
                anchors.topMargin: -20
                spacing: 24
                //anchors.centerIn: parent
                Button{
                    width: 48
                    height: 48
                    enabled: true
                    icon.source: brickController.run ? "qrc:/images/stop-large-symbolic.svg" : "qrc:/images/play-large-symbolic.svg"
                    onPressed: {
                        brickController.setRun(!brickController.run);
                    }

                    background: Rectangle{
                        color: brickController.run ? "#aaffaa" /*a5d7a7*/ : "#ffffff"
                        anchors.fill: parent
                        radius: 24
                        border.width: 1
                        border.color: "#727272"
                    }
                }
                Button{
                    width: 48
                    height: 48
                    enabled: brickController.run ? false : true
                    icon.source: "qrc:/images/reset-symbolic.svg"
                    background: Rectangle{
                        color: "#ffffff"
                        anchors.fill: parent
                        radius: 24
                        border.width: 1
                        border.color: "#727272"
                    }
                    onPressed: {
                        brickController.resetMeasurement()
                    }
                }
                Button{
                    width: 48
                    height: 48
                    enabled: brickController.run ? false : true
                    icon.source: "qrc:/images/document-save-symbolic.svg"
                    background: Rectangle{
                        color: "#ffffff"
                        anchors.fill: parent
                        radius: 24
                        border.width: 1
                        border.color: "#727272"
                    }
                }
            }
        //}


        // Row{
        //     id: buttonsRow
        //     anchors.horizontalCenter: parent.horizontalCenter
        //     anchors.top: ellipticalArcc.bottom
        //     spacing: 12
        //     Button{
        //         text: "Start"
        //         height: 48
        //         width: 120
        //         onPressed: {
        //             brickController.setRun(!brickController.run);
        //         }
        //         background: Rectangle{
        //             anchors.fill: parent
        //             radius: 12
        //             color: "#f2f2f2"
        //         }
        //     }
        //     Button{
        //         text: "Reset"
        //         onPressed: {
        //             brickController.setRun(!brickController.run);
        //         }
        //         background: Rectangle{
        //             anchors.fill: parent
        //             radius: 12
        //             color: "#f2f2f2"
        //         }
        //     }
        //     Button{
        //         text: "Zapisz"
        //         background: Rectangle{
        //             anchors.fill: parent
        //             radius: 12
        //             color: "#f2f2f2"
        //         }
        //     }
        // }
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

