import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend 1.0

Page {
    id: page

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

                    checkable: true

                    contentItem: ColumnLayout {
                        spacing: 10

                        Label {
                            text: deviceName
                            font.bold: true
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }

                        GridLayout {
                            id: grid
                            visible: false

                            columns: 2
                            rowSpacing: 10
                            columnSpacing: 10

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

                    states: [
                        State {
                            name: "expanded"
                            when: delegate.checked

                            PropertyChanges {
                                // TODO: When Qt Design Studio supports generalized grouped properties, change to:
                                //       grid.visible: true
                                target: grid
                                visible: true
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
