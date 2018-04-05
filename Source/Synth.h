/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "SynthSound.h"
#include "SynthVoice.h"

#include "../JuceLibraryCode/JuceHeader.h"

/*
* Note: The actual audio processing is done in SynthVoice.cpp
*/
class Synth : public Synthesiser
{
public:
    void handleMidiEvent( const MidiMessage & ) override; //only overridden for logging purposes

    void prepareToPlay( float sampleRate ); // sets the current playback sampleRate and call prepareToPlay on allVoices
};
