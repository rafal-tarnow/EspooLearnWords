import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend 1.0
import "../telemetry"

Page {
    id: page
    property string pageName: qsTr("Telemetry")

    onFocusChanged: {
        if(page.focus === true){
            console.log("DevicesPage loaded");
            //devicesModel.clear();
            devicesModel.searchDevices = true;
            busyIndicator.running = true;
        }else{
            console.log("DevicesPage unloaded");
            devicesModel.searchDevices = false;
            busyIndicator.running = false;
        }
    }

    MessagesModel{
        id: devicesModel
        searchDevices: true
    }

    Column {
        spacing: 40
        width: parent.width

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("Searching for devices ...")
        }

        BusyIndicator {
            id: busyIndicator
            anchors.horizontalCenter: parent.horizontalCenter
        }



        ColumnLayout {
            spacing: 20
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                id: button
                text: qsTr("Connect")
                Layout.alignment: Qt.AlignHCenter
            }
            Button {
                text: qsTr("Disconnect")
                Layout.alignment: Qt.AlignHCenter
            }

            TextField {
                id: deviceAddress
                focus: true
                Layout.alignment: Qt.AlignHCenter
                //Layout.fillWidth: true
                //Layout.minimumWidth: grid.minimumInputSize
                //Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                placeholderText: qsTr("Topic")
            }

            ListView {
                id: listview
                width: parent.width
                height: 320
                model: devicesModel
                ScrollBar.vertical: ScrollBar { }

                delegate: ItemDelegate {
                    id: delegate
                    width: parent.width
                    checkable: true

                    contentItem: ColumnLayout {
                        spacing: 10

                        //                        Label {
                        //                            text: deviceName
                        //                            font.bold: true
                        //                            elide: Text.ElideRight
                        //                            Layout.fillWidth: true
                        //                        }

                        RowLayout{
                            spacing: 20
                            anchors.fill: parent



                            Label {
                                id: devNameLabel
                                text: deviceName
                                font.bold: true
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }



                            ToolButton {
                                action: deviceDetails
                            }

                        }

                        GridLayout {
                            id: grid
                            visible: false

                            columns: 2
                            rowSpacing: 10
                            columnSpacing: 10

                            Label {
                                text: qsTr("Module type:")
                                Layout.leftMargin: 60
                            }

                            Label {
                                text: moduleType
                                font.bold: true
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }

                            Label {
                                text: qsTr("IP address:")
                                Layout.leftMargin: 60
                            }

                            Label {
                                text: ipAddress
                                font.bold: true
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }

                            Label {
                                text: qsTr("Port:")
                                Layout.leftMargin: 60
                            }

                            Label {
                                text: port
                                font.bold: true
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }

                            Label {
                                text: qsTr("Serial number:")
                                Layout.leftMargin: 60
                            }

                            Label {
                                text: serialNumber
                                font.bold: true
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }
                        }
                    }

                    Action {
                        id: deviceDetails
                        //icon.name: stackView.depth > 1 ? "back" : "drawer"
                        icon.name: "show_details"
                        checkable: true
                        onTriggered: {
                            //                    if (stackView.depth > 1) {
                            //                        stackView.pop()
                            //                        listView.currentIndex = -1
                            //                    } else {
                            //                        drawer.open()
                            //                    }
                        }
                    }

                    states: [
                        State {
                            name: "expanded"
                            // when: delegate.checked
                            when: deviceDetails.checked

                            PropertyChanges {
                                // TODO: When Qt Design Studio supports generalized grouped properties, change to:
                                grid.visible: true
                                //target: grid
                                //visible: true
                                deviceDetails.icon.name: "hide_details"
                            }
                        }
                    ]
                }

                //                delegate: Rectangle {
                //                    width: listview.width;
                //                    height: 25

                //                    //                    required color
                //                    //                    required property string name

                //                    Text {
                //                        text: deviceName
                //                        font.bold: true
                //                        elide: Text.ElideRight
                //                        Layout.fillWidth: true
                //                    }
                //                }
            }
        }
    }

    NewConnectionDialog {
        id: contactDialog
        onFinished: {
            console.log("Device address = ", topic);
            if (topic.length === 0)
                return
            devicesModel.newConnection(topic)
        }
    }

    RoundButton {
        text: qsTr("+")
        highlighted: true
        anchors.margins: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: {
            contactDialog.createConnection()
        }
    }



}
