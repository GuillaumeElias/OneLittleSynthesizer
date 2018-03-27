#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "EnvelopeUI.h"

//==============================================================================

class WaveShapeSlider : public Slider
{
	String getTextFromValue(double value) override
	{
		return Oscillator::waveShapeToString(value);
	}
};

//==============================================================================

class OneLittleSynthesizerAudioProcessorEditor : public AudioProcessorEditor, private KeyListener
{
public:
    OneLittleSynthesizerAudioProcessorEditor (OneLittleSynthesizerAudioProcessor&, AudioProcessorValueTreeState& );
    ~OneLittleSynthesizerAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    bool keyPressed (const KeyPress &key, Component *originatingComponent) override;
    bool keyStateChanged (bool isKeyDown, Component *originatingComponent) override;

    OneLittleSynthesizerAudioProcessor& processor;
    AudioProcessorValueTreeState& parameters;

    MidiKeyboardComponent midiKeyboard;

    //WAVE SHAPE
	WaveShapeSlider waveShapeSlider;
    Label waveShapeLabel;
    ScopedPointer<SliderAttachment> waveShapeAttachment;

    //FILTER FREQ
    Slider filterFrequencySlider;
    Label filterFrequencyLabel;
    ScopedPointer<SliderAttachment> filterFreqAttachment;

    //FILTER RESONANCE
    Slider filterResonanceSlider;
    Label filterResonanceLabel;
    ScopedPointer<SliderAttachment> filterResAttachment;

    //ENVELOPE
    EnvelopeUI envelopeUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneLittleSynthesizerAudioProcessorEditor)
};
