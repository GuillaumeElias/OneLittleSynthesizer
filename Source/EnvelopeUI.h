#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================

class EnvelopeUI : public Component, private AudioProcessorValueTreeState::Listener
{
public:
    EnvelopeUI(AudioProcessorValueTreeState& processorParameters);
    ~EnvelopeUI();

    void paint(Graphics&) override;
    void paintEnvelope(Graphics&);

    void resized() override;

private:

    void parameterChanged(const String& parameterID, float newValue ) override;


    AudioProcessorValueTreeState& parameters;

    //Attack slider
    Slider attackSlider;
    ScopedPointer<SliderAttachment> attackAttachment;

    //Decay slider
    Slider decaySlider;
    ScopedPointer<SliderAttachment> decayAttachment;

    //Sustain slider
    Slider sustainSlider;
    ScopedPointer<SliderAttachment> sustainAttachment;

    //Release slider
    Slider releaseSlider;
    ScopedPointer<SliderAttachment> releaseAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeUI)
};
