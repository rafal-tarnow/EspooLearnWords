import QtQuick
import Backend
import "./common"
import "./M0002"


Item{
    id: b0002Delegate
    anchors.fill: parent

    property var brickController : M0002ControllerExt{

    }

    BrickTitle{
        id: title
        anchors.top: b0002Delegate.top
        anchors.right: b0002Delegate.right
        anchors.left: b0002Delegate.left
        height: 50

        connectedIcon: "qrc:/images/temp_connected.svg"
        disconnectedIcon: "qrc:/images/temp_disconnected.svg"
    }

    BrickSwipe{
        id: swipe
        anchors.top: title.bottom
        anchors.right: b0002Delegate.right
        anchors.left: b0002Delegate.left
        anchors.bottom: b0002Delegate.bottom
        clip: true
    }

}
