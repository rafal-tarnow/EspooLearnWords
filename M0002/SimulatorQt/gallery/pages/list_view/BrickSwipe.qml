import QtQuick
import QtQuick.Controls
import "./M0002"

Item{

    SwipeView {
        id: detailsSwipeView
        anchors.fill: parent

        currentIndex: recipe.swipePageIndex
        interactive: recipe.detailsEnabled

        Loader {
            source: {
                if (model.brickType === "T0002") {
                    recipe.height = 140
                    return "/pages/list_view/T0002/T0002MainPage.qml"
                } else if (model.brickType === "B0002") {
                    recipe.height = 100
                    return "/pages/list_view/M0002/M0002MainPage.qml"
                }
            }
        }

        Item {
            id: secondPage
            Rectangle{
                anchors.fill: secondPage
                color: "green"

                border.color: "orange"
                border.width: 1
            }
        }
    }

    PageIndicator {
        opacity: recipe.detailsOpacity
        enabled: recipe.detailsEnabled
        count: detailsSwipeView.count
        currentIndex: detailsSwipeView.currentIndex
        anchors.bottom: detailsSwipeView.bottom
        anchors.horizontalCenter: detailsSwipeView.horizontalCenter
    }

    Component {
        id: rectR
        Rectangle{
            width: 50
            height: 50
            color: "red"
        }
    }
}
