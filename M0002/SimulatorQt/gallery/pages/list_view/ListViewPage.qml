import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import QtQuick.Dialogs
import Backend
import "."
import "./common"

Page {
    id: page

    property string pageName: "Dashboard"
    readonly property MyBricksList myBricks: backend.myBricks
    readonly property PseudoDNSServer dnsServer: backend.dns

    background: Rectangle{
        color: "#fafafa"
    }

    Connections {
        target: dnsServer
        function onHostFound(hostId, hostType, hostName, hostIp){
            console.log("Qml : on host found: yeyeye");
        }
    }

    Menu{
        id: deleteMenu

        property int indexToDelete
        property string brickNameToDelete

        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        modal: true

        Label {
            padding: 10
            font.bold: true
            width: parent.width
            horizontalAlignment: Qt.AlignHCenter
            text: deleteMenu.brickNameToDelete
        }
        MenuItem {
            text: qsTr("Remove ...")
            onTriggered: deleteWarningDialog.open()
        }

        WarningDialog {
            id: deleteWarningDialog
            parent: listView
            anchors.centerIn: parent
            width: parent.width > 265 ? 265 : parent.width
            standardButtons: Dialog.Yes | Dialog.No
            imageSource: "qrc:/images/warning_red.svg"
            warningText: qsTr("The brick will be removed, are you sure?")
            onAccepted: {
                myBricks.remove(deleteMenu.indexToDelete)
            }
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

        model: myBricks

        delegate: BrickDelegate{
            m_brickId: brickId
            m_brickType: brickType
            m_brickName: brickName
            width: listView.width
            onPressAndHold:{
                console.log("BrickDelegate Press and hold index = ", index)
                deleteMenu.brickNameToDelete = brickName
                deleteMenu.indexToDelete = index
                deleteMenu.open()
            }
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
            if(backend.myBricks.append(brickId, brickType, brickName) === false){
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
        Material.background: Material.Pink
        onClicked: {
            addBrickDialog.open()
        }
    }

    // RoundButton {
    //     id: runButton
    //     text: qsTr("Run")
    //     highlighted: true
    //     anchors.margins: 10
    //     anchors.right: plusButton.left
    //     anchors.bottom: parent.bottom
    //     onClicked: {
    //         dnsServer.startQueriesForAllHosts()
    //     }
    // }

    // RoundButton {
    //     id: stopButton
    //     text: qsTr("Stop")
    //     highlighted: true
    //     anchors.margins: 10
    //     anchors.right: runButton.left
    //     anchors.bottom: parent.bottom
    //     onClicked: {
    //         dnsServer.stopQueries()
    //     }
    // }
}
