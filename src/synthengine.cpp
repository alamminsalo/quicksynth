#include "synthengine.h"
#include <QDebug>

//Initialize singleton as null value
SynthEngine *SynthEngine::s_instance = 0;

SynthEngine *SynthEngine::getInstance(QObject *parent) {
    if (!s_instance) {
        s_instance = new SynthEngine(parent);
    }
    return s_instance;
}

SynthEngine::SynthEngine(QObject *parent) : QObject(parent)
{
    Stk::setSampleRate(globalvar::samplerate);

    try
    {
        m_dac = new RtWvOut(1);
    }
    catch (StkError &e)
    {
        qFatal(QString("Couldn't open audio output: %1").arg(e.getMessageCString()).toStdString().c_str());
    }
}

SynthEngine::~SynthEngine()
{
    m_dac->stop();
    delete m_dac;

    qDeleteAll(m_synths);
}

void SynthEngine::tick()
{
    double value = 0.0;
    foreach (Synth *s, m_synths)
    {
        value += s->tick();
    }

    m_dac->tick(value / synthCount());
}

Synth *SynthEngine::getSynth(uint index)
{
    if (index < m_synths.size()) {
        return m_synths.at(index);
    }

    return 0;
}

uint SynthEngine::synthCount() const
{
    return m_synths.size();
}
