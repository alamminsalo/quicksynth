#pragma once

#include <stk/SineWave.h>
#include <stk/Stk.h>
#include <stk/FileWvOut.h>
#include <vector>
#include <iostream>
#include <math.h>

#define SAMPLERATE 44100.0
#define TUNING 440.0
#define OCTAVE_ROOT 1.059463094
#define SAMPLE_SIZE 16

using namespace stk;

namespace synth
{

/*
 * Returns wanted seconds fitted with samplerate
 */
long seconds(int count)
{
	return SAMPLERATE * count;
}

/*
 * Returns frequency from given interval compared to A4 (440.0 hz)
 */
StkFloat itof(float interval)
{
	return TUNING * pow(OCTAVE_ROOT, interval);
}

StkFloat pitchShift(StkFloat f, float interval)
{
	return f * pow(OCTAVE_ROOT, interval);
}

/*
 * Returns polarized 1 based on input value
 */
StkFloat square(StkFloat val)
{
	return val > 0 ? StkFloat(1) : StkFloat(-1);
}

/*
 * Distorts given value by given ceiling
 */
StkFloat distort(StkFloat val, StkFloat ceil)
{
	//Invert ceil
	ceil = (ceil < 0 ? 0 : 1 - ceil);
	return (val > ceil || val < -ceil) ? square(val) : val;
}

/*
 * Scales given value with the volume scale given
 */
StkFloat scaleVolume(StkFloat val, double scale)
{
	return scale * val;
}

/*
 * Returns detuned vector of floats
 */
std::vector<StkFloat> unisonDetune(StkFloat freq, StkFloat amt, unsigned int count)
{
	std::vector<StkFloat> freqs;

	if (count == 1)
	{
		freqs.push_back(0);
	}

	else if (count > 1)
	{
		// Interval for each sound
		StkFloat interval = amt * 2.0 / (count - 1);
        StkFloat startFreq = freq - amt;

		for (int i=0; i < count; i++)
		{
			StkFloat detuned_freq = (startFreq + i * interval) - freq;
			freqs.push_back(detuned_freq);

			std::cout << "Detuned freq i=" << i << ": " << detuned_freq << std::endl;
		}
	}

	return freqs;
}

/*
 * Adds distributed amount of phase for each wave
 * 1.0 = 360 degrees of change
 */
void unisonPhase(std::vector<SineWave> &waves, StkFloat amt)
{
	if (amt > 1.0)
		amt = 1.0;
	else if (amt < -1.0)
		amt = -1.0;

	StkFloat startPhase = 0 - amt;
	StkFloat phaseInterval = amt * 2 / (waves.size() > 1 ? waves.size() - 1 : 1);

	std::cout << "Adding phase per wave: " << phaseInterval << std::endl;
	for (int i=0; i < waves.size(); i++)
	{
		StkFloat phase = startPhase + phaseInterval * i;
		waves[i].addPhase(phase);

		std::cout << "Phase " << i << ": " << phase << std::endl;
	}
}

/*
 * Synth oscillator class
 */
class Synth
{
	std::vector<SineWave> waves;
	std::vector<StkFloat> unisonFreqs;

	//Base frequency this synth is using in hz
	StkFloat key;

	//Pitch is added to base frequency
	StkFloat pitch;

	//Unison detune amount in hz
	StkFloat unisonAmount;

	//Square factor
	StkFloat squareAmount;

    //Volume
    StkFloat volume;
public:

	Synth()
	{
        key = itof(0);
		pitch = 0.0;
		unisonAmount = 0.0;
		squareAmount = 0.0;
	}

    void setKey(const StkFloat &key)
	{
        this->key = key;
	}

    void setPitch(const StkFloat &pitch)
	{
        this->pitch = pitch;
	}

    void setUnisonAmount(const StkFloat &unisonAmount)
	{
        this->unisonAmount = unisonAmount;
		unisonFreqs = unisonDetune(key, unisonAmount, waves.size());
	}

    void setSquareAmount(const StkFloat &squareAmount)
	{
        this->squareAmount = squareAmount;
	}

    void setVolume(const StkFloat &value)
    {
        volume = value;
    }

    void setUnisonCount(const unsigned int &count)
	{
		waves.clear();
		unisonFreqs.clear();
		
		for (unsigned int i=0; i < count; i++)
		{
			waves.push_back(SineWave());
		}

		if (count > 0)
		{
			setUnisonAmount(unisonAmount);
		}
	}

	StkFloat tick()
	{
		StkFloat val = 0;
		for (int i=0; i < waves.size(); i++)
		{
			waves[i].setFrequency(key + unisonFreqs[i] + pitch);

			StkFloat tick = waves[i].tick();
            if (squareAmount > 0.0)
			{
				if (squareAmount < 1.0)
				{
					tick = distort(tick, squareAmount);
				}

				else
				{
					tick = square(tick);
				}
			}

            val += tick;
		}

        return scaleVolume(val, volume);
	}
};

} //namespace synth

