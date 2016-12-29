#ifndef UTILS_H
#define UTILS_H

#include "./global.h"
#include <QString>
#include <qmath.h>
#include <QDebug>
#include <QList>

namespace utils
{

/*
 * Returns wanted seconds fitted with samplerate
 */
inline long seconds(int count)
{
    return globalvar::samplerate * count;
}

/*
 * Returns frequency from given interval compared to A4 (440.0 hz)
 */
inline double itof(float interval)
{
    return globalvar::tuning * pow(globalvar::root_step, interval);
}

/*
 * Converts note to freq
 * Must be format 'c4#', # is optional
 */
inline double stof(QString note)
{
    float interval = 0.0;

    /* Evaluate note */
    switch (note.at(0).toLatin1())
    {
    case 'g':
        interval += 2.0;
    case 'f':
        interval += 1.0;
    case 'e':
        interval += 2.0;
    case 'd':
        interval += 2.0;
    case 'c':
        interval += 1.0;
    case 'b':
        interval += 2.0;
    case 'a':
    default:
        //Do nothing
        break;
    }

    /* Evaluate octave */
    int oct = note.mid(1, 1).toInt();
    interval += oct * 12 - (4 * 12); //Remember A4 is the root position

    /* Check for upper mark */
    if (note.contains('#'))
        interval += 1.0f;

    qDebug() << "Translated " << note << " to " << interval;

    return interval;
}

inline double pitchShift(double f, float interval)
{
    return f * pow(globalvar::root_step, interval);
}

/*
 * Returns polarized 1 based on input value
 */
inline double square(double val)
{
    return val > 0 ? double(1) : double(-1);
}

/*
 * Distorts given value by given ceiling
 */
inline double distort(double val, double ceil)
{
    //Invert ceil
    ceil = (ceil < 0 ? 0 : 1 - ceil);
    return (val > ceil || val < -ceil) ? square(val) : val;
}

/*
 * Scales given value with the volume scale given
 */
inline double scaleVolume(double val, double scale)
{
    return scale * val;
}

/*
 * Returns detuned vector of floats
 */
inline int unisonDetune(double freq, double amt, unsigned int count, double freqs[])
{
    if (count == 1)
    {
        freqs[0] = 0.0;
    }

    else if (count > 1)
    {
        // Interval for each sound
        double interval = amt * 2.0 / (count - 1);
        double startFreq = freq - amt;

        for (unsigned int i=0; i < count; i++)
        {
            freqs[i] = (startFreq + i * interval) - freq;

            //qDebug() << "Detuned freq i=" << i << ": " << detuned_freq;
        }
    }

    return 1;
}

///*
// * Adds distributed amount of phase for each wave
// * 1.0 = 360 degrees of change
// */
//void unisonPhase(std::vector<SineWave> &waves, double amt)
//{
//    if (amt > 1.0)
//        amt = 1.0;
//    else if (amt < -1.0)
//        amt = -1.0;

//    double startPhase = 0 - amt;
//    double phaseInterval = amt * 2 / (waves.size() > 1 ? waves.size() - 1 : 1);

//    std::cout << "Adding phase per wave: " << phaseInterval << std::endl;
//    for (int i=0; i < waves.size(); i++)
//    {
//        double phase = startPhase + phaseInterval * i;
//        waves[i].addPhase(phase);

//        std::cout << "Phase " << i << ": " << phase << std::endl;
//    }
//}
}

#endif // UTILS_H
