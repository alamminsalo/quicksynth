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

void SynthEngine::setInterface(SynthInterface *interf)
{
    if (interf)
    {
        connect(interf, SIGNAL(noteOn(int)), this, SLOT(noteOn(int)), Qt::DirectConnection);
        connect(interf, SIGNAL(noteOff(int)), this, SLOT(noteOff(int)), Qt::DirectConnection);
        connect(interf, SIGNAL(pitch(float)), this, SLOT(setPitch(float)), Qt::DirectConnection);
        connect(interf, SIGNAL(squareAmount(float)), this, SLOT(setSquareAmount(float)), Qt::DirectConnection);
        connect(interf, SIGNAL(detuneAmount(float)), this, SLOT(setDetuneAmount(float)), Qt::DirectConnection);
        connect(interf, SIGNAL(unisonCount(unsigned int)), this, SLOT(setUnisonCount(unsigned int)), Qt::DirectConnection);
        connect(interf, SIGNAL(attack(float)), this, SLOT(setAttack(float)), Qt::DirectConnection);
        connect(interf, SIGNAL(decay(float)), this, SLOT(setDecay(float)), Qt::DirectConnection);
        connect(interf, SIGNAL(sustain(float)), this, SLOT(setSustain(float)), Qt::DirectConnection);
        connect(interf, SIGNAL(release(float)), this, SLOT(setRelease(float)), Qt::DirectConnection);
    }
}

void SynthEngine::tick()
{
    double value = 0.0;

    if (synthCount() > 0)
    {
        for (unsigned int i=0; i < m_synths.length(); i++)
        {
            Synth *s = m_synths.at(i);

            if (s->isPlaying())
                value += s->tick();
            else
            {
                delete m_synths.takeAt(i--);
            }
        }
    }

    //Clamp at +- 1.0 boundaries
    if (value > 1.0f)
        value = 1.0f;

    else if (value < -1.0f)
        value = -1.0f;

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
    if (s) {
        s->setSquareAmount(m_squareAmount);
        s->setUnisonAmount(m_unisonDetune);
        s->setUnisonCount(m_unisonCount);
        s->setADSREnvelope(m_attack, m_decay, m_sustain, m_release);
        m_synths << s;

        s->setVolume(1.0f / synthCount());
    }
}

void SynthEngine::run()
{
    qDebug() << "STARTED synthengine";
    while(m_running)
    {
        tick();
    }
    qDebug() << "Stopped synthengine";
}

void SynthEngine::noteOn(int key)
{
    //Remove possibly playing key
    killNote(key);

    //Setup new sound for that key
    Synth *s = new Synth();
    s->setKey(key);
    addSynth(s);
}

void SynthEngine::noteOff(int key)
{
    for (unsigned int i=0; i < m_synths.length(); i++)
    {
        Synth *s = m_synths.at(i);

        if (s->getKey() == key)
            s->setKeyOff();
    }
}

void SynthEngine::killNote(int key)
{
    for (unsigned int i=0; i < m_synths.length(); i++)
    {
        Synth *s = m_synths.at(i);

        if (s->getKey() == key)
            s->setPlaying(false);
    }
}

void SynthEngine::setPitch(float pitch)
{
    m_pitch = pitch;
    foreach (Synth *s, m_synths)
    {
        s->setPitch(m_pitch);
    }
}

void SynthEngine::setSquareAmount(float amount)
{
    qDebug() << "Set sqr: " << amount;
    m_squareAmount = amount;
    foreach (Synth *s, m_synths)
    {
        s->setSquareAmount(m_squareAmount);
    }
}

void SynthEngine::setDetuneAmount(float amount)
{
    qDebug() << "Set detune: " << amount;
    m_unisonDetune = amount;

    if (m_detuneMutex.tryLock(10))
    {
        foreach (Synth *s, m_synths)
        {
            s->setUnisonAmount(m_unisonDetune);
        }

        m_detuneMutex.unlock();
    }
}

void SynthEngine::setUnisonCount(unsigned int amount)
{
    qDebug() << "Set unison: " << amount;
    m_unisonCount = amount;

    if (m_unisonCountMutex.tryLock(10))
    {
        foreach (Synth *s, m_synths)
        {
            s->setUnisonCount(m_unisonCount);
        }

        m_unisonCountMutex.unlock();
    }
}

float SynthEngine::getRelease() const
{
    return m_release;
}

void SynthEngine::setRelease(float release)
{
    m_release = release;

    if (m_release <= 0.0f)
        m_release = 0.01f;

    updateADSR();
}

float SynthEngine::getSustain() const
{
    return m_sustain;
}

void SynthEngine::setSustain(float sustain)
{
    m_sustain = sustain;

    if (m_sustain <= 0.0f)
        m_sustain = 0.01f;


    updateADSR();
}

float SynthEngine::getDecay() const
{
    return m_decay;
}

void SynthEngine::setDecay(float decay)
{
    if (m_decay <= 0.0f)
        m_decay = 0.01f;

    m_decay = decay;
    updateADSR();
}

float SynthEngine::getAttack() const
{
    return m_attack;
}

void SynthEngine::setAttack(float attack)
{
    m_attack = attack;

    qDebug() << attack;

    if (m_attack <= 0.0f)
        m_attack = 0.01f;

    updateADSR();
}

void SynthEngine::updateADSR()
{
    foreach(Synth *s, m_synths)
    {
        s->setADSREnvelope(m_attack, m_decay, m_sustain, m_release);
    }
}
