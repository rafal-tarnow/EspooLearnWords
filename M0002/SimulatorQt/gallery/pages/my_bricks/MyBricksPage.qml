import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Qt.labs.settings 1.0
import "."
import Backend

Page {
    id: page
    property int controlsHeight: 40
    // background: Qt.transparent
    background: Rectangle {
        color: "#f0eef0"
    }
    ListModel {
        id: devicesModel
    }

    PseudoDNSServer{
        id: dnsServer
        onHostFound:  function (hostId, hostType, hostName, hostIp) {
            console.log("DNS host found " + hostId + " " + hostType + " " + hostName + " " + hostIp)
            devicesModel.append({ text: hostName, ip: hostIp})
        }
        Component.onCompleted: {
            dnsServer.startQueriesForAllHosts()
        }
    }

    MyBricksList{
        id: myBricksModel
        onBrickAlreadyAdded: function(brickType, brickName){
            warningDialog.open()
        }
    }

    Menu {
        id: contactMenu
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        modal: true

        Label {
            id: menuTitle
            padding: 10
            font.bold: true
            width: parent.width
            horizontalAlignment: Qt.AlignHCenter
            text: currentContact >= 0 ? contactView.model.get(currentContact).fullName : ""
        }
        //        MenuItem {
        //            text: qsTr("Edit...")
        //            onTriggered: contactDialog.editContact(contactView.model.get(currentContact))
        //        }
        MenuItem {
            text: qsTr("Remove")
            onTriggered: myBricksModel.remove(myBricksListView.currentBrick)
        }
    }



    Rectangle{
        id: tempRect
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        implicitHeight: 100
        //implicitWidth: 250
        //width: 250
        anchors.margins: 15

        clip:true

        Image{
            id: iconImage
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left

            Layout.alignment: Qt.AlignVCenter
            fillMode: Image.PreserveAspectFit
            sourceSize: Qt.size(width,height)
            source:  "qrc:/images/connected_icon.svg"
        }

        Text{
            id: scrollingText

            anchors.left: iconImage.right
            text: "Czujnik kuchnia tempearatura"
            font.bold: true



            NumberAnimation on x{
                from: 0
                to: -1*scrollingText.width/9
                loops: Animation.Infinite
                duration: 600000
            }
        }


        color: "white"
        radius: sliderRadius.value


    }

    MultiEffect {
        source: tempRect
        anchors.fill: tempRect
        shadowEnabled: true
        shadowOpacity: sliderOpacity.value


    }

    Slider {
        //anchors.top: myBricksListView.bottom

        anchors.bottom: sliderRadius.top
        id: sliderOpacity
        value: 0.35
        anchors.horizontalCenter: parent.horizontalCenter

        from: 0.0
        to: 1.0

        Text {
            text: parent.value.toFixed(2) // Wyświetlanie wartości z dokładnością do dwóch miejsc po przecinku
            anchors.verticalCenter: parent.verticalCenter // Wyśrodkowanie tekstu pionowo
            anchors.horizontalCenter: parent.horizontalCenter // Wyśrodkowanie tekstu poziomo
        }
    }

    Slider {
        id: sliderRadius
        //anchors.top: myBricksListView.bottom
        anchors.bottom: parent.bottom

        value: 10
        anchors.horizontalCenter: parent.horizontalCenter

        from: 0.0
        to: 30.0

        Text {
            text: parent.value.toFixed(2) // Wyświetlanie wartości z dokładnością do dwóch miejsc po przecinku
            anchors.verticalCenter: parent.verticalCenter // Wyśrodkowanie tekstu pionowo
            anchors.horizontalCenter: parent.horizontalCenter // Wyśrodkowanie tekstu poziomo
        }
    }

    ListView {
        id: myBricksListView
        model: myBricksModel

        property int currentBrick: -1

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: sliderOpacity.top
        anchors.top: tempRect.bottom
        //        width: parent.width
        //        height: parent.height


        focus: true
        boundsBehavior: Flickable.StopAtBounds

        //        section.property: "brickName"
        //        section.criteria: ViewSection.FirstCharacter
        //        section.delegate: SectionDelegate {
        //            width: myBricksListView.width
        //        }

        delegate: BrickDelegate {
            id: myBrickDelegate
            width: parent.width
            onPressAndHold: {
                console.log("Long press 11")
                myBricksListView.currentBrick = index
                menuTitle.text = model.brickName
                contactMenu.open()
            }

            Controller{
                id: controller

                Component.onCompleted: {
                    console.log("Component.onCompleted:")
                    dnsServer.onHostFound.connect(controller.onHostFoundHandler)

                    var ipaddr = dnsServer.getIpById(model.brickId)
                    console.log("ipaddr = " + ipaddr)
                    if(ipaddr !== "")
                        connectToBrick(ipaddr)
                }

                function onHostFoundHandler(hostId, hostType, hostName, hostIp) {
                    console.log("function onHostFoundHandler " + hostId + " " + hostType + " " + hostName + " " + hostIp)
                    console.log("isBrickConnected = " + isBrickConnected());
                    console.log("mybrickDelegate.brickId = " + myBrickDelegate.brick )
                    if (isBrickConnected() === false && myBrickDelegate.brickId === hostId) {
                        connectToBrick(hostIp)
                    }
                }

                onBrickInfo: function onInfo(id, type, name, ssid, pwd){
                    console.log("LLLLLLLLLLLLLLLL onBrickInfo: function onInfo(id, type, name, ssid, pwd) " + id + " " + type + " " + name + " " + ssid + " " + pwd)
                    if(model.id === id){
                        myBrickDelegate.brickConnected = true
                        if(model.name !== name){
                            console.log("UPPPPPPPPDATE !!!!!!!!!!")
                            myBricksModel.set(index, id, type, name)
                        }
                    }else{
                        controller.disconnectFromBrick()
                    }
                }

                onBrickConnected: {
                    controller.cmdGetInfo();
                }

                onBrickTcpErrorOccurred: {
                    if(isBrickConnected() === false){
                        var ipaddr = dnsServer.getIp(model.name)
                        if(ipaddr !== "")
                            connectToBrick(ipaddr)
                    }
                }

                onBrickDisconnected: {
                    myBrickDelegate.brickConnected = false

                    var ipaddr = dnsServer.getIpById(model.id)
                    if(ipaddr !== "")
                        connectToBrick(ipaddr)
                }

                function celsiusToFahrenheit(celsius) {
                    return (celsius * 9/5) + 32;
                }
            }
        }

        ScrollBar.vertical: ScrollBar { }
    }


    RoundButton {
        text: qsTr("+")
        highlighted: true
        anchors.margins: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: {
            dialog.open()
        }
    }

    Dialog {
        id: warningDialog
        title: qsTr("Warning")
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        standardButtons: Dialog.Ok

        Column {
            spacing: 10
            Text {
                text: "Brick already added!"
            }
        }

        onAccepted: {
            // Tutaj możesz dodać kod do wykonania po zaakceptowaniu ostrzeżenia
        }
    }

    Dialog {
        id: confirmDialog
        title: qsTr("Add new brick ?")
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        standardButtons: Dialog.Ok | Dialog.Cancel

        function confirm(brickId, brickName, brickType, brickIp) {
            confirmId.text = brickId
            confirmName.text = brickName
            confirmType.text = brickType
            confirmIP.text = brickIp
            open();
        }

        contentItem:Column{
            Label {
                id: confirmName
            }
            Label {
                id: confirmType
            }
            Label {
                id: confirmId
            }
            Label {
                id: confirmIP
            }
        }

        onRejected: {
            close()
            dialog.close()
        }

        onAccepted: {
            close()
            dialog.close()
            myBricksModel.append(confirmId.text, confirmType.text, confirmName.text);
        }
    }

    Dialog {
        id: dialog
        property int gap: 20
        x: gap
        y: gap
        width: parent.width - 2*gap
        height: parent.height - 2*gap
        title: qsTr("Avaliable Bricks")
        standardButtons: Dialog.Cancel
        onRejected: {
            close()
        }
        onClosed: {

        }

        onOpened: {

        }

        ListView {
            id: listView
            model: devicesModel

            anchors.fill: parent

            delegate: ItemDelegate{
                id: delegate
                width: parent.width
                checkable: true

                Controller{
                    id: controllerDel
                    onBrickConnected: {
                        console.log("Brick connected")
                        controllerDel.cmdGetInfo();
                    }
                    onBrickInfo: function(id, type, name, ssid, pswd) {
                        typeLabel.text = type
                        idLabel.text = id
                        disconnectFromBrick()
                    }
                    Component.onCompleted: {
                        connectToBrick(model.ip)
                    }
                }

                contentItem: ColumnLayout {
                    spacing: 10

                    RowLayout{
                        spacing: 20
                        anchors.fill: parent

                        ToolButton {
                            action: deviceDetails
                        }

                        Label {
                            id: brickName
                            text: model.text
                            font.bold: true
                            elide: Text.ElideRight
                            Layout.fillWidth: true

                        }

                    }

                    GridLayout {
                        id: grid
                        visible: false

                        columns: 2
                        rowSpacing: 10
                        columnSpacing: 10


                        Label {
                            text: qsTr("Brick Type:")
                            Layout.leftMargin: 60
                        }
                        Label {
                            id: typeLabel
                            font.bold: true
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                        Label {
                            text: qsTr("Brick Id:")
                            Layout.leftMargin: 60
                        }
                        Label {
                            id: idLabel
                            font.bold: true
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                        Label {
                            text: qsTr("Brick IP:")
                            Layout.leftMargin: 60
                        }
                        Label {
                            id: ipLabel
                            text: model.ip
                            font.bold: true
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                    }

                }

                Action {
                    id: deviceDetails
                    icon.name: "show_details"
                    checkable: true
                    onTriggered: {
                    }
                }


                states: [
                    State {
                        name: "expanded"
                        when: deviceDetails.checked

                        PropertyChanges {
                            // TODO: When Qt Design Studio supports generalized grouped properties, change to:
                            grid.visible: true
                            //target: grid
                            //visible: true
                            deviceDetails.icon.name: "hide_details"
                        }
                    }
                ]
                onClicked: {
                    confirmDialog.confirm(idLabel.text, model.text, typeLabel.text, model.ip)
                }
                Component.onCompleted: {
                    console.log("Delegate Item onCompleted()")
                }
            }

        }

    }



}
