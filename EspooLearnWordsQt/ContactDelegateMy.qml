import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

ItemDelegate {
    id: delegate

    checkable: true

    contentItem: ColumnLayout {
        spacing: 10


        GridLayout {
            id: grid
            visible: true

            columns: 2
            rowSpacing: 10
            columnSpacing: 10

            Label {
                text: fullName
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Label {
                text: city
                font.bold: true
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Label {
                text: address
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
                target: grid
                visible: true
            }
        }
    ]
}
