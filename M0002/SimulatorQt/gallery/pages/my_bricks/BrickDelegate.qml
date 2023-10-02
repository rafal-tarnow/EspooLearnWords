import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects

ItemDelegate {
    id: delegate
    checkable: true
    property bool brickConnected: false
    property string brickName: model.name
    property string brickId: model.id
    property real brickTempCelsius: 0.0
    property real brickTempFahrenheit: 0.0

    //        Rectangle {
    //            id: backgroundRect
    //            z: -1
    //            width: delegate.width // Dopasuj szerokość do delegata
    //            height: delegate.height // Dopasuj wysokość do delegata
    //            color: "red" // Ustaw kolor na czerwony
    //            radius: 10
    //        }


//    MultiEffect {
//        source: delegate
//        anchors.fill: delegate
//        shadowEnabled: true
//        shadowHorizontalOffset: 5
//        shadowVerticalOffset: 5

//    }


    contentItem:





        ColumnLayout{
        spacing: 10

        RowLayout{
            Image{
                id: connectionStatus
                Layout.alignment: Qt.AlignVCenter
                fillMode: Image.PreserveAspectFit
                //sourceSize: Qt.size(width,height)
                source: brickConnected ? "qrc:/images/connected_icon.svg" : "qrc:/images/disconnected_icon.svg"
            }

            Label {
                text: model.name
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
                Layout.leftMargin: 10
            }

            Label {
                text: settings.temperatureScale === 'Fahrenheit' ? (brickConnected ? (brickTempFahrenheit.toFixed(1) + '℉') : ('--℉')) : (brickConnected ? (brickTempCelsius.toFixed(1) + '℃') : ('--℃'))//'℉' : '℃'
                font.bold: true
                elide: Text.ElideRight
            }

            ToolButton {
                id: goToBrickButton
                visible: true
                action: goToBrick
                Image{
                    anchors.horizontalCenter: goToBrickButton.horizontalCenter
                    anchors.verticalCenter: goToBrickButton.verticalCenter
                    width:goToBrickButton.width*0.5
                    height: goToBrickButton.height*0.5
                    fillMode: Image.PreserveAspectFit
                    sourceSize: Qt.size(width,height)
                    source: "qrc:/images/go-forward2.svg"
                }
            }

        }

        GridLayout {
            id: grid
            visible: false

            columns: 2
            rowSpacing: 10
            columnSpacing: 10

            Label {
                text: qsTr("Brick type:")
                Layout.leftMargin: 60
            }

            Label {
                text: model.type
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Brick id:")
                Layout.leftMargin: 60
            }

            Label {
                text: model.id
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Brick IP:")
                Layout.leftMargin: 60
            }

            Label {
                text: ""
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
        }
    }



    Action {
        id: goToBrick
        //icon.source: "qrc:/images/go-forward2.svg"
        onTriggered: {
            //stackView.pushPage("qrc:/pages/ConfigM0002Page.qml", "Config Device",{ "ipAddress": ipLabel.text });
        }
    }

    states: [
        State {
            name: "expanded"
            when: delegate.checked

            PropertyChanges {
                // TODO: When Qt Design Studio supports generalized grouped properties, change to:
                //       grid.visible: true
                target: grid
                visible: true
            }
        }
    ]
}
