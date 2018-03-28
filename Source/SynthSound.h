/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

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
