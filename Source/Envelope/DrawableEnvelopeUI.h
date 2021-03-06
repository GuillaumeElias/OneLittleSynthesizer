/*
 * This file is part of the OneLittleSynthesizer project and is under GNU GPL v3 (see LICENSE).
 * You can reuse it in accordance with GPLv3. Note that it doesn't come with any warranty.
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DrawableEnvelope.h"
#include "AbstractEnvelopeUI.h"
#include <map>

//==============================================================================

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

//==============================================================================
class DrawableEnvelopeUI : public AbstractEnvelopeUI
{
    public:
        DrawableEnvelopeUI(AudioProcessorValueTreeState& processorParameters);

        void paint(Graphics&) override;
        void resized() override;

    private:
        void mouseDown( const MouseEvent & event) override;
        void mouseDrag( const MouseEvent & event) override;
        void mouseUp( const MouseEvent &event) override;

        void handleClick( int x, int y, bool drag);
        void fillAllPointsTillIndex(int index, float value, bool release);

        void paintContour(Graphics & g, const std::vector<Atomic<float>> & values, int paddingLeft);

        AudioProcessorValueTreeState& parameters;

        int height;
        int lastMouseDragIndex;
        float lastMouseDragValue;

        //Attack slider
        Slider attackSlider;
        ScopedPointer<SliderAttachment> attackAttachment;

        //Filter env amount
        Slider envAmountSlider;
        ScopedPointer<SliderAttachment> envAmountAttachment;

        //Loop button
        ToggleButton loopButton;
        ScopedPointer<ButtonAttachment> loopButtonAttachment;
};

