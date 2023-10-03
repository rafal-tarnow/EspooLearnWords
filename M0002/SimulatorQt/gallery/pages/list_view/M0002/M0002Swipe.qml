import QtQuick
import QtQuick.Controls

Pane {
    id: pane

    SwipeView {
        id: view
        currentIndex: 0
        anchors.fill: parent



        Item {
            id: firstPage
            Label {
                id: tempLabel
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                //text aligment
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                text: qsTr("24°C")
                //font.bold: true;
                font.pointSize: 96
            }

//            Rectangle{
//                z: -1
//                anchors.fill: tempLabel
//                color: "red"

//                border.color: "blue"
//                border.width: 1
//            }
        }
        Item {
            id: secondPage
//            Rectangle{
//                anchors.fill: secondPage
//                color: "yellow"

//                border.color: "orange"
//                border.width: 1
//            }
        }
    }

    PageIndicator {
        count: view.count
        currentIndex: view.currentIndex
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
