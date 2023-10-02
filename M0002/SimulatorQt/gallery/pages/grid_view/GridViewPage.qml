import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import "."

Page {
    id: page
    background: Rectangle{
        color: "yellow"
    }

    GridView {
        id: listView
        anchors.fill: parent
        model: RecipesModel {}
        delegate: recipeDelegate
        //columnCount: 3

        cellWidth: 100; cellHeight: 100


    }

    Component {
        id: recipeDelegate

        Item {
            id: recipe


                        required property string title

            property real detailsOpacity : 0
            property bool detailsEnabled: false

            //width: listView.width
            //height: 70

            width: 100
            height: 100


            Rectangle {
                id: background
                x: 2; y: 2; width: parent.width - x*2; height: parent.height - y*2
                color: "ivory"
                border.color: "orange"
                radius: 5

            }

            MouseArea {
                anchors.fill: background
                onClicked: recipe.state = 'Details';
            }

            Row {
                id: topLayout
                x: 10; y: 10; height: recipeImage.height; width: parent.width
                spacing: 10

                Image {
                    id: recipeImage
                    width: 50; height: 50
                    //source: recipe.picture
                }
//! [1]
                Column {
                    width: background.width - recipeImage.width - 20; height: recipeImage.height
                    spacing: 5

                    Text {
                        text: recipe.title
                        font.bold: true; font.pointSize: 16
                    }
                }
            }


            Button {
                y: 10
                anchors { right: background.right; rightMargin: 10 }
                opacity: recipe.detailsOpacity
                text: "Close"
                enabled: recipe.detailsEnabled
                onClicked: recipe.state = '';
            }

            states: State {
                name: "Details"

                PropertyChanges {
                    background.color: "white"

                    recipe {
                        // Make details visible
                        detailsOpacity: 1
                        detailsEnabled: true
                        x: 0
                        z: 2

                        // Fill the entire list area with the detailed view
                        height: listView.height
                        width: listView.width

                    }
                }

                PropertyChanges {
                    recipe.GridView.view.contentY: recipe.y
                    explicit: true;
                }

                // Disallow flicking while we're in detailed view
                PropertyChanges {
                    recipe.GridView.view.interactive: false
                }


            }

            transitions: Transition {
                // Make the state changes smooth
                ParallelAnimation {
                    ColorAnimation { property: "color"; duration: 500 }
                    NumberAnimation { duration: 300; properties: "detailsOpacity,x,z,contentY,height,width" }
                }
            }

        }
    }





}
