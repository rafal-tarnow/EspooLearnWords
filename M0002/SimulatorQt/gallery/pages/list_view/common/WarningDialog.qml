import QtQuick
import QtQuick.Controls

Dialog {
    id: warningDialog
    property string warningText

    contentHeight: logo.height + label.height

    modal: true
    title: qsTr("Warning!")
    standardButtons: Dialog.Close

    background: DialogBackground{}

    Column {
        id: column
        spacing: 20
        width: parent.width

        Image {
            id: logo
            width: parent.width / 4
            anchors.horizontalCenter: parent.horizontalCenter
            fillMode: Image.PreserveAspectFit
            sourceSize: Qt.size(width,height)
            source: "qrc:/images/warning.svg"
        }
        Label {
            id: label
            width: parent.width
            text: warningDialog.warningText
            wrapMode: Label.Wrap
        }
    }
}
