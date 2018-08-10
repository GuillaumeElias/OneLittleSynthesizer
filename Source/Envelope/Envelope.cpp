/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "Envelope.h"

//============================================================================
Envelope::Envelope( AudioProcessorValueTreeState* processorParameters, double splRate, int voiceNumber )
    : AbstractEnvelope( voiceNumber )
    , parameters( processorParameters )
    , sampleRate( splRate )
    , attack( INIT_ENV_ATTACK / 1000.f ) //converts milliseconds to seconds
    , decay( INIT_ENV_DECAY / 1000.f )
    , sustain( INIT_ENV_SUSTAIN )
    , release( INIT_ENV_RELEASE / 1000.f )
    , decayDelta ( 1 - INIT_ENV_SUSTAIN )
    , sampleIndex( 0 )
    , currentGain( 0.f )
    , hitReleaseGain( 0.f )
    , hitReleaseGainRatio ( 0.f )
{
    parameters->addParameterListener("envAttack", this);
    parameters->addParameterListener("envDecay", this);
    parameters->addParameterListener("envSustain", this);
    parameters->addParameterListener("envRelease", this);
}

//============================================================================
Envelope::~Envelope()
{
    parameters->removeParameterListener("envAttack", this);
    parameters->removeParameterListener("envDecay", this);
    parameters->removeParameterListener("envSustain", this);
    parameters->removeParameterListener("envRelease", this);
}

//============================================================================
void Envelope::noteOn()
{
    resetNote();
    currentPhase = ATTACK;
}

//============================================================================
void Envelope::noteOff( bool allowTailOff )
{
    if( allowTailOff )
    {
        if( currentPhase != OFF && currentPhase != RELEASE ) //ignore duplicated noteOff midi messages
        {
            triggerRelease();
        }
    }
    else
    {
        resetNote();
        notifyEndNote();
    }
}

//============================================================================
float Envelope::computeGain()
{
    float deltaTime = sampleIndex.get() / sampleRate;

    switch( currentPhase )
    {
        case ATTACK:

            currentGain = deltaTime / attack.get();

            if( currentGain >= 1.f )
            {
                sampleIndex = 0;
                currentPhase = DECAY;
            }
            break;

        case DECAY:

            currentGain = 1 - deltaTime * decayDelta.get() / decay.get();

            if( currentGain <= sustain.get() )
            {
                sampleIndex = 0;
                currentPhase = SUSTAIN;
            }

            break;

        case SUSTAIN:

            currentGain = sustain.get();

            break;

        case RELEASE:

            currentGain = hitReleaseGain.get() - deltaTime * hitReleaseGainRatio.get();

            if( currentGain <= 0.f )
            {
                resetNote();
                notifyEndNote();
            }

            break;

        default:
            currentGain = 0.f;
    }

    ++sampleIndex;
    notifyProgress(deltaTime, currentGain);

    return currentGain;
}

//============================================================================
void Envelope::resetNote()
{
    currentPhase = OFF;
    sampleIndex = 0;
    hitReleaseGain = 0.f;
}

//============================================================================
void Envelope::triggerRelease()
{
    hitReleaseGain = currentGain;
    hitReleaseGainRatio = hitReleaseGain.get() / release.get();
    sampleIndex = 0;
    currentPhase = RELEASE;
}

//============================================================================
void Envelope::parameterChanged(const String& parameterID, float newValue )
{
    if( parameterID == "envAttack" )
    {
        attack = newValue / 1000.f;
    }
    else if( parameterID == "envDecay" )
    {
        decay = newValue / 1000.f;
    }
    else if( parameterID == "envSustain" )
    {
        sustain = newValue;
        decayDelta = 1 - sustain.get();
    }
    else if( parameterID == "envRelease" )
    {
        release = newValue / 1000.f;
        if( currentPhase == RELEASE )
        {
            hitReleaseGainRatio = hitReleaseGain.get() / release.get();
        }
    }
}
