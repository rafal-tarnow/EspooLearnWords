import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Dialogs
import Backend
import KikoBricks
import "."
import "./common"

Page {
    id: page
    property string pageName: "Dashboard"

    readonly property MyBricksList myBricks: application.myBricks
    readonly property PseudoDNSServer dnsServer: application.dns

    background: Rectangle{
        color: "#fafafa"
    }

    Connections {
        target: Qt.application;
        function onStateChanged(inState) {
            console.log("XXXXXXXXXXXXXXXXXXXXXXXXXXX" + Qt.application.state)
        }
    }


    Rectangle {
        width: 100
        height: 100
        z: 3
        color: "blue"

        MouseArea {
            id: dragArea
            anchors.fill: parent
            drag.target: parent
        }
    }

    BrickDelegate{
        width: 200
        height: 100
        m_brickType: myBricks.data(0,myBricks.TypeRole)
        m_brickName: myBricks.data(0,myBricks.NameRole)
        z: 3

        MouseArea {
            id: brickDrag
            anchors.fill: parent
            drag.target: parent
        }
    }

    ListView {
        id: listView
        anchors.margins: 5
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        clip:true

        spacing: 5

        width: parent.width > 700 ? 700 : parent.width - 2*5

        // model:     MyBricksList{
        //     id: application
        // }

        model: myBricks

        delegate: BrickDelegate{
            m_brickType: brickType
            m_brickName: brickName
            width: listView.width
        }

        WarningDialog {
            id: warningDialog
            anchors.centerIn: parent
            width: parent.width > 265 ? 265 : parent.width
            warningText: qsTr("The brick has already been added.")
        }
    }



    AddBrickDialog{
        id: addBrickDialog
        model: dnsServer

        onAddBrick: function(brickId, brickName, brickType){
            if(application.myBricks.append(brickId, brickType, brickName) === false){
                warningDialog.open();
            }
        }
    }

    RoundButton {
        id: plusButton
        text: qsTr("+")
        highlighted: true
        anchors.margins: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: {
            addBrickDialog.open()
        }
    }

    RoundButton {
        id: runButton
        text: qsTr("Run")
        highlighted: true
        anchors.margins: 10
        anchors.right: plusButton.left
        anchors.bottom: parent.bottom
        onClicked: {
            dnsServer.startQueriesForAllHosts()
        }
    }

    RoundButton {
        id: stopButton
        text: qsTr("Stop")
        highlighted: true
        anchors.margins: 10
        anchors.right: runButton.left
        anchors.bottom: parent.bottom
        onClicked: {
            dnsServer.stopQueries()
        }
    }

    RoundButton {
        id: minusButton
        text: qsTr("-")
        highlighted: true
        anchors.margins: 10
        anchors.right: stopButton.left
        anchors.bottom: parent.bottom
        onClicked: {
            myBricks.remove(myBricks.rowCount() - 1)
        }
    }
}
