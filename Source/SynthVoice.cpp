/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "SynthVoice.h"
#include "SynthSound.h"

// =============================================================================
SynthVoice::SynthVoice( AudioProcessorValueTreeState * processorParameters )
    : parameters( processorParameters )
    , currentSynthSound( nullptr )
    , currentAngle (0)
    , angleDelta (0)
    , level (0)
    , filterFreq( INIT_FILTER_FREQUENCY )
    , filterRes ( INIT_FILTER_RESONANCE )
    , osc( processorParameters )
    , env( processorParameters, getSampleRate() )
{
    env.addEnvelopeListener(this);

    parameters->addParameterListener("filterFreq", this);
    parameters->addParameterListener("filterRes", this);

}

// =============================================================================
SynthVoice::~SynthVoice()
{
    parameters->removeParameterListener("filterFreq", this);
    parameters->removeParameterListener("filterRes", this);

    env.removeEnvelipeListener(this);
}

// =============================================================================
void SynthVoice::prepareToPlay()
{
    updateFilterCoefficients();
}

// =============================================================================
bool SynthVoice::canPlaySound (SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*> (sound) != nullptr;
}

// =============================================================================
void SynthVoice::startNote (int midiNoteNumber, float velocity,
                SynthesiserSound * sound,
                int /*currentPitchWheelPosition*/)
{
    currentSynthSound = dynamic_cast<SynthSound *> ( sound );
    currentAngle = 0.0;
    level = velocity * 0.15;

    double frequency = MidiMessage::getMidiNoteInHertz (midiNoteNumber);

    angleDelta = TWO_PI * frequency / getSampleRate();

    env.noteOn();
}

// =============================================================================
void SynthVoice::stopNote (float /*velocity*/, bool allowTailOff)
{
    env.noteOff( allowTailOff );
}

// =============================================================================
void SynthVoice::pitchWheelMoved (int /*newValue*/)
{
    // not implemented
}

// =============================================================================
void SynthVoice::controllerMoved (int /*controllerNumber*/, int /*newValue*/)
{
    // not implemented
}

// =============================================================================
void SynthVoice::renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    if (angleDelta != 0.0)
    {
        while (--numSamples >= 0)
        {
            //calculate currentSample value from oscillator, envelope and velocity
            float currentSample = (float) ( osc.renderWave(currentAngle) * env.computeGain() * level );

            //apply filter
            currentSample = filter.processSample(currentSample);

            for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                outputBuffer.addSample (i, startSample, currentSample);

            currentAngle += angleDelta;
            currentAngle = std::fmod(currentAngle, TWO_PI);

            ++startSample;
        }
    }
}

// =============================================================================
void SynthVoice::parameterChanged(const String& parameterID, float newValue )
{
    if(parameterID == "filterFreq")
    {
        filterFreq = newValue;
    }
    else if(parameterID == "filterRes")
    {
        filterRes = newValue;
    }

    if ( getSampleRate() > 0 ) //makes sure the voice has been initialized
    {
        updateFilterCoefficients();
    }
}

// =============================================================================
void SynthVoice::updateFilterCoefficients()
{
    filter.parameters->type = dsp::StateVariableFilter::Parameters<double>::Type::lowPass;
    filter.parameters->setCutOffFrequency(getSampleRate(), filterFreq, filterRes);
}


// =============================================================================
void SynthVoice::onEndNote()
{
    angleDelta = 0.0;
    currentSynthSound = nullptr;
    clearCurrentNote();
}
