import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "./M0002"


Item{

    M0002Controller{
        id: brickController

        Component.onCompleted: {
            dnsServer.onHostFound.connect(brickController.onHostFoundHandler)
            tryConnect();
        }

        onBrickConnected: {
            brickController.cmdGetInfo();
        }

        onBrickTcpErrorOccurred: {
            if(!connected){
                tryConnect();
            }
        }

        onBrickConnectedChanged: {
            recipe.connected = connected;
            if(!connected){
                tryConnect();
            }
        }

        onBrickInfo: function onInfo(id, type, name, ssid, pwd){
            if(model.brickId === id){
                if(model.brickName !== name){
                    myBricksModel.set(index, id, type, name)
                }
            }else{
                brickController.disconnectFromBrick()
            }
        }

        function tryConnect(){
            console.log("tryConnect()");
            var ipaddr = dnsServer.getIpById(model.brickId)
            if(ipaddr !== "")
                connectToBrick(ipaddr)
        }

        function onHostFoundHandler(hostId, hostType, hostName, hostIp) {
            console.log("onHostFoundHandler()");
            if (isBrickConnected() === false && model.brickId === hostId) {
                tryConnect();
            }
        }
    }

    Column{
        id: data

        opacity: !recipe.detailsOpacity
        enabled: !recipe.detailsEnabled

        anchors.top: parent.top

        Row{
            Image {
                width: 20
                height: 20

                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(width,height)
                source: "qrc:/images/temperature.svg"
            }


            Text {
                text: qsTr("Temperature: ") + brickController.temperature + qsTr("'C")
                font.pointSize: 16;
                //font.bold: true
                color: "#7a7b7a"
            }
        }
    }

    M0002Swipe{
        opacity: recipe.detailsOpacity
        enabled: recipe.detailsEnabled

        anchors.top: data.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        clip: true
    }
}
