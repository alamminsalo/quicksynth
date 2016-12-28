import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import com.synth.interface 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("QuickSynth")

    function randomNote()
    {
        //Returns random note
        var notes = "abcdefg";
        var upper = "#";
        return (notes[Math.round(Math.random() * (notes.length - 1))] + Math.round(Math.random() * 2.0 + 3.0))
    }

    Item {

        id: controlsArea

        anchors {
            top: parent.top
            bottom: pianoRoll.top
            left: parent.left
            right: parent.right
        }

        GridLayout {

            LabelDial {
                id: squareDial
                label: "Square"
                onPositionChanged: Synth.squareAmount(position)
            }

            LabelDial {
                id: detuneDial
                label: "Unison detune"
                from: 0
                to: 10
                on_ValueChanged: Synth.detuneAmount(_value)
            }

            LabelDial {
                id: unisonDial
                label: "Unison count"
                from: 1
                to: 8
                snapMode: Dial.SnapAlways
                stepSize: 1
                on_ValueChanged: Synth.unisonCount(_value)
            }
        }
    }


    RowLayout {
        id: pianoControls
        anchors {
            bottom: pianoRoll.top
            left: parent.left
            right: parent.right
        }

        SpinBox {
            id: octaveSelector
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            from: -3
            to: 3
            value: 0
        }
    }

    Item {
        id: pianoRoll

        signal keyOn(int key)
        signal keyOff(int key)

        onKeyOn: {
            Synth.noteOn(key + octaveSelector.value * 12)
        }

        onKeyOff: {
            Synth.noteOff()
        }

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        height: width * 0.2

        PianoRoll {
            id: piano0
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: parent.horizontalCenter
            }
            onKeyOn: pianoRoll.keyOn(key)
            onKeyOff: pianoRoll.keyOff(key)
        }

        PianoRoll {
            id: piano2
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.horizontalCenter
                right: parent.right
            }
            onKeyOn: pianoRoll.keyOn(key + 12)
            onKeyOff: pianoRoll.keyOff(key + 12)
        }
    }
}
