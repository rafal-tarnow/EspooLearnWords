import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "."

Component {
    id: brickDelegate

    Item {
        id: recipe

        property real detailsOpacity : 0
        property bool detailsEnabled: false
        property bool connected: false

        width: listView.width
        //height: 70

        //width: 100
        //height: 100


        Rectangle {
            id: background
            x: 0; y: 0; width: parent.width; height: parent.height
            color: "ivory"
            border.color: "orange"
            radius: 5

        }

        MouseArea {
            anchors.fill: background
            onClicked: recipe.state = 'Details';
        }

        Row {
            id: topLayout

            //                    x: 0;
            //                    y: 0;
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.topMargin: 10
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            height: connectionImage.height;
            //width: parent.width

            spacing: 10

            Image {
                id: connectionImage

                width: 50;
                height: 50

                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(width,height)
                source: recipe.connected ? "qrc:/images/connected_icon.svg" : "qrc:/images/disconnected_icon.svg"
            }

            Text {
                width: topLayout.width - 2*10 - connectionImage.width - 10 - closeButton.width
                text: model.brickName
                font.bold: true; font.pointSize: 16
                clip: true
            }


            Button {
                id: closeButton
                //   y: 10
                width: 0
                //                        anchors { right: background.right; rightMargin: 10 }
                opacity: recipe.detailsOpacity
                text: "Close"
                enabled: recipe.detailsEnabled
                onClicked: recipe.state = '';
            }
        }

        Loader {
            x: 10; width: parent.width - 20
            anchors { top: topLayout.bottom; topMargin: 10; bottom: parent.bottom; bottomMargin: 10 }
            //opacity: recipe.detailsOpacity

            source: {
                if (model.brickType === "T0002") {
                    recipe.height = 140
                    return "DetailsT0002.qml"
                } else if (model.brickType === "B0002") {
                    recipe.height = 100
                    return "DetailsB0002.qml"
                }
            }
        }

        states: State {
            name: "Details"

            PropertyChanges {
                background.color: "white"
                connectionImage {
                    // Make picture bigger
                    width: 25
                    height: 25
                }
                closeButton{
                    width: 100
                }

                recipe {
                    // Make details visible
                    detailsOpacity: 1
                    detailsEnabled: true
                    x: 0
                    z: 2

                    // Fill the entire list area with the detailed view
                    height: listView.height
                    width: listView.width

                }
            }

            PropertyChanges {
                recipe.ListView.view.contentY: recipe.y
                explicit: true;
            }

            // Disallow flicking while we're in detailed view
            PropertyChanges {
                recipe.ListView.view.interactive: false
            }


        }

        transitions: Transition {
            // Make the state changes smooth
            ParallelAnimation {
                ColorAnimation { property: "color"; duration: 500 }
                NumberAnimation { duration: 300; properties: "detailsOpacity,x,z,contentY,height,width" }
            }
        }

    }
}
