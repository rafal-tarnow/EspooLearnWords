import QtQuick
import QtQuick.Controls

Item {
    id: mainPage
    property bool details: recipe.detailsEnabled


    Image {
        id: tempImage
        x: 0
        y: 0
        width: 20
        height: 20

        fillMode: Image.PreserveAspectFit
        sourceSize: Qt.size(width,height)
        source: "qrc:/images/temperature.svg"
    }

    Text {
        id: tempLabel
        x:20
        y:0
        text: qsTr("Temperature: ")
        font.pointSize: 16;
        //font.bold: true
        color: "#7a7b7a"
    }

    Text {
        id: temp
        x:tempLabel.x + tempLabel.width + 10
        y: 0
        text: brickController.temperature.toFixed(1) + qsTr("°C")
        font.pointSize: 16
        //font.bold: true
        color: "#7a7b7a"
    }

    states:  State {
        name: "Details"
        when: mainPage.details

        PropertyChanges {
            tempImage{
                y: mainPage.parent.height/2
                width:50
                height: 50
            }
            temp{
                color: "#000000"
                x: 60
                y: mainPage.height/2
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
            ColorAnimation { property: "color"; duration: 200 }
            NumberAnimation { duration:200; properties: "detailsOpacity,x,z,y,contentY,height,width,opacity, font.pointSize" }
        }
    }
}
