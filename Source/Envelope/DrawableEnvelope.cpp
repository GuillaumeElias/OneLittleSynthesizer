/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "DrawableEnvelope.h"

//============================================================================
std::vector<Atomic<float>> DrawableEnvelope::valuesAttack( DRAWABLE_ENVELOPE_NB_VALUES, DRAWABLE_ENVELOPE_INIT_VALUES );
std::vector<Atomic<float>> DrawableEnvelope::valuesRelease( DRAWABLE_ENVELOPE_NB_VALUES, DRAWABLE_ENVELOPE_INIT_VALUES );

Atomic<float> DrawableEnvelope::attackTime = INIT_ENV_ATTACK / 1000.f; //ms to s
Atomic<float> DrawableEnvelope::releaseTime = INIT_ENV_RELEASE / 1000.f;
Atomic<float> DrawableEnvelope::sustainLevel = DRAWABLE_ENVELOPE_INIT_VALUES;

//============================================================================
DrawableEnvelope::DrawableEnvelope(AudioProcessorValueTreeState * processorParameters, double splRate, int voiceNumber)
    : AbstractEnvelope( voiceNumber )
    , parameters( processorParameters )
    , valueIndex( 0 )
    , sampleIndex( 0 )
    , sampleRate( splRate )
    , currentGain( 0.0f )
    , hitReleaseGainSustainRatio( 1.0f )
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
        hitReleaseGainSustainRatio = currentGain / sustainLevel.get();
    }
    else
    {
        currentPhase = OFF;
        hitReleaseGainSustainRatio = 1.0f;
        sampleIndex = 0;
        valueIndex = 0;
        notifyProgress(0, 0);
        notifyEndNote();
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
        return sustainLevel.get();
    }

    float totalTime = (currentPhase == ATTACK ) ? attackTime.get() : releaseTime.get();
    float deltaTime = sampleIndex / sampleRate;

    float xTime = deltaTime / totalTime * DRAWABLE_ENVELOPE_NB_VALUES;
    valueIndex = roundToInt( xTime );
    if( valueIndex >= DRAWABLE_ENVELOPE_NB_VALUES )
    {
        valueIndex = DRAWABLE_ENVELOPE_NB_VALUES - 1;
    }

    std::vector<Atomic<float>> * values = &valuesAttack;
    if(currentPhase == RELEASE)
    {
        values = &valuesRelease;
    }

    //MAKE THE VALUE CONTINUOUS (smooth between steps)
    currentGain = values->at(valueIndex).get();
    float diffTime = xTime - valueIndex;
    float nextValue = -1.f;
    if( valueIndex < values->size() - 1 )
    {
        nextValue = values->at(valueIndex + 1).get();
    }
    else if( currentPhase == ATTACK ) //if the next value is the sustain
    {
        nextValue = sustainLevel.get();
    }

    if( nextValue > 0 )
    {
        currentGain += (nextValue - currentGain) * diffTime; //add the decimal bit to make it continuous
    }

    if(currentPhase == RELEASE)
    {
        currentGain *= hitReleaseGainSustainRatio;
    }

    notifyProgress(deltaTime, currentGain);
    sampleIndex++;

    if(deltaTime >= totalTime)
    {
        if( currentPhase == ATTACK )
        {
            currentPhase = SUSTAIN;
            currentGain = sustainLevel.get();
            notifyProgress(deltaTime, currentGain);
        }
        else if( currentPhase == RELEASE )
        {
            currentPhase = OFF;
            currentGain = 0.0f;

            hitReleaseGainSustainRatio = 1.0f;
            sampleIndex = 0;
            valueIndex = 0;
            notifyProgress(0.0f, 0.0f);
            notifyEndNote();
        }
    }

    return currentGain;
}

//============================================================================
const std::vector<Atomic<float>> & DrawableEnvelope::getValuesAttack()
{
    return valuesAttack;
}

//============================================================================
const std::vector<Atomic<float>> & DrawableEnvelope::getValuesRelease()
{
    return valuesRelease;
}

//============================================================================
float DrawableEnvelope::getSustainLevel()
{
    return sustainLevel.get();
}

//============================================================================
float DrawableEnvelope::getAttackTime()
{
   return attackTime.get();
}

//============================================================================
float DrawableEnvelope::getReleaseTime()
{
    return releaseTime.get();
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
    valuesAttack = std::vector<Atomic<float>>( DRAWABLE_ENVELOPE_NB_VALUES, DRAWABLE_ENVELOPE_INIT_VALUES );
    valuesRelease = std::vector<Atomic<float>>( DRAWABLE_ENVELOPE_NB_VALUES, DRAWABLE_ENVELOPE_INIT_VALUES );
    sustainLevel = DRAWABLE_ENVELOPE_INIT_VALUES;
}
