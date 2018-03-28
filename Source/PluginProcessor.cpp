#include "PluginProcessor.h"
#include "PluginEditor.h"

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
								  Oscillator::waveShapeToString,
                                  nullptr);

    parameters.createAndAddParameter ("filterFreq",
                                  "Filter Frequency",
                                  String(),
                                  NormalisableRange<float> (0.f, 2000.0f), //0 to 2000 Hz
                                  INIT_FILTER_FREQUENCY,
                                  nullptr,
                                  nullptr);

    parameters.createAndAddParameter ("filterRes",
                                  "Filter Resonance",
                                  String(),
                                  NormalisableRange<float> (0.1f, 6.0f), //Q
                                  INIT_FILTER_RESONANCE,
                                  nullptr,
                                  nullptr);

    parameters.createAndAddParameter ("envAttack",
                                  "Envelope Attack",
                                  String(),
                                  NormalisableRange<float> (1.f, 5000.f), //1 to 5000 ms
                                  INIT_ENV_ATTACK,
                                  nullptr,
                                  nullptr);

    parameters.createAndAddParameter ("envDecay",
                                  "Envelope Decay",
                                  String(),
                                  NormalisableRange<float> (1.f, 5000.f), //1 to 5000 ms
                                  INIT_ENV_DECAY,
                                  nullptr,
                                  nullptr);

    parameters.createAndAddParameter ("envSustain",
                                  "Envelope Sustain",
                                  String(),
                                  NormalisableRange<float> (0.f, 1.f), //0 -> 1
                                  INIT_ENV_SUSTAIN,
                                  nullptr,
                                  nullptr);

    parameters.createAndAddParameter ("envRelease",
                                  "Envelope Sweet Release",
                                  String(),
                                  NormalisableRange<float> (1.f, 5000.f), //1 to 5000 ms
                                  INIT_ENV_SUSTAIN,
                                  nullptr,
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
    keyboardState.processNextMidiBuffer (midiMessages, 0, buffer.getNumSamples(), true);

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
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
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OneLittleSynthesizerAudioProcessor();
}