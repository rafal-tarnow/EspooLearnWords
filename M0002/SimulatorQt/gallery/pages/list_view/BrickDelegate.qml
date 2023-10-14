import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "."
import "./M0002"
import "./common"

ItemDelegate {
    id: brickDelegate

    property real detailsOpacity : 0
    property bool detailsEnabled: false
    property bool connected: false
    property int swipePageIndex: 0

    width: listView.width
    //height: listView.height


    background: Rectangle {
        id: background
        anchors.fill: parent
        color: "#ffffff"
        border.color: "#e0e0e0"
        radius: 5
    }

    MouseArea {
        anchors.fill: background
        onClicked: brickDelegate.state = 'Details';
    }

    Loader {
        id: content
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.bottomMargin: 5

        source: {
            if (model.brickType === "T0002") {
                return "T0002Delegate.qml"
            } else if (model.brickType === "B0002") {
                return "B0002Delegate.qml"
            }
        }

        Component.onCompleted: {
            if (model.brickType === "T0002") {
                brickDelegate.height = 200
            } else if (model.brickType === "B0002") {
                brickDelegate.height = 120
            }
        }

        Binding {
            target: content.item
            property: "details"
            value: detailsEnabled
        }
    }

    states: State {
        name: "Details"

        PropertyChanges {
            background.color: "white"

            brickDelegate {
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
            brickDelegate.ListView.view.contentY: brickDelegate.y
            explicit: true;
        }

        // Disallow flicking while we're in detailed view
        PropertyChanges {
            brickDelegate.ListView.view.interactive: false
        }
    }

    transitions: Transition {
        // Make the state changes smooth
        ParallelAnimation {
            NumberAnimation { duration: 250; properties: "detailsOpacity,x,z,contentY,height,width" }
        }
    }

}
