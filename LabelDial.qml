import QtQuick 2.7
import QtQuick.Controls 2.0

Dial {
    id: root
    property string label
    property double _value

    onPositionChanged: {
        _value = Math.round(from + (position * (to - from)))
    }

    stepSize: 0.01
    
    Label {
        id: squareAmountText
        anchors.centerIn: parent
        text: qsTr("%1: %2").arg(root.label).arg(root._value)
    }
}
