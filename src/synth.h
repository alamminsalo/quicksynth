#ifndef SYNTH_H
#define SYNTH_H

#include <QList>
#include <QDebug>
#include <qmath.h>
#include <stk/SineWave.h>
#include <stk/Stk.h>
#include <stk/FileWvOut.h>

#include "./global.h"
#include "./utils.h"

using namespace stk;

/*
 * Synth oscillator class
 */
class Synth
{
    QList<SineWave *> waves;
    QList<StkFloat> unisonFreqs;

	//Base frequency this synth is using in hz
	StkFloat key;

	//Pitch is added to base frequency
	StkFloat pitch;

	//Unison detune amount in hz
	StkFloat unisonAmount;

	//Square factor
	StkFloat squareAmount;

    //Volume
    StkFloat volume;
public:

    Synth();
    ~Synth();

    void setKey(const StkFloat &key);
    void setPitch(const StkFloat &pitch);
    void setUnisonAmount(const StkFloat &unisonAmount);
    void setSquareAmount(const StkFloat &squareAmount);
    void setVolume(const StkFloat &value);
    void setUnisonCount(const unsigned int &count);

    void clear();

    StkFloat tick();
};

#endif //SYNTH_H

