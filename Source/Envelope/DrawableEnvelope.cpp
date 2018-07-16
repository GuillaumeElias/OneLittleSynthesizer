/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "DrawableEnvelope.h"

//============================================================================
std::vector<float> DrawableEnvelope::valuesAttack( DRAWABLE_ENVELOPE_NB_VALUES, DRAWABLE_ENVELOPE_INIT_VALUES );
std::vector<float> DrawableEnvelope::valuesRelease( DRAWABLE_ENVELOPE_NB_VALUES, DRAWABLE_ENVELOPE_INIT_VALUES );

float DrawableEnvelope::attackTime = INIT_ENV_ATTACK / 1000.f; //ms to s
float DrawableEnvelope::releaseTime = INIT_ENV_RELEASE / 1000.f;
float DrawableEnvelope::sustainLevel = INIT_ENV_SUSTAIN;

//============================================================================
DrawableEnvelope::DrawableEnvelope(AudioProcessorValueTreeState * processorParameters, double splRate, int voiceNumber)
    : AbstractEnvelope( voiceNumber )
    , parameters( processorParameters )
    , valueIndex( 0 )
    , sampleIndex( 0 )
    , sampleRate( splRate )
{
}

//============================================================================
DrawableEnvelope::~DrawableEnvelope()
{
}

//============================================================================
void DrawableEnvelope::noteOn()
{
    currentPhase = ATTACK;
}

//============================================================================
void DrawableEnvelope::noteOff(bool allowTailOff)
{
    if( allowTailOff )
    {
        currentPhase = RELEASE;
    }
    else
    {
        currentPhase = OFF;
    }

    sampleIndex = 0;
    valueIndex = 0;
}

//============================================================================
float DrawableEnvelope::computeGain()
{
    if(currentPhase == OFF)
    {
        return 0.f;
    }

    if(currentPhase == SUSTAIN)
    {
        return sustainLevel;
    }

    float totalTime = (currentPhase == ATTACK ) ? attackTime : releaseTime;
    float deltaTime = sampleIndex / sampleRate;

    float xTime = deltaTime / totalTime * DRAWABLE_ENVELOPE_NB_VALUES;
    valueIndex = roundToInt( xTime );
    if( valueIndex >= DRAWABLE_ENVELOPE_NB_VALUES )
    {
        valueIndex = DRAWABLE_ENVELOPE_NB_VALUES - 1;
    }

    std::vector<float> * values = &valuesAttack;
    if(currentPhase == RELEASE)
    {
        values = &valuesRelease;
    }

    float gain = values->at(valueIndex);
    if( valueIndex < values->size() - 1 )
    {
        float diffTime = xTime - valueIndex;
        gain += (values->at(valueIndex + 1) - gain) * diffTime;
    }

    notifyProgress(deltaTime, gain);
    sampleIndex++;

    return gain;
}

//============================================================================
const std::vector<float> & DrawableEnvelope::getValuesAttack()
{
    return valuesAttack;
}

//============================================================================
const std::vector<float> & DrawableEnvelope::getValuesRelease()
{
    return valuesRelease;
}

//============================================================================
float DrawableEnvelope::getSustainLevel()
{
    return sustainLevel;
}

//============================================================================
float DrawableEnvelope::getAttackTime()
{
   return attackTime;
}

//============================================================================
float DrawableEnvelope::getReleaseTime()
{
    return releaseTime;
}

//============================================================================
void DrawableEnvelope::setValueAttack(int index, float value)
{
    valuesAttack[index] = value;
}

//============================================================================
void DrawableEnvelope::setValueRelease(int index, float value)
{
    valuesRelease[index] = value;
}

//============================================================================
void DrawableEnvelope::setAttackTime(float attack)
{
    attackTime = attack;
}

//============================================================================
void DrawableEnvelope::setSustainLevel(float sustain)
{
    sustainLevel = sustain;
}

//============================================================================
void DrawableEnvelope::setReleaseTime(float release)
{
    releaseTime = release;
}

//============================================================================
void DrawableEnvelope::resetValues()
{
    valuesAttack = std::vector<float>( DRAWABLE_ENVELOPE_NB_VALUES, DRAWABLE_ENVELOPE_INIT_VALUES );
    valuesRelease = std::vector<float>( DRAWABLE_ENVELOPE_NB_VALUES, DRAWABLE_ENVELOPE_INIT_VALUES );
}
