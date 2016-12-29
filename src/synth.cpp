#include "synth.h"

void Synth::setPlaying(bool value)
{
    playing = value;
}

Synth::Synth(QObject *parent)
    : QObject(parent)
{
    frequency = utils::itof(3);
    pitch = 0.0;
    unisonAmount = 0.0;
    squareAmount = 0.0;
    volume = 1.0;
    playing = false;
}

Synth::~Synth()
{
    clear();

    if (unisonFreqs)
        delete []unisonFreqs;
}

void Synth::setKey(const int &key)
{
    this->key = key;
    this->frequency = utils::itof(this->key);
    this->playing = true;

    adsr.keyOn();
}

void Synth::setKeyOff()
{
    adsr.keyOff();
}

void Synth::setPitch(const StkFloat &pitch)
{
    this->pitch = pitch;
}

void Synth::setUnisonAmount(const StkFloat &unisonAmount)
{
    this->unisonAmount = unisonAmount;

    unsigned int count = waves.size();

    //Recreate unisonFreqs table
    if (unisonFreqs)
        delete []unisonFreqs;

    unisonFreqs = new double[count];

    utils::unisonDetune(frequency, unisonAmount, count, unisonFreqs);
}

void Synth::setSquareAmount(const StkFloat &squareAmount)
{
    this->squareAmount = squareAmount;
}

void Synth::setVolume(const StkFloat &value)
{
    volume = value;
}

void Synth::setUnisonCount(const unsigned int &count)
{
    waves.reserve(count);

    while (count < waves.size())
    {
        delete waves.back();
        waves.pop_back();
    }

    while (count > waves.size())
    {
        waves.push_back(new SineWave());
    }

    if (count > 0)
    {
        setUnisonAmount(unisonAmount);
        setVolume(1.0f / count);
    }
}

void Synth::clear()
{
    foreach (const stk::SineWave *s, waves)
        delete s;

    waves.clear();
}

void Synth::reset()
{
    foreach (SineWave *wv, waves)
    {
        wv->reset();
    }
}

void Synth::setADSREnvelope(const StkFloat &atk, const StkFloat &decay, const StkFloat &sustain, const StkFloat &release)
{
    adsr.setAllTimes(atk, decay, sustain, release);
}

StkFloat Synth::tick()
{
    StkFloat val = 0;
    for (int i=0; i < waves.size(); i++)
    {
        waves[i]->setFrequency(frequency + unisonFreqs[i] + pitch);

        StkFloat tick = waves[i]->tick();
        if (squareAmount > 0.0)
        {
            if (squareAmount < 1.0)
            {
                tick = utils::distort(tick, squareAmount);
            }

            else
            {
                tick = utils::square(tick);
            }
        }

        val += tick;
    }

    StkFloat adsr_value = adsr.tick();

    if (playing && adsr.getState() == ADSR::IDLE)
        playing = false;

    return val * volume * adsr_value;
}

int Synth::getKey() const
{
    return key;
}

bool Synth::isPlaying() const
{
    return playing;
}
