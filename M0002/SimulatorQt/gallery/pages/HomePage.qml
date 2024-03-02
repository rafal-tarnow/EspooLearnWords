import QtQuick
import QtQuick.Controls
import "./controls"


Pane{
    property string pageName: qsTr("KIKO Bricks")
    padding: 0
    background: Rectangle{
        color: "#fafafa"
    }

    Flickable{
        id: bricksFlickable
        anchors.fill: parent
        contentHeight: pane.height
        boundsBehavior: Flickable.StopAtBounds
        ScrollIndicator.vertical: ScrollIndicator { }
        Component.onCompleted: {
            bricksFlickable.flickDeceleration = bricksFlickable.flickDeceleration/5;
            //bricksFlickable.maximumFlickVelocity = bricksFlickable.maximumFlickVelocity/2
        }

        Pane {
            id: pane
            width: parent.width

            background: Rectangle{
                color: "#fafafa"
            }

            Column{
                width: parent.width
                spacing: 15

                BrickTile{
                    width: parent.width
                    source: "qrc:/images/bricks/K0002_transparent.png"
                    title: "KIKO K0002"
                    description: "Unlock real-time temperature insights with our cutting-edge IoT module, powered by DS18B20"
                }
                BrickTile{
                    width: parent.width
                    source: "qrc:/images/bricks/K0003_transparent.png"
                    title: "KIKO K0003"
                    description: "Transform your space into a smart environment with our cutting-edge WiFi IoT module, capable of managing five relays for versatile applications."
                }
                BrickTile{
                    width: parent.width
                    source: "qrc:/images/bricks/K0004_transparent.png"
                    title: "KIKO K0004"
                    description: "Empower your DIY projects with our compact WiFi IoT module, offering precise control over a single relay for versatile applications."
                }

            }
        }
    }

    RoundButton {
        id: plusButton
        text: "→"
        highlighted: true
        anchors.margins: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Material.background: Material.Pink
        onClicked: {
            stackView.replace("qrc:/pages/list_view/ListViewPage.qml")
        }
    }
}
