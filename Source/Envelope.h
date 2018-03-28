/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Constants.h"

//==================================================================================

class EnvelopeListener
{
    public:
    virtual void onEndNote() {};
    virtual void onSampleIncrement() {};
};

//==================================================================================

class Envelope : private AudioProcessorValueTreeState::Listener
{
    public:
        Envelope( AudioProcessorValueTreeState* processorParameters, double sampleRate);
        ~Envelope();

        void addEnvelopeListener( EnvelopeListener * listener );
        void removeEnvelipeListener( EnvelopeListener * listener );

        void noteOn();
        void noteOff( bool allowTailOff );
        float computeGain();

    private:

        enum Phase { OFF, ATTACK, DECAY, SUSTAIN, RELEASE };

        void resetNote();
        void triggerRelease();
        void notifyEndNote();

        void parameterChanged(const String& parameterID, float newValue ) override;

        AudioProcessorValueTreeState* parameters;
        ListenerList<EnvelopeListener> listeners;

        double sampleRate;
        float attack, decay, sustain, release;
        Phase currentPhase;
        int sampleIndex;
        float currentGain, hitReleaseGain;

};
