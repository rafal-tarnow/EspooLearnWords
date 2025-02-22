// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend

Page {
    id: page
    property int controlsHeight: 40

    function convertToHex(input) {
        var hexString = "";
        for (var i = 0; i < input.length; i++) {
            var charCode = input.charCodeAt(i).toString(16);
            hexString += "0x" + charCode + " ";
        }
        return hexString.trim();
    }

    function logMessage(message){
        logAsciMessage(message)
        logHexMessage(convertToHex(message))
    }

    function logMessageIndex(index, message){
        logAsciMessage("[" + index + "] " + message)
        logHexMessage("[" + index + "] " + convertToHex(message))
    }

    function logHexMessage(message){
        hexModel.append({ text: message})
        //scroll messages to last one
        swipeView.itemAt(1).children[0].children[0].currentIndex = hexModel.count - 1
    }

    function logAsciMessage(message){
        asciModel.append({ text: message })
        //scroll messages to last one
        swipeView.itemAt(0).children[0].children[0].currentIndex = asciModel.count - 1
    }



    ColumnLayout {
        spacing: 5
        anchors.fill: parent

        ListModel {
            id: asciModel
        }

        ListModel {
            id: hexModel
        }

        TcpTerminal{
            id: tcpTerminal
            property int messageIndex: 0
            onConnected: {
                logAsciMessage("[Connected]");
            }
            onDisconnected: {
                logAsciMessage("[Disonnected]");
            }
            onPingRecived: {
                logAsciMessage("[Ping time = " + delay + "us]");
            }
            onDataReceived: {
                logMessageIndex(messageIndex++, data)
            }
            onTcpError:{
                logAsciMessage("[" + socketError + "]")
            }
        }


        ColumnLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: page.controlsHeight
            spacing: 5

            TextField {
                id: ipAddressField
                Layout.fillWidth: true
                Layout.preferredHeight: page.controlsHeight
                placeholderText: qsTr("IP Address")
            }

            RowLayout{
                Layout.fillWidth: true
                Layout.topMargin: 10
                Layout.preferredHeight: page.controlsHeight

                TextField {
                    id: portField
                    Layout.fillWidth: true
                    Layout.preferredHeight: page.controlsHeight
                    placeholderText: qsTr("Port")
                }

                Button {
                    text: qsTr("Connect")
                    Layout.fillWidth: true
                    Layout.preferredHeight: page.controlsHeight
                    onClicked: {
                        tcpTerminal.connectToServer(ipAddressField.text, portField.text)
                    }
                }
                Button {
                    text: qsTr("Disconnect")
                    Layout.fillWidth: true
                    Layout.preferredHeight: page.controlsHeight
                    onClicked: {
                        tcpTerminal.disconnectFromServer()
                    }
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
        }

        SwipeView {
            id: swipeView
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex
            z: -1

            Repeater {
                model: 2

                Pane {
                    id: pane
                    width: SwipeView.view.width
                    height: SwipeView.view.height

                    ListView {
                        id: listView
                        anchors.fill: parent

                        delegate: Text {
                            text: model.text
                            wrapMode: TextEdit.Wrap
                        }
                        Component.onCompleted: {
                            swipeView.itemAt(0).children[0].children[0].model = asciModel
                            swipeView.itemAt(1).children[0].children[0].model = hexModel
                        }
                    }
                }
            }
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("ASCI")
        }
        TabButton {
            text: qsTr("HEX")
        }
    }
}
