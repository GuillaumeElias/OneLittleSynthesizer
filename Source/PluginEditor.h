/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "PluginLookAndFeel.h"
#include "OctaveShifterUI.h"
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
    PluginLookAndFeel lookAndFeel;

    TextButton resetParametersButton;

    //WAVE SHAPE OSC 1
	WaveShapeSlider waveShapeSlider1;
    Label waveShapeLabel1;
    ScopedPointer<SliderAttachment> waveShapeAttachment1;

    //WAVE SHAPE OSC 2
    WaveShapeSlider waveShapeSlider2;
    Label waveShapeLabel2;
    ScopedPointer<SliderAttachment> waveShapeAttachment2;

    //OSC 2 FREQ OFFSET RATIO (frequency offset of oscillator 2 compated to oscillator 1, in ratio)
    Slider osc2FreqOffsetSlider;
    Label osc2FreqOffsetLabel;
    ScopedPointer<SliderAttachment> osc2FreqOffsetAttachment;

    //OSC 2 MOD 1 AMOUNT (amount of FM modulation from osc 2 to osc 1)
    Slider osc2Mod1Slider;
    Label osc2Mod1Label;
    ScopedPointer<SliderAttachment> osc2Mod1Attachment;

    //WAVE MIX
    Slider waveMixSlider;
    Label waveMixLabel;
    ScopedPointer<SliderAttachment> waveMixAttachment;

    //FILTER FREQ
    Slider filterFrequencySlider;
    Label filterFrequencyLabel;
    ScopedPointer<SliderAttachment> filterFreqAttachment;

    //FILTER RESONANCE
    Slider filterResonanceSlider;
    Label filterResonanceLabel;
    ScopedPointer<SliderAttachment> filterResAttachment;

    //Octave Shifter
    OctaveShifterUI octaveShifterUI;

    //ENVELOPES
    EnvelopeUI envelopeUI;
    DrawableEnvelopeUI drawableEnvelopeUI;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneLittleSynthesizerAudioProcessorEditor)
};
