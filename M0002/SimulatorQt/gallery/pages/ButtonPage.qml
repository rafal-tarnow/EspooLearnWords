// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick3D


ScrollablePage {
    id: page
    height: 300


    View3D {
        id: view
        anchors.fill: parent
        height: 600

        //! [environment]
        environment: SceneEnvironment {
            clearColor: "skyblue"
            backgroundMode: SceneEnvironment.Color
        }
        //! [environment]

        //! [camera]
        PerspectiveCamera {
            position: Qt.vector3d(0, 200, 300)
            eulerRotation.x: -30
        }
        //! [camera]

        //! [light]
        DirectionalLight {
            eulerRotation.x: -30
            eulerRotation.y: -70
        }
        //! [light]

        //! [objects]
        Model {
            position: Qt.vector3d(0, -200, 0)
            source: "#Cylinder"
            scale: Qt.vector3d(2, 0.2, 1)
            materials: [ DefaultMaterial {
                    diffuseColor: "red"
                }
            ]
        }

        Model {
            position: Qt.vector3d(0, 150, 0)
            source: "#Sphere"

            materials: [ DefaultMaterial {
                    diffuseColor: "blue"
                }
            ]

            //! [animation]
            SequentialAnimation on y {
                loops: Animation.Infinite
                NumberAnimation {
                    duration: 3000
                    to: -150
                    from: 150
                    easing.type:Easing.InQuad
                }
                NumberAnimation {
                    duration: 3000
                    to: 150
                    from: -150
                    easing.type:Easing.OutQuad
                }
            }
            //! [animation]
        }
        //! [objects]
    }
}

//ScrollablePage {
//    id: page

//    Component.onCompleted: {
//        console.log("Button Page created")
//    }
//    Component.onDestruction: {
//        console.log("Button Page destroyed")
//    }


//    Column {
//        spacing: 40
//        width: parent.width

//        Label {
//            width: parent.width
//            wrapMode: Label.Wrap
//            horizontalAlignment: Qt.AlignHCenter
//            text: qsTr("Button presents a push-button that can be pushed or clicked by the user. "
//                + "Buttons are normally used to perform an action, or to answer a question.")
//        }

//        ColumnLayout {
//            spacing: 20
//            anchors.horizontalCenter: parent.horizontalCenter

//            Button {
//                text: qsTr("First")
//                Layout.fillWidth: true
//            }
//            Button {
//                id: button
//                text: qsTr("Second")
//                highlighted: true
//                Layout.fillWidth: true
//            }
//            Button {
//                text: qsTr("Third")
//                enabled: false
//                Layout.fillWidth: true
//            }
//            Button {
//                text: qsTr("Push")
//                Layout.fillWidth: true
//                onClicked: {

//                }
//            }
//        }
//    }
//}
