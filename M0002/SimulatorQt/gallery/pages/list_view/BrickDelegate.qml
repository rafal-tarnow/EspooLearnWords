import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "."

ItemDelegate {
    id: recipe

    property real detailsOpacity : 0
    property bool detailsEnabled: false
    property bool connected: false
    property int swipePageIndex: 0

    width: listView.width

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#ffffff"
        //border.color: "#e0e0e0"
        border.color: "green"
        radius: 5
    }

    MouseArea {
        anchors.fill: background
        onClicked: recipe.state = 'Details';
    }


    Image {
        id: connectionImage

        x:10
        y:10
        width: 50
        height: 50

        fillMode: Image.PreserveAspectFit
        sourceSize: Qt.size(width,height)
        source: recipe.connected ? "qrc:/images/temp_connected.svg" : "qrc:/images/temp_disconnected.svg"
    }

    Text {
        id: brickNameTitle
       anchors.verticalCenter: connectionImage.verticalCenter
        anchors.leftMargin: 10
        anchors.left: connectionImage.right
        anchors.right: controlToolButton.left
        anchors.rightMargin: 10
        text: model.brickName

        //font.bold: true;
        font.pointSize: 24
        clip: true
    }

    ToolButton {
        id: controlToolButton
        width:0
        height: 50
        anchors.right: configToolButton.left
        anchors.verticalCenter: connectionImage.verticalCenter

        opacity: recipe.detailsOpacity
        enabled: recipe.detailsEnabled

        action: controlAction
        Action {
            id: controlAction
            icon.source: "qrc:/images/dashboard.svg"
            onTriggered: {
                swipePageIndex = 1;
                swipePageIndex = 0;
            }
        }
    }

    ToolButton {
        id: configToolButton
        width:0
        height: 50
        anchors.right: closeToolButton.left
        anchors.verticalCenter: connectionImage.verticalCenter

        opacity: recipe.detailsOpacity
        enabled: recipe.detailsEnabled

        action: configAction
        Action {
            id: configAction
            icon.source: "qrc:/images/config.svg"
            onTriggered: {
                swipePageIndex = 0;
                swipePageIndex = 1;
            }
        }
    }

    ToolButton {
        id: closeToolButton

        width:0
        height: 50
        anchors.right: parent.right
        anchors.verticalCenter: connectionImage.verticalCenter

        opacity: recipe.detailsOpacity
        enabled: recipe.detailsEnabled

        action: closeAction
        Action {
            id: closeAction
            icon.source: "qrc:/images/close.svg"
            onTriggered: {
                recipe.state = '';
            }
        }
    }



    //    }

    Loader {
        anchors.top: closeToolButton.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5

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
                width: 35
                height: 35
            }

            brickNameTitle{
                anchors.rightMargin: 0
            }

            controlToolButton{
                width: 50
            }

            configToolButton{
                width:50
            }

            closeToolButton{
                width:50
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
            ColorAnimation { property: "color"; duration: 200 }
            NumberAnimation { duration: 300; properties: "detailsOpacity,x,z,contentY,height,width" }
        }
    }

}
