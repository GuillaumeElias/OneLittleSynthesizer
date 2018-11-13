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
    , level (0)
    , osc2FrequencyOffsetRatio( 1.0f )
    , currentFilterFreq( INIT_FILTER_FREQUENCY )
    , lastFilterFreq( -1.f )
    , filterResParam ( INIT_FILTER_RESONANCE )
    , filterCutoffParam( INIT_FILTER_FREQUENCY )
    , filterEnvAmountParam( INIT_FILTER_ENV_AMOUNT )
    , waveMix( 0.5f )
    , osc1( processorParameters, "waveShape1" )
    , osc2( processorParameters, "waveShape2" )
    , env( processorParameters, getSampleRate(), voiceNumber )
    , drawableEnv( processorParameters, getSampleRate(), voiceNumber )
{
    env.addEnvelopeListener(this);

    parameters->addParameterListener("filterCutoffFreq", this);
    parameters->addParameterListener("filterRes", this);
    parameters->addParameterListener("filterEnvAmount", this);
    parameters->addParameterListener("waveMix", this);
    parameters->addParameterListener("osc2FreqOffset", this);

}

// =============================================================================
SynthVoice::~SynthVoice()
{
    parameters->removeParameterListener("filterCutoffFreq", this);
    parameters->removeParameterListener("filterRes", this);
    parameters->removeParameterListener("filterEnvAmount", this);
    parameters->removeParameterListener("waveMix", this);
    parameters->removeParameterListener("osc2FreqOffset", this);

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
    level = velocity * 0.15;

    double frequency = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
    osc1.setFrequency(frequency);
    osc2.setFrequency(frequency * osc2FrequencyOffsetRatio);

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
    if (level > 0.0)
    {
        while (--numSamples >= 0)
        {

            //calculate currentSample value from oscillator, envelope and velocity

            float osc1Sample = osc1.renderWave() * waveMix;
            float osc2Sample = osc2.renderWave() * (1 - waveMix);

            float currentSample = (float) ( ( osc1Sample + osc2Sample ) * env.computeGain() * level );

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
    else if (parameterID == "waveMix")
    {
        waveMix = newValue;
        return;
    }
    else if (parameterID == "osc2FreqOffset")
    {
        osc2FrequencyOffsetRatio = newValue;
        osc2.setFrequency(osc1.getFrequency() * osc2FrequencyOffsetRatio);
        return;
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

    level = 0.0;
    currentSynthSound = nullptr;
    clearCurrentNote();
}
