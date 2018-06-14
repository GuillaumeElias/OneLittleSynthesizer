/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Constants.h"
#include "EnvelopeInterface.h"

//==================================================================================

class Envelope : public EnvelopeInterface, private AudioProcessorValueTreeState::Listener
{
    public:
        Envelope( AudioProcessorValueTreeState* processorParameters, double sampleRate, int voiceNumber);
        ~Envelope();

        void addEnvelopeListener( EnvelopeListener * listener );
        void removeEnvelopeListener( EnvelopeListener * listener );

        void noteOn() override;
        void noteOff( bool allowTailOff ) override;
        float computeGain() override;

    private:

        void resetNote();
        void triggerRelease();
        void notifyEndNote();
        void notifyProgress( float deltaTime );

        void parameterChanged(const String& parameterID, float newValue ) override;

        AudioProcessorValueTreeState* parameters;
        ListenerList<EnvelopeListener> listeners;

        int voiceNumber;

        double sampleRate;
        float attack, decay, sustain, release;
        float decayDelta, hitReleaseGainRatio;
        EnvelopePhase currentPhase;
        int sampleIndex;
        float currentGain, hitReleaseGain;

};
