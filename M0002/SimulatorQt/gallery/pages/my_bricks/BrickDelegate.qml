import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ItemDelegate {
    id: delegate
    checkable: true
    property bool brickConnected: false
    property string brickName: model.brickName
    property real brickTempCelsius: 0.0
    property real brickTempFahrenheit: 0.0
    contentItem: ColumnLayout{
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
                text: model.brickName
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
                visible: false
                action: goToBrick
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
                text: model.brickType
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
        icon.source: "qrc:/images/go-forward2.svg"
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
