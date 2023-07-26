import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend

Page {
    id: page


    ListModel {
        id: devicesModel
    }

    ColumnLayout {
        spacing: 5
        anchors.fill: parent

        PseudoDNSServer{
            id: dnsServer
            onHostFound:  function (hostName, hostIp) {
                devicesModel.append({ text: hostName, ip: hostIp})
            }
            Component.onCompleted:{
                dnsServer.startQueriesForAllHosts()
            }
        }

        ListView {
            id: listView
            model: devicesModel

            Layout.fillWidth: true
            Layout.fillHeight: true

            delegate: ItemDelegate{
                id: delegate
                width: parent.width
                checkable: true

                contentItem: ColumnLayout {
                    spacing: 10

                    RowLayout{
                        spacing: 20
                        anchors.fill: parent

                        Label {
                            text: model.text
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
                            text: qsTr("Module IP:")
                            Layout.leftMargin: 60
                        }
                        Label {
                            text: model.ip
                            font.bold: true
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                    }

                }

                Action {
                    id: deviceDetails
                    icon.name: "show_details"
                    checkable: true
                    onTriggered: {
                    }
                }

                states: [
                    State {
                        name: "expanded"
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

        }

    }

}
