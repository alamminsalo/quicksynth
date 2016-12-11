#ifndef UTILS_H
#define UTILS_H

#include "./global.h"
#include <vector>

namespace utils
{

/*
 * Returns wanted seconds fitted with samplerate
 */
long seconds(int count)
{
    return globalvar::samplerate * count;
}

/*
 * Returns frequency from given interval compared to A4 (440.0 hz)
 */
double itof(float interval)
{
    return globalvar::tuning * pow(globalvar::root_step, interval);
}

double pitchShift(double f, float interval)
{
    return f * pow(globalvar::root_step, interval);
}

/*
 * Returns polarized 1 based on input value
 */
double square(double val)
{
    return val > 0 ? double(1) : double(-1);
}

/*
 * Distorts given value by given ceiling
 */
double distort(double val, double ceil)
{
    //Invert ceil
    ceil = (ceil < 0 ? 0 : 1 - ceil);
    return (val > ceil || val < -ceil) ? square(val) : val;
}

/*
 * Scales given value with the volume scale given
 */
double scaleVolume(double val, double scale)
{
    return scale * val;
}

/*
 * Returns detuned vector of floats
 */
std::vector<double> unisonDetune(double freq, double amt, unsigned int count)
{
    std::vector<double> freqs;

    if (count == 1)
    {
        freqs.push_back(0);
    }

    else if (count > 1)
    {
        // Interval for each sound
        double interval = amt * 2.0 / (count - 1);
        double startFreq = freq - amt;

        for (int i=0; i < count; i++)
        {
            double detuned_freq = (startFreq + i * interval) - freq;
            freqs.push_back(detuned_freq);

            std::cout << "Detuned freq i=" << i << ": " << detuned_freq << std::endl;
        }
    }

    return freqs;
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
