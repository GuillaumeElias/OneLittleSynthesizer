/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "DrawableEnvelope.h"

//============================================================================
DrawableEnvelope::DrawableEnvelope(AudioProcessorValueTreeState * processorParameters, double splRate)
    : values( DRAWABLE_ENVELOPE_NB_VALUES )
    , sampleRate( splRate )
    , totalTime( DRAWABLE_ENVELOPE_INIT_VALUES * 0.1 ) //steps of 100ms
{
    for(int i=0; i < DRAWABLE_ENVELOPE_NB_VALUES; i++)
    {
        values[i] = DRAWABLE_ENVELOPE_INIT_VALUES;
    }
}

//============================================================================
DrawableEnvelope::~DrawableEnvelope()
{
}

//============================================================================
void DrawableEnvelope::noteOn()
{
}

//============================================================================
void DrawableEnvelope::noteOff(bool allowTailOff)
{
    sampleIndex = 0;
    valueIndex = 0;
}

//============================================================================
float DrawableEnvelope::computeGain()
{
    float deltaTime = sampleIndex / sampleRate;

    valueIndex = roundToInt( deltaTime / totalTime );
    if( valueIndex >= DRAWABLE_ENVELOPE_NB_VALUES )
    {
        valueIndex = DRAWABLE_ENVELOPE_NB_VALUES - 1;
    }

    sampleIndex++;

    return values[valueIndex];
}
