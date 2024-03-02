import QtQuick
import Qt5Compat.GraphicalEffects

Rectangle{
    id: baseRect

    property alias source: backgroundImage.source
    property alias title: title.text
    property alias description: description.text

    height: width/1.7
    color: generatePastelColor()
    layer.enabled: true
    layer.effect: OpacityMask{
        maskSource: Rectangle{
            width: baseRect.width
            height: baseRect.height
            radius: 10
        }
    }

    function generatePastelColor() {
        var r = Math.floor(Math.random() * 106) + 150
        var g = Math.floor(Math.random() * 106) + 150
        var b = Math.floor(Math.random() * 106) + 150
        return "#" + r.toString(16) + g.toString(16) + b.toString(16)
    }

    Image{
        id: backgroundImage
        anchors.fill: baseRect
    }

    Rectangle{
        id: semiblackRect
        height: baseRect.height - title.y + title.anchors.topMargin
        anchors.bottom: baseRect.bottom
        anchors.left: baseRect.left
        anchors.right: baseRect.right
        color: "#99000000"
    }


    Text{
        id: title
        anchors.bottom: description.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: baseRect.height*0.04
        anchors.rightMargin: baseRect.height*0.05
        anchors.leftMargin: baseRect.height*0.05
        font.pointSize: baseRect.height*0.10
        text: "KIKO K0002"
        color: "white"
    }

    Text{
        id: description
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: baseRect.height*0.05
        wrapMode: Text.WordWrap
        font.pointSize: baseRect.height*0.042
        text: "Best module off all world"
        color: "white"
    }
}
