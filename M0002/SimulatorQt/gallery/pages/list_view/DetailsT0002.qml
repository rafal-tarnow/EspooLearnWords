import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend


Item{

    T0002Controller{
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
                text: qsTr("Temperature: 25.2'C")
                font.pointSize: 12; font.bold: true
                color: "#7a7b7a"
            }
        }

        Row{
            Image {
                width: 20
                height: 20

                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(width,height)
                source: "qrc:/images/humidity.svg"
            }
            Text {
                text: qsTr("Humidity: 75%")
                font.pointSize: 12; font.bold: true
                color: "#7a7b7a"
            }
        }
        Text {
            text: qsTr("Pressure: 1015hPa")
            font.pointSize: 12; font.bold: true
            color: "#7a7b7a"
        }
    }
}
