/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class OctaveShifterUI : public Component, private Button::Listener, private AudioProcessorValueTreeState::Listener
{
public:
    OctaveShifterUI(OneLittleSynthesizerAudioProcessor& p, AudioProcessorValueTreeState& processorParameters);
    ~OctaveShifterUI();

    void paint(Graphics&) override;

    void resized() override;

private:
    void parameterChanged(const String& parameterID, float newValue ) override;
    void buttonClicked(Button * button);
    void refreshButtonColours();

    OneLittleSynthesizerAudioProcessor& processor;
    AudioProcessorValueTreeState & parameters;

    int currentOctaveShift;
    Colour plusButtonInitColour;
    Colour minusButtonInitColour;

    TextButton plusButton;
    TextButton minusButton;
};

