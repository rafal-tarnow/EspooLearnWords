// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend

ScrollablePage {
    id: page

    function logConnected() {
        logText("Connected to server", "green");
    }

    function logDisconnected() {
        logTextArea.text += "Disconnected from server\n"
        logTextArea.cursorPosition = logTextArea.text.length
    }

    function logPingResponseTime(responseTime) {
        logTextArea.text += "Ping Response Time: " + responseTime + " us\n"
        logTextArea.cursorPosition = logTextArea.text.length
    }

    function logText(message, color) {
        logTextArea.text += "<font color='" + color + "'>" + message + "</font>\n";
        logTextArea.cursorPosition = logTextArea.text.length;
    }



    Column {
        spacing: 5
        width: parent.width

        TcpTerminal{
            id: tcpTerminal
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


        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("Button presents a push-button that can be pushed or clicked by the user. "
                       + "Buttons are normally used to perform an action, or to answer a question.")
        }

        ColumnLayout {
            width: parent.width
            spacing: 5
            anchors.horizontalCenter: parent.horizontalCenter

            Label {
                Layout.fillWidth: true
                wrapMode: Label.Wrap
                horizontalAlignment: Qt.AlignHCenter
                text: qsTr("IP address")
            }
            TextField {
                id: ipAddressField
                Layout.fillWidth: true
                placeholderText: qsTr("Enter IP Address")
            }
            TextField {
                id: portField
                Layout.fillWidth: true
                placeholderText: qsTr("Enter Port")
            }
            Button {
                text: "Connect"
                Layout.fillWidth: true
                onClicked: {
                    tcpTerminal.connectToServer(ipAddressField.text, portField.text)
                }
            }
            Button {
                text: "Disconnect"
                Layout.fillWidth: true
                onClicked: {
                    tcpTerminal.disconnectFromServer()
                }
            }
            Button {
                text: "Ping"
                Layout.fillWidth: true
                onClicked: {
                    tcpTerminal.sendPing()
                }
            }
            Button {
                text: qsTr("Clear console")
                Layout.fillWidth: true
                onClicked: {
                    logTextArea.clear()
                }
            }
            TextArea {
                id: logTextArea
                readOnly: true
                wrapMode: TextEdit.Wrap
                textFormat: TextEdit.PlainText
            }
        }
    }
}
