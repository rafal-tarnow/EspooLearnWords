import QtQuick
import Backend
import QtQuick.Controls
import "./common"
import "./T0002"


Item{
    id: delegate
    anchors.fill: parent

    property int pageIndex: 0
    property bool details: false
    property string brickIdFromModel
    property string brickNameFromModel
    readonly property T0002Controller brickController: backend.getT0002Controller(brickIdFromModel)

    BrickTitle{
        id: title
        anchors.top: delegate.top
        anchors.right: delegate.right
        anchors.left: delegate.left
        height: 50

        title: brickController.name
        connectedIcon: "qrc:/images/t0002_connected.svg"
        disconnectedIcon: "qrc:/images/t0002_disconnected.svg"
        connected: brickController.connected
        details: parent.details

        onCloseButton: {
            brickDelegate.state = '';
            brickDelegate.swipePageIndex = 1;
            brickDelegate.swipePageIndex = 0;
            delegate.pageIndex = 0;
            swipe.currentIndex = 0;
        }
    }

    SwipeView{
        id: swipe
        anchors.top: title.bottom
        anchors.right: delegate.right
        anchors.left: delegate.left
        anchors.bottom: delegate.bottom
        clip: true
        currentIndex: pageIndex
        interactive: delegate.details

        Loader {
            id: loader
            source: "/pages/list_view/T0002/T0002MainPage.qml"
            Binding {
                target: loader.item
                property: "details"
                value: delegate.details
            }
        }

        Loader {
            source: "/pages/list_view/common/BrickInfoPage.qml"
        }

        Loader {
            source: "/pages/list_view/common/NetworkSettings.qml"
        }
    }

    PageIndicator {
        opacity: delegate.details ? 1 : 0
        enabled: delegate.details
        count: swipe.count
        currentIndex: swipe.currentIndex
        anchors.bottom: delegate.bottom
        anchors.horizontalCenter: delegate.horizontalCenter
    }
}
