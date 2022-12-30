import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SortFilterProxyModel 0.2
import Backend 1.0
import ".."

//ScrollablePage {
Page {
    id: page

    AddEditDialog {
        id: contactDialog
        onFinished: {
            if (fullName.length === 0 && address.length === 0
                    && city.length === 0)
                return
            if (currentContact === -1)
                contactView.model.sourceModel.append(fullName, address, city)
            else
                contactView.model.sourceModel.set(currentContact, fullName, address, city)
        }
    }

    Dialog {
        id: deleteConfirmDialog

 //       signal finished(string itemToDelete)

        x: 10
        y: 10
        width: parent.width - 20
//        height: (parent.height / 2) - 20

        focus: true
        modal: true
        title: qsTr("Are You SSSS to delete?")
        standardButtons: Dialog.Ok | Dialog.Cancel

//        onAccepted: finished(form.fullName.text)
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
            text: currentContact >= 0 ? contactView.model.get(
                                            currentContact).fullName : ""
        }
        MenuItem {
            text: qsTr("Edit")
            onTriggered: contactDialog.editContact(contactView.model.get(
                                                       currentContact))
        }
        MenuItem {
            text: qsTr("Remove")
            //onTriggered: contactView.model.remove(currentContact)
            onTriggered: deleteConfirmDialog.open()
        }


    }

    //    TestModel{
    //        id: testModel
    //    }

    SqlQueryModel{
        id: sqlQueryModel
    }

    SortFilterProxyModel{
        id: proxyModel
        //sourceModel: testModel
        sourceModel: sqlQueryModel
        pattern: searchBar.text
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        SearchBar{
            id: searchBar
            Layout.fillWidth: true
            z:1
        }

        ContactView {
            id: contactView
            Layout.fillWidth: true
            Layout.fillHeight: true

            //            model: ContactModel {
            //                id: contactModel
            //            }

            model: proxyModel

            //        model: TestModel {
            //            //id: contactModel
            //        }

            //   model: sqlModel

            //    model: ListModel {
            //        id: contactModel
            //    }

            onPressAndHold: {
                currentContact = index
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
            currentContact = -1
            contactDialog.createContact()
        }
    }

}
