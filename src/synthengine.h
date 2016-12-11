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

signals:

public slots:

private:
    stk::RtWvOut *m_dac = 0;
    QList<Synth *> m_synths;
};

#endif // SYNTHENGINE_H
