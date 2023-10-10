import QtQuick
import Backend
import "./common"
import "./T0002"


Item{
    id: t0002Delegate
    anchors.fill: parent

    property var brickController : T0002ControllerExt{
    }

    BrickTitle{
        id: title
        anchors.top: t0002Delegate.top
        anchors.right: t0002Delegate.right
        anchors.left: t0002Delegate.left
        height: 50

        connectedIcon: "qrc:/images/t0002_connected.svg"
        disconnectedIcon: "qrc:/images/t0002_disconnected.svg"
    }

    BrickSwipe{
        id: swipe
        anchors.top: title.bottom
        anchors.right: t0002Delegate.right
        anchors.left: t0002Delegate.left
        anchors.bottom: t0002Delegate.bottom
        clip: true
    }

}
