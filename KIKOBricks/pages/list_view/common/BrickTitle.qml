import QtQuick
import QtQuick.Controls

Item{
    id: brickTitle
    property string title
    property bool connected: false
    property bool details
    property string connectedIcon
    property string disconnectedIcon

    signal closeButton()


    Image {
        id: connectionImage

        x: 0
        y: 0
        width: 30
        height: 30

        fillMode: Image.PreserveAspectFit
        sourceSize: Qt.size(width,height)
        source: connected ? connectedIcon : disconnectedIcon
    }

    Text {
        id: brickNameTitle
        anchors.verticalCenter: connectionImage.verticalCenter
        anchors.leftMargin: 10
        anchors.left: connectionImage.right
        anchors.right: closeToolButton.left
        anchors.rightMargin: 10
        text: title
        //horizontalAlignment: Text.AlignHCenter
        horizontalAlignment: brickNameTitle.contentWidth <= brickNameTitle.width ? Text.AlignHCenter : Text.AlignLeft
        //font.bold: true;
        font.pointSize: 24
        clip: true
    }

//    ToolButton {
//        id: controlToolButton
//        width:0
//        height: 50
//        anchors.right: closeToolButton.left
//        anchors.verticalCenter: connectionImage.verticalCenter

//        opacity: recipe.detailsOpacity
//        enabled: recipe.detailsEnabled

//        action: controlAction
//        Action {
//            id: controlAction
//            icon.source: "qrc:/images/dashboard.svg"
//            onTriggered: {
//                swipePageIndex = 1;
//                swipePageIndex = 0;
//            }
//        }
//    }

//    ToolButton {
//        id: configToolButton
//        width:50
//        height: 50
//        anchors.right: closeToolButton.left
//        anchors.verticalCenter: connectionImage.verticalCenter

////        opacity: recipe.detailsOpacity
////        enabled: recipe.detailsEnabled

//        action: configAction
//        Action {
//            id: configAction
//            icon.source: "qrc:/images/config.svg"
//            onTriggered: {
////                swipePageIndex = 0;
////                swipePageIndex = 1;
//            }
//        }
//    }

    ToolButton {
        id: closeToolButton

        width:0
        height: 50
        anchors.right: parent.right
        anchors.verticalCenter: connectionImage.verticalCenter

        //opacity: recipe.detailsOpacity
        //enabled: recipe.detailsEnabled

        action: closeAction
        Action {
            id: closeAction
            icon.source: "qrc:/images/close.svg"
            onTriggered: {
                closeButton()
            }
        }

//        transform: Scale {
//            id: closeToolScale
//            xScale: 0.0 // Skalowanie na początku do 0
//            yScale: 0.0 // Skalowanie na początku do 0
//        }
    }

    states:  State {
        name: "Details"
        when: brickTitle.details

        PropertyChanges {
            closeToolButton{
                width: 50
            }
        }

    }

    transitions: Transition {
        ParallelAnimation {
            NumberAnimation { duration:250; properties: "width" }
        }
    }

}
