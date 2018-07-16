/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Envelope/EnvelopeUI.h"
#include "Envelope/DrawableEnvelopeUI.h"

//==============================================================================

class WaveShapeSlider : public Slider
{
	String getTextFromValue(double value) override
	{
		return Oscillator::waveShapeToString(value);
	}
};

//==============================================================================

class OneLittleSynthesizerAudioProcessorEditor : public AudioProcessorEditor, private KeyListener, private Button::Listener
{
public:
    OneLittleSynthesizerAudioProcessorEditor (OneLittleSynthesizerAudioProcessor&, AudioProcessorValueTreeState& );
    ~OneLittleSynthesizerAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

    EnvelopeUI * getEnvelopeUI();
    DrawableEnvelopeUI * getDrawableEnvelopeUI();

private:
    void buttonClicked ( Button * button) override;
    bool keyPressed (const KeyPress &key, Component *originatingComponent) override;
    bool keyStateChanged (bool isKeyDown, Component *originatingComponent) override;

    OneLittleSynthesizerAudioProcessor& processor;
    AudioProcessorValueTreeState& parameters;

    MidiKeyboardComponent midiKeyboard;

    TextButton resetParametersButton;

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

    //ENVELOPES
    EnvelopeUI envelopeUI;
    DrawableEnvelopeUI drawableEnvelopeUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneLittleSynthesizerAudioProcessorEditor)
};
