pragma ComponentBehavior: Bound

import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Qt5Compat.GraphicalEffects

import "." as App
import "./pages/my_bricks"
import "./pages/list_view"
import "./pages/grid_view"
import "./pages/list_view/common"
import "./pages/controls"
import "./pages"

ApplicationWindow {
    id: window
    width: 360
    height: 520
    visible: true
    title: qsTr("KIKO Bricks")

    //! [orientation]
    readonly property bool portraitMode: window.width < window.height
    //! [orientation]

    function help() {
        let displayingControl = listView.currentIndex !== -1
        let currentControlName = displayingControl
            ? listView.model.get(listView.currentIndex).title.toLowerCase() : ""
        let url = "https://doc.qt.io/qt-6/"
            + (displayingControl
               ? "qml-qtquick-controls2-" + currentControlName + ".html"
               : "qtquick-controls2-qmlmodule.html");
        Qt.openUrlExternally(url)
    }



    Settings {
        id: settings
        property string temperatureScale //Celsius, Fahrenheit
        signal sendMessage(string msg, int compId)
        function setTemperatureScale(scale) {
            //if (scale !== temperatureScale) {
            //temperatureScale = scale;
            //temperatureScaleChanged(scale);
            //}
        }

    }

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: navigateBackAction.trigger()
    }

    Shortcut {
        sequence: StandardKey.HelpContents
        onActivated: window.help()
    }

    Action {
        id: navigateBackAction
        icon.name: stackView.depth > 1 ? "back" : "drawer"
        onTriggered: {
            if (stackView.depth > 1) {
                stackView.pop()
                listView.currentIndex = -1
            } else {
                drawer.open()
            }
        }
    }

    Shortcut {
        sequence: "Menu"
        onActivated: optionsMenuAction.trigger()
    }

    Action {
        id: optionsMenuAction
        icon.name: "menu"
        onTriggered: optionsMenu.open()
    }

    header: App.ToolBar {
        RowLayout {
            spacing: 20
            anchors.fill: parent
            anchors.leftMargin: !window.portraitMode ? drawer.width : undefined

            ToolButton {
                action: navigateBackAction
                visible: window.portraitMode
            }

            Item{
                Layout.fillWidth: true

                Image {
                    anchors.right: titleLabel.left
                    anchors.verticalCenter: titleLabel.verticalCenter
                    anchors.rightMargin: 10
                    width: 25
                    height: 25
                    fillMode: Image.PreserveAspectFit
                    sourceSize: Qt.size(width,height)
                    source: "qrc:/images/logo.svg"
                }

                Label {
                    id: titleLabel
                    anchors.centerIn: parent
                    text: stackView.currentItem.pageName
                    font.pixelSize: 20
                    elide: Label.ElideRight
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                }
            }

            ToolButton {
                action: optionsMenuAction

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    // Action {
                    //     text: qsTr("Settings")
                    //     onTriggered: settingsDialog.open()
                    // }
                    // Action {
                    //     text: qsTr("Help")
                    //     onTriggered: window.help()
                    // }
                    Action {
                        text: qsTr("About")
                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }



    Drawer {
        id: drawer

        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height
        modal: window.portraitMode
        interactive: window.portraitMode ? (stackView.depth === 1) : false
        position: window.portraitMode ? 0 : 1
        visible: !window.portraitMode

        background: Rectangle {
            color: "#ffffff"
            //right border
            Rectangle {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                width: 1
                color:"#e0e0e0"
            }
        }

        ListView {
            id: listView

            focus: true
            currentIndex: 0
            anchors.fill: parent

            model: PagesModel{}

            delegate: ItemDelegate {
                id: delegateItem
                width: ListView.view.width
                text: title
                highlighted: ListView.isCurrentItem

                //                contentItem: Text {
                //                    text: title
                //                    color: "#21be2b"
                //                    //font: control.font
                //                    elide: Text.ElideRight
                //                    verticalAlignment: Text.AlignVCenter
                //                }

                required property int index
                required property var model
                required property string title
                required property string source

                onClicked: {
                    listView.currentIndex = index
                    stackView.replace(source)
                    if (window.portraitMode)
                    drawer.close()
                }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

    StackView {

        id: stackView
        anchors.fill: parent
        anchors.leftMargin: !window.portraitMode ? drawer.width : undefined

        function pushPage(newPage, properties) {
            push(newPage, properties)
        }

        initialItem: HomePage{}
    }

    Dialog {
        id: settingsDialog
        x: Math.round((window.width - width) / 2)
        y: Math.round(window.height / 6)
        width: Math.round(Math.min(window.width, window.height) / 3 * 2)
        modal: true
        focus: true
        title: qsTr("Settings")

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.temperatureScale = temperatureBox.displayText
            settingsDialog.close()
        }
        onRejected: {
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 10

                Label {
                    text: qsTr("Temperature:")
                }

                ComboBox {
                    id: temperatureBox
                    property int tempIndex: -1
                    model: ListModel {
                        ListElement { text: qsTr("Celsius") }
                        ListElement { text: qsTr("Fahrenheit") }
                    }
                    Component.onCompleted: {
                        tempIndex = find(settings.temperatureScale, Qt.MatchFixedString)
                        if (tempIndex !== -1)
                        currentIndex = tempIndex
                    }
                    Layout.fillWidth: true
                }
            }
        }
    }

    Dialog {
        id: aboutDialog
        modal: true
        focus: true
        title: qsTr("About")
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        contentHeight: aboutColumn.height
        background: DialogBackground{}

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("Join the Kiko Bricks community and embark on a journey of innovation.")
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("With our mobile app, you can effortlessly assemble, configure, and control your IoT projects like never before.")
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }
}
