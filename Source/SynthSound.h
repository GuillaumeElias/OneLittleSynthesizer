#pragma once

#include <../JuceLibraryCode/JuceHeader.h>
#include "Constants.h"

class SynthSound : public SynthesiserSound
{
public:
    SynthSound( AudioProcessorValueTreeState* parameters );

    bool appliesToNote (int /*midiNoteNumber*/) override    { return true; }
    bool appliesToChannel (int /*midiChannel*/) override    { return true; }

    float getParameterValue(String paramId) const;
private:
    AudioProcessorValueTreeState* parameters;
};
