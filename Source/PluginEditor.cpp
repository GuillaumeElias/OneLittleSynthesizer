/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
OneLittleSynthesizerAudioProcessorEditor::OneLittleSynthesizerAudioProcessorEditor (OneLittleSynthesizerAudioProcessor& p, AudioProcessorValueTreeState& params)
    : AudioProcessorEditor (&p)
    , processor (p)
    , parameters (params)
    , midiKeyboard (p.keyboardState, MidiKeyboardComponent::horizontalKeyboard)
    , envelopeUI(params)
{
    setSize (450, 320);

    getLookAndFeel().setColour (Slider::thumbColourId, Colours::white);
    getLookAndFeel().setColour (Label::textColourId, Colours::white);

    //Wave shape
    waveShapeLabel.setText( "Wave shape", dontSendNotification);
    waveShapeLabel.setColour (Label::textColourId, Colours::white);
    waveShapeLabel.attachToComponent (&waveShapeSlider, true);

    waveShapeAttachment = new SliderAttachment (parameters, "waveShape", waveShapeSlider);
    waveShapeSlider.setTextBoxStyle (Slider::NoTextBox, false, 120, 90);
    waveShapeSlider.setPopupDisplayEnabled (true, false, this);

    //Frequency slider
    filterFrequencyLabel.setText( "Filter frequency", dontSendNotification);
    filterFrequencyLabel.setColour (Label::textColourId, Colours::white);
    filterFrequencyLabel.attachToComponent (&filterFrequencySlider, true);

    filterFreqAttachment = new SliderAttachment (parameters, "filterFreq", filterFrequencySlider);
    filterFrequencySlider.setTextBoxStyle (Slider::NoTextBox, false, 120, 90);
    filterFrequencySlider.setPopupDisplayEnabled (true, false, this);
    filterFrequencySlider.setSkewFactorFromMidPoint (800.0);

    //Resonance slider
    filterResonanceLabel.setText( "Filter resonance", dontSendNotification);
    filterResonanceLabel.setColour (Label::textColourId, Colours::white);
    filterResonanceLabel.attachToComponent (&filterResonanceSlider, true);

    filterResAttachment = new SliderAttachment (parameters, "filterRes", filterResonanceSlider);
    filterResonanceSlider.setTextBoxStyle (Slider::NoTextBox, false, 120, 90);
    filterResonanceSlider.setPopupDisplayEnabled (true, false, this);

    setFocusContainer(true);
    envelopeUI.setWantsKeyboardFocus(false);
    midiKeyboard.setWantsKeyboardFocus(false);

    //Making the components visible
    addAndMakeVisible(waveShapeLabel);
    addAndMakeVisible(waveShapeSlider);
    addAndMakeVisible(filterFrequencyLabel);
    addAndMakeVisible(filterFrequencySlider);
    addAndMakeVisible(filterResonanceLabel);
    addAndMakeVisible(filterResonanceSlider);
    addAndMakeVisible(midiKeyboard);
    addAndMakeVisible(envelopeUI);

    //Adding key listener for moving filter with left and right arrows
    this->addKeyListener(this);
}

//==============================================================================
OneLittleSynthesizerAudioProcessorEditor::~OneLittleSynthesizerAudioProcessorEditor()
{
}

//==============================================================================
void OneLittleSynthesizerAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

//==============================================================================
void OneLittleSynthesizerAudioProcessorEditor::resized()
{
    Rectangle<int> bounds (getLocalBounds().reduced (10));

    midiKeyboard.setBounds (bounds.removeFromBottom (70) );

    waveShapeLabel.setBounds(0, 20, bounds.getWidth(), 20);
    waveShapeSlider.setBounds (100, 20, bounds.getWidth() - 100 , 20);

    filterFrequencyLabel.setBounds(0, 60, bounds.getWidth(), 20);
    filterFrequencySlider.setBounds (100, 60, bounds.getWidth() - 100 , 20);

    filterResonanceLabel.setBounds(0, 100, bounds.getWidth(), 20);
    filterResonanceSlider.setBounds (100, 100, bounds.getWidth() - 100 , 20);

    envelopeUI.setBounds(0, 130, getWidth(), 100);

}

//==============================================================================
bool OneLittleSynthesizerAudioProcessorEditor::keyPressed (const KeyPress &key, Component *originatingComponent)
{
    if(key.isKeyCode( KeyPress::rightKey ) )
    {
        filterFrequencySlider.setValue( filterFrequencySlider.getValue() + 25.f );
    }
    else if( key.isKeyCode( KeyPress::leftKey ) )
    {
        filterFrequencySlider.setValue( filterFrequencySlider.getValue() - 25.f );
    }

	return midiKeyboard.keyPressed( key );
}

//==============================================================================
bool OneLittleSynthesizerAudioProcessorEditor::keyStateChanged (bool isKeyDown, Component *originatingComponent)
{
    return midiKeyboard.keyStateChanged(isKeyDown);
}
