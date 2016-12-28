import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id: root

    signal keyOn(int key)
    signal keyOff(int key)

    property real whiteWidth: root.width / 7
    property real blackWidth: whiteWidth / 2

    PianoKey {
        id: c
        key: -9
        anchors.left: parent.left
        width: whiteWidth
    }
    PianoKey {
        id: csharp
        anchors.horizontalCenter: c.right
        key: -8
        black: true
        z: 10
        width: blackWidth
    }
    PianoKey {
        id: d
        anchors.left: c.right
        key: -7
        width: whiteWidth
    }
    PianoKey {
        id: dsharp
        anchors.horizontalCenter: d.right
        black: true
        z: 10
        key: -6
        width: blackWidth
    }
    PianoKey {
        id: e
        anchors.left: d.right
        key: -5
        width: whiteWidth
    }
    PianoKey {
        id: f
        anchors.left: e.right
        key: -4
        width: whiteWidth
    }
    PianoKey {
        id: fsharp
        z: 10
        black: true
        anchors.horizontalCenter: f.right
        key: -3
        width: blackWidth
    }
    PianoKey {
        id: g
        anchors.left: f.right
        key: -2
        width: whiteWidth
    }
    PianoKey {
        id: gsharp
        z: 10
        anchors.horizontalCenter: g.right
        black: true
        key: -1
        width: blackWidth
    }
    PianoKey {
        id: a
        anchors.left: g.right
        key: 0
        width: whiteWidth
    }
    PianoKey {
        id: asharp
        z: 10
        anchors.horizontalCenter: a.right
        black: true
        key: 1
        width: blackWidth
    }
    PianoKey {
        id: b
        anchors.left: a.right
        anchors.right: parent.right
        key: 2
        width: whiteWidth
    }
}
