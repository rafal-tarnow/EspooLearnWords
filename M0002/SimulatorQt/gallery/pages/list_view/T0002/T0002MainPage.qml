import QtQuick
import QtQuick.Controls

Item {
    id: mainPage
    property bool details: recipe.detailsEnabled

    Column{
        id:smallView

        anchors.verticalCenter: parent.verticalCenter
        spacing: 5

        Row{

            Image {
                id: tempImage

                width: 20
                height: 20

                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(width,height)
                source: "qrc:/images/temperature.svg"
            }

            Text {
                id: tempLabel
                text: qsTr("Temperature: ")
                font.pointSize: 16;
                color: "#7a7b7a"
            }

            Text {
                id: temp

                text: brickController.temperature.toFixed(1) + qsTr("°C")
                font.pointSize: 16
                //font.bold: true
                color: "#7a7b7a"
            }
        }

        Row{
            Image {
                id: pressureImage
                width: 20
                height: 20
                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(width,height)
                source: "qrc:/images/pressure.svg"
            }

            Text {
                id: presureLabel
                text: qsTr("Pressure: ")
                font.pointSize: 16;
                color: "#7a7b7a"
            }

            Text {
                id: pressure
                text: brickController.temperature.toFixed(1) + qsTr("°C")
                font.pointSize: 16
                color: "#7a7b7a"
            }
        }

        Row{
            Image {
                id: humidityImage
                width: 20
                height: 20
                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(width,height)
                source: "qrc:/images/humidity.svg"
            }

            Text {
                id: humidityLabel
                text: qsTr("Humidity: ")
                font.pointSize: 16;
                color: "#7a7b7a"
            }

            Text {
                id: humidity
                text: brickController.temperature.toFixed(1) + qsTr("°C")
                font.pointSize: 16
                color: "#7a7b7a"
            }
        }

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


            text: brickController.temperature.toFixed(1) + qsTr("°C")
            font.pointSize: 56
            //font.bold: true

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
