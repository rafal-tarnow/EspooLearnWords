import QtQuick
import QtQuick.Controls


    SwipeView {
        id: detailsSwipeView
        currentIndex: recipe.swipePageIndex
        anchors.fill: parent

        Loader {
            source: {
                if (model.brickType === "T0002") {
                    recipe.height = 140
                    return "T0002MainPage.qml"
                } else if (model.brickType === "B0002") {
                    recipe.height = 100
                    return "M0002MainPage.qml"
                }
            }
        }

        Item {
            id: secondPage
            Rectangle{
                anchors.fill: secondPage
                color: "yellow"

                border.color: "orange"
                border.width: 1
            }
        }


    }

