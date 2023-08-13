import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend

Page {
    id: page
    property int controlsHeight: 40

    MyListModel{
        id: myBricksModel
        onBrickAlreadyAdded: function(brickType, brickName){
warningDialog.open()
        }
    }

    Menu {
        id: contactMenu
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        modal: true

        Label {
            padding: 10
            font.bold: true
            width: parent.width
            horizontalAlignment: Qt.AlignHCenter
            text: currentContact >= 0 ? contactView.model.get(currentContact).fullName : ""
        }
        MenuItem {
            text: qsTr("Edit...")
            onTriggered: contactDialog.editContact(contactView.model.get(currentContact))
        }
        MenuItem {
            text: qsTr("Remove")
            onTriggered: contactView.model.remove(currentContact)
        }
    }

    ListView {
        width: parent.width
        height: parent.height

        model: myBricksModel

        delegate: ItemDelegate {
            width: parent.width
            height: 50
            Row{

            Text {
                width: parent.width * 0.25
                text: model.brickName
            }
            Text {
                text: model.brickType
            }
            }
            onPressAndHold: {
                console.log("Long press 11")
                contactMenu.open()
            }
        }
    }

    RoundButton {
        text: qsTr("+")
        highlighted: true
        anchors.margins: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: {
            dialog.open()
        }
    }

    Dialog {
        id: warningDialog
        title: qsTr("Warning")
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        standardButtons: Dialog.Ok

        Column {
            spacing: 10
            Text {
                text: "Brick already added!"
            }
        }

        onAccepted: {
            // Tutaj możesz dodać kod do wykonania po zaakceptowaniu ostrzeżenia
        }
    }

    Dialog {
        id: confirmDialog
        title: qsTr("Add new brick ?")
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        standardButtons: Dialog.Ok | Dialog.Cancel

        function confirm(brickName, brickType, brickIp) {
            confirmName.text = brickName
            confirmType.text = brickType
            confirmIP.text = brickIp
            open();
        }

        contentItem:Column{
            Label {
                id: confirmName
            }
            Label {
                id: confirmType
            }
            Label {
                id: confirmIP
            }
        }

        onRejected: {
            close()
            dialog.close()
        }

        onAccepted: {
            close()
            dialog.close()
            myBricksModel.addBrick(confirmName.text,confirmType.text, confirmIP.text);
        }
    }

    Dialog {
        id: dialog
        property int gap: 20
        x: gap
        y: gap
        width: parent.width - 2*gap
        height: parent.height - 2*gap
        title: qsTr("Avaliable Bricks")
        standardButtons: Dialog.Cancel
        onRejected: {
            close()
        }
        onClosed: {
            devicesModel.clear()
            dnsServer.stopQueries()
        }

        onOpened: {
            dnsServer.startQueriesForAllHosts()
        }

        ListModel {
            id: devicesModel
        }

        PseudoDNSServer{
            id: dnsServer
            onHostFound:  function (hostName, hostIp) {
                devicesModel.append({ text: hostName, ip: hostIp})
            }
        }

        ListView {
            id: listView
            model: devicesModel

            anchors.fill: parent

            delegate: ItemDelegate{
                id: delegate
                width: parent.width
                checkable: true

                LocalM0002Controller{
                    id: controller
                    onBrickConnected: {
                        console.log("Brick connected")
                        controller.cmdGetTypeAndName();
                    }
                    onBrickTypeAndName: function(brickType, brickName) {
                        brickTypeLabel.text = brickType
                        disconnectFromBrick()
                    }
                    Component.onCompleted: {
                        connectToBrick(model.ip)
                    }
                }

                contentItem: ColumnLayout {
                    spacing: 10

                    RowLayout{
                        spacing: 20
                        anchors.fill: parent

                        ToolButton {
                            action: deviceDetails
                        }

                        Label {
                            id: brickName
                            text: model.text
                            font.bold: true
                            elide: Text.ElideRight
                            Layout.fillWidth: true

                        }

                    }

                    GridLayout {
                        id: grid
                        visible: false

                        columns: 2
                        rowSpacing: 10
                        columnSpacing: 10

                        Label {
                            text: qsTr("Brick IP:")
                            Layout.leftMargin: 60
                        }
                        Label {
                            id: ipLabel
                            text: model.ip
                            font.bold: true
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                        Label {
                            text: qsTr("Brick Type:")
                            Layout.leftMargin: 60
                        }
                        Label {
                            id: brickTypeLabel
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
                onClicked: {
                    confirmDialog.confirm(model.text, brickTypeLabel.text, model.ip)
                }
                Component.onCompleted: {
                    console.log("Delegate Item onCompleted()")
                }
            }

        }

    }



}
