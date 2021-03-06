/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Envelope/AbstractEnvelope.h"
#include "Oscillator/FMEngine.h"

#include <iomanip>
#include <sstream>

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
    String floatToStrWithDecimals(float number, int precision)
    {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << number;
        return stream.str();
    }

    //=========================================================================
    String floatToStr(float number)
    {
        return floatToStrWithDecimals(number, 0);
    }

    //=========================================================================
    void addVectorToTree(const std::vector<Atomic<float>> & vect, ValueTree & tree, const Identifier & id)
    {
        ValueTree vectorTree = tree.getOrCreateChildWithName(id, nullptr);
        vectorTree.removeAllChildren(nullptr);

        for(Atomic<float> f : vect)
        {
            ValueTree element(Identifier("element"));
            element.setProperty("value", f.get(), nullptr);
            vectorTree.appendChild(element, nullptr);
        }
    }

    //=========================================================================
    void iterateOnTree(const ValueTree & tree, const Identifier & subTreeId, const std::function< void(int, float) >& processElement)
    {
        ValueTree subTree = tree.getChildWithName( subTreeId );
        if( subTree.isValid() )
        {
            for( int i=0; i < subTree.getNumChildren(); i++)
            {
                processElement(i, subTree.getChild(i).getProperty("value"));
            }
        }
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

    parameters.createAndAddParameter ("waveShape1",       // parameter ID
                                  "Wave shape osc 1",       // parameter name
                                  String(),     // parameter label (suffix)
                                  NormalisableRange<float> (1.f, 4.f, 1.f),    // range
                                  INIT_WAVE_SHAPE,         // default value
								  Oscillator::waveShapeToString, //value to text
                                  nullptr);

    parameters.createAndAddParameter("waveShape2",
                                  "Wave shape osc 2",
                                  String(),
                                  NormalisableRange<float>(1.f, 4.f, 1.f),
                                  INIT_WAVE_SHAPE,
                                  Oscillator::waveShapeToString,
                                  nullptr);

    parameters.createAndAddParameter("osc2FreqOffset",
                                  "Osc 2 freq offset ratio",
                                  String(),
                                  NormalisableRange<float>(0.0f, MAX_OSC2_FREQUENCY_OFFSET_RATIO),
                                  1.0f,
                                  [](float val) { return floatToStrWithDecimals(val, 4); },
                                  nullptr);

    parameters.createAndAddParameter("osc2Mod1Amount",
                                  "Osc 2 mod 1 amount",
                                  String(),
                                  NormalisableRange<float>(0.0f, 3.0f), //0 -> 1
                                  1.0f,
                                  [](float val) { return floatToStr(val * 100) + " %"; },
                                  nullptr);

    parameters.createAndAddParameter("waveMix",
                                  "Wave mix",
                                  String(),
                                  NormalisableRange<float>(0.f, 1.f), //0 -> 1
                                  0.5f,
                                  [](float val) { return floatToStr(val * 100) + " %"; },
                                  nullptr);

    parameters.createAndAddParameter ("filterCutoffFreq",
                                  "Filter Frequency",
                                  String(),
                                  NormalisableRange<float> (0.f, MAX_FILTER_CUTOFF_FREQUENCY), //0 to 2000 Hz
                                  INIT_FILTER_FREQUENCY,
                                  [](float val){ return floatToStr(val) + " Hz"; },
                                  nullptr);

    parameters.createAndAddParameter ("filterRes",
                                  "Filter Resonance",
                                  String(),
                                  NormalisableRange<float> (0.1f, 6.0f), //Q
                                  INIT_FILTER_RESONANCE,
                                  [](float val){ return floatToStr(val * 10); },
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
                                  NormalisableRange<float> (1.f, 3000.f), //1 to 3000 ms
                                  INIT_ENV_DECAY,
                                  floatToStr,
                                  nullptr);

    parameters.createAndAddParameter ("envSustain",
                                  "Envelope Sustain",
                                  String(),
                                  NormalisableRange<float> (0.f, 1.f), //0 -> 1
                                  INIT_ENV_SUSTAIN,
                                  nullptr, //no need to show the value
                                  nullptr);

    parameters.createAndAddParameter ("envRelease",
                                  "Envelope Sweet Release",
                                  String(),
                                  NormalisableRange<float> (1.f, 3000.f), //1 to 3000 ms
                                  INIT_ENV_SUSTAIN,
                                  floatToStr,
                                  nullptr);

    parameters.createAndAddParameter ("filterAttack",
                                  "Filter Attack",
                                  String(),
                                  NormalisableRange<float> (0.f, 5000), //1 to 5000 ms
                                  INIT_FILTER_ENV_ATTACK,
                                  [](float val){ return floatToStr(val) + " ms"; },
                                  nullptr);

    parameters.createAndAddParameter ("filterEnvAmount",
                                  "Filter Attack",
                                  String(),
                                  NormalisableRange<float> (0.f, 1.f), //0 to 1
                                  INIT_FILTER_ENV_AMOUNT,
                                  [](float val){ return floatToStr(val * 100) + " %"; },
                                  nullptr);
    parameters.createAndAddParameter("loopDrawableEnvelope",
                                  "Loop Drawable Envelope",
                                  "Loop",
                                  NormalisableRange<float> (0.0f, 1.0f, 1.0f),
                                  0.0f,
                                  [](float value){ return value < 0.5f ? "Off" : "On"; },
                                  nullptr, false, true, true);
    parameters.createAndAddParameter("octaveShift",
                                  "Octave Shift",
                                  String(),
                                  NormalisableRange<float>(OCTAVE_SHIFT_MIN, OCTAVE_SHIFT_MAX, 1.0),
                                  0, nullptr, nullptr);

    parameters.state = ValueTree (Identifier ("OneLittleSynthesizer"));

    for(int i=0; i < NUMBER_OF_VOICES; i++)
    {
        synth.addVoice( new SynthVoice( &parameters, i ) );
    }

    synth.addSound( new SynthSound( &parameters ) );

    parameters.addParameterListener("filterAttack", this);
    parameters.addParameterListener("envRelease", this);
    parameters.addParameterListener("loopDrawableEnvelope", this);
    parameters.addParameterListener("waveMix", this);
    parameters.addParameterListener("osc2Mod1Amount", this);
}

