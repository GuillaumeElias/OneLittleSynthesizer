/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "Oscillator.h"
#include "Envelope.h"
#include "Constants.h"

#include <map>

class SynthVoice : public SynthesiserVoice, private EnvelopeListener, private AudioProcessorValueTreeState::Listener
{
public:
    SynthVoice( AudioProcessorValueTreeState * processorParameters );
    ~SynthVoice();

    void prepareToPlay();

    bool canPlaySound (SynthesiserSound* sound) override;

    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* /*sound*/,
                    int /*currentPitchWheelPosition*/) override;

    void stopNote (float /*velocity*/, bool allowTailOff) override;

    void pitchWheelMoved (int /*newValue*/) override;

    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override;

    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

private:

    void parameterChanged(const String& parameterID, float newValue ) override; //callback from parameter change
    void onEndNote() override; //callback from Envelope

    void updateFilterCoefficients();

    AudioProcessorValueTreeState * parameters;
    SynthSound * currentSynthSound;

    double currentAngle, angleDelta, level;
    float filterFreq, filterRes;

    Oscillator osc;
    Envelope env;
    dsp::StateVariableFilter::Filter<double> filter;
};
