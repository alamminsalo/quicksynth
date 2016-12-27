import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import com.synth.interface 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    function randomNote()
    {
        //Returns random note
        var notes = "abcdefg";
        var upper = "#";
        return (notes[Math.round(Math.random() * (notes.length - 1))] + Math.round(Math.random() * 2.0 + 3.0))
    }

    GridLayout {
        anchors.fill: parent

        Button {
            text: "Play"

            onPressedChanged: {

                if (pressed) {
                    Synth.setNoteOn(randomNote())
                }
                else
                    Synth.noteOff()

            }
        }

        LabelDial {
            id: squareDial
            label: "Square"
            onPositionChanged: Synth.squareAmount(position)
        }

        LabelDial {
            id: detuneDial
            label: "Detune"
            from: 0
            to: 50
            stepSize: 1
            snapMode: Dial.SnapAlways
            on_ValueChanged: Synth.detuneAmount(_value)
        }

        LabelDial {
            id: unisonDial
            label: "Voices"
            from: 1
            to: 8
            snapMode: Dial.SnapAlways
            stepSize: 1
            on_ValueChanged: Synth.unisonCount(_value)
        }
    }
}
