import QtQuick
import Backend
import "./common"
import "./T0002"


Item{
    id: t0002Delegate
    anchors.fill: parent

    property int pageIndex: 0
    property bool details: false

    Component.onCompleted:  {
        console.log("T0002Completed !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
    }

    property var brickController : T0002ControllerExt{
    }

    BrickTitle{
        id: title
        anchors.top: t0002Delegate.top
        anchors.right: t0002Delegate.right
        anchors.left: t0002Delegate.left
        height: 50

        title: brickName //model.brickName
        connectedIcon: "qrc:/images/t0002_connected.svg"
        disconnectedIcon: "qrc:/images/t0002_disconnected.svg"
        connected: brickController.connected
        details: parent.details

        onCloseButton: {
            brickDelegate.state = '';
            brickDelegate.swipePageIndex = 1;
            brickDelegate.swipePageIndex = 0;
            t0002Delegate.pageIndex = 0;
            swipe.setSwipeIndex(0);
        }
    }

    BrickSwipe{
        id: swipe
        anchors.top: title.bottom
        anchors.right: t0002Delegate.right
        anchors.left: t0002Delegate.left
        anchors.bottom: t0002Delegate.bottom
        clip: true
        swipeIndex: pageIndex
        details: t0002Delegate.details
    }
}
