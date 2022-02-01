import QtQuick 2.15

Rectangle {
    visible: false
  property var toFill: parent
  property color customColor: 'yellow' // instantiation site "can" (optionally) override
  property int customThickness: 1 // instantiation site "can" (optionally) override

  //color: Qt.rgba(Math.random(),Math.random(),Math.random(),1);
  anchors.fill: toFill
  z: 200
  color: 'transparent'
  border.color: customColor
  border.width: customThickness
}
