import QtQuick
import QtQuick.Controls

ItemDelegate {
    id: brickDelegate

    property int expandHeight: 300
    height: 200
    signal itemClicked


    background: Rectangle {
        id: background
        anchors.fill: parent
        color: "#ffffff"
        border.color: "#e0e0e0"
        radius: 5
    }

    MouseArea {
        anchors.fill: background
        //onClicked: brickDelegate.state = 'Details';
        onClicked: {
            console.log("NewT0002Delegate clicked !!")
            brickDelegate.state = 'Details'
        }
    }

    states: [
        State{
            name: "Details"

            PropertyChanges {
                brickDelegate{
                    height: expandHeight
                }
            }

            PropertyChanges {
                brickDelegate.ListView.view.contentY: brickDelegate.y
                explicit: true;
            }
        }
    ]

    transitions: Transition {
        // Make the state changes smooth
        ParallelAnimation {
            NumberAnimation { duration: 250; properties: "height,contentY" }
        }
    }
}
