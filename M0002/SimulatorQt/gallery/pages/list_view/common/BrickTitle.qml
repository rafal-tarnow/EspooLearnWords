import QtQuick
import QtQuick.Controls

Item{
    property string connectedIcon
    property string disconnectedIcon

    Image {
        id: connectionImage

        x: 0
        y: 0
        width: 50
        height: 50

        fillMode: Image.PreserveAspectFit
        sourceSize: Qt.size(width,height)
        source: recipe.connected ? connectedIcon : disconnectedIcon
    }

    Text {
        id: brickNameTitle
        anchors.verticalCenter: connectionImage.verticalCenter
        anchors.leftMargin: 10
        anchors.left: connectionImage.right
        anchors.right: closeToolButton.left
        anchors.rightMargin: 10
        text: model.brickName

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

        width:50
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
                recipe.state = '';
                recipe.swipePageIndex = 1;
                recipe.swipePageIndex = 0;
            }
        }
    }

}
