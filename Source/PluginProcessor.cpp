/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    //=========================================================================
    /* This function is used to filter out midi messages that are not supported at this point (i.e. everything but note on/off + sustain) */
    void filterMidiMessages(const MidiBuffer& midiBufferIn, MidiBuffer& midiBufferOut)
    {
        MidiBuffer::Iterator bufferIterator(midiBufferIn);
        MidiMessage midiEvent;
        int samplePosition;

        while (bufferIterator.getNextEvent(midiEvent, samplePosition))
        {
            if (midiEvent.isNoteOn() || midiEvent.isNoteOff() || midiEvent.isSustainPedalOn() || midiEvent.isSustainPedalOff())
            {
                midiBufferOut.addEvent(midiEvent, samplePosition);
            }
        }
    }

    //=========================================================================
    String floatToStr(float number)
    {
        return String( roundToInt(number) );
    }
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter();

//==============================================================================
OneLittleSynthesizerAudioProcessor::OneLittleSynthesizerAudioProcessor()
    : AudioProcessor ( BusesProperties().withOutput ("Output", AudioChannelSet::stereo(), true) )
    , loggerFile( File::getCurrentWorkingDirectory().getFullPathName() + "\\OneLittleSynthesizer.log" )
    , logger( loggerFile, "OneLittleSynthesizer", 0 )
    , parameters ( *this, nullptr )
{
    Logger::setCurrentLogger(&logger);

    parameters.createAndAddParameter ("waveShape",       // parameter ID
                                  "Wave shape",       // parameter name
                                  String(),     // parameter label (suffix)
                                  NormalisableRange<float> (1.f, 4.f, 1.f),    // range
                                  INIT_WAVE_SHAPE,         // default value
								  Oscillator::waveShapeToString, //value to text
                                  nullptr);

    parameters.createAndAddParameter ("filterFreq",
                                  "Filter Frequency",
                                  String(),
                                  NormalisableRange<float> (0.f, 2000.0f), //0 to 2000 Hz
                                  INIT_FILTER_FREQUENCY,
                                  floatToStr,
                                  nullptr);

    parameters.createAndAddParameter ("filterRes",
                                  "Filter Resonance",
                                  String(),
                                  NormalisableRange<float> (0.1f, 6.0f), //Q
                                  INIT_FILTER_RESONANCE,
                                  floatToStr,
                                  nullptr);

    parameters.createAndAddParameter ("envAttack",
                                  "Envelope Attack",
                                  String(),
                                  NormalisableRange<float> (1.f, 5000.f), //1 to 5000 ms
                                  INIT_ENV_ATTACK,
                                  floatToStr,
                                  nullptr);

    parameters.createAndAddParameter ("envDecay",
                                  "Envelope Decay",
                                  String(),
                                  NormalisableRange<float> (1.f, 5000.f), //1 to 5000 ms
                                  INIT_ENV_DECAY,
                                  floatToStr,
                                  nullptr);

    parameters.createAndAddParameter ("envSustain",
                                  "Envelope Sustain",
                                  String(),
                                  NormalisableRange<float> (0.f, 1.f), //0 -> 1
                                  INIT_ENV_SUSTAIN,
                                  nullptr, //no need for remapping
                                  nullptr);

    parameters.createAndAddParameter ("envRelease",
                                  "Envelope Sweet Release",
                                  String(),
                                  NormalisableRange<float> (1.f, 5000.f), //1 to 5000 ms
                                  INIT_ENV_SUSTAIN,
                                  floatToStr,
                                  nullptr);

    parameters.state = ValueTree (Identifier ("OneLittleSynthesizer"));

    for(int i=0; i < NUMBER_OF_VOICES; i++)
    {
        synth.addVoice( new SynthVoice( &parameters ) );
    }

    synth.addSound( new SynthSound( &parameters ) );
}

//==============================================================================
OneLittleSynthesizerAudioProcessor::~OneLittleSynthesizerAudioProcessor()
{
	Logger::setCurrentLogger(nullptr);
}

//==============================================================================
const String OneLittleSynthesizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

//==============================================================================
bool OneLittleSynthesizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

//==============================================================================
bool OneLittleSynthesizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

//==============================================================================
bool OneLittleSynthesizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

//==============================================================================
double OneLittleSynthesizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================
int OneLittleSynthesizerAudioProcessor::getNumPrograms()
{
    return 0;
}

//==============================================================================
int OneLittleSynthesizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

//==============================================================================
void OneLittleSynthesizerAudioProcessor::setCurrentProgram (int index)
{
}

//==============================================================================
const String OneLittleSynthesizerAudioProcessor::getProgramName (int index)
{
    return {};
}

//==============================================================================
void OneLittleSynthesizerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void OneLittleSynthesizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.prepareToPlay(sampleRate); //calls setCurrentPlaybackSampleRate
    keyboardState.reset();
    reset();

    String message;
    message << "OneLittleSynthesizer prepares to play" << newLine;
    message << " samplesPerBlockExpected = " << samplesPerBlock << newLine;
    message << " sampleRate = " << sampleRate;
    logger.writeToLog (message);
}

//==============================================================================
void OneLittleSynthesizerAudioProcessor::releaseResources()
{
    keyboardState.reset();
}

//==============================================================================
#ifndef JucePlugin_PreferredChannelConfigurations
bool OneLittleSynthesizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
void OneLittleSynthesizerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    MidiBuffer filteredMidiMessages;
    filterMidiMessages(midiMessages, filteredMidiMessages);

    keyboardState.processNextMidiBuffer (filteredMidiMessages, 0, buffer.getNumSamples(), true);

    synth.renderNextBlock(buffer, filteredMidiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool OneLittleSynthesizerAudioProcessor::hasEditor() const
{
    return true;
}

//==============================================================================
AudioProcessorEditor* OneLittleSynthesizerAudioProcessor::createEditor()
{
    return new OneLittleSynthesizerAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void OneLittleSynthesizerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // SAVE STATE
    ValueTree state = parameters.copyState(); //thread-safe yeay
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

//==============================================================================
void OneLittleSynthesizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    //RESTORE STATE
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr && xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (ValueTree::fromXml (*xmlState)); //thread-safe ye
}

//==============================================================================
void OneLittleSynthesizerAudioProcessor::setParameterValue(String paramName, float value)
{
    auto parameter = parameters.getParameter(paramName);
    auto range = parameters.getParameterRange(paramName);

    parameter->beginChangeGesture();
    parameter->setValueNotifyingHost(range.convertTo0to1(value));
    parameter->endChangeGesture();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OneLittleSynthesizerAudioProcessor();
}
