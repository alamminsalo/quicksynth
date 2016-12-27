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

signals:
    void started();
    void stopped();
    void shutdown();

public slots:
    void run();
    void noteOn(float key);
    void noteOff();
    void setKey(float key);
    void setPitch(float pitch);
    void setSquareAmount(float amount);
    void setDetuneAmount(float amount);
    void setUnisonCount(unsigned int amount);

private:
    stk::RtWvOut *m_dac = 0;
    QList<Synth *> m_synths;
    bool m_playing = false;
    bool m_running = true;
};

#endif // SYNTHENGINE_H
