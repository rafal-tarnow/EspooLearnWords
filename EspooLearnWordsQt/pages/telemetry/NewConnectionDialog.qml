import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: dialog

    signal finished(string topic)

    function createConnection() {

        dialog.title = qsTr("New Connection");
        dialog.open();
    }

    y: 10
    x: (parent.width - width)/2
    width: parent.width - 20 > 300 ? 300 : parent.width - 20

    focus: true
    modal: true
    title: qsTr("New Connection")
    standardButtons: Dialog.Ok | Dialog.Cancel

    contentItem: GridLayout {
        id: form
        property int minimumInputSize: 120
        property string placeholderDeviceAddress: qsTr("device address")
        property alias deviceAddress: deviceAddress
        rows: 4
        columns: 2

        Label {
            text: qsTr("Address")
            Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
        }

        TextField {
            id: deviceAddress
            focus: true
            Layout.fillWidth: true
            Layout.minimumWidth: grid.minimumInputSize
            Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
            placeholderText: form.placeholderDeviceAddress
        }

    }

    onAccepted: finished(form.deviceAddress.text)
}
