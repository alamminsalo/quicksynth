#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QObject>
#include <QList>
#include <stk/SineWave.h>
#include "global.h"

using namespace stk;

class WaveForm : public SineWave
{
public:
    WaveForm();

    /// Method for allowing custom waveforms setting
    bool setTable(StkFrames &table);

    // Generator interface
public:
    StkFrames &tick(StkFrames &frames, unsigned int channel);

private:
    StkFrames m_table;
};

#endif // WAVEFORM_H
