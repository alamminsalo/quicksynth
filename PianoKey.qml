import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: root

    property int key
    property bool black: false

    border {
        width: 2
        color: mArea.pressed ? "#aaa" : "#111"
    }

    color: root.black ? "black" : "white"

    anchors {
        top: parent.top
        bottom: parent.bottom
        bottomMargin: black ? parent.height * 0.4 : 0
    }

    signal noteOn()
    signal noteOff()

    MouseArea {
        id: mArea
        anchors.fill: parent
        propagateComposedEvents: false
        onPressedChanged: {
            if (pressed)
                root.parent.keyOn(root.key)
            else
                root.parent.keyOff(root.key)
        }
    }
}
