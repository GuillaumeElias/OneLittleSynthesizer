/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "FMEngine.h"
#include "../Constants.h"

Atomic<float> FMEngine::osc2mod1Amount = 0.0f;
Atomic<float> FMEngine::waveMix = 0.5f;

//===============================================================================
FMEngine::FMEngine(Oscillator * oscillator1, Oscillator * oscillator2, double splRate)
    : osc1(oscillator1)
    , osc2(oscillator2)
    , sampleRate(splRate)
{
}

//===============================================================================
float FMEngine::renderAndMixWaves()
{
    //render osc2 (modulator for FM)
    float osc2Sample = osc2->renderWave();

    //computing additional angle (for FM) based on both oscillators
    float angleDeltaInc = TWO_PI * osc1->getFrequency() * (1 + osc2mod1Amount.get() * osc2Sample) / sampleRate;

    //render osc1 and mix the two waves
    float osc1Sample = osc1->renderWave(angleDeltaInc) * waveMix.get();
    osc2Sample = osc2Sample * (1 - waveMix.get());

    return osc1Sample + osc2Sample;
}

//===============================================================================
void FMEngine::setOsc2Mod1Amount(float osc2modAmt)
{
    osc2mod1Amount = osc2modAmt;
}

//===============================================================================
void FMEngine::setWaveMix(float mix)
{
    waveMix = mix;
}
