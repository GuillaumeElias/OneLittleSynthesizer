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

        float renderWave();
        void setFrequency(double frequency);
        double getFrequency() const;

		static String waveShapeToString(float waveShapeFloat);
        static void setSampleRate(double sampleRate);

    private:

        float renderSine();
        float renderSaw();
        float renderTriangle();
        float renderSquare();
        void currentAngleTick();

        void parameterChanged(const String& parameterID, float newValue ) override;

        static double sample_rate;

        AudioProcessorValueTreeState * parameters;
        WaveShape currentWaveShape;
        double currentAngle, angleDelta, frequency;

        String waveShapeParameterName;

};
