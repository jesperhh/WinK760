import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtGraphicalEffects 1.0

Item {
    id: root
    width: 350
    height: 350

    Rectangle
    {
        id: background
        anchors.fill: parent

        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#000000";
            }
            GradientStop {
                position: 1.00;
                color: "#363636"
            }
        }
    }

    Item {
        id: contentRoot
        clip: false
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 20

        Text {
            id: caption
            color: "#ffffff"
            text: qsTr("Logitech K760 Control Panel")
            font.pointSize: 18
            font.family: "Segoe UI"
        }

        CheckBox {
            id: check_box1
            y: 70
            width: parent.width
            text: "When this box is checked, press F1, F2, etc. keys to use the standard function keys, and press and hold the Fn key to use the enhanced functions printed on the keys."
            checked: appContext.RemapFunctionKeys
            onCheckedChanged: { appContext.RemapFunctionKeys = checked }
            style: CheckBoxStyle {
                id: style1
                label: Text {
                    width: check_box1.width - 24
                    color: "white"
                    text: check_box1.text
                    clip: false
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
            }
        }

        Button {
            id: battery
            text: "Battery: " + appContext.Battery
            onClicked: appContext.refreshBattery();
        }
    }

    Item {
        id: loading
        visible: appContext.Locked
        property real offset: 0.0
        property real cX: root.width / 2.0
        property real cY: root.height / 2.0
        anchors.fill: parent

        NumberAnimation on offset {
            from: 0.0
            to: Math.PI * 2.0
            duration: 3000
            onStopped: { restart(); }
        }

        Rectangle {
            anchors.fill: parent
            color: "#000000"
            opacity: 0.5
        }
        MouseArea {
            anchors.fill: parent
        }

        Text {
            text: qsTr("Updating ...")
            anchors.centerIn: parent
            font.pointSize: 20
            color: "white"
        }

        Repeater {
            id: repeater
            model: 20
            Rectangle {
                smooth: true
                color: "white"
                width: 20 - index / 2.0
                height: width
                radius: width / 2.0
                x: Math.cos(loading.offset + index * 2.0 * Math.PI / repeater.model) * loading.cX * 0.7 + loading.cX - radius
                y: Math.sin(loading.offset + index * 2.0 * Math.PI / repeater.model) * loading.cY * 0.7 + loading.cY - radius
            }
        }
    }
}
