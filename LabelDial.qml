import QtQuick 2.7
import QtQuick.Controls 2.0

Dial {
    id: root

    property string label
    property double _value
    property bool valueAsInteger: true

    onPositionChanged: {
        _value = from + (position * (to - from))

        if (valueAsInteger)
            _value = Math.round(_value)
    }

    stepSize: 0.01
    
    Label {
        id: squareAmountText
        anchors.centerIn: parent
        text: qsTr("%1: %2").arg(root.label).arg(root._value)
    }
}
