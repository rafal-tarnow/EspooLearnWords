import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "."
import "./common"

Page {
    id: page
    property string pageName: "Dashboard"

    background: Rectangle{
        color: "#fafafa"
    }


    ListModel {
        id: devicesModel
    }

    Connections {
        target: Qt.application;
        function onStateChanged(inState) {
            console.log("XXXXXXXXXXXXXXXXXXXXXXXXXXX" + Qt.application.state)
        }
    }


    PseudoDNSServer{
        id: dnsServer
        onHostFound:  function (hostId, hostType, hostName, hostIp) {
            console.log("DNS host found " + hostId + " " + hostType + " " + hostName + " " + hostIp)
            devicesModel.append({ name: hostName, ip: hostIp})
        }

        Component.onCompleted: {

            devicesModel.clear()
            dnsServer.startQueriesForAllHosts()
        }


        // Component.onDestroyed: {
        //                    dnsServer.stopQueries()
        // }
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

        model:     MyBricksList{
            id: myBricksModel
        }

        delegate: BrickDelegate{

        }
    }

    Dialog {
        id: dialog
        property int gap: 20
        x: gap
        y: gap
        width: parent.width - 2*gap
        height: parent.height - 2*gap
        title: qsTr("Avaliable Bricks")
        standardButtons: Dialog.Cancel

        background: Rectangle {
            id: background
            anchors.fill: parent
            color: "#ffffff"
            border.color: "#e0e0e0"
            radius: 5
        }

        onOpened: {
            console.log("Dialog opened()")
        }

        onClosed: {
            console.log("Dialog closed()")

        }

        ListView{
            id: dialogView
            anchors.fill: parent

            model: dnsServer

            delegate: ItemDelegate{
                id: delegate
                width: parent.width
                height: 50

                contentItem: DataLine{
                    width: parent.width
                    icon: "qrc:/images/wifi.svg"
                    label: NameFromDns //model.IdFromDns
                    leftMarginValue: 0
                    //value: "test"
                    //unit: qsTr("°C")
                }
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
            dialog.open()
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
}
