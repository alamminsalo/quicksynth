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
        m_dac = new RtWvOut(1, Stk::sampleRate(), 0, globalvar::buffer_size);
    }
    catch (StkError &e)
    {
        qFatal(QString("Couldn't open audio output: %1").arg(e.getMessageCString()).toStdString().c_str());
    }
}

SynthEngine::~SynthEngine()
{
    m_running = false;

    qDeleteAll(m_synths);

    m_dac->stop();
    delete m_dac;
}

void SynthEngine::tick()
{
    double value = 0.0;

    if (synthCount() > 0 && m_playing)
    {
        foreach (Synth *s, m_synths)
        {
            value += s->tick();
        }
        value /= synthCount();
    }

    m_dac->tick(value);
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

void SynthEngine::addSynth(Synth *s)
{
    if (s)
        m_synths << s;
}

void SynthEngine::setInterface(SynthInterface *interf)
{
    if (interf)
    {
        connect(interf, SIGNAL(noteOn(float)), this, SLOT(noteOn(float)), Qt::DirectConnection);
        connect(interf, SIGNAL(noteOff()), this, SLOT(noteOff()), Qt::DirectConnection);
        connect(interf, SIGNAL(pitch(float)), this, SLOT(setPitch(float)), Qt::DirectConnection);
        connect(interf, SIGNAL(squareAmount(float)), this, SLOT(setSquareAmount(float)), Qt::DirectConnection);
        connect(interf, SIGNAL(detuneAmount(float)), this, SLOT(setDetuneAmount(float)), Qt::DirectConnection);
        connect(interf, SIGNAL(unisonCount(unsigned int)), this, SLOT(setUnisonCount(unsigned int)), Qt::DirectConnection);
    }
}

#include <QThread>
void SynthEngine::run()
{
    qDebug() << "STARTED synthengine";
    qDebug() << "Thread: " << QThread::currentThreadId();
    while(m_running)
    {
        tick();
    }
    qDebug() << "Stopped synthengine";
}

void SynthEngine::noteOn(float key)
{
    setKey(key);

    foreach (Synth *s, m_synths)
    {
        s->reset();
    }

    m_playing = true;
}

void SynthEngine::noteOff()
{
    m_playing = false;
}

void SynthEngine::setKey(float key)
{
    foreach (Synth *s, m_synths)
    {
        s->setKey(key);
    }
}

void SynthEngine::setPitch(float pitch)
{
    foreach (Synth *s, m_synths)
    {
        s->setPitch(pitch);
    }
}

void SynthEngine::setSquareAmount(float amount)
{
    qDebug() << "Set sqr: " << amount;
    foreach (Synth *s, m_synths)
    {
        s->setSquareAmount(amount);
    }
}

void SynthEngine::setDetuneAmount(float amount)
{
    qDebug() << "Set detune: " << amount;
    foreach (Synth *s, m_synths)
    {
        s->setUnisonAmount(amount);
    }
}

void SynthEngine::setUnisonCount(unsigned int amount)
{
    qDebug() << "Set unison: " << amount;
    foreach (Synth *s, m_synths)
    {
        s->setUnisonCount(amount);
    }
}
