import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    ColumnLayout {
        id: columnLayout1
        anchors.fill: parent

        GroupBox {
            id: groupBox1
            width: 200
            height: 200
            Layout.fillWidth: true
            title: qsTr("Group Box")
        }
    }
}