//==============================================================================
OneLittleSynthesizerAudioProcessor::~OneLittleSynthesizerAudioProcessor()
{
	Logger::setCurrentLogger(nullptr);
    parameters.removeParameterListener("filterAttack", this);
    parameters.removeParameterListener("envRelease", this);
    parameters.removeParameterListener("loopDrawableEnvelope", this);
    parameters.removeParameterListener("waveMix", this);
    parameters.removeParameterListener("osc2Mod1Amount", this);
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
    OneLittleSynthesizerAudioProcessorEditor * editor = new OneLittleSynthesizerAudioProcessorEditor (*this, parameters);

    EnvelopeUI * envelopeUI = editor->getEnvelopeUI();
    DrawableEnvelopeUI * drawableEnvelopeUI = editor->getDrawableEnvelopeUI();

    for(int i=0; i < NUMBER_OF_VOICES; i++)
    {
        SynthVoice * synthVoice = dynamic_cast<SynthVoice *> ( synth.getVoice(i) );
		synthVoice->setEnvelopeUpdater(std::make_unique<EnvelopeUIUpdater>(envelopeUI));
		synthVoice->setDrawableEnvUpdater(std::make_unique<EnvelopeUIUpdater>(drawableEnvelopeUI));
		envelopeUI->addEnvelope(i,synthVoice->getEnvelope());
		drawableEnvelopeUI->addEnvelope(i, synthVoice->getDrawableEnvelope());
	}

    return editor;
}

//==============================================================================
void OneLittleSynthesizerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // SAVE STATE
    ValueTree state = parameters.copyState(); //thread-safe yeay
    state.setProperty("drawableEnvSustain", DrawableEnvelope::getSustainLevel(), nullptr);
    addVectorToTree( DrawableEnvelope::getValuesAttack(), state, "drawableEnvAttackValues");
    addVectorToTree( DrawableEnvelope::getValuesRelease(), state, "drawableEnvReleaseValues");

    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

//==============================================================================
void OneLittleSynthesizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // RESTORE STATE
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr && xmlState->hasTagName (parameters.state.getType()))
    {
        ValueTree state = ValueTree::fromXml (*xmlState);
        parameters.replaceState (state); //thread-safe ye

        if(state.hasProperty("drawableEnvSustain"))
        {
            DrawableEnvelope::setSustainLevel( state.getProperty("drawableEnvSustain" ) );
        }

        iterateOnTree( state ,"drawableEnvAttackValues", [](int i, float value)
        {
            DrawableEnvelope::setValueAttack(i, value );
        });

        iterateOnTree( state ,"drawableEnvReleaseValues", [](int i, float value)
        {
            DrawableEnvelope::setValueRelease(i, value );
        });

    }
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

//============================================================================
void OneLittleSynthesizerAudioProcessor::parameterChanged(const String& parameterID, float newValue )
{
    if( parameterID == "filterAttack" )
    {
        DrawableEnvelope::setAttackTime( newValue / 1000.f );
    }
    else if( parameterID == "envRelease" )
    {
        DrawableEnvelope::setReleaseTime( newValue / 1000.f );
    }
    else if( parameterID == "loopDrawableEnvelope" )
    {
        DrawableEnvelope::setLoop( newValue > 0.5f );
    }
    else if (parameterID == "waveMix")
    {
        FMEngine::setWaveMix(newValue);
    }
    else if (parameterID == "osc2Mod1Amount")
    {
        FMEngine::setOsc2Mod1Amount(newValue);
    }

    //repaint editor if available
    if( auto editor = dynamic_cast<OneLittleSynthesizerAudioProcessorEditor *> ( getActiveEditor() ) )
    {
        editor->getDrawableEnvelopeUI()->repaint();
    }
}
