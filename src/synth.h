#ifndef SYNTH_H
#define SYNTH_H

#include <vector>
#include <QDebug>
#include <qmath.h>
#include <stk/SineWave.h>
#include <stk/ADSR.h>
#include <stk/Stk.h>
#include <stk/FileWvOut.h>

#include "./global.h"
#include "./utils.h"

using namespace stk;

/*
 * Synth oscillator class
 */
class Synth: public QObject
{
    Q_OBJECT

    std::vector<SineWave *> waves;
    double *unisonFreqs = 0;

    int key;

	//Base frequency this synth is using in hz
    StkFloat frequency;

	//Pitch is added to base frequency
	StkFloat pitch;

	//Unison detune amount in hz
	StkFloat unisonAmount;

	//Square factor
	StkFloat squareAmount;

    //Volume
    StkFloat volume;

    //Playing
    bool playing;

    //ADSR envelope
    ADSR adsr;

public:

    explicit Synth(QObject *parent = 0);
    ~Synth();

    void setKey(const int &key);
    void setKeyOff();
    void setPitch(const StkFloat &pitch);
    void setUnisonAmount(const StkFloat &unisonAmount);
    void setSquareAmount(const StkFloat &squareAmount);
    void setVolume(const StkFloat &value);
    void setUnisonCount(const unsigned int &count);

    void clear();
    void reset();

    void setADSREnvelope(const StkFloat &atk, const StkFloat &decay, const StkFloat &sustain, const StkFloat &release);

    StkFloat tick();
    int getKey() const;
    bool isPlaying() const;
    void setPlaying(bool value);
};

#endif //SYNTH_H

