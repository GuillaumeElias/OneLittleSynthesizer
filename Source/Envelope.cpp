#include "Envelope.h"

//============================================================================
Envelope::Envelope( AudioProcessorValueTreeState* processorParameters, double splRate )
    : parameters( processorParameters )
    , sampleRate( splRate )
    , attack( INIT_ENV_ATTACK / 1000.f ) //converts milliseconds to seconds
    , decay( INIT_ENV_DECAY / 1000.f )
    , sustain( INIT_ENV_SUSTAIN )
    , release( INIT_ENV_RELEASE / 1000.f )
    , currentPhase( OFF )
    , sampleIndex( 0 )
    , currentGain( 0.f )
    , hitReleaseGain( 0.f )
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
void Envelope::addEnvelopeListener( EnvelopeListener * listener )
{
    listeners.add( listener );
}

//============================================================================
void Envelope::removeEnvelipeListener( EnvelopeListener * listener )
{
    listeners.remove( listener );
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
    float deltaTime = sampleIndex / sampleRate;

    switch( currentPhase )
    {
        case ATTACK:

            currentGain = deltaTime / attack;

            if( currentGain >= 1.f )
            {
                sampleIndex = 0;
                currentPhase = DECAY;
            }
            break;

        case DECAY:

            currentGain = 1 - deltaTime * sustain / decay;

            if( currentGain <= sustain )
            {
                sampleIndex = 0;
                currentPhase = SUSTAIN;
            }

            break;

        case SUSTAIN:

            currentGain = sustain;

            break;

        case RELEASE:

            currentGain = hitReleaseGain - deltaTime * hitReleaseGain / release;

            if( currentGain <= 0.f )
            {
                resetNote();
                notifyEndNote();
            }

            break;

        default:
            currentGain = 0.f;
    }

    sampleIndex++;

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
    sampleIndex = 0;
    currentPhase = RELEASE;
}

//============================================================================
void Envelope::notifyEndNote()
{
    for( auto listener : listeners.getListeners() )
    {
        listener->onEndNote();
    }
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
    }
    else if( parameterID == "envRelease" )
    {
        release = newValue / 1000.f;
    }
}
