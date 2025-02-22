import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Backend 1.0

ScrollablePage {
    id: page
    property string pageName: stackView.deviceName

    Column {
        spacing: 40
        width: parent.width

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Switch is an option button that can be dragged or toggled on or off. "
                + "Switches are typically used to select between two states."
        }

        M0001Controller{
            Component.onCompleted: {
                initialize(stackView.deviceName)
            }
            id: device
            onDeviceInited: {
                firstSwitch.enabled = true;
                firstSwitch.checked = getTurnOn();
                networkMode.enabled = true;
                if(getAPMode()){
                    networkMode.currentIndex = 0;
                }else{
                    networkMode.currentIndex = 1;
                }
            }
        }

        Column {
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter

            ComboBox {
                id: networkMode
                model: ["AP mode", "WiFi mode"]
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: false
                onActivated: {
                    if(networkMode.currentIndex === 0){
                        device.setNetworkConfiguration(true);
                    }else if(networkMode.currentIndex === 1){
                        device.setNetworkConfiguration(false);
                    }
                }
            }

            Switch {
                id: firstSwitch
                text: "First"
                enabled: false
                onToggled: {
                    device.turnOnLed = checked;
                }
            }
            Switch {
                text: "Second"
                checked: true
            }
            Switch {
                text: "Third"
                enabled: false
            }
        }
    }
}

