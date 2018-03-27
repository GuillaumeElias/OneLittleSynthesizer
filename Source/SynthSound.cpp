#include "SynthSound.h"

// =============================================================================
SynthSound::SynthSound( AudioProcessorValueTreeState * processorParameters )
    : parameters( processorParameters )
{
}

// =============================================================================
float SynthSound::getParameterValue(String paramId) const
{
    return *parameters->getRawParameterValue(paramId);
}
