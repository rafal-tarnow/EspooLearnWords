import QtQuick
import QtQuick.Controls

Item {
    id: mainPage
    property bool details: recipe.detailsEnabled


    Image {
        id: tempImage
        anchors.leftMargin: 25
        anchors.top: mainPage.top
        anchors.left: mainPage.left
        width: 20
        height: 20

        fillMode: Image.PreserveAspectFit
        sourceSize: Qt.size(width,height)
        source: "qrc:/images/temperature.svg"
    }

    Text {
        id: tempLabel
        anchors.leftMargin: 25
        anchors.left: tempImage.right
        anchors.verticalCenter: tempImage.verticalCenter

        text: qsTr("Temperature: ")
        font.pointSize: 16;
        //font.bold: true
        color: "#7a7b7a"
    }

    Text {
        id: temp
        anchors.left: tempLabel.right
        anchors.verticalCenter: tempImage.verticalCenter

        text: brickController.temperature.toFixed(1) + qsTr("°C")
        font.pointSize: 16
        //font.bold: true
        color: "#7a7b7a"
    }

    states:  State {
        name: "Details"
        when: mainPage.details

        AnchorChanges{

            target: tempImage

            anchors.top: undefined
            anchors.left: undefined

            anchors.right: temp.left
            anchors.verticalCenter: temp.verticalCenter
        }

        AnchorChanges{
            target: temp

            anchors.left: undefined

            anchors.verticalCenter: mainPage.verticalCenter
            anchors.horizontalCenter: mainPage.horizontalCenter
        }

        PropertyChanges {
            tempImage{
                //y: mainPage.parent.height/2
                width:50
                height: 50
            }

            temp{
                color: "#000000"
                font.pointSize: 48
            }

            tempLabel{
                opacity: 0
                enabled: false
            }

        }
    }


    transitions: Transition {
        // Make the state changes smooth
        ParallelAnimation {
            AnchorAnimation{duration: 200}
            ColorAnimation { property: "color"; duration: 200 }
            NumberAnimation { duration:200; properties: "detailsOpacity,x,z,y,contentY,height,width,opacity, font.pointSize" }
        }
    }
}
