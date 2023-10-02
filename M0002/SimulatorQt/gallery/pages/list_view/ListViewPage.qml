import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "."

Page {
    id: page
    background: Rectangle{
        color: "yellow"
    }

    PseudoDNSServer{
        id: dnsServer
        onHostFound:  function (hostId, hostType, hostName, hostIp) {
            console.log("DNS host found " + hostId + " " + hostType + " " + hostName + " " + hostIp)
        }
        Component.onCompleted: {
            dnsServer.startQueriesForAllHosts()
        }
    }

    //    MyBricksList{
    //        id: myBricksModel
    //    }

    ListView {
        id: listView
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        width: parent.width > 700 ? 700 : parent.width

        model:     MyBricksList{
            id: myBricksModel
        }
        delegate: BrickDelegate{

        }

        //columnCount: 3

        //cellWidth: 100; cellHeight: 100


    }







}
