import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    width: 500
    height: 300
    visible: true

    ListView {
        anchors { fill: parent; margins: 20 }
        model: sqlModel
        reuseItems : true
        delegate: Text {
            text: fullName + ' - ' + city + ' - ' + address
        }
    }
}
