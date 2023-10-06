import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "./M0002"


Item{

//    T0002ControllerExt{
//        id: brickController
//    }



    Column{
        anchors.top: parent.top

        Row{
            Image {
                width: 20
                height: 20

                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(width,height)
                source: "qrc:/images/temperature.svg"
            }

            Text {
                text: qsTr("Temperature: 25.2'C")
                font.pointSize: 16;
                //font.bold: true
                color: "#7a7b7a"
            }
        }

        Row{
            Image {
                width: 20
                height: 20

                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(width,height)
                source: "qrc:/images/humidity.svg"
            }
            Text {
                text: qsTr("Humidity: 75%")
                font.pointSize: 16;
                //font.bold: true
                color: "#7a7b7a"
            }
        }
        Row{
            Image {
                width: 20
                height: 20

                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(width,height)
                source: "qrc:/images/pressure.svg"
            }
            Text {
                text: qsTr("Pressure: 1015hPa")
                font.pointSize: 16;
                //font.bold: true
                color: "#7a7b7a"
            }
        }
    }
}
