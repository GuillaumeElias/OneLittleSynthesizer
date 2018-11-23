/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Oscillator : private AudioProcessorValueTreeState::Listener
{
    public:
        enum WaveShape { SINE, SAW, TRIANGLE, SQUARE, SILENCE };

        Oscillator(AudioProcessorValueTreeState * processorParameters, const String & waveShapeParameterName);
        ~Oscillator();

        virtual float renderWave(float angleInc = 0); //renders the currentSample based on internal currentAngle + additional angleInc (0 by default)
        void setFrequency(double frequency);
        double getFrequency() const;

		static String waveShapeToString(float waveShapeFloat);
        static void setSampleRate(double sampleRate);


    protected:
        WaveShape currentWaveShape;
        double currentAngle, angleDelta, frequency;

    private:

        float renderSine();
        float renderSaw();
        float renderTriangle();
        float renderSquare();
        void currentAngleTick();

        void parameterChanged(const String& parameterID, float newValue ) override;

        static double sample_rate;

        AudioProcessorValueTreeState * parameters;

        String waveShapeParameterName;

};
