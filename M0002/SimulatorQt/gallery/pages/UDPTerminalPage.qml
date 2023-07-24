// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend

Page {
    id: page

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
        asciModel.append({ text: message })
        hexModel.append({ text: convertToHex(message)})

        //scroll messages to last one
        swipeView.itemAt(0).children[0].children[0].currentIndex = asciModel.count - 1
        swipeView.itemAt(1).children[0].children[0].currentIndex = hexModel.count - 1
    }

    ColumnLayout {
        spacing: 5
        anchors.fill: parent

        UdpTerminal{
            id: udpTerminal
            onDataReceived: {
                logMessage(data)
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.topMargin: 10
            TextField {
                id: portField
                Layout.fillWidth: true
                placeholderText: qsTr("Enter Port")
            }
            Button {
                id: bindButton
                text: qsTr("Bind")
                onClicked: {
                    udpTerminal.bind(portField.text)
                    bindButton.highlighted = true
                }
            }
        }
        Button {
            text: qsTr("Close")
            Layout.fillWidth: true
            onClicked: {
                bindButton.highlighted = false
                udpTerminal.close()
            }
        }

        SwipeView {
            id: swipeView
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

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
                            //textFormat: Qt.RichText
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
