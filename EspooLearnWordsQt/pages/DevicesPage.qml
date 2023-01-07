import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend 1.0

Page {
    id: page
    property string pageName: qsTr("Devices")


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
            anchors.horizontalCenter: parent.horizontalCenter
        }

        DevicesManager{
            id: devicesModel
            searchDevices: true
        }

        ColumnLayout {
            spacing: 20
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter



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

                    onCheckedChanged:  {
                        devicesModel.searchDevices = false;
                        stackView.deviceName = devNameLabel.text;
                        page.parent.push("qrc:/pages/M0001ControlPage.qml");
                    }

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



}
