import QtQuick
import QtQuick.Controls
import "./M0002"

Item{
    id: brickSwipe
    property int swipeIndex
    property bool details

    function setSwipeIndex(index) {
        console.log("setSwipeIndex");
        // Możesz tu wykonać dowolne operacje
        detailsSwipeView.currentIndex = index;
    }

    SwipeView {
        id: detailsSwipeView
        anchors.fill: parent

        currentIndex: swipeIndex
        interactive: details

        Loader {
            id: loader
            source: {
                if (model.brickType === "T0002") {
                    //recipe.height = 200
                    return "/pages/list_view/T0002/T0002MainPage.qml"
                } else if (model.brickType === "B0002") {
                    //recipe.height = 120
                    return "/pages/list_view/M0002/M0002MainPage.qml"
                }
            }

            Binding {
                target: loader.item
                property: "details"
                value: brickSwipe.details
            }
        }

        Loader {
            source: "/pages/list_view/common/NetworkSettings.qml"
        }


    }

    PageIndicator {
        opacity: details ? 1 : 0
        enabled: details
        count: detailsSwipeView.count
        currentIndex: detailsSwipeView.currentIndex
        anchors.bottom: detailsSwipeView.bottom
        anchors.horizontalCenter: detailsSwipeView.horizontalCenter
    }

}
