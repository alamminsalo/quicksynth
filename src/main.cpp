#include <stk/Stk.h>
#include <stk/RtWvOut.h>
#include <QDebug>
#include "synth.h"

using namespace stk;

int main()
{
    Stk::setSampleRate(SAMPLERATE);

    synth::Synth s;
    s.setUnisonCount(1);
    s.setKey(synth::itof(-12));
    s.setUnisonAmount(0.0);
    s.setSquareAmount(0.66);
    s.setVolume(1.0 / 3);

    RtWvOut *dac = 0;

    try
    {
        dac = new RtWvOut(1);
    }
    catch (StkError &e)
    {
        qDebug() << "Error: " << e.getMessageCString();
    }

    StkFloat value;
    for (unsigned int i=0; i < synth::seconds(5); i++)
    {
        value = 0;

        value += s.tick();

        dac->tick(value);
    }

    delete dac;

    return 0;
}
