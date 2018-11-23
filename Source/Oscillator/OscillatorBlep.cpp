/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 * This file is inspired by Martin Finke's Making Audio Plugins tutorial.
 */

#include "OscillatorBlep.h"
#include "../Constants.h"

 //======================================================================================
OscillatorBlep::OscillatorBlep(AudioProcessorValueTreeState * processorParams, const String & waveShapeParamName)
    : Oscillator(processorParams, waveShapeParamName)
{
}

//======================================================================================
float OscillatorBlep::renderWave(float angleInc)
{
    float sample = Oscillator::renderWave(angleInc);

    if (currentWaveShape == WaveShape::SQUARE || currentWaveShape == WaveShape::SAW)
    {
        double t = currentAngle / TWO_PI;
        sample += polyBlep(t);

        if (currentWaveShape == SQUARE)
        {
            sample -= polyBlep(fmod(t + 0.5, 1.0));
        }
    }

    return sample;
}

 // PolyBLEP by Tale
 // (slightly modified by Martin Finke, god bless this man)
//======================================================================================
float OscillatorBlep::polyBlep(float t)
{
    float dt = angleDelta / TWO_PI;
    // 0 <= t < 1
    if (t < dt) {
        t /= dt;
        return t + t - t * t - 1.0f;
    }
    // -1 < t < 0
    else if (t > 1.0f - dt) {
        t = (t - 1.0f) / dt;
        return t * t + t + t + 1.0f;
    }
    // 0 otherwise
    else return 0.0f;
}




