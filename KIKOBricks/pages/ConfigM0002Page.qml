// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend

ScrollablePage {
    id: page
    property string pageName: qsTr("Config Device")

    property string ipAddress

    Component.onCompleted: {
        console.log("Config Page created")
        console.log("IP = " + ipAddress)
        connectingDialog.open()
    }
    Component.onDestruction: {
        console.log("Config Page destroyed")
    }


    Column {
        spacing: 10
        width: parent.width

        Controller{
            id: controller
            onBrickConnected: {
                console.log("Brick connected");
                connectingDialog.close();
                controller.cmdGetInfo();
                controller.cmdGetId();
                controller.cmdGetType();
                controller.cmdGetName();
                controller.cmdGetNetworkConfig();
            }
            onBrickDisconnected:{

            }
            onBrickInfo: function(id, type, name, ssid, pswd){
                console.log("QQQQQQQQQ" + " onBrickInfo()");
                console.log(id)
                console.log(type)
                console.log(name)
                console.log(ssid)
                console.log(pswd)
            }
            onBrickId: function(id){
                console.log("QQQQQQQQQ" + " onBrickId()");
                console.log(id)
            }

            onBrickType: function(brickType) {
                brickTypeLabel.text = qsTr("Brick Type: ") + brickType
                console.log("QQQQQQQQQ" + " onBrickType()");
                console.log(brickType)
            }
            onBrickName: function(brickName) {
                deviceName.text = brickName
                console.log("QQQQQQQQQ" + " onBrickName()");
                console.log(brickName)
            }
            onBrickNetworkSettings: function(ssid, psswd) {
                wifiNetworkName.text = ssid
                wifiPassword.text = psswd
            }
            Component.onCompleted: {
                connectToBrick(ipAddress)
            }
        }

        TextField {
            id: deviceName
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("Brick name")
        }

        TextField {
            id: wifiNetworkName
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("WiFi Network Name")
        }

        TextField {
            id: wifiPassword
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("WiFi password")
        }

        RowLayout{
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width < 300 ? parent.width : 300
            visible: false
            RadioButton {
                id: dynamicIP
                text: qsTr("Dynamic IP")
                checked: true
                Layout.alignment: Qt.AlignLeft

            }
            RadioButton {
                id: staticIPButton
                text: qsTr("Static IP")
                Layout.alignment: Qt.AlignRight
            }
        }

        TextField {
            id: ip
            anchors.horizontalCenter: parent.horizontalCenter
            visible: staticIPButton.checked ? true : false
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("IP")
        }

        TextField {
            id: gateway
            anchors.horizontalCenter: parent.horizontalCenter
            visible: staticIPButton.checked ? true : false
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("Gateway")
        }

        TextField {
            id: netmast
            anchors.horizontalCenter: parent.horizontalCenter
            visible: staticIPButton.checked ? true : false
            width: parent.width < 300 ? parent.width : 300
            placeholderText: qsTr("Netmask")
        }

        RowLayout{
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width < 300 ? parent.width : 300
            Button {
                id: buttonSave
                text: qsTr("Save Config")
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    controller.cmdSaveBrickName(deviceName.text)
                    controller.cmdSaveNetworkConfig(wifiNetworkName.text, wifiPassword.text)
                }
            }
        }

        Dialog {
            id: connectingDialog

            x: (parent.width - width) / 2
            y: (parent.height - height) / 2

            title: qsTr("Connecting")
            standardButtons: Dialog.Cancel
            onRejected: {
                stackView.pop()
            }
            Column{
                anchors.fill: parent
                Label {
                    text: qsTr("Connecting to brick ......")
                }

                BusyIndicator {
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Label {
                    text: controller.lastTcpError
                }
            }
        }
    }

    footer:
        Label {
        id: brickTypeLabel
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width < 300 ? parent.width : 300
        text: qsTr("Brick Type: ")
    }




}
