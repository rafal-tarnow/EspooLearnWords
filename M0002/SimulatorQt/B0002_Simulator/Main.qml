import QtQuick
import QtQuick.Window
import QtQuick.Controls
import Backend

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")


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
}
