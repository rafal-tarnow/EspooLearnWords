/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.settings
import SortFilterProxyModel 0.2

import "." as App

ApplicationWindow {
    id: window
    //flags: Qt.FramelessWindowHint
    property int currentContact: -1
    // nie działa flags: Settings.lockPosition ? flags | Qt.FramelessWindowHint : flags & ~Qt.FramelessWindowHint

    width: 320
    height: 480
    visible: true
    title: qsTr("Espoo")

    Settings {
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
        property alias flags: window.flags //TODO to jest źle zrobione bo przechowuje dwa stany
        property alias lockPosition: lockPositionCheckBox.checked //TODO to jest źle zrobione bo przechowuje dwa stany
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

    Action {
        id: optionsMenuAction
        icon.name: "menu"
        onTriggered: optionsMenu.open()
    }

    header: App.ToolBar{
        id: mainHeader
        z: 1
        RowLayout{
            spacing: 20
            anchors.fill: parent


            ToolButton {
                action: navigateBackAction
            }

            Label{
                id: titleLabel
                //text: listView.currentItem ? listView.currentItem.text : "Espoo"
                text: stackView.currentItem.pageName
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton{
                action: optionsMenuAction

                Menu{
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    CheckBox {
                        id: lockPositionCheckBox
                        checked: true
                        text: qsTr("Lock Position")
                        onCheckedChanged: {
                            //flags: Settings.stayOnTop ? root.flags | Qt.WindowStaysOnTopHint : root.flags & ~Qt.WindowStaysOnTopHint
                            if(checked)
                                window.flags = window.flags | Qt.FramelessWindowHint
                            else
                                window.flags = window.flags & ~Qt.FramelessWindowHint
                        }
                    }

                    Action{
                        text: "Settings"
                    }
                    Action{
                        text: "Quit"
                        onTriggered: window.close()
                    }
                }

            }

        }


    }

    Drawer {
        id: drawer
        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height
        interactive: stackView.depth === 1

        ListView {
            id: listView

            focus: true
            currentIndex: -1
            anchors.fill: parent

            headerPositioning: ListView.OverlayHeader
            header: Pane {
                id: header
                z: 2
                width: parent.width

                contentHeight: logoDrawrer.height

                Image {
                    id: logoDrawrer
                    width: parent.width
                    source: "images/qt-logo.png"
                    fillMode: implicitWidth > width ? Image.PreserveAspectFit : Image.Pad
                }

                MenuSeparator {
                    parent: header
                    width: parent.width
                    anchors.verticalCenter: parent.bottom
                    visible: !listView.atYBeginning
                }
            }

            delegate: ItemDelegate {
                width: listView.width
                text: model.title
                highlighted: ListView.isCurrentItem
                onClicked: {
                    listView.currentIndex = index
                    stackView.push(model.source)
                    drawer.close()
                }
            }

            model: ListModel {
//                ListElement { title: "Devices"; source: "qrc:/pages/DevicesPage.qml" }
                ListElement { title: "Telemetry"; source: "qrc:/pages/MqttMessagesPage.qml" }
//                ListElement { title: "Dictionary"; source: "qrc:/pages/DictionaryPage.qml" }
//                ListElement { title: "BusyIndicator"; source: "qrc:/pages/BusyIndicatorPage.qml" }
//                ListElement { title: "Button"; source: "qrc:/pages/ButtonPage.qml" }
//                ListElement { title: "CheckBox"; source: "qrc:/pages/CheckBoxPage.qml" }
//                ListElement { title: "ComboBox"; source: "qrc:/pages/ComboBoxPage.qml" }
//                ListElement { title: "DelayButton"; source: "qrc:/pages/DelayButtonPage.qml" }
//                ListElement { title: "Dial"; source: "qrc:/pages/DialPage.qml" }
//                ListElement { title: "Dialog"; source: "qrc:/pages/DialogPage.qml" }
//                ListElement { title: "Delegates"; source: "qrc:/pages/DelegatePage.qml" }
//                ListElement { title: "Frame"; source: "qrc:/pages/FramePage.qml" }
//                ListElement { title: "GroupBox"; source: "qrc:/pages/GroupBoxPage.qml" }
//                ListElement { title: "PageIndicator"; source: "qrc:/pages/PageIndicatorPage.qml" }
//                ListElement { title: "ProgressBar"; source: "qrc:/pages/ProgressBarPage.qml" }
//                ListElement { title: "RadioButton"; source: "qrc:/pages/RadioButtonPage.qml" }
//                ListElement { title: "RangeSlider"; source: "qrc:/pages/RangeSliderPage.qml" }
//                ListElement { title: "ScrollBar"; source: "qrc:/pages/ScrollBarPage.qml" }
//                ListElement { title: "ScrollIndicator"; source: "qrc:/pages/ScrollIndicatorPage.qml" }
//                ListElement { title: "Slider"; source: "qrc:/pages/SliderPage.qml" }
//                ListElement { title: "SpinBox"; source: "qrc:/pages/SpinBoxPage.qml" }
//                ListElement { title: "StackView"; source: "qrc:/pages/StackViewPage.qml" }
//                ListElement { title: "SwipeView"; source: "qrc:/pages/SwipeViewPage.qml" }
//                ListElement { title: "Switch"; source: "qrc:/pages/SwitchPage.qml" }
//                ListElement { title: "TabBar"; source: "qrc:/pages/TabBarPage.qml" }
//                ListElement { title: "TextArea"; source: "qrc:/pages/TextAreaPage.qml" }
//                ListElement { title: "TextField"; source: "qrc:/pages/TextFieldPage.qml" }
//                ListElement { title: "ToolTip"; source: "qrc:/pages/ToolTipPage.qml" }
//                ListElement { title: "Tumbler"; source: "qrc:/pages/TumblerPage.qml" }
            }

            ScrollIndicator.vertical: ScrollIndicator { }

//            footer: ItemDelegate {
//                id: footer
//                text: qsTr("Footer")
//                width: parent.width

//                MenuSeparator {
//                    parent: footer
//                    width: parent.width
//                    anchors.verticalCenter: parent.top
//                }
//            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        property string deviceName: ""

        //        anchors.top: header.bottom
        //        height: (window.height - header.height)/2
        //        width: window.width
        //        z: 3


//        Timer {
//            interval: 60*1000; running: true; repeat: false
//            onTriggered: stackView.push("qrc:/pages/DictionaryPage.qml")
//        }


        initialItem: Pane {
            id: pane
            property string pageName: "Espoo"

            Image {
                id: logo
                width: pane.availableWidth / 2
                height: pane.availableHeight / 2
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -50
                fillMode: Image.PreserveAspectFit
                //source: "images/qt-logo.png"
                source: "images/qtlogo.svg"
            }

            Label {
                text: "Qt Quick Controls provides a set of controls that can be used to build complete interfaces in Qt Quick."
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
            }
        }
    }

}
