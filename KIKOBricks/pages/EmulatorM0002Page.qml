// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend

Page {
    id: page

    ColumnLayout {
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter
        //anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        //height: parent.height

        EmulatorM0002{
            id: emulatorM0002
            temp: dial.value
        }

        ColumnLayout {
            Layout.topMargin: 10
            Layout.fillWidth: true
            TextField {
                id: serverName
                Layout.fillWidth: true
                placeholderText: qsTr("Server Name")
            }

            RowLayout{
                Layout.fillWidth: true
                TextField {
                    id: moduleName
                    Layout.fillWidth: true
                    placeholderText: qsTr("Module Name")
                }

                Switch {
                    text: qsTr("Start")
                    onToggled: {
                        if (checked) {
                            emulatorM0002.start(moduleName.text, serverName.text)
                        } else {
                            emulatorM0002.stop()
                        }
                    }
                }
            }
        }


        Label {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Temp: ") + dial.value.toFixed(1)
        }
        Dial {
            id: dial
            Layout.alignment: Qt.AlignHCenter
            value: 0.5
            from: -40.0
            to: 120
            stepSize: 0.1

        }

    }
}
