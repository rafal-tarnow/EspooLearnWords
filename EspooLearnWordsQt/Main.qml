

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
import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import Backend 1.0
import SortFilterProxyModel 0.2
import Qt.labs.settings

import "." as App

ApplicationWindow {
    id: window
    //flags: Qt.FramelessWindowHint
    property int currentContact: -1
    // nie działa flags: Settings.lockPosition ? flags | Qt.FramelessWindowHint : flags & ~Qt.FramelessWindowHint

    width: 320
    height: 480
    visible: true
    title: qsTr("Contact List")

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
        icon.name: "drawer"
        //icon.name: stackView.depth > 1 ? "back" : "drawer"
        onTriggered: {
//            if (stackView.depth > 1) {
//                stackView.pop()
//                listView.currentIndex = -1
//            } else {
                drawer.open()
//            }
        }
    }

    Action {
        id: optionsMenuAction
        icon.name: "menu"
        onTriggered: optionsMenu.open()
    }

    header: App.ToolBar{
        RowLayout{
            spacing: 20
            anchors.fill: parent


            ToolButton {
                action: navigateBackAction
            }

            Label{
                id: titleLabel
                text: /*listView.currentItem ? listView.currentItem.text :*/ "Espoo"
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
                }

            }

        }


    }

    Drawer {
        id: drawer
        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height
        //interactive: stackView.depth === 1

        ListView {
            id: listView

            focus: true
            currentIndex: -1
            anchors.fill: parent

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
                ListElement { title: "BusyIndicator"; source: "qrc:/pages/BusyIndicatorPage.qml" }
                ListElement { title: "Button"; source: "qrc:/pages/ButtonPage.qml" }
                ListElement { title: "CheckBox"; source: "qrc:/pages/CheckBoxPage.qml" }
                ListElement { title: "ComboBox"; source: "qrc:/pages/ComboBoxPage.qml" }
                ListElement { title: "DelayButton"; source: "qrc:/pages/DelayButtonPage.qml" }
                ListElement { title: "Dial"; source: "qrc:/pages/DialPage.qml" }
                ListElement { title: "Dialog"; source: "qrc:/pages/DialogPage.qml" }
                ListElement { title: "Delegates"; source: "qrc:/pages/DelegatePage.qml" }
                ListElement { title: "Frame"; source: "qrc:/pages/FramePage.qml" }
                ListElement { title: "GroupBox"; source: "qrc:/pages/GroupBoxPage.qml" }
                ListElement { title: "PageIndicator"; source: "qrc:/pages/PageIndicatorPage.qml" }
                ListElement { title: "ProgressBar"; source: "qrc:/pages/ProgressBarPage.qml" }
                ListElement { title: "RadioButton"; source: "qrc:/pages/RadioButtonPage.qml" }
                ListElement { title: "RangeSlider"; source: "qrc:/pages/RangeSliderPage.qml" }
                ListElement { title: "ScrollBar"; source: "qrc:/pages/ScrollBarPage.qml" }
                ListElement { title: "ScrollIndicator"; source: "qrc:/pages/ScrollIndicatorPage.qml" }
                ListElement { title: "Slider"; source: "qrc:/pages/SliderPage.qml" }
                ListElement { title: "SpinBox"; source: "qrc:/pages/SpinBoxPage.qml" }
                ListElement { title: "StackView"; source: "qrc:/pages/StackViewPage.qml" }
                ListElement { title: "SwipeView"; source: "qrc:/pages/SwipeViewPage.qml" }
                ListElement { title: "Switch"; source: "qrc:/pages/SwitchPage.qml" }
                ListElement { title: "TabBar"; source: "qrc:/pages/TabBarPage.qml" }
                ListElement { title: "TextArea"; source: "qrc:/pages/TextAreaPage.qml" }
                ListElement { title: "TextField"; source: "qrc:/pages/TextFieldPage.qml" }
                ListElement { title: "ToolTip"; source: "qrc:/pages/ToolTipPage.qml" }
                ListElement { title: "Tumbler"; source: "qrc:/pages/TumblerPage.qml" }
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

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
            onTriggered: contactView.model.remove(currentContact)
        }
    }

    TestModel{
        id: testModel
    }

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
