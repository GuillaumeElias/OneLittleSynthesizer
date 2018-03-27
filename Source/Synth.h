#pragma once

#include "SynthSound.h"
#include "SynthVoice.h"

#include "../JuceLibraryCode/JuceHeader.h"

/*
* Note: The actually audio processing is done in SynthVoice.cpp
*/
class Synth : public Synthesiser
{
public:
    void handleMidiEvent( const MidiMessage & ) override; //only overridden for logging purposes

    void prepareToPlay( float sampleRate ); // sets the current playback sampleRate and call prepareToPlay on allVoices
};
