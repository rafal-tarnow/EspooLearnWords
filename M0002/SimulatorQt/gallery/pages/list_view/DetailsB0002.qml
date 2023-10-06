import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Backend
import "./M0002"


Item{

    M0002Swipe{
        id: swipe

        interactive: recipe.detailsEnabled
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        clip: true

    }

    PageIndicator {
        opacity: recipe.detailsOpacity
        enabled: recipe.detailsEnabled
        count: swipe.count
        currentIndex: swipe.currentIndex
        anchors.bottom: swipe.bottom
        anchors.horizontalCenter: swipe.horizontalCenter
    }
}
