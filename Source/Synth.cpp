#include "Synth.h"

//=====================================================================================
void Synth::handleMidiEvent( const MidiMessage & message)
{
    Synthesiser::handleMidiEvent( message );
}

//=====================================================================================
void Synth::prepareToPlay( float sampleRate )
{
    setCurrentPlaybackSampleRate(sampleRate);

    for(int i=0; i < getNumVoices(); ++i)
    {
        SynthVoice * synthVoice = dynamic_cast<SynthVoice * > (getVoice(i));
        synthVoice->prepareToPlay();
    }
}
