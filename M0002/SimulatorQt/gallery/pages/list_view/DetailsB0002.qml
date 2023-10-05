import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "./M0002"


Item{

//    background: Rectangle{
//        color: "green"
//    }
    M0002ControllerExt{
        id:brickController
    }




    M0002Swipe{
        id: swipe
        opacity: recipe.detailsOpacity
        enabled: recipe.detailsEnabled

        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        clip: true

//        background: Rectangle{
//            color: "orange"
//        }
    }

    PageIndicator {
        count: swipe.count
        currentIndex: swipe.currentIndex
        anchors.bottom: swipe.bottom
        anchors.horizontalCenter: swipe.horizontalCenter
    }
}
