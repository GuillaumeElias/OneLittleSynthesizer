/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

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

    Oscillator::setSampleRate(sampleRate);

    for(int i=0; i < getNumVoices(); ++i)
    {
        SynthVoice * synthVoice = dynamic_cast<SynthVoice * > (getVoice(i));
        synthVoice->prepareToPlay();
    }

}
