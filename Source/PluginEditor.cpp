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
    , drawableEnvelopeUI(params)
{
    setSize (370, 420);

    setLookAndFeel(&lookAndFeel);

    //Reset parameters button
    resetParametersButton.setButtonText("Reset");
    resetParametersButton.setColour(Slider::thumbColourId, Colours::white);
    resetParametersButton.addListener(this);

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

    filterFreqAttachment = new SliderAttachment (parameters, "filterCutoffFreq", filterFrequencySlider);
    filterFrequencySlider.setTextBoxStyle (Slider::NoTextBox, false, 120, 90);
    filterFrequencySlider.setPopupDisplayEnabled (true, false, this);
    filterFrequencySlider.setSkewFactorFromMidPoint (800.0);

    //Resonance slider
    filterResonanceLabel.setText( "Filter resonance", dontSendNotification);
    filterResonanceLabel.setColour (Label::textColourId, Colours::white);
    filterResonanceLabel.attachToComponent (&filterResonanceSlider, true);

    filterResAttachment = new SliderAttachment (parameters, "filterRes", filterResonanceSlider);
    filterResonanceSlider.setTextBoxStyle (Slider::NoTextBox, false, 120, 90);

    setFocusContainer(true);
    envelopeUI.setWantsKeyboardFocus(false);
    drawableEnvelopeUI.setWantsKeyboardFocus(false);
    midiKeyboard.setWantsKeyboardFocus(false);

    //Making the components visible
    addAndMakeVisible(resetParametersButton);
    addAndMakeVisible(waveShapeLabel);
    addAndMakeVisible(waveShapeSlider);
    addAndMakeVisible(filterFrequencyLabel);
    addAndMakeVisible(filterFrequencySlider);
    addAndMakeVisible(filterResonanceLabel);
    addAndMakeVisible(filterResonanceSlider);
    addAndMakeVisible(midiKeyboard);
    addAndMakeVisible(envelopeUI);
    addAndMakeVisible(drawableEnvelopeUI);

    //Adding key listener for moving filter with left and right arrows
    this->addKeyListener(this);
}

//==============================================================================
OneLittleSynthesizerAudioProcessorEditor::~OneLittleSynthesizerAudioProcessorEditor()
{
    resetParametersButton.removeListener(this);
    setLookAndFeel (nullptr);
}

//==============================================================================
void OneLittleSynthesizerAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

//==============================================================================
void OneLittleSynthesizerAudioProcessorEditor::resized()
{
    Rectangle<int> bounds (getLocalBounds());

    midiKeyboard.setBounds (bounds.removeFromBottom (70) );

    resetParametersButton.setBounds(bounds.getWidth() - 55, 10, 45, 20);

    waveShapeLabel.setBounds(0, 40, bounds.getWidth(), 20);
    waveShapeSlider.setBounds (100, 40, bounds.getWidth() - 100 , 20);

    filterFrequencyLabel.setBounds(0, 80, bounds.getWidth(), 20);
    filterFrequencySlider.setBounds (100, 80, bounds.getWidth() - 100 , 20);

    filterResonanceLabel.setBounds(0, 120, bounds.getWidth(), 20);
    filterResonanceSlider.setBounds (100, 120, bounds.getWidth() - 100 , 20);

    envelopeUI.setBounds(0, 150, getWidth(), 100);
    drawableEnvelopeUI.setBounds(0, 250, getWidth(), DRAWABLE_ENVELOPE_HEIGHT - 1);
}

//==============================================================================
EnvelopeUI * OneLittleSynthesizerAudioProcessorEditor::getEnvelopeUI()
{
    return &envelopeUI;
}

//==============================================================================
DrawableEnvelopeUI * OneLittleSynthesizerAudioProcessorEditor::getDrawableEnvelopeUI()
{
    return &drawableEnvelopeUI;
}

//==============================================================================
void OneLittleSynthesizerAudioProcessorEditor::buttonClicked( Button * button )
{
    if( button == &resetParametersButton )
    {
        processor.setParameterValue("waveShape", INIT_WAVE_SHAPE);
        processor.setParameterValue("filterCutoffFreq", INIT_FILTER_FREQUENCY);
        processor.setParameterValue("filterRes", INIT_FILTER_RESONANCE);
        processor.setParameterValue("envAttack", INIT_ENV_ATTACK);
        processor.setParameterValue("envDecay", INIT_ENV_DECAY);
        processor.setParameterValue("envSustain", INIT_ENV_SUSTAIN);
        processor.setParameterValue("envRelease", INIT_ENV_RELEASE);
        processor.setParameterValue("filterAttack", INIT_FILTER_ENV_ATTACK);
        processor.setParameterValue("filterEnvAmount", INIT_FILTER_ENV_AMOUNT);

        DrawableEnvelope::resetValues();
        drawableEnvelopeUI.repaint();
    }
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
