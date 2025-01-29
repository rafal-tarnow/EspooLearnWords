// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls

Pane {
    id: pane

    SwipeView {
        id: view
        currentIndex: 1
        anchors.fill: parent

        Repeater {
            model: 3

            Pane {
                width: SwipeView.view.width
                height: SwipeView.view.height

                Column {
                    spacing: 40
                    width: parent.width

                    Label {
                        width: parent.width
                        wrapMode: Label.Wrap
                        horizontalAlignment: Qt.AlignHCenter
                        text: qsTr("SwipeView provides a navigation model that simplifies horizontal paged scrolling. "
                        + "The page indicator on the bottom shows which is the presently active page.")
                    }

                    Image {
                        source: "../images/arrows.png"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
    }

    PageIndicator {
        count: view.count
        currentIndex: view.currentIndex
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}


//import QtQuick
//import QtQuick.Controls

//Pane {
//    id: pane

//    SwipeView {
//        id: view
//        currentIndex: 1
//        anchors.fill: parent


//        Repeater {
//            model: 3

//            Pane {
//                width: SwipeView.view.width
//                height: SwipeView.view.height

//                // Definiowanie różnych zawartości w zależności od indeksu strony
//                Loader {
//                    sourceComponent: {
//                        if (index === 0) {
//                            return page1Component;
//                        } else if (index === 1) {
//                            return page2Component;
//                        } else {
//                            return page3Component;
//                        }
//                    }
//                }
//            }
//        }
//    }

//    PageIndicator {
//        count: view.count
//        currentIndex: view.currentIndex
//        anchors.bottom: parent.bottom
//        anchors.horizontalCenter: parent.horizontalCenter
//    }

//    // Definicje komponentów dla różnych stron
//    Component {
//        id: page1Component

//        Column {
//            spacing: 40
//            width: parent.width

//            Label {
//                width: parent.width
//                wrapMode: Label.Wrap
//                horizontalAlignment: Qt.AlignHCenter
//                text: qsTr("Zawartość strony 1")
//            }

//            Image {
//                source: "../images/arrows.png"
//                anchors.horizontalCenter: parent.horizontalCenter
//            }
//        }
//    }

//    Component {
//        id: page2Component

//        Column {
//            spacing: 40
//            width: parent.width

//            Label {
//                width: parent.width
//                wrapMode: Label.Wrap
//                horizontalAlignment: Qt.AlignHCenter
//                text: qsTr("Zawartość strony 2")
//            }

//            // Dodaj inne elementy dla strony 2
//        }
//    }

//    Component {
//        id: page3Component

//        Column {
//            spacing: 40
//            width: parent.width

//            Label {
//                width: parent.width
//                wrapMode: Label.Wrap
//                horizontalAlignment: Qt.AlignHCenter
//                text: qsTr("Zawartość strony 3")
//            }

//            // Dodaj inne elementy dla strony 3
//        }
//    }
//}
