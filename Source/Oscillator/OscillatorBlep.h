/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 * This file is inspired by Martin Finke's Making Audio Plugins tutorial.
 */

#pragma once
#include "Oscillator.h"

/** This class extends the naive approach of rendering waves in Oscillator, adding PolyBLEP anti-aliasing correction */
class OscillatorBlep : public Oscillator
{
    public:
        OscillatorBlep(AudioProcessorValueTreeState * processorParams, const String & waveShapeParamName);
        virtual float renderWave(float angleInc = 0) override; //adds PolyBLEP correction to parent function

    private:
        float polyBlep(float t);
};
