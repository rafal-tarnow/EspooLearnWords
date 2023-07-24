import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import Backend
import "." as App

ApplicationWindow {
    id: window
    width: 360
    height: 520
    visible: true
    title: qsTr("TEST")

    //! [orientation]
    readonly property bool portraitMode: window.width < window.height


    function logConnected() {
        logTextArea.text += "Connected to server\n"
        logTextArea.cursorPosition = logTextArea.text.length
    }

    function logDisconnected() {
        logTextArea.text += "Disconnected from server\n"
        logTextArea.cursorPosition = logTextArea.text.length
    }

    function logPingResponseTime(responseTime) {
        logTextArea.text += "Ping Response Time: " + responseTime + " us\n"
        logTextArea.cursorPosition = logTextArea.text.length
    }

    Action {
        id: navigateBackAction
        icon.name: stackView.depth > 1 ? "back" : "drawer"
        onTriggered: {
            if (stackView.depth > 1) {
                stackView.pop()
                listView.currentIndex = -1
            } else {
                drawer.open()
            }
        }
    }

    TcpClient{
        id: tcpClient
        onConnected: {
            logConnected();
        }
        onDisconnected: {
            logDisconnected();
        }
        onPingRecived: {
            logPingResponseTime(delay);
        }
    }
    header: App.ToolBar{
        RowLayout {
            spacing: 20
            anchors.fill: parent
            anchors.leftMargin: !window.portraitMode ? drawer.width : undefined

            ToolButton {
                action: navigateBackAction
                visible: window.portraitMode
            }
        }
    }
    Drawer {
        id: drawer

        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height
        modal: window.portraitMode
        interactive: window.portraitMode ? (stackView.depth === 1) : false
        position: window.portraitMode ? 0 : 1
        visible: !window.portraitMode

        ListView {
            id: listView

            focus: true
            currentIndex: -1
            anchors.fill: parent

            model: ListModel {
                ListElement { title: qsTr("Button"); source: "qrc:/pages/ButtonPage.qml" }
                ListElement { title: qsTr("TCP terminal"); source: "qrc:/pages/TCPTerminalPage.qml" }
                ListElement { title: qsTr("UDP terminal"); source: "qrc:/pages/UDPTerminalPage.qml" }
            }

            delegate: ItemDelegate {
                id: delegateItem
                width: ListView.view.width
                text: title
                highlighted: ListView.isCurrentItem

                required property int index
                required property var model
                required property string title
                required property string source

                onClicked: {
                    listView.currentIndex = index
                    stackView.push(source)
                    if (window.portraitMode)
                        drawer.close()
                }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    StackView {
        id: stackView

        anchors.fill: parent
        anchors.leftMargin: !window.portraitMode ? drawer.width : undefined

        initialItem: Pane {
            id: pane

            Image {
                id: logo
                width: pane.availableWidth / 2
                height: pane.availableHeight / 2
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -50
                fillMode: Image.PreserveAspectFit
                source: "images/qt-logo.png"
            }

            Label {
                text: qsTr("Qt Quick Controls provides a set of controls that can be used to build complete interfaces in Qt Quick.")
                anchors.margins: 20
                anchors.top: logo.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: arrow.top
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                wrapMode: Label.Wrap
            }

            Image {
                id: arrow
                source: "images/arrow.png"
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                visible: window.portraitMode
            }
        }
    }

/*
    Column {
        width: parent.width

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("IP address")
        }

        TextField {
            id: ipAddressField
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: qsTr("Enter IP Address")
        }
        TextField {
            id: portField
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: qsTr("Enter Port")
        }
        Button {
            text: "Connect"
            onClicked: {
                tcpClient.connectToServer(ipAddressField.text, portField.text)
            }
        }
        Button {
             text: "Disconnect"
             onClicked: {
                 tcpClient.disconnectFromServer()
             }
         }
        Button {
            text: "Ping"
            onClicked: {
                tcpClient.sendPing()
            }
        }
        Button {
            text: qsTr("Clear console")
            onClicked: {
                logTextArea.clear()
            }
        }

        TextArea {
            id: logTextArea
//            width: 380
//            height: 200
//            anchors.top: portField.bottom
//            anchors.margins: 10
            readOnly: true
            wrapMode: TextEdit.Wrap
            textFormat: TextEdit.PlainText
        }


    }
*/
}
