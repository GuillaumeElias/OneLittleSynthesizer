/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "SynthVoice.h"
#include "SynthSound.h"

// =============================================================================
SynthVoice::SynthVoice( AudioProcessorValueTreeState * processorParameters, int voiceNumber )
    : parameters( processorParameters )
    , currentSynthSound( nullptr )
    , currentAngle (0)
    , angleDelta (0)
    , level (0)
    , currentFilterFreq( INIT_FILTER_FREQUENCY )
    , lastFilterFreq( -1.f )
    , filterResParam ( INIT_FILTER_RESONANCE )
    , filterCutoffParam( INIT_FILTER_FREQUENCY )
    , filterEnvAmountParam( INIT_FILTER_ENV_AMOUNT )
    , osc( processorParameters )
    , env( processorParameters, getSampleRate(), voiceNumber )
    , drawableEnv( processorParameters, getSampleRate(), voiceNumber )
{
    env.addEnvelopeListener(this);

    parameters->addParameterListener("filterCutoffFreq", this);
    parameters->addParameterListener("filterRes", this);
    parameters->addParameterListener("filterEnvAmount", this);

}

// =============================================================================
SynthVoice::~SynthVoice()
{
    parameters->removeParameterListener("filterCutoffFreq", this);
    parameters->removeParameterListener("filterRes", this);
    parameters->removeParameterListener("filterEnvAmount", this);

    env.removeEnvelopeListener(this);
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
    drawableEnv.noteOn();
}

// =============================================================================
void SynthVoice::stopNote (float /*velocity*/, bool allowTailOff)
{
    env.noteOff( allowTailOff );
    drawableEnv.noteOff( allowTailOff );
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

            //compute filter cutoff freq from drawableEnvelope gain and filterFreq parameter
            float totalFilterFreq = filterCutoffParam + drawableEnv.computeGain() * MAX_FILTER_CUTOFF_FREQUENCY * filterEnvAmountParam;
            if( totalFilterFreq <= 0.f )
            {
                currentSample = 0.f; //if filter cutoff is 0 then the note is silent
            }
            else
            {
                //set filter freq based on drawableEnvelopeGain
                setCurrentFilterFreq( totalFilterFreq );

                //apply filter
                currentSample = filter.processSample(currentSample);
            }


            for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                outputBuffer.addSample (i, startSample, currentSample);

            currentAngle += angleDelta;
            currentAngle = std::fmod(currentAngle, TWO_PI);

            ++startSample;
        }
    }
}

// =============================================================================
Envelope * SynthVoice::getEnvelope()
{
    return &env;
}

// =============================================================================
DrawableEnvelope * SynthVoice::getDrawableEnvelope()
{
    return &drawableEnv;
}

// =============================================================================
void SynthVoice::parameterChanged(const String& parameterID, float newValue )
{
    if(parameterID == "filterCutoffFreq")
    {
        filterCutoffParam = newValue;
    }
    else if(parameterID == "filterRes")
    {
        filterResParam = newValue;
    }
    else if(parameterID == "filterEnvAmount")
    {
        filterEnvAmountParam = newValue;
    }

    if ( getSampleRate() > 0 ) //makes sure the voice has been initialized
    {
        updateFilterCoefficients();
    }
}

// =============================================================================
void SynthVoice::setCurrentFilterFreq( float newFilterFreq )
{
    if( lastFilterFreq == newFilterFreq ) return;

    lastFilterFreq = currentFilterFreq;
    currentFilterFreq = newFilterFreq;

    filter.parameters->setCutOffFrequency(getSampleRate(), currentFilterFreq, filterResParam);
}

// =============================================================================
void SynthVoice::updateFilterCoefficients()
{
    filter.parameters->type = dsp::StateVariableFilter::Parameters<double>::Type::lowPass;
    filter.parameters->setCutOffFrequency(getSampleRate(), currentFilterFreq, filterResParam);
}


// =============================================================================
void SynthVoice::onEndNote(int /*voiceNumber*/)
{
    drawableEnv.noteOff(false);

    angleDelta = 0.0;
    currentSynthSound = nullptr;
    clearCurrentNote();
}
