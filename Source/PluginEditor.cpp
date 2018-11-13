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
    setSize (370, 530);

    setLookAndFeel(&lookAndFeel);

    //Reset parameters button
    resetParametersButton.setButtonText("Reset");
    resetParametersButton.setColour(Slider::thumbColourId, Colours::white);
    resetParametersButton.addListener(this);

    //Wave shape Osc 1
    waveShapeLabel1.setText( "Wave shape 1", dontSendNotification);
    waveShapeLabel1.setColour (Label::textColourId, Colours::white);
    waveShapeLabel1.attachToComponent (&waveShapeSlider1, true);

    waveShapeAttachment1 = new SliderAttachment (parameters, "waveShape1", waveShapeSlider1);
    waveShapeSlider1.setTextBoxStyle (Slider::NoTextBox, false, 120, 90);
    waveShapeSlider1.setPopupDisplayEnabled (true, false, this);

    //Wave shape Osc 2
    waveShapeLabel2.setText("Wave shape 2", dontSendNotification);
    waveShapeLabel2.setColour(Label::textColourId, Colours::white);
    waveShapeLabel2.attachToComponent(&waveShapeSlider2, true);

    waveShapeAttachment2 = new SliderAttachment(parameters, "waveShape2", waveShapeSlider2);
    waveShapeSlider2.setTextBoxStyle(Slider::NoTextBox, false, 120, 90);
    waveShapeSlider2.setPopupDisplayEnabled(true, false, this);

    //Osc 2 Frequency offset
    osc2FreqOffsetLabel.setText("Osc2 freq offset", dontSendNotification);
    osc2FreqOffsetLabel.setColour(Label::textColourId, Colours::white);
    osc2FreqOffsetLabel.attachToComponent(&osc2FreqOffsetSlider, true);

    osc2FreqOffsetAttachment = new SliderAttachment(parameters, "osc2FreqOffset", osc2FreqOffsetSlider);
    osc2FreqOffsetSlider.setTextBoxStyle(Slider::NoTextBox, false, 120, 90);
    osc2FreqOffsetSlider.setPopupDisplayEnabled(true, false, this);

    //Wave mix
    waveMixLabel.setText("Wave mix", dontSendNotification);
    waveMixLabel.setColour(Label::textColourId, Colours::white);
    waveMixLabel.attachToComponent(&waveMixSlider, true);

    waveMixAttachment = new SliderAttachment(parameters, "waveMix", waveMixSlider);
    waveMixSlider.setTextBoxStyle(Slider::NoTextBox, false, 120, 90);
    waveMixSlider.setPopupDisplayEnabled(true, false, this);

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
    addAndMakeVisible(waveShapeLabel1);
    addAndMakeVisible(waveShapeSlider1);
    addAndMakeVisible(waveShapeLabel2);
    addAndMakeVisible(waveShapeSlider2);
    addAndMakeVisible(osc2FreqOffsetLabel);
    addAndMakeVisible(osc2FreqOffsetSlider);
    addAndMakeVisible(waveMixLabel);
    addAndMakeVisible(waveMixSlider);
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

    waveShapeLabel1.setBounds(0, 40, bounds.getWidth(), 20);
    waveShapeSlider1.setBounds (100, 40, bounds.getWidth() - 100 , 20);

    waveShapeLabel2.setBounds(0, 80, bounds.getWidth(), 20);
    waveShapeSlider2.setBounds(100, 80, bounds.getWidth() - 100, 20);

    osc2FreqOffsetLabel.setBounds(0, 120, bounds.getWidth(), 20);
    osc2FreqOffsetSlider.setBounds(100, 120, bounds.getWidth() - 100, 20);

    waveMixLabel.setBounds(0, 160, bounds.getWidth(), 20);
    waveMixSlider.setBounds(100, 160, bounds.getWidth() - 100, 20);

    filterFrequencyLabel.setBounds(0, 200, bounds.getWidth(), 20);
    filterFrequencySlider.setBounds (100, 200, bounds.getWidth() - 100 , 20);

    filterResonanceLabel.setBounds(0, 240, bounds.getWidth(), 20);
    filterResonanceSlider.setBounds (100, 240, bounds.getWidth() - 100 , 20);

    envelopeUI.setBounds(0, 270, getWidth(), 100);
    drawableEnvelopeUI.setBounds(0, 360, getWidth(), DRAWABLE_ENVELOPE_HEIGHT - 1);
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
        processor.setParameterValue("waveShape1", INIT_WAVE_SHAPE);
        processor.setParameterValue("waveShape2", INIT_WAVE_SHAPE);
        processor.setParameterValue("osc2FreqOffset", 1.0f);
        processor.setParameterValue("osc2Mod1Amount", 0.0f);
        processor.setParameterValue("waveMix", 0.5f);
        processor.setParameterValue("filterCutoffFreq", INIT_FILTER_FREQUENCY);
        processor.setParameterValue("filterRes", INIT_FILTER_RESONANCE);
        processor.setParameterValue("envAttack", INIT_ENV_ATTACK);
        processor.setParameterValue("envDecay", INIT_ENV_DECAY);
        processor.setParameterValue("envSustain", INIT_ENV_SUSTAIN);
        processor.setParameterValue("envRelease", INIT_ENV_RELEASE);
        processor.setParameterValue("filterAttack", INIT_FILTER_ENV_ATTACK);
        processor.setParameterValue("filterEnvAmount", INIT_FILTER_ENV_AMOUNT);
        processor.setParameterValue("loopDrawableEnvelope", 0.0f);

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
