/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include <map>
#include "../JuceLibraryCode/JuceHeader.h"
#include "AbstractEnvelope.h"
#include "../Constants.h"

//==============================================================================

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
class EnvelopeUI;
class EnvelopeUIUpdater : public AsyncUpdater
{
    public:
        EnvelopeUIUpdater(EnvelopeUI * envelopeUI);
         void handleAsyncUpdate () override;
    private:
        EnvelopeUI * envelopeUI;
};

//==============================================================================

class EnvelopeUI : public Component, public EnvelopeListener, private AudioProcessorValueTreeState::Listener
{
public:
    EnvelopeUI(AudioProcessorValueTreeState& processorParameters);
    ~EnvelopeUI();

    void paint(Graphics&) override;
    void paintEnvelope(Graphics&);

    void resized() override;

    void onEndNote( int voiceNumber ) override;
    void onProgress( int voiceNumber, const EnvelopeProgress & progress ) override;

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

    std::map <int, EnvelopeProgress> envProgressMap;

    EnvelopeUIUpdater updater;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeUI)
};
