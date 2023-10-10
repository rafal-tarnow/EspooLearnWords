import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "."
import "./M0002"
import "./common"

ItemDelegate {
    id: recipe

    property real detailsOpacity : 0
    property bool detailsEnabled: false
    property bool connected: false
    property int swipePageIndex: 0

    width: listView.width


    background: Rectangle {
        id: background
        anchors.fill: parent
        color: "#ffffff"
        border.color: "#e0e0e0"
        radius: 5
    }

    MouseArea {
        anchors.fill: background
        onClicked: recipe.state = 'Details';
    }

    Loader {
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
    }

    states: State {
        name: "Details"

        PropertyChanges {
            background.color: "white"
//            connectionImage {
//                // Make picture bigger
//                width: 35
//                height: 35
//            }

//            brickNameTitle{
//                anchors.rightMargin: 0
//            }

//            controlToolButton{
//                width: 50
//            }

//            configToolButton{
//                width:50
//            }

//            closeToolButton{
//                width:50
//            }

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
            ColorAnimation { property: "color"; duration: 250 }
            NumberAnimation { duration: 250; properties: "detailsOpacity,x,z,contentY,height,width" }
        }
    }

}
