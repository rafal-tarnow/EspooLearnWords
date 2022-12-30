import QtQuick
import QtQuick.Window
import QtQuick.Controls 2.12
import QtQuick.Dialogs
import "colorservice.js" as Service



Window {
    id: mWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    GridView {
        id : gridView
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: buttonRead.top
        cellWidth: 80; cellHeight: 80

        model: GridModel {}
        delegate: Column {
            Image {
                anchors.top: parent.top
//                anchors.left: parent.left
//                anchors.right: parent.right
//                anchors.bottom: gridText.top
                width: 80
                height: 80
                source: portrait;

            }
            Text {
                id: gridText
                anchors.bottom: parent.bottom
                text: name;
            }
        }
    }



    Button {
        id: buttonRead
        anchors.bottom: parent.bottom
        text: 'Read Colors'
        onClicked: {
            Service.getColors(function(response) {
                print('handle get colors response: ' + JSON.stringify(response))
                gridView.model.clear()
                const entries = response.data
                for(let i=0; i<entries.length; i++) {
                    gridView.model.append(entries[i])
                }
            })
        }
    }

    Button {
        id: buttonNewColour
        anchors.bottom: parent.bottom
        anchors.left: buttonRead.right
        anchors.right: parent.right
        text: 'New Color'
        onClicked: {
            colorDialog.open()
//            Service.getColors(function(response) {
//                print('handle get colors response: ' + JSON.stringify(response))
//                gridView.model.clear()
//                const entries = response.data
//                for(let i=0; i<entries.length; i++) {
//                    gridView.model.append(entries[i])
//                }
//            })
        }
    }

    Dialog {
        id: colorDialog
        title: "Please choose a color"
        anchors.horizontalCenter: parent.horizontalCenter
        onAccepted: {
            console.log("You chose: " + colorDialog.color)
//            Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
//            Qt.quit()
        }
        Component.onCompleted: visible = false
    }
}

