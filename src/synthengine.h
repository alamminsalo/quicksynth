#ifndef SYNTHENGINE_H
#define SYNTHENGINE_H

/// \class SynthEngine
/// \author Antti Lamminsalo
///
/// \brief Engine for handling multiple synth playback.
///
/// Singleton

#include <QObject>
#include <QList>
#include <stk/Stk.h>
#include <stk/RtWvOut.h>
#include "synth.h"
#include "synthinterface.h"
#include <QMutex>

class SynthEngine : public QObject
{
    Q_OBJECT

    static SynthEngine *s_instance;

    explicit SynthEngine(QObject *parent = 0);

public:
    static SynthEngine *getInstance(QObject *parent = 0);

    ~SynthEngine();

    void tick();

    Synth *getSynth(uint index);
    uint synthCount() const;
    void addSynth(Synth *s);

    void setInterface(SynthInterface *interf);

    float getAttack() const;
    float getDecay() const;
    float getSustain() const;
    float getRelease() const;

signals:
    void started();
    void stopped();
    void shutdown();

public slots:
    void run();
    void noteOn(int key);
    void noteOff(int key);
    void killNote(int key);
    void setPitch(float pitch);
    void setSquareAmount(float amount);
    void setDetuneAmount(float amount);
    void setUnisonCount(unsigned int amount);
    void setRelease(float release);
    void setSustain(float sustain);
    void setDecay(float decay);
    void setAttack(float attack);

    void updateADSR();

private:
    stk::RtWvOut *m_dac = 0;
    QList<Synth *> m_synths;

    bool m_playing = false;
    bool m_running = true;

    QMutex m_detuneMutex;
    QMutex m_squareMutex;
    QMutex m_pitchMutex;
    QMutex m_unisonCountMutex;

    float m_squareAmount = 0.0;
    float m_unisonDetune = 0.0;
    float m_pitch = 0.0;
    unsigned int m_unisonCount = 1;

    //ADSR
    float m_attack = 0.1;
    float m_decay = 1.0;
    float m_sustain = 0.5;
    float m_release = 1.0;
};

#endif // SYNTHENGINE_H
