import QtQuick
import QtQuick.Controls

Dialog {
    id: dialog
    property int gap: 20
    property alias model: dialogView.model
    property string brickIdToAdd
    property string brickNameToAdd
    property string brickTypeToAdd

    signal addBrick(brickId: string, brickName: string, brickType: string)

    x: gap
    y: gap
    width: parent.width - 2*gap
    height: parent.height - 2*gap
    title: qsTr("Avaliable Bricks")
    standardButtons: Dialog.Cancel

    background: DialogBackground{}

    onOpened: {}
    onClosed: {}

    ListView{
        id: dialogView
        anchors.fill: parent

        delegate: ItemDelegate{
            id: delegate
            width: parent.width
            height: 50
            //height: 200

            onClicked: {
                brickIdToAdd = model.IdFromDns
                brickNameToAdd = model.NameFromDns
                brickTypeToAdd = model.TypeFromDns
                confirmationDialog.open()
            }

            contentItem:
            //     DataLine{
            //     width: parent.width
            //     icon: "qrc:/images/wifi.svg"
            //     label: model.NameFromDns
            //     leftMarginValue: 0
            // }

            Column{
                DataLine{
                    width: parent.width
                    icon: "qrc:/images/wifi.svg"
                    label: model.IdFromDns
                    leftMarginValue: 0
                }
                DataLine{
                    width: parent.width
                    icon: "qrc:/images/wifi.svg"
                    label: model.NameFromDns
                    leftMarginValue: 0
                }
                DataLine{
                    width: parent.width
                    icon: "qrc:/images/wifi.svg"
                    label: model.TypeFromDns
                    leftMarginValue: 0
                }
                DataLine{
                    width: parent.width
                    icon: "qrc:/images/wifi.svg"
                    label: model.IpFromDns
                    leftMarginValue: 0
                }
            }
        }
    }

    Dialog {
        id: confirmationDialog
        title: qsTr("Confirmation")
        anchors {
            centerIn: parent
        }
        width: dialog.width > 265 ? 265 : dialog.width
        modal: true
        background: DialogBackground{}
        standardButtons: Dialog.Yes | Dialog.No

        Column {
            spacing: 20
            anchors.fill: parent
            Label {
                text: qsTr("Add " + brickNameToAdd + "?")
            }
        }

        onAccepted: {
            dialog.addBrick(dialog.brickIdToAdd, dialog.brickNameToAdd, dialog.brickTypeToAdd)
            dialog.close()
        }
    }
}
