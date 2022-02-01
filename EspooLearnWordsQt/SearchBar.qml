import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import QtQuick.Controls.Material 2.15

Pane {
    property bool filterFavorite: false
    property alias text: textField.displayText
    Material.elevation: 1
    Material.background: "white"
    leftPadding: 0
    topPadding: 0
    bottomPadding: 0
    rightPadding: 24

    RowLayout {
        id: rowLayout
        anchors.fill: parent

        Item {
            width: 48 - rowLayout.spacing
            ColorImage {
                anchors.centerIn: parent
                source: "images/search.svg"
                color: Material.color(Material.Grey)
            }
        }
        TextField{
            id: textField
            Layout.fillWidth: true
            topPadding: 0
            bottomPadding: 0
            focus: true
            selectByMouse: true
            background: null
            placeholderText: "Search ..."
        }
        RoundButton{
            Material.elevation: 0
            contentItem: ColorImage {
                source: filterFavorite ? "images/star.svg" : "images/star_border.svg"
                color: filterFavorite ? Material.color(Material.Amber) : Material.color(Material.Grey)
            }
            onClicked: filterFavorite = !filterFavorite
        }
    }
}
