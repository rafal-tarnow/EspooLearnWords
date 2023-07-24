// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend

Page {
    id: page

    ColumnLayout {
        spacing: 5
        anchors.fill: parent

        UdpTerminal{
            id: udpTerminal
            onDataReceived: {
                console.log("Received data:", data)
                logTextArea.append(data)
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

                swipeView.itemAt(0).children[0].children[0].append("Sialala")
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

                    ScrollView {
                        id: view
                        anchors.fill: parent

                        TextArea {
                            id: logTextArea
                            anchors.fill: parent
                            readOnly: true
                            wrapMode: TextEdit.Wrap
                            textFormat: Qt.RichText
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
