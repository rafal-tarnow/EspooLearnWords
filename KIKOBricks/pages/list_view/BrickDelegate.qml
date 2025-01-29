import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "."
import "./M0002"
import "./K0004"
import "./K0007"
import "./common"

ItemDelegate {
    id: brickDelegate
    property real detailsOpacity : 0
    property bool detailsEnabled: false
    property bool connected: false
    property int swipePageIndex: 0
    property string m_brickId
    property string m_brickType
    property string m_brickName


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
        onPressAndHold: {
            brickDelegate.pressAndHold()
        }
    }

    Loader {
        id: content
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.bottomMargin: 5

        source: {
            if (m_brickType === "T0002") {
                return "T0002Delegate.qml"
            } else if(m_brickType === "J0001") {
                return "J0001Delegate.qml"
            }else if (m_brickType === "K0002") {
                return "K0002Delegate.qml"
            } else if (m_brickType === "K0004") {
                return "K0004Delegate.qml"
            }else if (m_brickType === "K0007") {
                return "K0007Delegate.qml"
            }
        }

        Component.onCompleted: {
            if (m_brickType === "T0002") {
                brickDelegate.height = 200
            } else if (m_brickType === "J0001") {
                brickDelegate.height = 160
            } else if (m_brickType === "K0002") {
                brickDelegate.height = 120
            }else if (m_brickType === "K0004") {
                brickDelegate.height = 120
            }else if (m_brickType === "K0007") {
                brickDelegate.height = 120
            }
        }

        Binding {
            target: content.item
            property: "details"
            value: detailsEnabled
        }

        Binding {
            target: content.item
            property: "brickNameFromModel"
            value: brickDelegate.m_brickName
        }

        Binding {
            target: content.item
            property: "brickIdFromModel"
            value: brickDelegate.m_brickId
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
