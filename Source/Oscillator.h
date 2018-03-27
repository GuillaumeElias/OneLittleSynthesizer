#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Oscillator : private AudioProcessorValueTreeState::Listener
{
    enum WaveShape { SINE, SAW, TRIANGLE, SQUARE, SILENCE};

    public:
        Oscillator(AudioProcessorValueTreeState * processorParameters);
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

};
