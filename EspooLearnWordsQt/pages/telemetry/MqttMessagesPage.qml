import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtCharts
import Backend 1.0
import CustomPlot 1.0
import CustomTableModel 1.0
import "../telemetry"

Page {
    id: page
    anchors.fill: parent
    property string pageName: qsTr("Telemetry")

    onFocusChanged: {
        if(page.focus === true){
            console.log("DevicesPage loaded");
            //devicesModel.clear();
            devicesModel.searchDevices = true;
        }else{
            console.log("DevicesPage unloaded");
            devicesModel.searchDevices = false;
        }
    }

    MessagesModel{
        id: devicesModel
        searchDevices: true


    }

    CustomTableModelItem{
        id :dataModel
    }

    ListView {
        id: listview
        anchors.fill: parent
        model: devicesModel
        ScrollBar.vertical: ScrollBar { }

        delegate: ItemDelegate {
            id: delegate
            width: parent.width
            checkable: true

            contentItem: ColumnLayout {
                spacing: 10

                RowLayout{
                    spacing: 20




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

                ColumnLayout {
                    id: grid
                    spacing: 5
                    visible: false
                    GridLayout {



                        columns: 2
                        rowSpacing: 5
                        columnSpacing: 5

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
                    GridLayout {

                        columns: 1
                        rowSpacing: 5
                        columnSpacing: 5


                        ChartView {
                            //model: mapper
                            title: "Two Series, Common Axes"
                            //anchors.fill: parent
                            Layout.fillWidth: true
                            height: 300
                            legend.visible: false
                            antialiasing: true

                            LineSeries{
                                VXYModelMapper {
                                    id: mapper
                                    model: dataModel // QAbstractItemModel derived implementation
                                    xColumn: 0
                                    yColumn: 1
                                }
                            }
                        }

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


    NewConnectionDialog {
        id: contactDialog
        onFinished: {
            console.log("Device address = ", topic);
            if (topic.length === 0)
                return
            devicesModel.newConnection(topic)
        }
    }

    DisconnectDialog {
        id: disconnectDialog
        onDisconnectFromHost: {
            devicesModel.disconnectFromHost()
            contactDialog.createConnection()
        }
    }

    RoundButton {
        text: qsTr("+")
        highlighted: true
        anchors.margins: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: {
            if(devicesModel.isDisconnectedFromHost()){
                contactDialog.createConnection()
            }else{
                disconnectDialog.open()
            }
        }
    }



}
