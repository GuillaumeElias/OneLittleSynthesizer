/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Constants.h"
#include "AbstractEnvelope.h"

//==================================================================================

class Envelope : public AbstractEnvelope, private AudioProcessorValueTreeState::Listener
{
    public:
        Envelope( AudioProcessorValueTreeState* processorParameters, double sampleRate, int voiceNumber);
        ~Envelope();

        void noteOn() override;
        void noteOff( bool allowTailOff ) override;
        float computeGain() override;

    private:
        void resetNote();
        void triggerRelease();

        void parameterChanged(const String& parameterID, float newValue ) override;

        AudioProcessorValueTreeState* parameters;

        double sampleRate;
        Atomic<float> attack, decay, sustain, release;
        Atomic<float> decayDelta, hitReleaseGainRatio, hitReleaseGain;
        Atomic<int> sampleIndex;
        float currentGain;

};
