/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Oscillator : private AudioProcessorValueTreeState::Listener
{
    enum WaveShape { SINE, SAW, TRIANGLE, SQUARE, SILENCE};

    public:
        Oscillator(AudioProcessorValueTreeState * processorParameters, const String & waveShapeParameterName);
        ~Oscillator();

        float renderWave( double currentPhase );

		static String waveShapeToString(float waveShapeFloat);

    private:
        float renderSine(double currentPhase);
        float renderSaw(double currentPhase);
        float renderTriangle(double currentPhase);
        float renderSquare(double currentPhase);

        void parameterChanged(const String& parameterID, float newValue ) override;

        AudioProcessorValueTreeState * parameters;
        WaveShape currentWaveShape;
        String waveShapeParameterName;

};
