import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "."

Page {
    id: page
    property string pageName: "Dashboard"

//    background: Rectangle{
//        //color: "yellow"
//        color: "#fafafa"
//    }

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

//        Rectangle {
//            z: -1
//            anchors.fill: parent
//            color: "yellow"
//            border.color: "blue"
//        }

        //columnCount: 3

        //cellWidth: 100; cellHeight: 100


    }







}
