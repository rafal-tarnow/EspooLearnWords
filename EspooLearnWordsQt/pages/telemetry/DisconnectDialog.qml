import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {

    signal disconnectFromHost()

    y: 10
    x: (parent.width - width)/2
    width: parent.width - 20 > 300 ? 300 : parent.width - 20

    parent: Overlay.overlay

    modal: true
    title: qsTr("Confirmation")
    standardButtons: Dialog.Ok | Dialog.Cancel

    Column {
        spacing: 20
        anchors.fill: parent
        Label {
            text: qsTr("You are currently connected.\n Disconnect?")
        }
    }

    onAccepted: disconnectFromHost()
}
