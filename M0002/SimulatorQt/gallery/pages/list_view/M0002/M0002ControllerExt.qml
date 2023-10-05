import QtQuick
import Backend

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
