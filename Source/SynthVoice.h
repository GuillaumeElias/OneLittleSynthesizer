/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "Oscillator.h"
#include "Constants.h"
#include "Envelope/Envelope.h"
#include "Envelope/DrawableEnvelope.h"

#include <map>

class SynthVoice : public SynthesiserVoice, private EnvelopeListener, private AudioProcessorValueTreeState::Listener
{
public:
    SynthVoice( AudioProcessorValueTreeState * processorParameters, int voiceNumber);
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

    Envelope * getEnvelope();
    DrawableEnvelope * getDrawableEnvelope();

private:

    void parameterChanged(const String& parameterID, float newValue ) override; //callback from parameter change
    void onEndNote( int /*voiceNumber*/ ) override; //callback from Envelope

    void setCurrentFilterFreq( float newFilterFreq );
    void updateFilterCoefficients();

    AudioProcessorValueTreeState * parameters;
    SynthSound * currentSynthSound;

    double currentAngle, angleDelta, level;
    float currentFilterFreq, lastFilterFreq; //filter cutoff freq at current sample
    float filterResParam, filterCutoffParam, filterEnvAmountParam;

    Oscillator osc;
    Envelope env;
    DrawableEnvelope drawableEnv;
    dsp::StateVariableFilter::Filter<double> filter;
};
