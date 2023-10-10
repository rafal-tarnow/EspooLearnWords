// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

pragma ComponentBehavior: Bound

import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects

import "." as App
import "./pages/my_bricks"
import "./pages/list_view"
import "./pages/grid_view"

ApplicationWindow {
    id: window
    width: 360
    height: 520
    visible: true
    title: qsTr("Qt Quick Controls")

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

    required property var builtInStyles

    Settings {
        id: settings
        property string style
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

            Label {
                id: titleLabel
                text: stackView.currentItem.pageName
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                action: optionsMenuAction

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    Action {
                        text: qsTr("Settings")
                        onTriggered: settingsDialog.open()
                    }
                    Action {
                        text: qsTr("Help")
                        onTriggered: window.help()
                    }
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


        ListView {
            id: listView

            focus: true
            currentIndex: -1
            anchors.fill: parent

            model: ListModel {
                ListElement { title: qsTr("My Bricks"); source: "qrc:/pages/my_bricks/MyBricksPage.qml" }
                ListElement { title: qsTr("Bricks in network"); source: "qrc:/pages/DevicesInNetworkPage.qml" }
                ListElement { title: qsTr("Config M0002"); source: "qrc:/pages/ConfigM0002Page.qml" }
                ListElement { title: qsTr("Emulator M0002"); source: "qrc:/pages/EmulatorM0002Page.qml" }
                ListElement { title: qsTr("TCP terminal"); source: "qrc:/pages/TCPTerminalPage.qml" }
                ListElement { title: qsTr("UDP terminal"); source: "qrc:/pages/UDPTerminalPage.qml" }
                ListElement { title: qsTr("List View"); source: "qrc:/pages/list_view/ListViewPage.qml" }
                ListElement { title: qsTr("Grid"); source: "qrc:/pages/grid/GridPage.qml" }
                ListElement { title: qsTr("Grid View"); source: "qrc:/pages/grid_view/GridViewPage.qml" }
                ListElement { title: qsTr("BusyIndicator"); source: "qrc:/pages/BusyIndicatorPage.qml" }
                ListElement { title: qsTr("Button"); source: "qrc:/pages/ButtonPage.qml" }
                ListElement { title: qsTr("CheckBox"); source: "qrc:/pages/CheckBoxPage.qml" }
                ListElement { title: qsTr("ComboBox"); source: "qrc:/pages/ComboBoxPage.qml" }
                ListElement { title: qsTr("DelayButton"); source: "qrc:/pages/DelayButtonPage.qml" }
                ListElement { title: qsTr("Dial"); source: "qrc:/pages/DialPage.qml" }
                ListElement { title: qsTr("Dialog"); source: "qrc:/pages/DialogPage.qml" }
                ListElement { title: qsTr("Delegates"); source: "qrc:/pages/DelegatePage.qml" }
                ListElement { title: qsTr("Frame"); source: "qrc:/pages/FramePage.qml" }
                ListElement { title: qsTr("GroupBox"); source: "qrc:/pages/GroupBoxPage.qml" }
                ListElement { title: qsTr("PageIndicator"); source: "qrc:/pages/PageIndicatorPage.qml" }
                ListElement { title: qsTr("ProgressBar"); source: "qrc:/pages/ProgressBarPage.qml" }
                ListElement { title: qsTr("RadioButton"); source: "qrc:/pages/RadioButtonPage.qml" }
                ListElement { title: qsTr("RangeSlider"); source: "qrc:/pages/RangeSliderPage.qml" }
                ListElement { title: qsTr("ScrollBar"); source: "qrc:/pages/ScrollBarPage.qml" }
                ListElement { title: qsTr("ScrollIndicator"); source: "qrc:/pages/ScrollIndicatorPage.qml" }
                ListElement { title: qsTr("Slider"); source: "qrc:/pages/SliderPage.qml" }
                ListElement { title: qsTr("SpinBox"); source: "qrc:/pages/SpinBoxPage.qml" }
                ListElement { title: qsTr("StackView"); source: "qrc:/pages/StackViewPage.qml" }
                ListElement { title: qsTr("SwipeView"); source: "qrc:/pages/SwipeViewPage.qml" }
                ListElement { title: qsTr("Switch"); source: "qrc:/pages/SwitchPage.qml" }
                ListElement { title: qsTr("TabBar"); source: "qrc:/pages/TabBarPage.qml" }
                ListElement { title: qsTr("TextArea"); source: "qrc:/pages/TextAreaPage.qml" }
                ListElement { title: qsTr("TextField"); source: "qrc:/pages/TextFieldPage.qml" }
                ListElement { title: qsTr("ToolTip"); source: "qrc:/pages/ToolTipPage.qml" }
                ListElement { title: qsTr("Tumbler"); source: "qrc:/pages/TumblerPage.qml" }
            }

            delegate: ItemDelegate {
                id: delegateItem
                width: ListView.view.width
                text: title
                highlighted: ListView.isCurrentItem

                required property int index
                required property var model
                required property string title
                required property string source

                onClicked: {
                    listView.currentIndex = index
                    stackView.pushPage(source)
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

        function replacePage(newPage, title, properties) {
            replace(currentItem, newPage, properties)
        }

        function pushPage(newPage, properties) {
            push(newPage, properties)
        }

        initialItem: Pane {
            id: pane
            background: Qt.transparent
            property string pageName: "Babbon App test"
            Image {
                id: logo
                width: pane.availableWidth / 2
                height: pane.availableHeight / 2
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -50
                fillMode: Image.PreserveAspectFit
                source: "images/baboon.png"
            }

            Label {
                text: qsTr("Aspoo app provides a set of functions that can be used to build complete IoT System.")
                anchors.margins: 20
                anchors.top: logo.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: arrow.top
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                wrapMode: Label.Wrap
            }

            Image {
                id: arrow
                source: "images/arrow.png"
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                visible: window.portraitMode
            }
        }
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
            settings.style = styleBox.displayText
            settings.temperatureScale = temperatureBox.displayText
            settingsDialog.close()
        }
        onRejected: {
            styleBox.currentIndex = styleBox.styleIndex
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 10

                Label {
                    text: qsTr("Style:")
                }

                ComboBox {
                    id: styleBox
                    property int styleIndex: -1
                    model: window.builtInStyles
                    Component.onCompleted: {
                        styleIndex = find(settings.style, Qt.MatchFixedString)
                        if (styleIndex !== -1)
                        currentIndex = styleIndex
                    }
                    Layout.fillWidth: true
                }
            }
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

            Label {
                text: qsTr("Restart required")
                color: "#e41e25"
                opacity: styleBox.currentIndex !== styleBox.styleIndex ? 1.0 : 0.0
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
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

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("The Qt Quick Controls module delivers the next generation user interface controls based on Qt Quick.")
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: qsTr("In comparison to Qt Quick Controls 1, Qt Quick Controls "
                           + "are an order of magnitude simpler, lighter, and faster.")
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }

}
