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

            anchors.fill: parent

            LabelDial {
                id: squareDial
                label: "Square"
                valueAsInteger: false
                onPositionChanged: Synth.squareAmount(position)
            }

            GroupBox {

                title: "Unison"

                RowLayout {

                    LabelDial {
                        id: detuneDial
                        label: "Unison detune"
                        from: 0
                        to: 10
                        valueAsInteger: false
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


            GroupBox {

                title: "ADSR"

                RowLayout {

                    LabelDial {
                        id: atkDial
                        label: "Attack"
                        from: 0.01
                        to: 10.0
                        valueAsInteger: false
                        stepSize: 0
                        onPositionChanged: Synth.attack(position * to)
                    }

                    LabelDial {
                        id: decDial
                        label: "Decay"
                        from: 0.01
                        to: 10.0
                        valueAsInteger: false
                        stepSize: 0
                        onPositionChanged: Synth.decay(position * to)
                    }

                    LabelDial {
                        id: susDial
                        label: "Sustain"
                        from: 0.0
                        to: 1.0
                        valueAsInteger: false
                        stepSize: 0
                        onPositionChanged: Synth.sustain(position * to)
                    }

                    LabelDial {
                        id: relDial
                        label: "Release"
                        from: 0.01
                        to: 10.0
                        valueAsInteger: false
                        stepSize: 0
                        onPositionChanged: Synth.release(position * to)
                    }
                }
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

        function applyOctave(key)
        {
            return key + octaveSelector.value * 12
        }

        onKeyOn: {
            Synth.noteOn(applyOctave(key))
        }

        onKeyOff: {
            Synth.noteOff(applyOctave(key))
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
