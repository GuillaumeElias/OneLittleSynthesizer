/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DrawableEnvelope.h"
#include <map>

//==============================================================================

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
class DrawableEnvelopeUI;
class DrawableEnvelopeUIUpdater : public AsyncUpdater
{
    public:
        DrawableEnvelopeUIUpdater(DrawableEnvelopeUI * envelopeUI);
         void handleAsyncUpdate () override;
    private:
        DrawableEnvelopeUI * envelopeUI;
};

//==============================================================================
class DrawableEnvelopeUI : public Component, public EnvelopeListener
{
    public:
        DrawableEnvelopeUI(AudioProcessorValueTreeState& processorParameters);

        void paint(Graphics&) override;
        void resized() override;

        void onEndNote( int voiceNumber ) override;
        void onProgress( int voiceNumber, const EnvelopeProgress & progress ) override;

    private:
        void mouseDown( const MouseEvent & event) override;
        void mouseDrag( const MouseEvent & event) override;

        void handleClick( int x, int y);

        void paintContour(Graphics & g, const std::vector<Atomic<float>> & values, int paddingLeft);

        AudioProcessorValueTreeState& parameters;

        int height;

        //Attack slider
        Slider attackSlider;
        ScopedPointer<SliderAttachment> attackAttachment;

        //Filter env amount
        Slider envAmountSlider;
        ScopedPointer<SliderAttachment> envAmountAttachment;

        std::map <int, EnvelopeProgress> envProgressMap;

        DrawableEnvelopeUIUpdater updater;
};

