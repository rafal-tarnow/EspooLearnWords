// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Shapes
import Qt.labs.animation

Item {
    id: root
    //color: "#06000000"
    implicitWidth: 256
    implicitHeight: 256

    property real value
    property real max_value : 80
    property string unit

    readonly property int stroke: 18

    onMax_valueChanged: {
        if (max_value === 0) {
            max_value = 80;
        }
    }

    // Behavior on value {
    //     NumberAnimation {
    //         duration: 1000
    //         easing.type: Easing.InOutQuad
    //     }
    // }

    Shape {
        anchors.fill: parent

        ShapePath {
            fillColor: "transparent"
            strokeColor: "#ebebeb"
            strokeWidth: root.stroke
            capStyle: ShapePath.RoundCap

            PathAngleArc {
                centerX: root.width/2
                centerY: root.height/2
                radiusX: root.width/2 - root.stroke/2
                radiusY: root.height/2 - root.stroke/2
                startAngle: -210
                sweepAngle: 240
            }
        }


        ShapePath {
            fillColor: "transparent"
            strokeColor: "#ffb690"
            strokeWidth: root.stroke
            capStyle: ShapePath.RoundCap

            PathAngleArc {
                id: arc
                centerX: root.width/2
                centerY: root.height/2
                radiusX: root.width/2 - root.stroke/2
                radiusY: root.height/2 - root.stroke/2
                startAngle: -210
                sweepAngle : (value / max_value) * 240

                BoundaryRule on sweepAngle {
                    minimum: 0
                    maximum: 240
                }
            }
        }
    }

    Image{
        id: capOrange
        width: root.stroke
        height: root.stroke
        source: "qrc:/images/cap_orange.svg"
        //source: "qrc:/images/humidity.svg"
        sourceSize: Qt.size(width,height)

        transform: [
            Translate {
                x: root.width/2 - arc.radiusX - capOrange.width/2
                y: root.height/2 - capOrange.height/2
            },
            Rotation {
                angle: arc.sweepAngle - 30;
                origin.x: root.width/2
                origin.y: root.height/2
            }
        ]

        // Rectangle{
        //     anchors.fill: parent
        //     color: "#440000ff"
        // }

    }


    Text {
        anchors.centerIn: parent
        id: bigValue
        text: value.toFixed(1)
        font.pointSize: 56
    }

    Text{
        text: unit
        font.pointSize: 20
        anchors.top: bigValue.bottom
        anchors.horizontalCenter: bigValue.horizontalCenter
    }

}
