import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "."

Page {
    id: page
    property string pageName: "Dashboard"

    background: Rectangle{
        color: "#fafafa"
    }


    ListModel {
        id: devicesModel
    }

    PseudoDNSServer{
        id: dnsServer
        onHostFound:  function (hostId, hostType, hostName, hostIp) {
            console.log("DNS host found " + hostId + " " + hostType + " " + hostName + " " + hostIp)
            devicesModel.append({ name: hostName, ip: hostIp})
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
            devicesModel.clear()
            dnsServer.startQueriesForAllHosts()
        }

        onClosed: {
            console.log("Dialog closed()")
               dnsServer.stopQueries()
        }

        ListView{
            id: dialogView
            anchors.fill: parent

            model: devicesModel

            delegate: ItemDelegate{
                id: delegate
                width: parent.width
                height: 50

                contentItem: Label {
                    text: name
                    font.bold: true
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }
            }
        }
    }

    RoundButton {
        text: qsTr("+")
        highlighted: true
        anchors.margins: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: {
            dialog.open()
        }
    }
}
