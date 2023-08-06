// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend

Page {
    id: page
    property int controlsHeight: 40

    ListModel {
        id: asciModel
    }

    ListModel {
        id: hexModel
    }

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

        UdpTerminal{
            id: udpTerminal
            property int messageIndex: 0
            onDataReceived: {
                logMessageIndex(messageIndex++, data)
                lastDatagramLabel.text = qsTr("Data from: ") + formIp + ":" + fromPort
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.topMargin: 10
            Layout.preferredHeight: page.controlsHeight
            TextField {
                id: portField
                Layout.fillWidth: true
                Layout.preferredHeight: page.controlsHeight
                placeholderText: qsTr("Enter Port")
            }
            Button {
                id: bindButton
                Layout.preferredHeight: page.controlsHeight
                text: qsTr("Bind")
                onClicked: {
                    udpTerminal.bind(portField.text)
                    bindButton.highlighted = true
                    logAsciMessage("[Bind to port: " + portField.text + "]")
                }
            }
            Button {
                text: qsTr("Close")
                Layout.preferredHeight: page.controlsHeight
                onClicked: {
                    bindButton.highlighted = false
                    udpTerminal.close()
                    logAsciMessage("[UDP socket closed]")
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

        Label {
            id: lastDatagramLabel
            Layout.fillWidth: true
            text: qsTr("Data from:")
            font.pixelSize: 12
            color: "gray"
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.preferredHeight: page.controlsHeight
            Switch {
                id: broadcast
                text: qsTr("Broadcast")
            }
            TextField {
                id: ipToSend
                Layout.fillWidth: true
                Layout.preferredHeight: page.controlsHeight
                placeholderText:qsTr("IP send address")
                readOnly: broadcast.checked
                color: broadcast.checked ? "gray" : "black"
            }
            TextField {
                id: portToSend
                Layout.fillWidth: true
                Layout.preferredHeight: page.controlsHeight
                placeholderText:qsTr("send port")
            }

        }

        RowLayout{
            Layout.fillWidth: true
            Layout.bottomMargin: 5
            TextField {
                id: dataToSend
                Layout.fillWidth: true
                placeholderText:tabBar.currentIndex === 0 ? qsTr("ASCI data to send") : qsTr("HEX data to send");
            }
            Button {
                id: sendButton
                text: qsTr("Send")
                onClicked: {
                    if(tabBar.currentIndex === 0){
                        if(broadcast.checked){
                            udpTerminal.sendBroadcast(dataToSend.text, portToSend.text)
                        }else{
                            udpTerminal.send(dataToSend.text, ipToSend.text, portToSend.text)
                        }
                    }else if(tabBar.currentIndex === 1){
                        if(broadcast.checked){
                            udpTerminal.sendBroadcastHexString(dataToSend.text, portToSend.text)
                        }else{
                            udpTerminal.sendHexString(dataToSend.text, ipToSend.text, portToSend.text)
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
