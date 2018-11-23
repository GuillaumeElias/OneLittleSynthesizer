/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "Oscillator.h"

/* This class is used as a layer between the SynthVoice and the oscillators, to modulate their frequencies and mixing them together*/
class FMEngine
{
public:

    FMEngine(Oscillator * oscillator1, Oscillator * oscillator2, double sampleRate);

    float renderAndMixWaves(); //renders both oscillators, applies Frequency Modulation and mix them together

    static void setOsc2Mod1Amount(float);
    static void setWaveMix(float);
private:

    static Atomic<float> osc2mod1Amount, waveMix;

    double sampleRate;

    Oscillator * osc1;
    Oscillator * osc2;
};
