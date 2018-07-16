/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Synth.h"

class OneLittleSynthesizerAudioProcessor  : public AudioProcessor, private AudioProcessorValueTreeState::Listener
{
public:

    OneLittleSynthesizerAudioProcessor();
    ~OneLittleSynthesizerAudioProcessor();


    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

  #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect () const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setParameterValue(String paramName, float value);

    MidiKeyboardState keyboardState;

private:

    void parameterChanged(const String& parameterID, float newValue ) override;

    File loggerFile;
    FileLogger logger;

    Synth synth;

    AudioProcessorValueTreeState parameters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneLittleSynthesizerAudioProcessor)
};
